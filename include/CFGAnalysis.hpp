#pragma once

#include <cstdint>

#include <vector>

#include <gtirb/gtirb.hpp>

namespace stcfg
{
    struct CFGInfo
    {
    public:
        std::uint64_t from;
        std::uint64_t to;
        gtirb::EdgeLabel label;

    public:
        CFGInfo(const std::uint64_t myFrom, const std::uint64_t myTo, gtirb::EdgeLabel myLabel)
        : from(myFrom), to(myTo), label(std::move(myLabel))
        {}
    };

    [[nodiscard]] std::vector<CFGInfo> extractCFGInfo(const gtirb::CFG &cfg) noexcept(false);
}
