#include <iostream>

class Point3d {
    int m_x, m_y, m_z;
    public:
    void setValues(int x, int y, int z) {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    void print() {
        std::cout<<"<"<<m_x<<", "<<m_y<<", "<<m_z<<">";
    }

    bool isEqual(Point3d p) {
        return p.m_x == m_x && p.m_y == m_y && p.m_z == m_z;
    }
};

int main() {
    Point3d point;
    point.setValues(1, 2, 3);
    point.print();
    std::cout << "\n";
    return 0;
}