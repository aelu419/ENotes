#include <iostream>
#include <Config.h>
#include <string>
#include <Math.h>

int main(int argc, char *argv[])
{
    std::cout << "e^" << argv[1] << " = " << CustomMath::exp(std::stod(argv[1])) << std::endl;
    std::cout << "Generated constant is " << CustomMath::print_generated() << std::endl;
}