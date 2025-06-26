#include <fstream>
#include <stdexcept>

#include <gtirb/gtirb.hpp>

#include "Init.hpp"
#include "GlobalStatus.hpp"

static gtirb::Context context;

const gtirb::IR &stcfg::init(const char filename[]) noexcept(false)
{
    using namespace std;

    ifstream fin(filename);
    if (fin.fail())
        throw runtime_error("Failed opening file \"" + string(filename) + "\"!");

    auto t = gtirb::IR::load(context, fin);
    if (!t)
        throw runtime_error("Failed loading from IR file \"" + string(filename) + "\"!");

    return *t.get();
}

const gtirb::Section &stcfg::findSectionText(const gtirb::IR &ir) noexcept(false)
{
    using namespace std;

    int i = 0;
    const gtirb::Section *ret = nullptr;
    for (const auto &section: ir.findSections(".text"))
    {
        i += 1;
        ret = &section;
        if (i != 1)
            throw runtime_error("More than one section \".text\"!");
    }

    if (ret == nullptr)
        throw runtime_error("No section \".text\"!");

    if (!ret->getAddress().has_value())
        throw runtime_error("Invalid address of section \".text\"!");
    global.textBase = static_cast<uint64_t>(ret->getAddress().value());

    return *ret;
}
