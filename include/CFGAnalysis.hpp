#ifndef __INCLUDED_CFG_ANALYSIS__
#define __INCLUDED_CFG_ANALYSIS__

#include <utility>
#include <vector>
#include <ostream>

#include <gtirb/IR.hpp>

#include "Result.hpp"

namespace stcfg
{
    class CFGAnalysisResult final:
        public Result
    {
    private:
        const void *addr;
        size_t refTimes;
        char *reason;

    public:
        CFGAnalysisResult(const void *const maddr, const size_t mrefTimes, const char *const mreason) noexcept(false)
        : addr(maddr), refTimes(mrefTimes)
        {
            if (mreason == nullptr) reason = nullptr;
            else
            {
                reason = new char [strlen(mreason) + 1];
                strcpy(reason, mreason);
            }
        }

        CFGAnalysisResult(CFGAnalysisResult &&tar) noexcept
        : addr(tar.addr), refTimes(tar.refTimes), reason(tar.reason) { tar.reason = nullptr; }

        ~CFGAnalysisResult() override { delete [] reason; }

        CFGAnalysisResult &operator =(CFGAnalysisResult &&rhs) noexcept
        {
            if (this == &rhs) return *this;

            addr = rhs.addr;
            refTimes = rhs.refTimes;
            delete [] reason;
            reason = rhs.reason;

            rhs.reason = nullptr;
            return *this;
        }

    private:
        void print(std::ostream &out) const override
        {
            out << addr << "::" << reason << "::" << refTimes;
        }
    };

    struct EdgeDesc
    {
    public:
        const gtirb::CfgNode *from;
        const gtirb::CfgNode *to;
        gtirb::EdgeLabel label;

    public:
        EdgeDesc(const gtirb::CfgNode *const mfrom, const gtirb::CfgNode *const mto, gtirb::EdgeLabel mlabel)
        : from(mfrom), to(mto), label(std::move(mlabel)) {}

        friend std::ostream &operator <<(std::ostream &lhs, const EdgeDesc &rhs)
        {
            return lhs << rhs.from << " -> " << rhs.to << ": " << rhs.label;
        }
    };

    enum class CFGAnalyzeMode { IN, OUT };

    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::IR &ir);

    std::vector<EdgeDesc> getEdges(const gtirb::CFG &cfg);

    std::vector<CFGAnalysisResult> analyzeDeg(const std::vector<EdgeDesc> &edges, CFGAnalyzeMode mode);
}

#endif
