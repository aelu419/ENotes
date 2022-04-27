- [If-Else](#if-else)
- [Switch](#switch)
  - [Fallthrough & Scoping](#fallthrough--scoping)
- [Goto](#goto)
- [Loops](#loops)
  - [While](#while)
  - [For](#for)
- [Exits](#exits)
  - [Break](#break)
  - [Continue](#continue)
  - [Standard Exit](#standard-exit)
- [Intro to Testing](#intro-to-testing)
  - [Informal Testing](#informal-testing)
  - [Preserving Tests](#preserving-tests)
  - [Automating Tests](#automating-tests)
  - [Integration Testing](#integration-testing)
  - [Code Coverage](#code-coverage)
- [Common Semantic Errors](#common-semantic-errors)
- [Detecting and Handling Errors](#detecting-and-handling-errors)
  - [Handling Errors](#handling-errors)
  - [Standard Input Invalid](#standard-input-invalid)
  - [Input Validation](#input-validation)
- [Assertion](#assertion)

# If-Else
- a good practice is to put brackets around single statements
- dangling else paired up with last unmatched if
  - *ex.*
    ```c++
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
    ```c++
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
    ```c++
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
        ```c++
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
    ```c++
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
        ```c++
        case 0: {
            int y {};
            break;
        }
        ```

# Goto
- unconditional jump
- *ex.*
    ```c++
    statementLabel:
        foo();
        bar();
        goto statementLabel;
    ```
- cannot jump outside function
- cannot jump forward/backwards over initialization
- goto should be avoided due to its promotion of sphagetti code (unclear execution path)

# Loops
## While
```c++
while(condition)
    statement;
```
- for intentional infinite loops, use ``while(true)``
- loop variables should always be signed
```c++
do
    statement;
while(condition);
```
- ``do-while``s are less commonly used due to their unclear nature
  - it is clearer to have the loop condition at the top

## For
```c++
for (init; cond; update)
    statement;
```
- ``for`` loops can also produce infinite loops
```c++
for (;;)
    statement;
```
- multiple variables
  - *ex.*
      ```c++
      for(int x{}, y{}; x < 10 && y < 10; x++, y--)
          ;
      ```

# Exits
## Break
- exit the entire loop
## Continue
- exit the current iteration and start the next (after evaluating conditions)

- a good practice is to use ``break`` and ``continue`` only if they add more clarity to the code
  - use the same for early ``return``s (i.e. ``return`` statements that aren't the last one in the function)

## Standard Exit
- ``std::atexit(*void())`` sets the function to call at program exit
  - *ex.* runs ``foo`` after ``a``, and exits the program immediately
    ```c++
    foo() {
        // clean up code
    }
    
    int main() {
        std::atexit(foo);
        //...a
        if(cond)
            std::exit();
        //...b
        return;
    }
    ```
- it is possible to register multiple clean up programs, they will be called by the reverse order by which they are registered
- clean up functions are necessary because ``std::exit()`` skips over necessary program clean up
- ``std::abort()`` terminates the program abnormally
- ``std::terminate()`` is called implicitely upon exception not handled
  - it calls ``std::abort()`` within
- for best practice, avoid using these

# Intro to Testing
- *def.* **scope creep**: adding features not originally planned for
- write modularly and minimally, test early and often

## Informal Testing
- write tests that are not formally defined (as unit tests, or have assertions)
  - *ex.* call ``foo`` in ``main`` and print some outputs, see if those outputs are right

## Preserving Tests
- move informal testing code to a separate function

## Automating Tests
- make the testing function decide whether the outputs are correct or not, instead of manually examining
- **unit testing frameworks** are usually used for this purpose

## Integration Testing
- test the coordination of many different features

## Code Coverage
- *def.* descriptions of the portion of the program that is exercised in the testing process
    1. **statement coverage**: percent of statements ran
    2. **branch coverage**: percent of branches encountered
    3. **loop coverage**: aka the 012 test: every loop should be ran under 0, 1, and 2 iterations. if those work, then the loop is unlikely to have further issues
- cover different inputs by using **equivalent categories**

# Common Semantic Errors
1. conditional logic error (``>=`` vs. ``>``)
2. unintentional infinite loops
3. off-by-one (in loops, the end condition)
4. operator precedence
5. floating point precision issues
6. integer vs. floating point division
7. accidental null
8. incorrect scoping
  
# Detecting and Handling Errors
- in defensive programming, it is good practice to anticipate the errors and handle them gracefully
  - i.e. testing the **sad paths** of the program, instead of just the **happy path**
## Handling Errors
1. handle within the function
   * prompt again, or cancel the operation (and print)
2. pass error back to caller
   * *ex.* changing a ``void`` function to a ``bool``, with the return value denoting method exit state
3. fatal errors
   * preemptively exit the program, using halting statments such as ``std::exit(n)``
4. exceptions
   * see ``throw``-``catch`` later on
   * mainly for passing the error back to the caller
## Standard Input Invalid
- slightly more detailed explanation for the extraction operator ``>>``
  - *def.* **buffer**: piece of memory for temporarily storing data
  - if there is already data in the buffer, ``std::cin`` extracts from it
  - other wise,
    - user prompted to input, stops upon ``ENTER``
    - extracts as much data as necessary
    - the rest is left over
## Input Validation
- types of validation
  - **inline**: validate *as* input
  - validate the string before type conversion
  - let ``std::cin`` handle the conversion, and handle the error
- common input issues
  - meaningless input passing extraction
  - additional inputs after a valid input
    - ``std::cin.ignore(n, '\n')`` ignores the first n characters or until a `\n` is reached
    - alternatively, use ``std::numeric_limits<std::streamsize>::max()`` which returns the largest number that a stream size can hold
      - use ``#include <limits>``
  - extraction fails
    ```c++
    if (std::cin.fail()) {
      // handle extraction failure
      std::cin.clear();
      ignoreLine(); // calls std::cin.ignore with streamsize max
    }
    ```
  - semantically makes sense, but overflows after extraction suceeds
    - in this case, ``std::cin`` goes into failure, but also clamps the input into range
  
# Assertion
- *def.* **precondition**: conditions that must be true before some code
- *def.* **invariant**: conditions that must be true *while* that code is executing
- *def.* **postcondition**: conditions that must be true after some code
- ``assert`` is a preprocessor macro residing in ``<cassert>``
  - *ex.* ``assert(a > b)``
  - *ex.* ``assert(a > b && "there is more of a than b")``
    - note that the string literal evaluates to ``true``, so this effectively adds a comment inside the code
- assertions are used for logically impossible events, while exceptions are used for more common errors
- assertions are sometimes used to indicate needing implementation
- use ``NDEBUG`` to compile without assertions and improve performance slightly
  - this is usually for shipped versions of the program
- ``static_assert(cond, msg)`` is evaluated by the compiler instead, and can check variables in the environment
  - *ex.* ``static_assert(sizeof(int)==2, "Size of int too large")``
- 