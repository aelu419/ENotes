#include <iostream>
#include <algorithm>
#include <limits>
#include <string>

int main() {
    int size;

    std::cout<<"How many names would you like to enter?";
    std::cin>>size;

    std::string* names = { new std::string[size]{} };

    std::cout<<size<<std::endl;

    for (int i = 0; i < size; i++) {
        std::cout<<"Enter name #"<<i+1<<":";
        std::getline(std::cin >> std::ws, names[i]); // remember the std::ws for whitespace
    }

    std::sort(names, names + size);

    std::cout<<"Here is your sorted list\n";
    for (int i = 0; i < size; i++) {
        std::cout<<"Name #"<<i<<" "<<names[i]<<"\n";
    }

    delete[] names;
    names = nullptr;

    return 0;
}