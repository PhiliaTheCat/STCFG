#pragma once

#include <cstdint>

#include <vector>

#include <gtirb/gtirb.hpp>

#include "FunctionInfo.hpp"

namespace stcfg
{
    [[nodiscard]] std::vector<FunctionInfo> extractFunctionInfo(const gtirb::Section &sectionText) noexcept(false);

    void extractText(const gtirb::Section &sectionText, std::uint8_t *heap) noexcept(false);

    void findStarts(std::vector<FunctionInfo> &infos, const std::uint8_t *heap) noexcept(false);
}
