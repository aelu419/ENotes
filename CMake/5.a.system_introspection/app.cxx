#include <iostream>
#include <Config.h>
#include <string>
#ifdef USE_MATH
#include <Math.h>
#endif

int main(int argc, char *argv[]) {
    std::cout<<"e^"<< argv[1] << " = " <<
        #ifdef USE_MATH
        CustomMath::exp(std::stod(argv[1]))
        #else
        0
        #endif
        <<std::endl;
}