#define GTIRB_WRAP_UTILS_IN_NAMESPACE

#include <cstdint>
#include <vector>
#include <stdexcept>

#include <gtirb/gtirb.hpp>

#include "CFGAnalysis.hpp"
#include "GlobalStatus.hpp"

[[nodiscard]] std::vector<stcfg::CFGInfo> stcfg::extractCFGInfo(const gtirb::CFG &cfg) noexcept(false)
{
    std::vector<CFGInfo> result;

    for (const auto &codeBlock: blocks(cfg))
    {
        const auto &t1 = codeBlock.getAddress();
        if (!t1.has_value())
            throw std::runtime_error("Existing codeblock without a valid address!");
        auto from = static_cast<std::uint64_t>(t1.value());

        for (const auto &pair: cfgSuccessors(cfg, &codeBlock))
        {
            const auto target = pair.first;
            const auto &label = pair.second;

            const auto actual = gtirb::dyn_cast<const gtirb::CodeBlock>(target);
            if (actual == nullptr)
                continue;

            const auto &t2 = actual->getAddress();
            if (!t2.has_value())
                throw std::runtime_error("Existing codeblock without a valid address!");
            auto to = static_cast<std::uint64_t>(t2.value());

            result.emplace_back(from, to, label);
        }
    }

    return result;
}
