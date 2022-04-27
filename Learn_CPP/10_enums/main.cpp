#include <iostream>

enum class Animal {
    pig,
    chicken,
    goat,
    cat,
    dog,
    duck,
};

// don't forget to add constexpr when possible
constexpr std::string_view getAnimalName(Animal a) {
    switch (a) {
        case Animal::pig: return "pig";
        case Animal::chicken: return "chicken";
        case Animal::goat: return "goat";
        case Animal::cat: return "cat";
        case Animal::dog: return "dog";
        case Animal::duck: return "duck";
        default: return "???";
    }
}

void printNumberOfLegs(Animal a) {
    int nLegs{0};
    switch (a) {
        case Animal::pig:
        case Animal::goat:
        case Animal::cat:
        case Animal::dog:
            nLegs = 4;
            break;
        case Animal::chicken:
        case Animal::duck:
            nLegs = 2;
            break;
    }

    std::cout<<nLegs<<std::endl;
}

int main() {
    std::cout<<getAnimalName(Animal::cat);
    printNumberOfLegs(Animal::cat);
    std::cout<<getAnimalName(Animal::chicken);
    printNumberOfLegs(Animal::chicken);

    return 0;
}