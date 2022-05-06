#include <iostream>
#include<array>
#include<vector>
#include<numeric>
#include<string>

/**
 * Q1
 */
enum items {
    health_potion,
    torch,
    arrow,
    max_types_of_items
};

// the tutorial defines the following as abbreviation
// using inventory_t = std::array<int, items::max_types_of_items;

int countTotalItems(const std::array<int, max_types_of_items>& arr) {
    return std::reduce(arr.begin(), arr.end());
}

/**
 * Q2
 */
struct student {
    std::string firstName;
    int grade;
};

/**
 * Q3
 */
template <typename T>
void swap(T* a, T* b) {
    T temp{b};
    *b = *a;
    *a = temp;
}

// the same can be written with reference which is safer
template <typename T>
void swap_ref(T& a, T& b) {
    T temp{b};
    b = a;
    a = temp;
}

/**
 * Q4
 */
void print_c_str(const char* str) {
    for(auto p = str; *p != '\0'; p++) {
        std::cout<<*p;
    }
    std::cout<<std::endl;
}

int main() {
    // Q1
    std::array<int, max_types_of_items> player{ 2, 5, 10 };
    std::cout << player[items::torch] << std::endl;

    // Q2
    int nStudents{};
    std::cin>>nStudents;
    std::vector<student> students(nStudents);
    for(int i = 0; i < nStudents; i++) {
        std::string name{};
        int grade{};
        std::getline(std::cin >> std::ws, name);
        std::cin>>std::ws>>grade;
        students[i] = student{ name, grade };
    }
    for(auto i : students) {
        std::cout<<i.firstName<<" got a grade of "<<i.grade<<std::endl;
    }
}