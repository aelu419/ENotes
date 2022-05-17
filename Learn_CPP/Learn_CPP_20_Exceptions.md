- [Why](#why)
- [Basic Handling](#basic-handling)
- [Caution](#caution)
- [Exception Specifications](#exception-specifications)
  - [`Noexcept`](#noexcept)
  - [Exception Safety](#exception-safety)
  - [Dynamic Exception Specifiers](#dynamic-exception-specifiers)

# Why
- to replace unexplained return codes in the program
  - and remove related checking (`if`, `switch`, etc.)
- fit cases that cannot produce return codes, such as `void` functions and constructors
- handle default crash cases not covered by exceptions that are already handled
- reduce duplicate code where there are many points of failure that produce the same crash
- provide quick way of re-throwing the error

# Basic Handling
- `throw t` can be applied to any value `T t`
- ```c++
  try {
      throw T{};
  } catch (T t) {
      // error handling with thrown t
  } catch (S s) {
      // error handling with thrown s
  }
  ```
  - if the current `try` block is insufficient for handling the exception, the control is resided to *its* enclosing `try` block, which might be the caller fo the current function
    - this process is called **stack unwinding** (stack as in call stack)
    - destructors for class-typed local variables are guaranteed to be called, unless the exception occurred during their constructors
    - instead of having the class handle all primitive/pointer members, use wrappers that handle their own memory, like `std::unique_ptr<T>` instead of `T*`

- error log `std::cerr` similar to `std::cout`

- **catch-all**: `catch(...)`

# Exception as Object
- primarily to distinguish different types of exceptions in the catch statement
    - if all exections were `int` codes, the `catch(int e)` block will need to handle various different types
    - instead, define `MyException` and catch for `const MyException&`
- exception classes follow inheritance just like normal classes
    - base exception object catchers will also catch derived exception objects
- ``std`` library functions throw exceptions that inherit from `std::exception`, which is a small interface class in `<exception>`
    - for most of the time, if custom exceptions weren't used, catching `std::exception` is a good enough alternative to catch-all
    - `std::exception::what()` returns a description of the exception
- as `std::exception` is an interface, it cannot be directly instantiated
    - instead, instantiate `std::runtime_error` (located in `<stdexcept>`) with a string
- `std::exception` can be derived to create custom exception classes
    ```c++
    #include <exception>
    #include <string>
    #include <string_view>

    class ArrayException : public std::exception {
        private: std::string m_e{};
        public:
        ArrayException(const std::string& e): m_e{e}{}
        const char* what() const noexcept override {
            return m_e.c_str();
        }
    };
    ```
  - `noexcept` is an addition since `C++11` and forces the current (and derived, if `virtual`) implementations to be non-throwing
    - all functions without the `noexcept` keyword is considered to be potentially throwing
    - as a result, `noexcept` functions can only call other `noexcept` functions
  - since `what` in `std::exception` is a `virtual noexcept` function, all derivations must also mark it as `noexcept override`, otherwise the implementation will not be resolved
  - when deriving from `std::runtime_error` instead of `std::exception`, the `what` function does not need to be overriden
    - instead, just provide a `MyException(const std::string& msg)` constructor that uses the `std::runtime_error(const std::string& error)` constructor

# Rethrowing
- throw an exception within the `catch` block in order to pass it to the caller's handler
- note that throwing the original exception may cause slicing
    ```c++
    catch(Base& base) { // here, the exception is copy-initialized into the base parameter
        throw base;
    }
    ```
- instead, just use they `throw` keyword by itself to throw the whole unsliced exception
    ```c++
    catch(Base& base) {
        throw;
    }
    ```

# Function Try Block
- ```c++
  Derived() try: Base{} {
      // ...
  } catch (MyException e) {
      // catches exceptions from the base and derived constructors
      // by the end of the catch block, if the exception is not explicitely re-thrown, it will be rethrown implicitly
  }
  ```
  - note that this syntax can be used outside constructor functions, even non-member functions, but it is very rarely used that way
- note that the object is considered "dead" upon catching the exception in the function try block
  - for this reason, cleaning up the object will cause undefined behavior

# Caution
- due to the unpredictable flow of the program within `try`, be careful when cleaning up resources
  - opened files may not be closed
  - dynamically allocated resources may not be deallocated
    - note that this cannot be handled in `catch` because the variable is not in scope (unless it is declared outside of the `try` block)
    - an alternative way to handle it is to immediately assigned the initialized value to a smart-pointer, which cleans up after itself
- exceptions should *never* be thrown in destructors
- exception handling does add a small cost, so use exceptions when the issue is rare, serious, and the context itself does not provide a good way to just return an error code
  
# Exception Specifications
- most uses are deprecated as a language feature, except `noexcept`
## `Noexcept`
- `noexcept` actually takes a parameter
  - `noexcept(true)` means no throwing
  - `noexcept(false)` means potentially throwing
- Destructors are `noexcept` by default
  - for implicitly declared functions,  `noexcept` is the default for
    - all default constructors
    - all default assignment operators
    - all default comparison operators
    - if any of the above happen to call a non-`noexcept` function/operator, then they are no longer marked `noexcept` implicitly
  - outside the above, all functions, constructors, and operators are marked non-`noexcept`
- `noexcept` can also be used as an operator
  - `noexcept(expr)` evaluates whether `expr` involves potentially-throwing statements

## Exception Safety
- this was mentioned before in the operator overloading section
- levels (each in addition to the previous)
  1. no guarantee
  2. basic guarantee: no leaks, object still usable
  3. strong guarantee: no side effects upon failure
  4. no-throw: doesn't throw anything upon failure
  5. no-fail: always succeed

- be careful when to use `noexcept` because removing later may damage other code that rely on it
- compilers and the `std` library will notice `noexcept` markers and use it to optimize the underlying code paths

## Dynamic Exception Specifiers
- similar to `Java`'s `throws` keyword
- removed in `C++ 17` and `C++ 20` due to several factors from compiler, syntax compatibility with templates, and misuse
