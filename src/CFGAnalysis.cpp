#include <vector>

#include <gtirb/CFG.hpp>

#include "CFGAnalysis.hpp"

namespace stcfg
{
    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::CFG &cfg,
        const boost::iterator_range<gtirb::IR::const_symbol_iterator> &symbols)
    {
        std::vector<CFGAnalysisResult> results;

        const auto & nodes = gtirb::nodes(cfg);
        for (const auto &node: nodes)
        {
            cfgSuccessors(cfg, &node);
        }

        return results;
    }
}
