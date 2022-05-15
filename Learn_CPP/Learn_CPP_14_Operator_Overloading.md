
# Operators as Functions
- operators are implemented as functions and can be overloaded by changing the signature
  - *ex.* `+` is `operator+ (param1, param2)` where the type of `param1` and `param2` is determined from the code at compile time
  - the compiler usually comes with built in operators with fundamental data types as operands
- most operators can be overloaded, except certain restricted ones like ``::``
- operators can only be overloaded, not created
## Operators as Friend Functions
- ```c++
  class Dollar {
      // ...
      friend Dollar operator+(const Dollar &a, const Dollar& b);
  };

  Dollar operator+(const Dollar &a, const Dollar &b) {
      return Dollar{a.amount + b.amount};
  }
  ```
  - note that this `friend` function can be defined within the `Dollar` class and will work the same
    - such implementation will not need a `Dollar` class prefix
- operator parameter order is fixed
    - for differently typed parameters, the order changes the signature
        - `operator+ (Fraction, int)` is not the same as `operator+ (int, Fraction)`
- operator implementations can reference other operator implementations
- for user data types, the compiler will find suitable casting methods that includes constructors
    - for example, `1` can be casted to `Fraction(1)` (equivalent of `Fraction(1, 1)`)
## Operators as Normal Functions
- without access to private members
- no `friend` keyword
- needs prototype stated
    - commonly, inside the `.h` file for the user defined `class`, but *not* in the `class` definition itself
    - ```c++
      class Dollar {
          // ...
      };

      // function prototype in the .h file, to be implemented in the .cpp file
      Dollar operator+ (const Dollar& a, const Dollar& b);
      ```

## Overloading with Member Functions
- must be member of the *left* operand (if not unary)
  - the left operand becomes `this`
  - the other operands become function parameters
- note that this is different from having a `friend` member function as implementation
  - in that case, the implementation follows the same rules as a `friend` operator function outside the class
- certain operator functions can only be overloaded as member functions
  - *ex.* `=`, `[]`, `()`, `->`
- when the original class is not accessible, member operator functions (or member functions in general) cannot be added
  - *ex.* the `<<` operator bewteen `std::ostream` and `Player` cannot be overloaded as member because `std::ostream` is implemented elsewhere
  - *ex.* the `+` operator between `int` and `Fraction` cannot be overloaded as member because the left operand type `int` is not a class
- preference rules
  - member function if unary or required (like `[]`)
  - prefer member function if the left operand is modified
  - prefer normal function over `friend` function unless private member access is required

# Operator Examples
## IO Operators
- ```c++
  class Player {
      // ...
      // friend operator function prototype
      friend std::ostream& operator<< (std::ostream& out, const Player& p);
  };

  std::ostream& operator<< (std::ostream& out, const Player& p) {
      out << p.m_name; // access private member
      return out; // for chaining
  }

  // ...
  std::cout<<Player{"bob"}; // returns std::cout and also prints
  ```
  - overloading for `std::cin` is the same except the type is `std::istream` instead of `std::ostream`

## Unary Operators
- ```c++
  class Fraction {
    // ...
    Fraction operator-() const;
  };

  Fraction Fraction::operator-() const {
    // member functions have access to private members and *this
    return Fraction{-m_numerator, m_denominator};
  }
  ```
- other unary operators include ``!``, ``+``

## Comparison Operators
- of function type `bool (const T&, const T&)`
- reduce redundancy by referencing other comparison operators
  - *ex.* ``operator>=`` is the same as ``!operator<``
  - usually, full implementations of ``==`` and ``<`` are enough for all comparison operators to be implemented

## Increment and Decrement
- prefix and postfix versions are differentiate by the `int` parameter
  - the ``++`` postfix actually accepts an `int` parameter
  - *ex.* `x++(25)` increments by 25
- both forms return `*this` which is of type `T&`
- ```c++
  class Fraction {
    // ...
    Fraction& operator++();
    Fraction& operator++(int);
  };

  // prefix
  Fraction& operator++() {
    m_numerator += m_denominator;
    return *this;
  }

  // postfix
  Fraction& operator++(int) {
    Digit temp{*this};
    ++(*this);
    return temp; // return prior state as a copy
  }
  ```
  ```c++
  // postfix with an actual argument
  Farction& operator++(int inc) {
    Digit temp{*this};
    m_numerator += inc * m_denominator;
    return temp;
  }
  ```
- for this reason, prefix-increment is usually faster than postfix-increment by a slight amount since it doesn't involve temporary variables

## Subscript
- for a user defined collection with a container of type `T`, the `operator[]` should return `T&`
  - this way it can be assigned to
- `const` and non-`const` versions of `operator[]` can be defined separately
  - ```c++
    class IntArrayHolder {
      private:
      int m_list[10}{ ... };
      public:
      int& operator[] (int index);
      int& operator[] (int index) const;
    };
    ```
- a common way to write `operator[]` is to add assertions for the access
  - located in `<cassert>`, usage: `assert(boolExpression)`
- `operator[]` is towards the object itself, not the pointer to the object
  - ```c++
    IntArrayHolder* arr{ new IntArrayHolder{} };
    arr[2] = 3; // error
    ```
  - the compiler interprets the above statement as an array *of* `IntArrayHolder` have been created and the program is trying to access the 3rd element of that array
    - this is because the pointer is recognized as an array variable when it is used with `[]`
    - the correct way is `(*arr)[2] = 3`
- `operator[]` can take non-`int` parameters as well, including ones that are not `int`-like
  - for example, a dictionary implementation can be indexed by a `std::string_view`
  - multiple parameters for `operator[]` will be supported starting from `C++23`

## Parenthesis Operator
- return type and parameters are free
- `const` and non-`const` are differentiated, similar to `operator[]`
- `operator()` is often used to implement functors (classes that operate like functions, but have member functions and variables

## Overloading Typecasts
- member function converting type `U` to `V` will be under `class U` with prototype `operator V();` or `operator V() const;`
  - does not take parameters
  - does not have return type (implicitely `V`)
- the conversion can be invoked by `static_cast<V>` on a `U` instance

## Copy Constructor
- direct and uniform initializations will call the **copy constructor** if they are in the form `T variable{anotherTVariable}`
  - a copy constructor is provided implicitly which copies the memory of the original instance
- copy constructors can be explicitly defined by providing a constructor in the form `T(const T&)`
- to prevent copies, explicitly define a copy constructor and then make it `private`
- some compilers *elide* copy constructors
  - this just means instead of going through the extra step of creating an anonymous instance and use that as parameter, the program treats the initialization as a direct/uniform one
  - *ex.* for a copy constructor that doesn't do any extra modifications on member values, `T{ T{ paramsForAnotherConstructor } }` may be shortened to `T{ paramsForAnotherConstructor }` by the compiler
- since `C++ 17` some cases of elision are *mandatory*
  - just be careful if statements in the copy constructor (with no effect on member values) are skipped
- copy initialization for classes (in the form `T variableName = value`)
  - evaluates as `T variableName(value)`
  - `T variableName = T(params)` may call both the constructor *and* and the copy constructor, which is inefficient (at least before the mandatory elision in `C++17`)
    - a good practice is just to avoid them
- copy constructors may also be called when class instances are
  - passed by value to a parameter
  - returned by value
  - elision may also happen in these cases

## Converting Constructors
- constructors that can be used for implicit type conversions
  - *ex.* `printFraction(const Fraction&)` can take `1` as a parameter because the constructor `Fraction(int numerator)` is a conversion constructor from `int` to `Fraction`
  - all constructors are treated by the compiler as conversion constructors
  - implicit conversion works for all kinds of initialization (direct, uniform, copy)
- to prohibit implicit conversion, a `explicit` keyword can be added in front of the constructor, so that
  - it will not accept parameters that are implicitly converted
    - *ex.* 
      ```c++
      class Fraction {
        // ...
        explicit Fraction(int numerator) // ...
      }

      // ...
      void printFraction(const Fraction& f) {
        // ...
      }

      // ...
      printFraction(1); // not accepted, since 1 cannot be converted to Fraction since the conversion method is marked explicit
      printFraction(static_cast<Fraction>(1)); //accepted, this conversion is explicit
      ```
- to prohibit explicit conversion as well, mark `delete` for the conversion constructor
  - *ex.* `Fraction(int) = delete` along with other constructor declarations

## Assignment Operator
- copy values from one object to another *pre-existing* object
  - although the same symbol, the copy constructor is different because
    - it is called only when a new object has to be created before the copying could happen
    - if a new object does not have to be created, then assignment happens
  - generally done in the form `T& operator= (const T& t)`, must be member function of `T`
    - returns `*this` in order to chain the operation like `a = b = c;`
- `C++` permits self assignment
  - this can be checked by `this==&t` which checks for address equality
  - copy and swap idiom
    - needed when the `class` manages a resource
    - *rule of three*: if any of the following 3 are implemented explicitly, then it is good practice to implement *all* of them explicitly
      - copy constrtuctor
      - copy assignment
      - destructor
      - since `c++11`, two new operators are added: move assignment and dtor
    - implementation 1
      - ```c++
        {
          if (this != &other) {
            delete[] m_arr;
            m_arr = nullptr;

            m_size = other.m_size;
            m_arr = m_size ? new int[m_size] : nullptr;
            // copy from other.m_arr to m_arr
          }
          return *this;
        }
        ```
        - the self assignment is rarely true and slows down most of the cases
        - exception guarantee
          - *basic*: no leaks, no crashes
          - *strong*: the exposed state of the object remains unchanged after a failed modification
        - note that the above implementation only provides basic exception guarantee
        - more code and temporary memory use
      - implementation 2 (copy and swap)
        - first, define a `public` `friend` swap function
          ```c++
          using std::swap; // enable ADL
          swap(a.m_size, b.m_size);
          swap(a.m_arr, b.m_arr);
          ```
        - then, use swap in `operator=`
          ```c++
          ArrayWrapper& operator=(ArrayWrapper other) { // note that this already handles the copy by value
            swap(*this, other);
            return *this;
          }
          ```
        - another implementation with an intermediate step is
          ```c++
          ArrayWrapper& operator=(const ArrayWrapper& other) {
            ArrayWrapper temp(other);
            swap(*this, temp);
            return *this;
          }
          ```
- default assignment operator
  - copies each member by value
  - can be disallowed by using the `delete` keyword
    - ```c++
      ArrayWrapper(const ArrayWrapper& copy) = delete; // disallow copy constructor
      ArrayWrapper& operator= (const ArrayWrapper& other) = delete; // disallow assignment
      ```
## Shallow vs. Deep Copy
- shallow copy copies member values
- deep copy also handles any underlying managed resource
  - this requires explicit copying
  - need to check for invalid pointers, self assignment, etc

## Operators in Function Templates
- template functions sometimes requires certain operators to be defined
  - *ex.* a templated `max` function requires `operator<` or `operator>` to be defined, depending on implementation
