#include "Math.h"
#include <cmath>

#ifdef USE_MATH
#include "arithmetics.h"
#endif

namespace CustomMath
{
    double exp(double x)
    {
#ifdef USE_MATH
        return Inner::exp(x);
#else
        return std::exp(x);
#endif
    }

    double print_generated() {
#ifdef USE_MATH
        return Inner::print_generated();
#else
        return 0;
#endif
    }
}
