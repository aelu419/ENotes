- [Pointers for Inherited Classes](#pointers-for-inherited-classes)
- [Virtual Member Functions](#virtual-member-functions)
  - [`Final`](#final)
  - [Covariant Return Types](#covariant-return-types)
- [Virtual Destructors](#virtual-destructors)
- [Easy/Late Binding](#easylate-binding)
  - [Virtual Table](#virtual-table)
- [Abstract Class](#abstract-class)
  - [Interface Class](#interface-class)
- [Virtual Base Class](#virtual-base-class)
- [Object Slicing](#object-slicing)
- [Dynamic Casting](#dynamic-casting)
- [Output with Operator<<](#output-with-operator)

# Pointers for Inherited Classes
- derived classes can be pointed to by a base class pointer
  - the base class pointer can only view base class members
  - for this reason, `sizeof` will return the size of the base class object within the derived class object
- common use case is to
  - reduce number of (non-template) functions to handle different derivatives of the same base
  - enable arrays that are based on the base class but contain a heterogeneous group of derived class objects
- note that when accessing the base class pointer, there is no polymorphism

# Virtual Member Functions
- virtual functions resolve to the *most* derived implementation instead of the base implementation
  - this enables polymorphism for derived classes
- use a `virtual` keyword on the base class implementation
  - the derived class does not need to write `virtual`
    - as long as it is an override, and that the accessed implementation is `virtual` the implementation resolution would work
    - *ex.*
      ```c++
      class A {
          // ..
          virtual void foo();
      };
      class B: public A {
          // ..
          void foo();
      };
      class C: public B {
          void foo();
      };

      // ...
      A a{};
      B b{};
      C c{};

      A& cAsA{ c };
      cAsA.foo(); // calls c.foo, because A allows function resolution on foo

      B& cAsB{ c };
      cAsB.foo(); // calls b.foo, because B does not allow function resolution on foo
      ```
  - optionally, ad a `override` after the parameter list
    - `virtual void f(int) override;`
- because of the order issue in construction and destruction, do not call `virtual` functions from constructors and destructors
  - the intended method may not have been defined yet, or has already been destroyed
- `virtual` functions add overhead via resolution, so avoid unless necessary

## `Final`
- the `final` keyword can be used after the parameter list to prevent further derivations
  - ```c++
    class A {
        // ...
        virtual void foo();
    };
    class B {
        // ...
        void foo() override final();
    };
    class C {
        // ...
        void foo() override; // NOT ALLOWED
    };
    ```
  - similarly, the `final` keyword can be applied on a class level to prevent inheritance
    - ```c++
      class A final {
          // ...
      }
      class B : public A { // NOT ALLOWED
          // ...
      }
      ```
- to avoid `virtual`, use the scope resolution
  - `base.Base::foo()` instead of `base.foo()`

## Covariant Return Types
- a case where the overrided implementation can have a different return type comparing to the original
- if the implementation returns a class object pointer
  - the original implementation can return a base class pointer
  - the derived implementation can return a derived class pointer
  - in this case, the derived implementation will still be considered a valid overload

# Virtual Destructors
- when an object is destroyed (especially manually), the destructor is based on the referenced type instead of the actual type
  - ```c++
    Derived *derived = { new Derived() };
    Base *base { derived };
    delete base; // ~Base is called instead of ~Derived
    ```
- for this reason, prefer `virtual` destructors if a class is ever inherited by another
- otherwise, use `protected` destructor that disallow deletion from pointer
  - however this limits the class in some way, so the modification is not encouraged
- the alternative is to mark the destructor `final` and keep it `public`

# Easy/Late Binding
- **binding**: process that convertsg identifies into addresses
  - used for both variables and functions
- direct function calls use *early binding* (or *static binding*) done by the compiler (linker)
  - the linker provides an address that the assembly code will jump to for the function
- *late binding* (or *dynmaic binding*) is done with function pointers
  - calling a function from its function pointer is  called an *indirect function call*
## Virtual Table
- `C++` uses a lookup table
  - aliases: virtual table, vtable, firtual function table, virtual method table, dispatch table
  - each `class` is assigned a virtual table upon compilation, and each entry in the table is a function pointer pointing to the most derived version accessible by that class
  - the compiler adds a hidden member pointer `__vptr`, set altumatically when a class instance is created, pointing to the virtual table
    - this pointer value is fixed, unlike the `this` pointer
    - note that since the member is private, it is per-derivation, so each derivation is at least one byte larger than its parent because of this extra pointer
  - when accessing a virtual method, `__vptr` is dereferenced to reference the virtual table on the designated level of derivation, and then the function is accessed from the implmentation in the virtual table entry
    - by definition, that implementation will be the most derived implementation relative to the point of access

# Abstract Class
- a **pure virtual function** is a `virtual` function without a body
  - aka. **abstract function**
  - any class with an abstract function becomes an abstract base class (but without the explicit `abstract` keyword as in `Java`)
- abstract classers cannot be instantiated
- to force a `virtual` funcion *with* a body to be purely virtual, add `=0` after the parameter list
  - `virtual void foo() const = 0;`
  - to call the base implementation, use `Base::foo` in the derived implementation

## Interface Class
- no member variables, all member functions are purely virtual
- `C++` does not have a designated keyword for interfaces, instead just mark all interfaces with class names starting with `I`
- abstract calsses still have virtual tables
  - the virtual table entry for a pure virtual function is often null pointer or an error printing function, depending on the implementation

# Virtual Base Class
- solves the diamond problem
- use the `virtual` keyword when specifying the type of inheritance
  - `Scanner: virtual public PoweredDevice`
- normally, multiple inheritance would lead to the base class object to be constructed multiple times
  - `virtual` multiple inheritance forces single time of construction
- the most derived class will be reponsible of creating all base class objects (starting from the first `virtual` inheritance in the chain)
  - needs to call multiple constructors in the initializer list
  - `Scanner(): PoweredDevice{}`
  - `Printer(): PoweredDevice{}, Scanner{}{}` for a derived class `Printer` that inherits `Scanner` publically (but not virtually)

# Object Slicing
- instead of assigning the refernce of a derived object to a base object, assign the derived object *directly* to a base class variable
  - ```c++
    Derived derived{};
    Base base{ derived };
    ```
  - the assigned value will be *sliced*, i.e. only the base class part of the derived class object will be assigned
  - the virtual functions will resolve to the base implementation instead of virtual implementation after slicing (basically polymorphism stops working)
- note that this also applies to function parameters
  - *ex.* accidentally using a by-value instead of by-reference parameter
  - ```c++
    void foo(const Base base); // note the lack of reference
    // ...
    Derived derived{};
    foo(derived); // passes a Derived object which will be sliced upon copying
    ```
- another way of slicing is through templated containers
  - `std::vector<Base> v{}` will accept *copied* `Base` objects upon pushing
  - it is not possible to replace with `<Base&>`, because references cannot be reassigned after initialization, and `std::vector` uses assignment in many cases
  - it is possible to repalce with `<Base*>`
    - however, this allows for `nullptr`, which may or may not be desirable
  - another alternative is `std::ref`
- "hybrid"
  - ```c++
    Derived d1{};
    Derived d2{};
    Base& b{};
    
    b = d1;
    ```
    - at the last line, the Base portion of d1 is assigned to the location of b (and subsequently d2)
    - d2 will have the base portion of d1 and the derived portion of d2
- for the above complications, object slicing is not recommended

# Dynamic Casting
- casting a derived pointer to a base pointer is called **upcasting**
- casting a base pointer to a derived pointer is called **down-casting**
  - in this case, dynamic casting is needed because the exact class of the object cannot be determined at compile time, especially for function calls
  - `dynamic_cast<T>` can be used similar to `static_cast<T>`
    - commonly it is in the form `dynamic_cast<Derived*>(someBaseClassPointer)`
    - upon failure, a `nullptr` is returned
    - for references, an exception will be through instead, because there is no null reference
  - down-casting could also be done with `static_cast`, but it will not provide any safe check, although slightly faster
- `virtual` functions shouuld be preferred over *any* type of casting
- note that `dynamic_cast` depends on the Run-time Type Information feature (RTTI) which could be turned off upon compilation
  - turning it off will cause all `dynamic_cast`s in the compiled program to function incorrectly

# Output with Operator<<
- functions that use `std::cout` can be overrided virtually just like other functions
  - however they do not go well when used in the middle of another `std::cout`
  - ```c++
    class Base {
        // ...
        virtual void foo() const {
            std::cout<<"base";
        }
    };

    class Derived: public Base {
        // ...
        virtual void foo() const override {
            std::cout<<"derived";
        }
    };

    // ...
    std::cout<<derived.foo()<<std::endl;
    ```
- although this problem can be solved with overriding `operator<<`, it is impossible to make that override `virtual`
  - however it is possible to call a `virtual` function within `operator<<`
  - ```c++
    friend std::ostream& operator<<(std::ostream& out, const Base& b) {
        return b.foo(out);
    }
    virtual std::ostream& foo() const {
        out << "someString";
        return out;
    }
    ```
    - such an implementation works for any derived class, with no need for re-implementation
