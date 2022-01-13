- [If-Else](#if-else)
- [Switch](#switch)
  - [Fallthrough & Scoping](#fallthrough--scoping)
- [Goto](#goto)

# If-Else
- a good practice is to put brackets around single statements
- dangling else paired up with last unmatched if
  - *ex.*
    ```c
    if (a)
        if (b) // else is paired to this if
            foo();
    else
        bar();
    ```
- this issue can be prevented by blocking the nested if's
- *def.* **null statements**: ``;``

# Switch
- *ex.*
    ```c
    switch (<cond>) {
        case <val1>: // these are called labels
            //...
        case <val2>:
            //...
        default:
            //...
    }
    ```
- ``cond`` must evaluate to an integral type
  - the reason is that ``switch`` is meant to be highly optimized. on the back side, it is implemented using ``jump tables`` which acts like a indexable array
  - this also means that switch cases have weird collisiions such as between ``54`` and ``'6'`` (which has ASCII of 54)

## Fallthrough & Scoping
- without ``break``/``return``, execution continues (and overflows into subsequent cases)
  - for C++17, ``[[fallthrough]]`` can be used to replace ``break`` to indicate intentional fallthrough (although not using it is also legal)
  - *ex.*
    ```c
    switch(v) {
        case a:
            // ...
            [[fallthrough]];
        case b:
            // ...
            break;
    }
    ```
- variables can be *declared* but not *initialized* outside labels
    - *ex.*
        ```c
        switch(v) {
            int a; // this is legal
            int b {}; // this is illegal!
            case 0:
                a = 10; //legal
                break;
        }
        ```
- label-internal variables share scope with other labels
  - *ex.*
    ```c
    switch(v) {
        case 0:
            int x; // this (declaration) is legal
            int y {}; // this (initialization) is illegal

            break;
        case 1:
            x = 5; // this (assignment) is legal
            break;
    }
    ```
  - The reason why initializations are illegal is that they require *execution at runtime*, meaning that the compiler requires all variables to be initialized throughout the *entire* scope
    - to solve this issue, open a new scope
      - *ex.*
        ```c
        case 0: {
            int y {};
            break;
        }
        ```

# Goto

