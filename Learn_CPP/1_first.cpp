#include <iostream> 

int main() {
    // value initialization
    int x; 
    // int y(); // not allowed
    // int z{}; // not allwed
    std::cout << x; // not initialized to 0!

    int x { 5 };
    std::cout << "Hello world" << x;

    return 0;
}