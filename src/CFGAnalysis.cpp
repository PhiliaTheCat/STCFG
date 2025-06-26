#include <cstdint>
#include <vector>
#include <stdexcept>
#include <set>

#include <gtirb/gtirb.hpp>

#include "CFGAnalysis.hpp"
#include "GlobalStatus.hpp"

[[nodiscard]] std::vector<stcfg::CFGInfo> stcfg::extractCFGInfo(const gtirb::CFG &cfg) noexcept(false)
{
    std::vector<CFGInfo> result;
    std::set<gtirb::UUID> check;

    for (const auto &codeBlock: blocks(cfg))
        check.insert(codeBlock.getUUID());

    return result;
}
