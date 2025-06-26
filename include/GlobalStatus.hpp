#pragma once

#include <cstdint>

namespace stcfg
{
    struct GlobalStatus
    {
    public:
        std::uint64_t textBase;
        std::uint64_t textSize;

    private:
        GlobalStatus() = default;

    public:
        GlobalStatus(const GlobalStatus &) = delete;
        GlobalStatus(GlobalStatus &&) = delete;

        GlobalStatus &operator=(const GlobalStatus &) = delete;
        GlobalStatus &operator=(GlobalStatus &&) = delete;

        static GlobalStatus &getInstance()
        {
            static GlobalStatus instance;

            return instance;
        }
    };

    inline GlobalStatus &global = GlobalStatus::getInstance();
}
