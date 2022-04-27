#include <iostream>

struct Ad {
    int times{};
    float percentage{}; // the tutorial uses double instead of float
    float earning{};
};

Ad getAd() {
    Ad ad{};
    std::cin >> ad.times;
    std::cin >> ad.percentage;
    std::cin >> ad.earning;
    return ad;
}

void printAd(const Ad& ad) {
    std::cout << ad.times << " " << ad.percentage << " " << ad.earning << std::endl;
}

struct Fraction {
    int n{};
    int d{1};
};

Fraction mult(const Fraction& f1, const Fraction& f2) {
    return { f1.n * f2.n, f1.d * f2.d };
}

// similar get and print functions for fraction

int main () {

    // ISSUE: do not use local variables unless necessary
    // int t;
    // float p, e;
    // std::cin >> t;
    // std::cin >> p;
    // std::cin >> e;

    // Ad ad { t, p, e };
    Ad ad { getAd() };
    printAd(ad);

    return  0;
}