#include <iostream>
#include <string>

template<typename T>
class Pair1 {
    private:
    T m_a, m_b;
    public:
    Pair1(const T& a, const T& b): m_a{a}, m_b{b} {}

    const T& first() const { return m_a; }
    const T& second() const { return m_b; }
};

template <typename T, typename S>
class Pair {
    private:
    T m_a;
    S m_b;
    public:
    Pair(const T& a, const S& b): m_a{a}, m_b{b} {}

    const T& first() const { return m_a; }
    const S& second() const { return m_b; }
};

template<typename S>
class SVPair : public Pair<std::string, S> {
    public:
    SVPair(const std::string& key, const S& val): Pair<std::string, S>{ key, val } {}
};

int main() {
    Pair1<int> p1 { 5, 8 };
    Pair1<double> p2 { 2.3, 4.5 };

    std::cout << "Pair: " << p1.first() << ' ' << p1.second() << std::endl;
    std::cout << "Pair: " << p2.first() << ' ' << p2.second() << std::endl;

    Pair<int, double> p3 { 5, 6.7 };
    const Pair<double, int> p4{ 2.3, 4 };

    std::cout << "Pair: " << p3.first() << ' ' << p3.second() << std::endl;
    std::cout << "Pair: " << p4.first() << ' ' << p4.second() << std::endl;

    SVPair<int> svp { "Hello", 5 };
    std::cout << "Pair: " << svp.first() << ' ' << svp.second() << std::endl;
}