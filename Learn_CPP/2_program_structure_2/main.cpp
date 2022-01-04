#include "io.h"
#include <iostream>

// this is the second version of quiz 2.x, using header files instead of single

int main()
{
    // int a, b;
    // a = readNumber();
    // b = readNumber();

    // corr: a better answer would be:
    int a { readNumber() };
    int b { readNumber() };

    writeAnswer(a + b);
    return 0;
}