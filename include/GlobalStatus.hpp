#pragma once

#include <cstdint>

namespace stcfg
{
    struct GlobalStatus
    {
    public:
        uint64_t textBase;
        uint64_t textSize;

        GlobalStatus() = default;
    };

    inline GlobalStatus global;
}
