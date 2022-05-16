#include <iostream>
#include <string>

class Fruit {
    private:
    std::string name;
    std::string color;

    public:
    Fruit(const std::string& name, const std::string& color) : 
        name{ name }, color{ color } { }

    const std::string& getName() { return name; }
    const std::string& getColor() { return color; }
};

class Apple : public Fruit {
    protected:
    Apple(const std::string& name, const std::string& color) : Fruit{ name, color } {}
    public:
    Apple(const std::string& color) : Fruit { "Apple", color}{}
};

class GrannySmith : public Apple {
    public:
    GrannySmith() : Apple{"Granny Smith Apple", "red"} {

    }
};

class Banana : public Fruit {
    public:
    Banana() : Fruit { "Banana", "Yellow"} {}
};

int main()
{
	Apple a{ "red" };
	Banana b;
	GrannySmith c;

	std::cout << "My " << a.getName() << " is " << a.getColor() << ".\n";
	std::cout << "My " << b.getName() << " is " << b.getColor() << ".\n";
	std::cout << "My " << c.getName() << " is " << c.getColor() << ".\n";

	return 0;
}