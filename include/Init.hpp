#pragma once

#include <gtirb/gtirb.hpp>

namespace stcfg
{
    [[nodiscard]] const gtirb::IR &init(const char filename[]) noexcept(false);

    [[nodiscard]] const gtirb::Section &findSectionText(const gtirb::IR &ir) noexcept(false);

    [[nodiscard]] inline const gtirb::CFG &getCFG(const gtirb::IR &ir) { return ir.getCFG(); }
}
