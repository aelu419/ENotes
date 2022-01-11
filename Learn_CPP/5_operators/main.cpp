#include <iostream>

int main() {
    int a {3}, b{4}, c;

    c = b = a;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    c += (a++, b++, 3);
    std::cout << c << std::endl;

    return 0;
}