#include <iostream>
#include <vector>
#include <stdexcept>

#include <gtirb/IR.hpp>
#include <gtirb/CFG.hpp>
#include <gtirb/Symbol.hpp>

#include "Init.hpp"
#include "CFGAnalysis.hpp"
#include "Result.hpp"

int main(const int argCnt, const char *argVals[])
{
    try
    {
        const gtirb::IR &ir = stcfg::init(argCnt, argVals);
        const gtirb::CFG &cfg = ir.getCFG();
        const auto symbols = ir.symbols();

        const auto &CFGresults = stcfg::analyzeCFG(cfg, symbols);
        for (const auto &result: CFGresults)
            std::cout << result << std::endl;

        return 0;
    }
    catch (const std::runtime_error &msg)
    {
        std::cerr << msg.what() << std::endl;
        return 1;
    }
}
