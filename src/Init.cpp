#include <cstdint>

#include <fstream>

#include <string>

#include <stdexcept>

#include <gtirb/gtirb.hpp>

#include "Init.hpp"
#include "GlobalStatus.hpp"

static gtirb::Context context;

const gtirb::IR &stcfg::init(const char filename[]) noexcept(false)
{
    std::ifstream fin(filename);
    if (fin.fail())
        throw std::runtime_error("Failed opening file \"" + std::string(filename) + "\"!");

    auto t = gtirb::IR::load(context, fin);
    if (!t)
        throw std::runtime_error("Failed loading from IR file \"" + std::string(filename) + "\"!");

    return *t.get();
}

const gtirb::Section &stcfg::findSectionText(const gtirb::IR &ir) noexcept(false)
{
    int i = 0;
    const gtirb::Section *ret = nullptr;
    for (const auto &section: ir.findSections(".text"))
    {
        i += 1;
        ret = &section;
        if (i != 1)
            throw std::runtime_error("More than one section \".text\"!");
    }

    if (ret == nullptr)
        throw std::runtime_error("No section \".text\"!");

    if (!ret->getAddress().has_value())
        throw std::runtime_error("Invalid address of section \".text\"!");
    global.textBase = static_cast<std::uint64_t>(ret->getAddress().value());

    return *ret;
}
