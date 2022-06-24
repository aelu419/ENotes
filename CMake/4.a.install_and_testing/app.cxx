#include <iostream>
#include <Config.h>
#include <string>
#ifdef USE_MATH
#include <Math.h>
#endif

int main(int argc, char *argv[]) {
    std::cout<<"1 + "<< argv[1] << " = "<<
        #ifdef USE_MATH
        add(std::stod(argv[1]), 1)
        #else
        0
        #endif
        <<std::endl;
}