#include <iostream>

void fall (const double, int);

int main() {
    double h0;
    std::cout << "Enter the height of the tower in meters: ";
    std::cin >> h0;
    fall(h0, 0);
    return 0;
}

void fall(const double h0, int t) {
    constexpr double g = 9.8;
    double dh = g * t * t / 2.0;
    std::cout << "At " << t << " seconds, ";
    if (dh < h0) {
        std::cout << "the ball is at height: " << h0-dh << " meters" 
            << std::endl;
        fall(h0, t + 1);
    } else {
        std::cout << "the ball is on the ground." << std::endl;
    }
}