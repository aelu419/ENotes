#include <iostream>
#include <iterator>

template <typename T>
T* find(T* start, T* end, T val) {
    for (T* i = start; i < end; i++) {
        if (*i == val) return i;
    }
    return nullptr;
}

int main()
{
    int arr[]{ 2, 5, 4, 10, 8, 20, 16, 40 };

    // Search for the first element with value 20.
    int* found{ find<int>(std::begin(arr), std::end(arr), 20) };

    // If an element with value 20 was found, print it.
    if (found != std::end(arr))
    {
        std::cout << *found << '\n';
    }

    return 0;
}