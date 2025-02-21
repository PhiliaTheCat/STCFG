#include <iostream>
#include <vector>
#include <stdexcept>

#include <gtirb/IR.hpp>

#include "Init.hpp"
#include "CFGAnalysis.hpp"
#include "Result.hpp"

int main(const int argCnt, const char *argVals[])
{
    try
    {
        const gtirb::IR &ir = stcfg::init(argCnt, argVals);

        const auto CFGAnalysisResults = stcfg::analyzeCFG(ir);
        for (const auto &result: CFGAnalysisResults)
            std::cout << result << std::endl;

        return 0;
    }
    catch (const std::runtime_error &msg)
    {
        std::cerr << msg.what() << std::endl;
        return 1;
    }
}
