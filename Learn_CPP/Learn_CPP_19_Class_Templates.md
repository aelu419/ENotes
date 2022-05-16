- [Template Classes](#template-classes)
- [Template Parameters](#template-parameters)
- [Template Specialization](#template-specialization)
  - [On the Function Level](#on-the-function-level)
  - [On the Class Level](#on-the-class-level)
- [Partial Specialization](#partial-specialization)
  - [Partial Templates for Pointers](#partial-templates-for-pointers)

# Template Classes
- similar to functions, classes can be created at compile time using templtes
  - ```c++
    template <typname T>
    class Array {
        // ...
    };
    ```
- the `.h`, `.cpp` split for normal class declaration/definitions don't work the same way with templates
- solutions
  1. put everything in the header
  2. put everything in an inline file (`.ihl`) and include it at the *bottom* of the header file
    - note that sometimes the compiler compiles the `.ihl` as well (by itself, outside its included version in the `.h`)
    - if any errors are caused by the inclusion, exclude the `.ihl` file in the compilation process
  3. include a third `.cpp` file that
    - `include` the header and code (`.cpp`)
    - explicitly instantiate template classes for certain `T`
      - ```c++
        #include "Array.h"
        #include "Array.cpp"

        template class Array<int>;
        template class Array<double>;
        // ... include all other use cases in the project
        ```

# Template Parameters
- *ex.* `template <typename T, int size>` can be used for a container of type `<T, size>`
- this allows for "dynamic" array size at compile time, as long as `size` is passed as a `constexpr` value
  - ```c++
    constexpr size{5};
    Array<int, size> arr; // creates a length 5 array with a backing array of type int[5]
    ```
  - within the `Array` implementation, there is no need to do `new int[size]`

# Template Specialization
- specialize on a specific group of types instead of all types in general
## On the Function Level
- outside the `template` class, define a member function with a specific case of the tempalated type
  ```c++
  template <typename T>
  class Storage {
      // ...
      Storage(T value) : m_value{ value } {}

      ~Storage() = default; // explicit deconstructor in order to be specialized
  };

  // override the Storage constructor for C-style strings
  template<>
  void Storage<char*> Storage(char* value) {
      // dynamically determine the length of @value up to \0,
      // and then initialize a new char[length] variable to be stored in m_value
      // ...
  }

  // override the Storage destructor for C-style strings
  template<>
  Storage<char*>::~Storage() {
      delete[] m_value;
  }
  ```
  - note that the deconstructor is explicit only because `C++` only allows speciailziation for explicitly declared functions
## On the Class Level
- the entire class can be specialized
- ```c++
  template<>
  class Storage<bool> {
      // ...
  }
  ```
  - note that it is possible to *remove* functions from the implementation of a specialized class, since the missing functions do not default back to the generic implementation for `T`
  - it is good practice to not add/remove functionality between different specializations

# Partial Specialization
- the prior secion describes how to specialize the entire template, however this might not be optimal for tempalte parameters like `size`
    - ```c++
      template<>
      void print(Array<char, 14>& arr); // only handles that specific length
      ```
- the best implementation will handle the `char` part but leaves `size` up to the template
    - this requires the use of partial template
    - ```c++
      template<int size>
      void print(Array<char, size>* arr);
      ```
- entire classes can be partiallly speciailized
    - this is mainly to get around the limitation that member functions can't be directly partially specialized
    - however this leads to lots of duplicate code, similar to class-level specializations
- the alternative is to declare a child class for a specific type that implements the method differently
    - (and make the base implementation `virtual`)
    - *ex.*
      ```c++
      template<typename T, int size>
      class StaticArray_Base {
          // ...
          void print() {
              // print normally
          }
      };

      template<int size>
      class StaticArray<double, size>: public StaticArray_Base<double, size> {
          public:
          void print() {
              // print in scientific notation
          }
      };
      ```
## Partial Templates for Pointers
- recall `Storage` class with need to deep copy the `char*` type
    - other pointer types also need specialization
    - instead of specializing the class separately for each type of pointer, do a pointer speciailization
    - ```c++
      template <typename T>
      class Storage<T*> {
          // ...
      }
      ```
      - all template type instances with pointer types will resolve to this specialization instead
      - note that this pointer speciailziation doesn't cover the special case of C-style strings, for strings the previous `char*` specialization is still needed
    