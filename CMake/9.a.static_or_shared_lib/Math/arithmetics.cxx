#include "Math.h"
#include <cmath>
#include "Generated.h"

namespace CustomMath {
    // note the addition of the Inner namespace
    namespace Inner {
        double exp(double a) {
            #ifdef EXP_AVAILABLE
            return std::exp(a);
            #else
            return 0;
            #endif
        }

        double print_generated() { return GENERATED_CONSTANT; }
    }

}