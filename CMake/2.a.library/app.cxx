#include <iostream>
#include <Config.h>
#ifdef USE_MATH
#include <Math.h>
#endif

int main() {
    std::cout<<"1 + 2 = "<<
        #ifdef USE_MATH
        add(1, 2)
        #else
        0
        #endif
        <<std::endl;
}