#ifndef __INCLUDED_RESULT__
#define __INCLUDED_RESULT__

#include <ostream>

namespace stcfg
{
    class Result
    {
    public:
        virtual ~Result() = 0;

    private:
        virtual void print(std::ostream &out) const = 0;

        friend std::ostream &operator <<(std::ostream &lhs, const Result &rhs)
        {
            rhs.print(lhs);
            return lhs;
        }
    };
}

#endif
