- [Coercion](#coercion)
  - [Integral -> Floating Point Promotion](#integral---floating-point-promotion)
  - [Numeric Conversion](#numeric-conversion)
    - [Arithmetic Conversion](#arithmetic-conversion)
  - [C-Style Casting](#c-style-casting)
- [Type Alias](#type-alias)
- [Default Arguments](#default-arguments)
- [Function Templates](#function-templates)

# Coercion
- *naming.* automatic/implicit type conversion
- compiler determines if type conversion is possible, if so, create data of the designated type that matches the *value* of the original type
- in some cases, types can be converted by just copying the data, instead of doing any actual type conversion
## Integral -> Floating Point Promotion
- *def.* **numeric promotion**: narrower to wider type conversion
  - preserves value
  1. floating point promotions
     * ``float`` $\to$ ``double``
  2. integral promotions
     * by default, both signed and unsigned values are converted to (signed) int as long as the range holds
  
## Numeric Conversion
- may result in data loss
- usually, compilers warn about narrowing conversions, but not unsigned/signed conversions
- a good practice is to minimize the use of narrowing conversions, and use ``static_cast<T>`` when needed
### Arithmetic Conversion
- certain arithmetic operators demand both sides to be of the same type
- issues occur at signed/unsigned operations since unsigned values have higher priority

## C-Style Casting
- ``(type_t)var``
- in ``C++``, ``C`` style casts have another equivalent representation ``type_t(var)``
- ``C`` style casts should be avoided due to their potentially undefined behavior
  - at runtime, they are sometimes doing static casting, sometimes const casting, and sometimes reinterpret casting

# Type Alias
- ```c
  using <alias> = <type>
  ```
- aliases are not *type safe* as the compiler does not check for implicit conversions
  - as long as underlying type passes, implicit conversions are always considered legal
- semantically equivalent with ``typedef``
  - ```c
    typedef <type> <alias>
    ```
  - not encouraged due to confusion (which of type/alias comes first?)
- type aliases follow the same scoping as variable names
- type aliases improve simplicity and legibility of code
  - however, adding a new name will always complicate code, so only do it when improvement is clear

# Type Deduction
## For Variables
- ``auto``: let compiler figure out type based on the value assigned
  - ```c
    auto c{5.0}; // double
    ```
- type deduction usually drops the ``const``
  - ```c
    const int y{1};
    auto x{y}; // int, not const int
    ```
- if need const, do ``const auto`` instead
- *case.* strings
  - by default, ``auto`` deduces ``string``s to ``char*``
  - if need to be string, suffix the literal with ``s`` (``std::string``) or ``sv`` (``std::string_view``)
    - ```c
      auto s { "foo"s };
      ```
- type deduction may obscure arithmetics, but for objects they are generally agreed to be safe

## For Functions
- ```c
  auto foo(int bar) {
    return bar + 1; // deduces to be an int
  }
  ```
- cannot be used in function declarations
  - for this reason, avoid ``auto`` functions
- ``auto`` as mark for upcoming trailing return type
- ```c
  auto foo(int bar) -> int {
    return bar + 1;
  }
  ```
- this is compatible with function declarations

# Function Overloading
- *def.* **overload resolution**: resolving which overloaded version of a function is called based on its signature
- function signatures are determined by the *number* and *type* of parameters
- function signatures are also dependent on whether it is a ``const`` function or not, and if it is a ``ref``
- the ellipsis parameter ``...`` is considered a unique type of parameter
- function overloading is done on the compiler via *name mangling*
  - namely, the original generated name based on the identifier is mangled according to the function signature, thereby keeping each overloaded version unique
  - *ex.*
  ```c
  int fcn(); // --> __fcn_v
  int fcn(int); // --> __fcn_i
  ```
    - this is just an example, there is no actual standard for such mangling
- when a function call matches none of the signatures
  - the compiler goes through all the currently known signatures and try type conversions
    - if no matching, try next set of type conversions
      - if sequence ends, compiler error
    - if single matching, complete
    - if more than one matching,  ambiguous match error
  - sets of type conversions by priority
    - exact
    - trivial
      - *ex.* ``const`` <-> non-``const``, ``ref`` <-> non-``ref``
    - numeric promotion
    - numeric conversion
    - user-defined conversions
    - try if coverable by ellipsis
  - ambiguous matches
  - overload resolution for multiple parameters
    - the chosen overload must be *at least as good* a match on all parameters, with one parameter *better* than all other overloads
  
# Default Arguments
- ```c
  int foo(int bar = 3, bab = 4);
  ```
  - can only be rightmost parameter
    ```c
    foo(3); // bar = 3, bab = 4
    foo(,3); // attempt to bar = 3, bab = 3, but not actually legal
    ```
- the default value can only be written in *either* the declaration *or* the definition, but not both
  - the agreed-upon practice is to write in declaration
- note that overloaded functions with default parameters may lead to ambiguity
```c
int foo(int bar);
int foo(int bar, int bab = 0);

foo(0, 1); // no ambiguity
foo(0); // ambiguous
```

# Function Templates
- create patterns to write functions based on
  - works with types that are not currently supported at the time of writing the template
  - templates opterate on **template types** instead of regular variable types
  - ```c++
    template <typename T> // template parameter declaration
    // ex.1
    T max(T x, T y) {
      return (x > y) ? x : y;
    }

    // use cases
    max<int>(0, 1);
    max<double>(0, 0.5);
    max<>(0, 1); // note that this makes use of template argument deduction to max<int>

    // ex.2
    template <typename T, typename U>
    U conv(T x) {
      //...
    }
    ```
    - the process of using a function template is called **function template instantiation**
      - the result of which is a **function instance**
      - if used via function call, the instantiation is **implicit**
  - template function instances can be used in coordinance with regular functions
    - ```c
      // continue from ex.1
      int max(int x, int y) {
        // impl.
      }

      // uses
      max<>(0, 1); // calls template instance with T=int
      max(0, 1);  // calls non-template max
                  // if the non-template max does not exist, this will redirect to the template max
                  // this syntax provides a way to overload templates
      max<>(0, 0.5); // this results in ambiguous typing
      ```
  - template functions can have non-template parameters
    - ```c
      template <typename T>
      T foo(T x, int y);
      ```
  - ! note that instantiated functions may not compile
  - ``auto`` for function templates
    - for multiply-typed function parameters, the return value may be ambiguous
    - ```c
      template <typename T, typename U>
      T add(T x, U y) { return x + y; }
      
      add<int, double>(1, 0); // unguarded narrowing conversion

      template <typename T, typename U>
      auto add(T x, U y) { return x + y; } // return type will be promoted correctly

      ```
      - for ``C++20``, if each parameter could be a *unique* type, then the template definition can be abbreviated with ``auto``
      - ```c
        auto ternary(auto x, auto y, auto z) {
          // ...
        }
        // this is the equivalent 'full' template form
        template <typename T, typename U, typename V>
        auto ternary(T x, U y, V z) {
          // ...
        }
        ```
  - note that ``static`` variables are not shared between different template function instances, and that there can only be one instance for any unique set of template type names
    - ``max<int>`` calls will share one static variable, while ``max<double>`` calls share another
  - writing functions based on template declarations
    - ```c
      // continue from ex.2
      template<> // 
      ```
- 