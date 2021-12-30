# Characteristics of the C/C++ Language

- Compiled
- "Trust the programmer" principle: low level, high performance, precise memory control
- Wide compatibility for ANSI C (C89) and ISO C (C90)

# Programming as Solving a Problem
1. Formulate the problem
2. Determine *how* to solve the problem
   * a good solution is clear, modular, well-documented, and robust
3. Program (``.cpp`` files)
4. Compile (``.cpp`` $\to$ ``.o``)
5. Linking (``.o``, ``stdlib``, ``other libraries`` $\to$ ``.exe``)
6. Test
7. Debug

# Syntax & Errors
- rules to construct a program with the C++ language, such that it is valid
- breaking syntax rules result in the compilor giving a *syntax error*
- *ex.* 
   ```c
   [Running] cd "/Users/alantao/OneDrive - Georgia Institute of Technology/hobby/courses/notes/Learn_CPP/" && g++ 0_first.cpp -o 0_first && "/Users/alantao/OneDrive - Georgia Institute of Technology/hobby/courses/notes/Learn_CPP/"0_first
   0_first.cpp:6:39: error: expected ';' after expression
      std::cout << "Hello World" << endl // print (send text "to" std::cout)
                                       ^
                                       ;
   1 error generated.
   ```

# 1.1 Quiz Incorrect
- **What is a function**: a block of code that may take input/output and could be called
   * *correct answer: a series of statements that are run sequentially*
- **Syntax error**: violating grammar rules of the language
   * *correct answer: + at compile time*

# 1.2 Comments
## Single Line
```c
// this is a single line comment
```
## Multi Line
```c
/* first instance */

/* This is purely a beautified format
 * removing the starting * per line does not have
 * actual effect
 * /
```

## Proper Use
- For description of what a something does
  - use the proper level of description (this program, this function, these following lines, ...)
- Describe step-by-step procedure **within** a program/function
  - *ex.*
      ```c
      // to ..., we will
      // 1. do a
      // 2. do b
      // 3. return c
      ```
- At the single line level, comments describe *why* something is written, in a way that is meaningful
  - *ex. bad*
      ```c
      // add a to b
      b += a;
      ```
  - *ex. good 1*
      ```c
      // simulate a die toss (from 0 to 5)
      d = std::rand() % 6;
      ```
  - *ex. good 2*
      ```c
      // linked list used instead of array because ...
      ```
      - this helps remind self about why certain choices were taken
- Commenting out code
  1. to test where syntax errors occurred (if without hints)
  2. to remove broken code
  3. to isolate a semantic section, in order to find semantic errors
  4. to mute alternative snippets

# Objects and Variables
- *def.* **data**: any information that can be moved, processed, or stored by the computer
- *def.* **RAM**: *aka. memory* an "array" storing "values"
- while c++ does not provide direct access to memory, it is allowed to access memory through objects
- objects could be *named* or *unnamed*
  - *def.* **variables**: named objects
    - *def.* **identifier**: the name of a variable

## Definition
   ```c
   int x; // DEFINES a variable of type int called "x"
   ```
   - at runtime, a definition causes a instantiation
   - *def.* **instantiation**: at run-time, allocating a fixed number of memory and recognizing that it belongs to some object of a certain data type
   - *def.* **data type**: tells the compiler what type of value the object will store; tells the program how to *interpret* memory
     - in c++, the data type MUST be known at compile-time
   - multiple definition:
      ```c
      int a, b; // not recommended, bad for comment clarity
      ```

   - ## Quiz 1.3 Incorrect
## Declaration
   - **Copy Assignment**:
      - *ex.*
        ```c
        int a;
        a = 10; // = is the assignment operator
        ```
        Doing so *copies* 10 into the memory region associated with $a$
   - **Initialization**
      - *def.*: define and assign at the same time\
      <br>
      1. Copy Initialization
         ```c
         T a = b; // copies RHS to LHS
         ```
      2. Direct Initialization
         ```c
         // selects the matching constructor for type T
         // then write the result directly to a
         // ! narrow conversion is permitted in some compilers
         T a(b);
         ```
      3. (Direct) Brace Initialization
         ```c
         // basically direct initialization,
         // but narrow conversion is NOT permitted
         int width { 4.5 }; // --> throws error
         ```
      - value initialization (see [code](./1_first.cpp))
        - zero initialization: the special case of value initialization when it is defaulted to 0
      - **prefer direct brace when possible**
  
## Basic IO
- Insertion operator ```<<```