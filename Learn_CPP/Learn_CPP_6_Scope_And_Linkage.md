- [Blocks](#blocks)
  - [Namespace Alias](#namespace-alias)
- [Local Variables](#local-variables)

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
