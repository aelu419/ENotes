#include <Math.h>
#include <cmath>

namespace CustomMath {
    double add(double a, double b) { return a + b; }
    double sub(double a, double b) { return a - b; }
    double mult(double a, double b) { return a * b; }
    double div(double a, double b) { return a / b; }
    double exp(double a) {
        #ifdef EXP_AVAILABLE
        return std::exp(a);
        #else
        return 0;
        #endif
    }
}