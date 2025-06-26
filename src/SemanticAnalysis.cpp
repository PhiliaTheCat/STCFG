#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>

#include <gtirb/gtirb.hpp>

#include "SemanticAnalysis.hpp"
#include "FunctionInfo.hpp"
#include "GlobalStatus.hpp"

std::vector<stcfg::FunctionInfo> stcfg::extractFunctionInfo(const gtirb::Section &sectionText) noexcept(false)
{
    if (!sectionText.getSize().has_value())
        throw std::runtime_error("Section \".text\" does not have a valid size!");
    global.textSize = sectionText.getSize().value();

    const auto heap = new std::uint8_t [global.textSize];
    extractText(sectionText, heap);

    std::vector<FunctionInfo> ret;
    findStarts(ret, heap);

    delete [] heap;

    return ret;
}

void stcfg::extractText(const gtirb::Section &sectionText, std::uint8_t *heap) noexcept(false)
{
    std::size_t i = 0;

    for (const auto &byteInterval: sectionText.byte_intervals())
    {
        const auto p = byteInterval.rawBytes<uint8_t>();
        memcpy(heap, p, byteInterval.getSize());

        heap += byteInterval.getSize();
        i += byteInterval.getSize();
    }

    if (i != sectionText.getSize().value())
        throw std::runtime_error("Failed loading section \".text\"!");
}

void stcfg::findStarts(std::vector<FunctionInfo> &infos, const std::uint8_t *heap) noexcept(false)
{
    int status = 0;
    for (std::size_t i = 0; i < global.textSize; ++i)
    {
        switch (status)
        {
        case 0:
            if (heap[i] == 0xf3) status = 1;
            break;

        case 1:
            if (heap[i] == 0x0f) status = 2;
            else status = 0;
            break;

        case 2:
            if (heap[i] == 0x1e) status = 3;
            else status = 0;
            break;

        case 3:
            if (heap[i] == 0xfa)
            {
                status = 0;
                infos.emplace_back(i - 3);
            }
            else
                status = 0;
            break;

        default:
            throw std::runtime_error("Unknown status!");
            break;
        }
    }
}
