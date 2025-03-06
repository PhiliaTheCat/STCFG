#include <vector>
#include <algorithm>

#include <gtirb/CFG.hpp>
#include <gtirb/Node.hpp>

#include "CFGAnalysis.hpp"

namespace stcfg
{
    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::IR &ir)
    {
        const gtirb::CFG &cfg = ir.getCFG();
        std::vector<EdgeDesc> edges = getEdges(cfg);

        std::sort(edges.begin(), edges.end(),
            [](const EdgeDesc &a, const EdgeDesc &b) -> bool { return a.to < b.to; });
        std::vector<CFGAnalysisResult> inDegResults = analyzeDeg(edges, CFGAnalyzeMode::IN);

        return inDegResults;
    }

    std::vector<EdgeDesc> getEdges(const gtirb::CFG &cfg)
    {
        std::vector<EdgeDesc> edges;

        for (const auto &node: nodes(cfg))
        {
            for (const auto &pair: gtirb::cfgSuccessors(cfg, &node))
                edges.emplace_back(&node, pair.first, pair.second);
        }

        return edges;
    }

    std::vector<CFGAnalysisResult> analyzeDeg(const std::vector<EdgeDesc> &edges, const CFGAnalyzeMode mode)
    {
        std::vector<CFGAnalysisResult> results;

        const void *p = nullptr;
        size_t total = 0;
        size_t ret = 0;
        size_t brc = 0;
        size_t call = 0;

        for (const auto &edge: edges)
        {
            if (p != edge.to && mode == CFGAnalyzeMode::IN && p != nullptr)
            {
                if (total >= 5)
                {
                    if (ret >= total / 2)
                        results.emplace_back(p, total, "Returned to here multiple times, "
                            "could be an exception handler!");
                    if (brc >= total / 2)
                        results.emplace_back(p, total, "Branched to here multiple times, "
                            "could be the end of a switch statement, of part of a loop!");
                    if (call >= total / 2)
                        results.emplace_back(p, total, "Called multiple times, "
                            "could be a hot function!");
                }
                p = nullptr;
                total = 0;
                ret = 0;
                brc = 0;
                call = 0;
            }
            else if (p == nullptr)
            {
                switch (mode)
                {
                case CFGAnalyzeMode::IN:
                    p = edge.to;
                    break;
                case CFGAnalyzeMode::OUT:
                    p = edge.from;
                    break;
                }
            }

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
}
