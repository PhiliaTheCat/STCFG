#include <vector>
#include <algorithm>

#include <gtirb/CFG.hpp>
#include <gtirb/Node.hpp>

#include "CFGAnalysis.hpp"

namespace stcfg
{
    static std::vector<CFGAnalysisResult> mergeResults(
        std::vector<CFGAnalysisResult> &inDegResults,
        std::vector<CFGAnalysisResult> &outDegResults);
}

namespace stcfg
{
    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::IR &ir)
    {
        const gtirb::CFG &cfg = ir.getCFG();
        std::vector<EdgeDesc> edges = getEdges(cfg);

        std::sort(edges.begin(), edges.end(),
            [](const EdgeDesc &a, const EdgeDesc &b) -> bool { return a.to < b.to; });
        std::vector<CFGAnalysisResult> inDegResults = analyzeInDeg(edges);

        std::sort(edges.begin(), edges.end(),
            [](const EdgeDesc &a, const EdgeDesc &b) -> bool { return a.from < b.from; });
        std::vector<CFGAnalysisResult> outDegResults = analyzeOutDeg(edges);

        return mergeResults(inDegResults, outDegResults);
    }

    std::vector<EdgeDesc> getEdges(const gtirb::CFG &cfg)
    {
        std::vector<EdgeDesc> edges;

        for (const auto &node: nodes(cfg))
        {
            for (const auto &pair: gtirb::cfgSuccessors(cfg, &node))
                edges.emplace_back(node.getUUID(), pair.first->getUUID(), pair.second);
        }

        return edges;
    }

    std::vector<CFGAnalysisResult> analyzeInDeg(const std::vector<EdgeDesc> &edges)
    {
        std::vector<CFGAnalysisResult> results;

        const void *p = nullptr;
        size_t total = 0;
        size_t ret = 0;
        size_t brc = 0;
        size_t call = 0;

        for (const auto &edge: edges)
        {
            if (p != edge.to && p != nullptr)
            {
                if (total >= 5)
                {
                    if (ret >= total / 2)
                        results.emplace_back(CFGAnalyzeMode::IN, p, "Exception handler");
                    if (brc >= total / 2)
                        results.emplace_back(CFGAnalyzeMode::IN, p, "Switch statement, if-else chain, or loop");
                    if (call >= total / 2)
                        results.emplace_back(CFGAnalyzeMode::IN, p, "Hot function");
                }
                p = nullptr;
                total = 0;
                ret = 0;
                brc = 0;
                call = 0;
            }
            else if (p == nullptr)
                p = edge.to;

            if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Return)
                ++ret;
            else if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Branch)
                ++brc;
            else if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Call)
                ++call;

            ++total;
        }

        return results;
    }

    std::vector<CFGAnalysisResult> analyzeOutDeg(const std::vector<EdgeDesc> &edges)
    {
        std::vector<CFGAnalysisResult> results;

        for (const EdgeDesc &edge: edges)
        {
            if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Call)
                results.emplace_back(CFGAnalyzeMode::OUT, edge.from, "Regular function call");
            else if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Syscall)
                results.emplace_back(CFGAnalyzeMode::OUT, edge.from, "System funtion call");
        }

        return results;
    }

    static std::vector<CFGAnalysisResult> mergeResults(
        std::vector<CFGAnalysisResult> &inDegResults,
        std::vector<CFGAnalysisResult> &outDegResults)
    {
        std::vector<CFGAnalysisResult> results;
        for (CFGAnalysisResult &result: inDegResults)
            results.emplace_back(std::move(result));
        for (CFGAnalysisResult &result: outDegResults)
            results.emplace_back(std::move(result));

        return results;
    }
}
