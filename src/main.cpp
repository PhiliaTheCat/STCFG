#include <iostream>
#include <vector>
#include <stdexcept>

#include <gtirb/gtirb.hpp>

#include "Init.hpp"
#include "SemanticAnalysis.hpp"
#include "FunctionInfo.hpp"
#include "CFGAnalysis.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Invalid invocation!\n"
            "Usage: stcfg <gtirb file>" << std::endl;

        return 1;
    }

    try
    {
        const gtirb::IR &ir = stcfg::init(argv[1]);

        const gtirb::CFG &cfg = stcfg::getCFG(ir);
        const gtirb::Section &sectionText = stcfg::findSectionText(ir);

        // Already sorted by address.
        std::vector<stcfg::FunctionInfo> funcInfos = stcfg::extractFunctionInfo(sectionText);
        std::vector<stcfg::CFGInfo> cfgInfos = stcfg::extractCFGInfo(cfg);

        std::cout << cfgInfos.size() << std::endl;

        return 0;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Fetal: " << e.what() << std::endl;
        return 1;
    }
}
