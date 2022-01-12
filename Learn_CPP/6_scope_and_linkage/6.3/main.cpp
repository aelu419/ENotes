#include <iostream>

int main() {
    int a{}, b{}; // a and b created
    std::cin>>a;
    std::cin>>b;

    if (a > b) { // swap if order incorrect
        std::cout << "swapping values" << std::endl;
        int t = a;
        a = b;
        b = t;
    }

    std::cout<<a<<" "<<b << std::endl;

    return 0;
} // a and b dies