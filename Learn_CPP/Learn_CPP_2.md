- [Functions](#functions)
- [Function Parameters & Arguments](#function-parameters--arguments)
- [Local Scope](#local-scope)
- [Forward Declaration](#forward-declaration)
  - [Quiz 2.6 Incorrect](#quiz-26-incorrect)
- [Multi-file Programs](#multi-file-programs)
- [Namespace](#namespace)
  - [Standard Namespace (std)](#standard-namespace-std)
- [Preprocessor](#preprocessor)
- [Header Files](#header-files)

# Functions
- *def.* sequence of reusable statements written to perform one job
- nested functions are not supported
- *def.* **metasyntactic variable**: things like ```foo``` ```bar``` that are used as placeholders across languages
- *def.* **status code**: return value of the ```main``` function
  - c++ permits ignoring return statement for main, which defaults to $0$
- c++ disallows incompatible values sent/assigned to functions/variables
  - *ex.* ```std::cout << null``` leads to compiler error

# Function Parameters & Arguments
- *def.* **parameter**: the received data corresponding to a predefined slot from the function's perspective
- *def.* **argument** the data passed to the function from the caller's perspective
- as with C, in C++, arguments are passed *by value*, which means they are copied
  - however, there is no guarantee for the order by which these values are evaluated
    - *ex.*
        ```c
        std::cout << sum(a(), b());
        ```
    - in this example, either ```a()``` or ```b()``` could be called first
- Create functions when...
  - code is reused
  - code has clear inputs and outputs
  - code to perform one task
    - when a function becomes multi-tasked or too long, refactor

# Local Scope
- *def.* **scope**: a *compile* time property, stating where in the source code could the identifier be accessed
  - in C++, an identifier is accessible...
    - *from it's point of definition*
    - *until the first close ```}``` after that point
- local variables are destroyed upon function return
- *def.* **lifetime**: the time between a variable's creation and destruction, which is a *runtime* property
  - it is often up to the optimizer to decide when the machine actually creates and destroys variables
  - the order or creation/destruction often depends on their order on the *stack*
- it is good practice to define local variables close to their first use

# Forward Declaration
- functions are not accessbile *before* their point of declaration
- *def.* **declaration**: telling the compiler that an entity exists
- to avoid issues of keeping ```main``` at top, use **forward declaration**
  - *ex.*
      ```c
      int foo(int bar); // this is a FUNCTION PROTOTYPE for foo
      int main() {
          // call foo
      }
      int foo(int bar) { // this is a FUNCTION DEFINITION
          // ...
      }
      ```
    - note that function prototypes are *statements*
  - *ex.2* it is possible (although not recommended) to skip para meter naming in prototypes
    ```c
    int foo(int);
    ```
- a function protytpe without a defined function body will compile *unless it is actually called*
  - in that case, a linker error will occur
- **one definition rule**:
  - within a given file, a function, object, type, or template must have only one definition
    - violation causes a redefinition error in the compiler
  - within a program, an object or normal function can only have one definition
    - violation may cause a redefinition error in the linker
  - types, templates, inline functions are allowed to have identical definitions across different files
    - undefined behavior
- variable definitions are automatically declarations
- *def.* **pure declarations**: declarations that aren't definitions
## Quiz 2.6 Incorrect
- compiler does not detect mismatches between function prototypes and function definitions. that is what the linker does

# Multi-file Programs
- at compile time, function definitions outside the main ```.cpp``` file could be linked, as long as the compiler knows they are present
  - this is done by using function prototypes
  - *ex.*
    - ```main.cpp```
        ```c
        #include <iostream>
        int add(int, int);
        int main() { 
            // use add()
        }
        ```
    - ```add.cpp```
        ```c
        int add(int x, int y) {
            // definition
        }
        ```
- when splitting files, dependencies are included *per file*

# Namespace
- *def.* **naming collision**: at compile time, each file is compiled separately, so multiple definitions could be created at different points. at link time, this becomes an issue as the linker doesn't know which one to prefer
  -  the linker will throw an error even if the collision is for a function that is never called
- *def.* **namespace**: a scope with a name that identifiers could be associated with. within it, identifiers must be unique
  - to access, use ```namespace::identifier```, where the ```::``` is the **scope resolution operator**
  - *ex.* ```std::cout```
- *def.* **global namespace**: the default namespace that identifiers associate with when they are not defined in any namespace, class, or function
- !not encouraged
  - using ```using namespace``` directive
  - why? naming collision with other namespace identifiers
    - naming collision may occur with identifiers that are *later added* to the namespace, after your code is written

## Standard Namespace (std)

# Preprocessor
- translational stage before compilation that *manipulates the text of the code*
  - source code $\to$ translation unit (removed comments, ...)
- the preprocessor is *part* of translation
  - it scans through the file for preprocessor directives and perform the tasks they specify
    - ```using namespace``` directives are not *preprocessor* directives
  - ```#include <file>``` replaces the line with text from ```file```, it is used almost exclusively to include header files, but that is not the only use
  - ```#define identifier substitution``` replaces all instances of ```identifier``` in non-directive code with ```substitution```
    - usually, ```identifier``` is named in capitalized snake case, such as ```MY_NAME```
  - *def.* **object-like directives**: directives for literals used as constants (i.e. without parameter lists)
    - object-like directives could have *no* substitute text, meaning they would be replaced by *nothing*
  - conditional compilation macros
    - ```#ifdef id``` passes if ```id``` has been ```#define```d previously
      - ```#ifndef id``` does the opposite
      - ```#endif``` closes the scope
    - ```#if 0``` blocks
    - conditional directives *can* be nested
  - directives are *per file*, since they run as intermediate step of translation, which is a per file process

# Header Files
