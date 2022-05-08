#include <iostream>
#include <string>
#include <array>
#include <algorithm>

struct Student {
    std::string_view name;
    int score;
};

int main() {
    std::array<Student, 5> arr {
        {
            { "albert", 3 },
            { "ben", 5 },
            { "christine", 2 }
        }
    };

    auto max = std::max_element(arr.begin(), arr.end(), [](Student a, Student b) -> bool {
        return a.score < b.score;
    });
    
    std::cout<<max->name<<std::endl;
}