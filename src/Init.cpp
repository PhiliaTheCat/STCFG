#include <fstream>
#include <stdexcept>

#include <gtirb/Context.hpp>
#include <gtirb/IR.hpp>

#include "Init.hpp"

static gtirb::Context context;

namespace stcfg
{
    const gtirb::IR &init(const int &argCnt, const char *argVal[]) noexcept(false)
    {
        if (argCnt != 2)
            throw std::runtime_error("Invalid invocation!\n"
                "Usage: stcfg <gtirb file>");

        std::ifstream fin(argVal[1], std::ios::binary | std::ios::in);
        if (fin.fail())
            throw std::runtime_error("Specified file unavailable!");

        const gtirb::IR &ret = **gtirb::IR::load(context, fin);

        fin.close();
        return ret;
    }
}
