#include <iostream>

double getDouble();
void Operate(double a, double b);

int main() {
    double a {getDouble()};
    double b {getDouble()};
    Operate(a, b);
    return 0;
}

double getDouble() {
    double x;
    std::cout << "Enter a double value: ";
    std::cin >> x;
    return x;
}

void Operate(double a, double b) {
    char c;
    std::cout << "Enter one of the following: +, -, *, or /: ";
    std::cin >> c;
    if (c == '+') std::cout << a + b << std::endl;
    else if (c == '-') std::cout << a - b << std::endl;
    else if (c == '*') std::cout << a * b << std::endl;
    else if (c == '/') std::cout << a / b << std::endl;
}