#ifndef __INCLUDED_CFG_ANALYSIS__
#define __INCLUDED_CFG_ANALYSIS__

#include <utility>
#include <vector>
#include <ostream>

#include <gtirb/IR.hpp>

#include "Result.hpp"

namespace stcfg
{
    enum class CFGAnalyzeMode { IN, OUT };

    inline std::ostream &operator <<(std::ostream &lhs, const CFGAnalyzeMode rhs)
    {
        switch (rhs)
        {
        case CFGAnalyzeMode::IN:
            lhs << "IN";
            break;

        case CFGAnalyzeMode::OUT:
            lhs << "OUT";
            break;
        }

        return lhs;
    }

    class CFGAnalysisResult final:
        public Result
    {
    private:
        CFGAnalyzeMode mode;
        const void *addr;
        char *reason;

    public:
        CFGAnalysisResult(const CFGAnalyzeMode mmode, const void *const maddr,
            const char *const mreason) noexcept(false)
        : mode(mmode), addr(maddr)
        {
            if (mreason == nullptr) reason = nullptr;
            else
            {
                reason = new char [strlen(mreason) + 1];
                strcpy(reason, mreason);
            }
        }

        CFGAnalysisResult(const CFGAnalysisResult &) = delete;

        CFGAnalysisResult(CFGAnalysisResult &&tar) noexcept
        : mode(tar.mode), addr(tar.addr), reason(tar.reason) { tar.reason = nullptr; }

        ~CFGAnalysisResult() override { delete [] reason; }

        CFGAnalysisResult &operator =(const CFGAnalysisResult &) = delete;

        CFGAnalysisResult &operator =(CFGAnalysisResult &&rhs) noexcept
        {
            if (this == &rhs) return *this;

            mode = rhs.mode;
            addr = rhs.addr;
            delete [] reason;
            reason = rhs.reason;

            rhs.reason = nullptr;
            return *this;
        }

    private:
        void print(std::ostream &out) const override
        {
            out << mode << "::" << addr << "::" << reason;
        }
    };

    struct EdgeDesc
    {
    public:
        const gtirb::UUID &from;
        const gtirb::UUID &to;
        gtirb::EdgeLabel label;

    public:
        EdgeDesc(const gtirb::UUID &mfrom, const gtirb::UUID &mto, gtirb::EdgeLabel mlabel)
        : from(mfrom), to(mto), label(std::move(mlabel)) {}

        // Lvalue reference used, copy constructor deleted to prevent accidental overwrite
        EdgeDesc &operator =(const EdgeDesc &rhs) = delete;

        // Same problem
        EdgeDesc &operator =(EdgeDesc &&rhs) = delete;
    };

    std::vector<CFGAnalysisResult> analyzeCFG(const gtirb::IR &ir);

    std::vector<EdgeDesc> getEdges(const gtirb::CFG &cfg);

    std::vector<CFGAnalysisResult> analyzeInDeg(const std::vector<EdgeDesc> &edges);
    std::vector<CFGAnalysisResult> analyzeOutDeg(const std::vector<EdgeDesc> &edges);
}

#endif
