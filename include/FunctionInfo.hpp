#pragma once

#include <cstdint>
#include <ostream>
#include <iomanip>

#include "GlobalStatus.hpp"

namespace stcfg
{
    struct FunctionInfo
    {
    public:
        uint64_t start;

    public:
        FunctionInfo() = default;

        FunctionInfo(const uint64_t mstart)
        : start(mstart) {}

        friend std::ostream &operator <<(std::ostream &lhs, const FunctionInfo &rhs)
        {
            return lhs << "0x" << std::hex << std::setfill('0') << std::setw(16)
                << rhs.start + global.textBase;
        }
    };
}
