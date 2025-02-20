#ifndef __INCLUDED_CFG_ANALYSIS__
#define __INCLUDED_CFG_ANALYSIS__

#include <vector>
#include <ostream>

#include <gtirb/CFG.hpp>
#include <gtirb/IR.hpp>
#include <gtirb/Symbol.hpp>

#include "Result.hpp"

namespace stcfg
{
    class CFGAnalysisResult final:
        public Result
    {
    private:
        char *name;
        size_t refTimes;
        void *addr;

    public:
        CFGAnalysisResult(const char mname[], const size_t mrefTimes, void *maddr) noexcept(false)
        : name(new char [strlen(mname) + 1] {}),
        refTimes(mrefTimes), addr(maddr) { strcpy(name, mname); }

        ~CFGAnalysisResult() override { delete [] name; }

    private:
        void print(std::ostream &out) const override
        {
            out << name << "@" << addr
                << "::" << refTimes;
        }
    };

    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::CFG &cfg,
        const boost::iterator_range<gtirb::IR::const_symbol_iterator> &symbols);
}

#endif
