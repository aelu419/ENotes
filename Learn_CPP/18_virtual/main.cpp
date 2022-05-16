#include <iostream>
#include <string>
#include <vector>

class Point {
    private:
    int x, y, z;
    public:
    Point(int x, int y, int z): x{x}, y{y}, z{z} {}
    std::string description() const {
        return "P("+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+")";
    }
};

class Shape {
    public:
    virtual std::ostream& print(std::ostream& out) const = 0;
    friend std::ostream& operator<< (std::ostream& out, const Shape& s) {
        return s.print(out);
    }

    virtual ~Shape() = default;
};

class Triangle: public Shape {
    private:
    Point a;
    Point b;
    Point c;
    
    public:
    Triangle(const Point& a, const Point& b, const Point& c): a{a}, b{b}, c{c} {}
    virtual std::ostream& print(std::ostream& out) const override {
        return out << "Triangle " + a.description() + ", " + b.description() + ", " + c.description();
    }
};

class Circle: public Shape {
    private:
    Point center;
    int radius;
    public:
    Circle(const Point& c, const int r): center{c}, radius{r} {}
    virtual std::ostream& print(std::ostream& out) const override {
        return out << "Circle " + center.description() + ", " + std::to_string(radius);
    }
    int getRadius() { return radius; }
};

int getLargestRadius(const std::vector<Shape*>& v) {
    int max = 0;
    for (auto s : v) {
        if (auto c = dynamic_cast<Circle*>(s)) {
            if (int r = c->getRadius(); r > max) {
                max = r;
            }
        }
    }
    return max;
}

int main() {
    Circle c{ Point{1, 2, 3}, 7};
    Triangle t{Point{1, 2, 3}, Point{4, 5, 6}, Point{7, 8, 9}};
    std::cout << c << std::endl;
    std::cout << t << std::endl;

    std::vector<Shape*> v {
        new Circle{Point{1, 2, 3}, 7},
        new Triangle{Point{1, 2, 3}, Point{4, 5, 6}, Point{7, 8, 9}},
        new Circle{Point{4, 5, 6}, 3}
    };

    for (const auto* s : v) {
        std::cout << *s << std::endl;
    }

    std::cout << "The largest radius is " << getLargestRadius(v) << std::endl;

    for (const auto* s : v) {
        delete s;
    }
}