#include <iostream>
#include <vector>

#include <gtirb/gtirb.hpp>

#include "Init.hpp"
#include "SemanticAnalysis.hpp"
#include "FunctionInfo.hpp"
#include "GlobalStatus.hpp"

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

        const gtirb::Section &sectionText = stcfg::findSectionText(ir);

        std::vector<stcfg::FunctionInfo> infos = stcfg::extractFunctionInfo(sectionText);
        for (const auto &info: infos)
            std::cout << info << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Fetal: " << e.what() << std::endl;
        return 1;
    }
}
