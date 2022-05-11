- [Function Pointers](#function-pointers)
- [Stack & Heap](#stack--heap)
  - [Heap](#heap)
  - [Call Stack](#call-stack)
  - [Std::Vector Capacity](#stdvector-capacity)
  - [Std::Vector as Stack](#stdvector-as-stack)
  - [Recursion](#recursion)
- [Commandline Arguments](#commandline-arguments)
  - [Numeric Arguments](#numeric-arguments)
  - [Ellipsis](#ellipsis)
- [Lambdas](#lambdas)
  - [Generic Lambdas](#generic-lambdas)
  - [Return Types](#return-types)
  - [Captures](#captures)
    - [Dangling Captured References](#dangling-captured-references)
  - [Copying Lambdas](#copying-lambdas)

# Function Pointers
- functions are stored in memory as well
- evoking function identifier without parameters will return their pointer
- creating a non-constant function pointer
  - ``T (*fPtr)(K);`` creates a pointer to a function taking in type K and returning type T
- assigning values to a function pointer
  - ``fPtr = &foo;``
  - note that assigning will check for narrowing types
    - *ex.* a function returning ``double`` cannot be assigned to a function pointer of type ``int(*)()``
  - function pointers take ``nullptr``
- calling a function pointer
  - explicit: ``(*fPtr)( params );``
  - implicit: ``fPtr( params );``
  - *default parameters do not work with function pointers*
    - default paramters are resolved at compile time!
- using function pointers as arguments
  - ```c++
    void selectionSort(int* arr, int size, bool (*precedes)(int, int)) {
        //...
    }
    bool ascending(int x, int y) { return x < y; }

    // ...
    selectionSort(arr, length, ascending);
    ```
  - function pointers arguments can have default values
  - ```c++
    void selectionSort(int* array, int size, bool (*precedes)(int, int) = ascending);
    ```
- type aliasing with function pointers
  - ```c++
    using PrecedenceFunction = bool(*)(int, int);
    ```
- alternative naming with ``std::function``
  - located in ``<functional>``
  - ``std::function<T(K)>`` is the equivalent of ``T(*)(K)``
  - the same aliasing could be done on ``std::function``
  - ```c++
    using PrecedenceFunction = std::function<bool(int, int)>;
    ```
- type inference for function pointers
  - ``auto precedes{ &ascending };``

# Stack & Heap
- the memory a program uses consists of 
  - *code/text* read-only, containing the compiled program
  - *bss/uninitialized data* zero-initialized global and static variables
  - *data/initialized data* initialized global and static variables
  - *heap* dynamically alocated variables
  - *call stack* function parameters, local variables, etc.

## Heap
- allocating memory on the heap is comparativelyu slow
- allocated memory stays allocated unless de-allocated or program ends (OS takes care)
- must be accessed through pointer, adding some overhead
- large

## Call Stack
- stack grows as functions are called
  - function args and local variables are stored on the stack
  - upon function return, stack frame is cleared and open for subsequent overwrites
- allocating on stack is faster
  - determined at compile time, not dynamic
- does not persist beyond function scope
- relatively small

## Std::Vector Capacity
- length vs. capacity
  - length is the valid elements count
  - capacity is the upper limit for elements that can be stored, not all are valid
  - ``std::vector::size``, ``std::vector::capacity``
  - ``[]`` and ``.at()`` considers length, not capacity
  - ``std::vector::reserve(size_t)`` is the capacity-equivalent of ``std::vector::resize``
- capacity can be increased automatically when adding elements beyond original capacity
  - the exact additional capacity allocated depends on the compilation of ``std::vector``, but always at least the necessary amount

## Std::Vector as Stack
- ``std::vector::push_back(T)``
- ``std::vector::back()`` returns ``T``
- ``std::vector::pop_back()`` returns ``T``

## Recursion
> not recorded as it is general programming
- **memoization**: ``C++`` (or ``C`` in general) supports memoization by ``static`` variables within the function that stores data between different function invocations

# Commandline Arguments
- the full ``main`` definition:
  - ```c++
    int main(int argc, char* argv[]) {
        // argc records the number of args
        // argv[argc] returns a C-style string
    }
    ```
- the first argument is usually the program's name, although some implementations leave it as empty string

## Numeric Arguments
- conversion method is typically through ``std::stringstream``
  - located in ``<sstream>``
  - *ex.*
    ```c++
    std::stringstream sconvert{ argv[1] }; // expecting a number at the first argument after program name
    int argint{};
    if(!(convert >> argint)) argint = 0; // check for conversion failure
    ```

## Ellipsis
> not recommended by the tutorial, skipped
- check out parameter packs since ``C++11`` and fold expressions since ``C++17``

# Lambdas
- *def.* **anonymous function** functions defined within another function
  - ```c++
    [ captuerClause ] (parameters) -> returnType {
        statements;
    }
    ```
  - capture clause and parameters can both be empty
  - return type is optional when type inference for return is preferred instead
- lambdas can be stored in a variable
  - ```c++
    auto isEven{
        [](int i)
        {
            return (i%2) == 0;
        }
    }
    ```
  - the type of ``isEven`` is not ``bool(*)(int)``, it is some underlying type generated by the compiler
    - this is because lambdas aren't functions, they are **functors** that overload the operator ``operator()``
    - ``C++`` doesn't actually allow for nested functions
  - with an empty capture clause, lambdas can be stored in regular function pointers as if it is
    - ```c++
      double (*add)(double, double) {
          [](double a, double b) { return a + b; }
      }
      ```
  - with empty/non-empty capture clauses, they can be stored in ``std::function``
  - ```c++
    std::function<double(double, double)> add {
        [](double a, double b) { return a + b; }
    }
    ```
  - since ``C++17``, the type argument can be omitted
- prior to ``C++20``, ``auto`` cannot be used for lambdas as parameters
  - ``void repeat(int reptitions, const auto& fn);``
  - the caller wouldn't know what parameters and return types ``fn`` has
  - instead, use ``std::function``
  - ``auto`` parameters also cannot be separated into header and source files
## Generic Lambdas
- since ``C++14``, lambdas can use ``auto`` parameters simlar to how functions handle ``auto`` parameters
  - lambdas making use of ``auto`` parameters are called **generic lambdas**
  - note that for each resolution of ``auto`` parameters, a unique underlying type will be generated for the associated lambda
  - this is usually not a problem until ``static`` variables are concerned
    - with different resolutions, the ``static`` variable is not shared since the underlying type of the lambdas are different
## Return Types
- Lambdas can return different types if there is no ``auto`` (inference forces return types to agree within the lambda)
  - this can be solved through explicit casting at ``return``
  - use a explicit return type instead of ``auto``, then the different return statemenets will be casted implicitely to that type
## Captures
- note that unlike ``JS``, lambdas are not nested functions and are therefore not able to access certain variables of the function context without extra specifications
  - lambdas *can* access outer variables when their values are determined at the time of compilation
    - for example a ``constexpr`` and or ``static`` variable that does not rely on any function for initialization
    - global variables can always be accessed
  - ```c++
    std::string search{};
    std::cin >> search;
    auto found{ std::find_if(arr.begin(), arr.end(), [](std::string_view str) {
      return str::find(search) != std::string_view::npos; // npos denotes not found
    })};
    ```
  - in this example, ``search`` cannot be accessed
- capture clause can be used to specify the context members that are allowed to access
  - ```c++
    auto found{ std::find_if(arr.begin(), arr.end(), [search](std::string_view str) {
      // ...
    })};
    ```
  - a *clone* of the outer level ``search`` is created within the lambda function at the point of definition
    - however, the type of the captured variable is not exactly the same
  - note that this defaults to ``const``, in this case the lambda cannot modify captured values
  - to make *all* members mutable, add the ``mutable`` keyword
    - note that capture only occurs at definition, and the captured variables persist between calls
      - upon invocation, lambdas do not re-capture whatever value those members are at the point of invocation
    - ```c++
      int initialValue;
      std::cin>>initialValue;
      auto decrement{[initialValue]() mutable {
        --initialValue;
        std::cout<<initialValue<<std::endl;
      }};
      ```
    - the decrement will only be reflected *within* the lambda calls
- to edit the actual variable, capture by reference instead
  - ```c++
    [&initialValue]() {
      --initialValue;
    }
    ```
  - even without the mutable keyword, this form of lambda can modify the value within and outside the lambda itself
  - this allows for counting invocations through incrementing an outside counter within the lambda
- **default captures**: shorthands for capturing all mentioned variables within the lambda
  - ``[=]`` stands for capturing everything by value
  - ``[&]`` stands for capturing everything by reference
- variable definition within the capture expression
  - ``[area{width, height}]`` captures ``width`` and ``height`` and creates the captured variable area that persists across lambda calls
    - if ``mutable``, ``area`` will be overwritten in subsequent lambda calls if indeed mutated

### Dangling Captured References
- if the lambda persists beyond its capturing context, the reference captures will be dangling
  - ```c++
    auto makeNamer(const std::string& name) {
      return [&]() {
        std::cout << "My name is"<< name << std::endl;
      }
    }
    ```
  - when the returned value of makeNamer is invoked, it will be referencing a dangling reference because the ``name`` variable is already out of scope

## Copying Lambdas
- this is possible because lambdas are objects
- when copying a lambda, the captured variables are copied as well by value
- ```c++
  int i{0};
  auto count{[i]() mutable {
    std::cout << i++ << std::endl;
  }};
  count(); // 0
  auto count2{ count };
  count(); // 1
  count2(); // 1
  ```
- ```c++
  void invoke(const std::function<void()>& fn) {
    fn();
  }

  // ...
  invoke(count); // 0
  invoke(count); // 0
  ```
  - each ``invoke`` copies the ``count`` lambda by value and uses that to *initialize* a new ``std::function<void()>`` object, so the invocation doesn't actually happen on the original ``count``
  - the solution is to use ``std::reference_wrapper`` in ``<functional>``, which forces the compiler to copy the reference instead of the value itself
    - the alias is ``std::ref``
    - ```c++
      invoke(std::ref(count));
      ```
    - this is only the case because of the underlying type conversion from whatever the lambda's exact type is to the ``std::function`` type
    - when the exact type of the lambda is used, there is no need to force ``std::ref``
- due to this above complication, mutating lambdas are usually not preferred