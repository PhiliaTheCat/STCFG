#include <vector>
#include <algorithm>

#include <gtirb/CFG.hpp>
#include <gtirb/Node.hpp>

#include "CFGAnalysis.hpp"

namespace stcfg
{
    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::IR &ir)
    {
        std::vector<CFGAnalysisResult> results;
        const gtirb::CFG &cfg = ir.getCFG();
        std::vector<EdgeDesc> edges = getEdges(cfg);

        std::sort(edges.begin(), edges.end(),
            [](const EdgeDesc &a, const EdgeDesc &b) -> bool { return a.to < b.to; });
        analyzeInDeg(results, edges);

        return results;
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

    void analyzeInDeg(std::vector<CFGAnalysisResult> &results, const std::vector<EdgeDesc> &edges)
    {
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
                p = edge.to;

            if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Return)
                ++ret;
            else if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Branch)
                ++brc;
            else if (get<gtirb::EdgeType>(edge.label.value()) == gtirb::EdgeType::Call)
                ++call;

            ++total;
        }
    }
}
