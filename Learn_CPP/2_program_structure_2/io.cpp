#include "io.h"
#include <iostream>

int readNumber()
{
    int input;
    std::cin >> input;
    return input;
}

void writeAnswer(int answer)
{
    std::cout << "The answer is: " << answer << std::endl;
}