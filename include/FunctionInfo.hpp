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
        std::uint64_t start;

    public:
        FunctionInfo(std::uint64_t myStart): start(myStart) {}

        friend std::ostream &operator <<(std::ostream &lhs, const FunctionInfo &rhs)
        {
            using namespace std;
            lhs << "Start point: 0x"
                << hex << setw(16) << setfill('0')
                << rhs.start + global.textBase;

            return lhs;
        }
    };
}
