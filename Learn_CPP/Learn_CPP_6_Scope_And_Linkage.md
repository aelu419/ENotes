- [Blocks](#blocks)
  - [Namespace Alias](#namespace-alias)
- [Local Variables](#local-variables)
- [Global Variables](#global-variables)
- [Variable Shadowing](#variable-shadowing)
- [Internal Linkage](#internal-linkage)
- [External Linkage](#external-linkage)
- [Why Avoid Non-Const Global Variable](#why-avoid-non-const-global-variable)
  - [initialization order problem](#initialization-order-problem)
  - [When to use Non-Const Global Variable](#when-to-use-non-const-global-variable)
- [Global Constants Across Files](#global-constants-across-files)
  - [Pre C++17](#pre-c17)
  - [Post C++17](#post-c17)

# Blocks
- *alias* compound statement, block statement
- *def.*: zero or more statements treated by the compiler as one single statement
- ```{ statements }```
- ```c
  //outer block
  {
      // inner block / nested block
      {
          //...
      }
  }
  ```
- *def.* **nesting level/nesting depth**: maximum number of nested blocks within a function
    - compilers typically support ~100 levels, but it varies
    - the standard states it supports 256
- a good practice is to keep nesting level below 3

# Namespace
## Namespace Definition
```c
namespace foo {
    // define variables and member functions here
}
```
- *def.* **user-defined namespace**: as above
- it is possible to define a namespace across different files
  - for example, the ```std``` namespace goes across many different files like ```iostream```, ```cmath```, etc.
  - *DO NOT EXTEND ``std`` YOURSELF!*
  - define namespaces in header files
- Nested namespaces
  - *ex.1* nested definition
    ```c
    namespace foo {
        namespace goo {
            // ...
        }
    }
    ```
  - *ex.2* "direct" definition (C++17)
    ```c
    namespace foo::goo {
        //...
    }
    ```
- note that:
  - in C++, namespaces are primarily to prevent collisions, not to provide information hierarchy (as in C#). Do not overly nest namespaces even if the practice is common in C#
    - *ex. C++* ```std```
    - *ex. C#* ``UnityEngine.Experimental.Rendering. ...`` 
## Namespace Alias
```c
namespace active = foo::goo;
active::bar();
```

- use namespaces when exporting your code as a library
- separate out reusable (cross-project, potentially) code into namespaces like ``physics`` and ``math``, instead of having them together with application-specific code

# Local Variables
- within a scope, variable identifiers need to be unique
- *def.* **storage duration**: rules governing the creation and destruction times of a variable
  - **automatic storage duration**: created at point of def., destroyed at end of scope
  - duration is *not* the same as **lifetime**, which is the actual runtime times of creation/destruction
- local variables have *no* **linkage**
  - *def.* whether other declarations of the name refer to the same object
  - whereas scope governs singular declarations, linkage governs multiple declarations
- a good practice is to define variables in the most limiting scope

# Global Variables
- declared outside of functions
- *naming* start with ```g``` or ``g_``
- *def.* **global scope**: visible from point of definition until the end of file
- *def.* **static duration**: created upon program start, and destroyed upon program end
- unlike local variables, global variables are zero-initialized by default

# Variable Shadowing
- *alias.* name-hiding, shadowing
- there is no limit for shadowing between nested blocks
  ```c
  int foo;
  if (bar) {
    int foo; // this shadows the outer foo
  }
  ```
- shadowing should generally be avoided

# Internal Linkage
- if different files have variables ```foo``` both with internal linkage, then those variables will be treated as independent (per file)
- *def.* **internal variables** global variables with internal linkage
  - for ``const`` and ``constexpr``, they have internal linkage by default
  - for other global variables, use ``static``
    - ``static`` is a **storage class specifier**
- using ``static`` on functions make them only visible within the file

# External Linkage
- methods and non-constant variables are by default external
- for constant variables to be interenal, use ``extern`` on global
- *def.* **forward declarations** for variables, similar to function prototype declarations but with ``extern``
  - in this context, ``extern`` just denotes the statement being a forward declaration, instead of being a storage class
  - *ex.*
    - in ``a.cpp``
      ```c
      int g_x {2};
      extern const int g_y {3};
      ```
    - in ``main.cpp``
      ```c
      extern int g_x;
      extern const int g_y;

      int main() {
        // use g_x and g_y here
      }
      ```
- technically, all variables have *file* scope
  - they can be used outside, but it needs external linkage to be specified
- but informally, this *file scope + extern link* setup is called a *global scope*

# Why Avoid Non-Const Global Variable
- makes program path unpredictable
- makes functions less self-contained (and thus harder to test)
- good practices include
  - using local variables, and keep declarations close to point of use
  - do not use global variables, at least not for decision making
## initialization order problem
1. **static initialization** runs first, initializing everything that has constant/literal right hand side
2. **dynamic initialization** runs next, initializing everything with non-constant right hand side (method call, for example)
- do not have global variables depend on the values of other global variables, since the dependency might not be initialized yet
- the same order issue exists between different files

## When to use Non-Const Global Variable
- as log file
  - *ex.* ``std::in``, ``std::out``
- good practices include
1. wrap with namespace
2. encapsulate using accessor/modifier functions
   * this way there is no need for ``extern``
   * *ex.*
      ```c
      namespace constants {
        const int size;
      }

      int getSize() {
        return constants::size;
      }
      ```

# Global Constants Across Files
## Pre C++17

## Post C++17

