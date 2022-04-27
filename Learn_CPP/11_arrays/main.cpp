#include <iostream>
#include <iterator>

int main() {

    int target{-1};
    while (target < 1 || target > 9) {
        std::cout<<"number between 1 and 9: ";
        std::cin>>target;
        if (std::cin.fail())
            std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsizzez>::max(), '\n');
    }

    constexpr int array[]{ 4, 6, 7, 3, 8, 2, 1, 9, 5 };

    int idx{-1};
    for (int i{}; i < static_cast<int>(std::size(array)); i++) {
        std::cout<<array[i]<<" ";
        if (array[i] == target)
            idx = i;
    }
    std::cout<<std::endl;

    if (idx == -1) std::cout<<"not found"<<std::endl;
    else std::cout<<idx<<std::endl;

    return 0;
}