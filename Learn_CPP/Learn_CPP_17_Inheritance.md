- [General Syntax](#general-syntax)
- [Constructors of Inherited Classes](#constructors-of-inherited-classes)
- [Access Specifiers for Inherited Classes](#access-specifiers-for-inherited-classes)
- [Inherited Functions](#inherited-functions)
  - [Polynmorphism](#polynmorphism)
  - [Visibility Change](#visibility-change)
- [Multiple Inheritance](#multiple-inheritance)
  - [Diamond Problem](#diamond-problem)

# General Syntax
- generally used for "is-a" relationships
  ```c++
  class DerivedClass : public SuperClass {
      // ...
  }
  ```

# Constructors of Inherited Classes
- by default, a drived class instance calls the base class's default constructor first
- base class members cannot be initialized in initializer list, but could be accessed in the body of constructors
  - this is to prevent the possibility of a `const` variable initialized twice (the initializer list of the derived class is executed *after* the constructor of the base class finishes)
  - the derived class can call base class constructors in the initializer list
    - *ex.* `Derived(int id): Base{id} { ... }`
  - the derived class can only call its immediate parent's constructor, which *could* in turn call another base class's constructor
- inherited destructors are called in the oposite order with the most derived class called first

# Access Specifiers for Inherited Classes
- `private` prevents derived classes from accessing the base class's members
- `protected` allows for derived classes to access
- acces modifiers modifying the base class in the derived class's declaration
  - `public`: the highest level of access is `public`, everythingi below `public` stays at the same visibility when referenced from the derived class
  - `protected`: `public` base class members will become `protected` when accessed from child class objects
    - this is rarely used because it's essentially the same with `private` inheritance
  - `private`: all members of the base class is considered private when trying to access them from the derived class
  - note that, *within* the derived class the accessibilities remain the same, just that others outside the inheritance have visibilities modified

# Inherited Functions

## Polynmorphism
- implementations of the same method is prioritized for the derived class comparing to the base class
  - this allows for polymorphism for different derived classes
- derived implementations can call `BaseClassName::functionNames(functionPararms)` when overriding, so that the new implementation is additional instead of redoing
- to force call base implementation, use `static_cast<BaseClass>` on the object first, and then call the function
## Visibility Change
- note that derived classes can always just add a getter to the inherited `protected` variable, so prefer `private` over `protected` unless needed
  - another way to publicize `protected` members is to do `using BaseClassName(param_t params);` under the `public` section of the derived class
  - ``` c++
    class Base {
        // ...
        protected:
        void doThing();
    }

    class Derived: public Base {
        // ...
        public:
        using Base::doThing;
    }
     ```
  - the same could be done to hide a base class fucntion by `using` under `private`
  - alternatively, do `= delete;` for the function
    ``` c++
    class Derived: public Base {
        // ...
        void doThing() = delete;
    }
    ```

# Multiple Inheritance
- `class DerivedClass: public BaseA, public BaseB {}`
- problems with methods with the same signature
  - the caller can resolve the clash using `derivedObject.BaseClassName::functionName()`
## Diamond Problem
- `B` and `C` inherits `A`
- `D` inherits `B` and `C`
- to avoid complexities, preferr single inheritance over multiple inheritance