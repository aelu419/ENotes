#include <string>

class MyClass {
private:
    std::string name;
public:
    bool settingA{};
    bool settingB{};
    bool settingC{};

    MyClass(std::string name) : name{name} {};

    friend class MyClassBuilder; // for exposure
    static MyClassBuilder create(std::string name) {
        return MyClassBuilder{name};
    }
};

class MyClassBuilder {
private:
    MyClass myClass;
public:
    MyClassBuilder(std::string name) : myClass{name} {}

    // type conversion back to the custom type
    operator MyClass() const { return std::move(myClass); }

    MyClassBuilder& setA() {
        myClass.settingA = true;
        return *this; // for chaining
    }

    MyClassBuilder& setB() {
        myClass.settingB = true;
        return *this; // for chaining
    }

    MyClassBuilder& setC() {
        myClass.settingC = true;
        return *this; // for chaining
    }
};

int main() {
    MyClass m = MyClass::create("bob").setA().setC();
}