- [Value Categories](#value-categories)
  - [LValue](#lvalue)
  - [RValue](#rvalue)
  - [LValue Reference](#lvalue-reference)
- [Pointers](#pointers)
  - [Null Pointer](#null-pointer)
  - [Const Pointer](#const-pointer)
  - [Pass by Address](#pass-by-address)
  - [Return by Reference/Address](#return-by-referenceaddress)
- [Type Deduction with Pointers/References](#type-deduction-with-pointersreferences)
  - [Top vs. Low Level Const](#top-vs-low-level-const)
  - [Deduction for Pointers](#deduction-for-pointers)
- [Program-defined Types](#program-defined-types)
  - [Structs](#structs)
  - [Enums](#enums)

# Value Categories 
## LValue
- left/locator value: a function or object that persists beyond the expression
  - i.e. a value with an identity (can be retrieved through ``&``)
  - lvalue expressions can be either modifiable or not (``const`` or ``constexpr``)
## RValue
- right value: values that don't persist beyond the expression
- note that the assignment operator checks at compile time that
  - left side must be modifiable lvalue
  - right side must be rvalue
    - lvalues are implicitly converted to rvalue

## LValue Reference
- *def.* **reference** alias for an existing object
- lvalue reference: ``<lvalue_t>& name``
  - *ex.* ``int& ref { x } `` for ``int x``
  - it is also acceptible to write ``<lvalue_t> &name``
  - note that ``ref`` can both be written to and read from, and the values will be passed to and from the same address as ``x``
- lvalue references must be initialized with a *modifiable* lvalue object
  - the process is called **reference binding**
  - the original lvalue object is called the **referent**
- note that lvalue references cannot change binding by assignment
  - the changing is called **reseating**
- the lifespan of the reference is *independent* from the referent
  - ```c
    int x { 0 };
    {
        int& ref { x };
    } // ref is destroyed while x persists
  - the opposite case where the referent is destroyed first is called **dangling reference**, leading to undefined behavior
- initializing a reference with another reference leads to the second reference binding to the *same* referent
  - it does not become a "reference to reference" as if for pointer
  
- *case.* reference to constant
  - the ``const`` modifier prevents writing to the referent
  - the accessibility of the reference is *independent* fromn that of the referent
  - *ex.*
    - ```c
      int x{0}; // can both read and write
      const int& ref{x}; // can read but not write
      ```
  - *spec. case* ``const int& ref {5};`` is legal and converts the rvalue 5 to a constant lvalue
- *appl.* pass by reference
  - ``foo (type_t& bar);`` for modifiable lvalue references
  - ``foo (const type_t& bar);`` for unmodifiable lvalue references
    - typically, prefer ``const`` ref to avoid accidental modifications
    - ``const type_t&`` accepts both modifiable and un-modifiable lvalue references, as well as rvalue references (which would be converted)
    - typically, prefer pass by ref for *complex* datatypes that are expensive to copy
      - for primitive types, just use pass by value

# Pointers
- ``&foo`` address of ``foo``
- ``*foo`` the ``type_t`` stored at a section of memory beginning at ``foo``
- declaration
  - ``type_t* foo`` preferable, ``type_t *foo`` accepted
  - ``type_t* foo, bar`` will only make ``foo`` the pointer
    - the correct way is ``type_t* foo, * bar``
- by default, uninitialized pointers are *null pointer*s
- typically, references are safer than pointers but are less versatile (only one level of pointing, no re-seating)
- the size of a pointer depends on the architecture and the executable
  - a ``32 bit`` machine means that the pointers are 32 bits large
  - likewise for ``64 bit``
  - the type of the pointer is implicitely referred by the type of the addressed variable
  - similar to lvalue references, pointers can be dangling
## Null Pointer
- value initializing without an address will make the pointer a null pointer
  - without value initialization, the pointer will be whatever is currently in the memory
  - ``int* bar{};``
  - the value of the null pointer is denoted with ``nullptr``
    - it is possible to initialize with
      - ``nullptr``: ``type_t* bar{ nullptr };``
  - implicitely, ``(bool) nullptr`` is ``false``
  - ``(int) nullptr`` is not necessarily 0, although most modern architectures use that by convention
- a passing null check does not guarantee non-dangling
- a good practice is to assign pointer ``nullptr`` after the pointed object is no longer valid
- ``nullptr`` is of the type ``std::nullptr_t``
  - note that this allows for function overloading that accepts a ``std::nullptr_t`` type, i.e. it only accepts ``nullptr`` as a valid argument, and it preceeds all other overloads for this particular value due to no need for conversion
- favor references over pointers for safety

## Const Pointer
- normal pointers cannot point to ``const`` values
- **pointers to ``const`` values** can point to ``const`` values, and *can* be re-seated
  - ``const type_t* ptr``
- **``const`` pointers** cannot be re-seatec but is read-only
  - ``type_t* const ptr``
- a ``const`` pointer to a ``const`` variable cannot be reseated or assigned value
  - ``const type_t* const ptr``

## Pass by Address
- pass by value, but for the pointer to the variable instead of the variable itself
- ``assert(ptr)`` halts the program in debug builds if ``ptr`` is ``nullptr``

## Return by Reference/Address
- must ensure that the referenced object has life span beyond the program itself
  - this includes both allocated memory in the heap
  - as well as static variables
- avoid returning non-const static variables
  - *ex.*
  - ```c
    const int& getItr() {
      static int itr{0};
      itr++;
      return itr; // here, the int itr is casted to int&
    }

    // when the receiving variables are references
    const int& a{getItr()}; // 1 first, then becomes 2 as itr goes to 2
    const int& b{getItr()}; // 2

    // when the receiving variables are non-references, the value is copied
    const int c{getItr()}; // 3
    const int d{getItr()}; // 4
    ```
- it is ok to return by reference if the reference is passed as an argument in the first place

# Type Deduction with Pointers/References
- type deduction drops references, which could be reapplied as ``auto&``

## Top vs. Low Level Const
- **top**: to the object itself, ``const int bar``, ``int* const ptr``
- **low**: to the reference, ``const int& ref``, ``const int* ptr``
- only *top* level ``const``s are dropped upon type deduction
  - note that for ``const int&``, the reference is dropped *first*, meaning that the ``const`` becomes a top level and also gets dropped, although it is meant to be low-level
- it is good practice to reapply ``const`` no matter if ``C++`` drops it or not, just for the sake of clarity

## Deduction for Pointers
- pointers are not dropped
- ``auto*`` vs ``auto``
  - ``auto*`` drops and reapplies the pointer
- const pointers
  - the same rules and best practices apply to pointers similarly as references
  - ``const auto* const ptr`` guaranteed constant pointer to constant value

# Program-defined Types
## Structs
- conventions
  - pascal case, no ``_t``
  - use header file with the same name as the program defined type
  - for every file referencing the type, the header file should be included
- type definitions do not need to adhere to one-definition rule
  - one per ``translation unit`` instead of per entire program
- program vs. user defined
  - user-defined includes types that are defined in non-core cpp libraries as well as actual custom types
  - *ex.* ``std::string`` is a user defined type
    - program-defined excludes these cases

## Enums
- ``enum`` marks an unscoped enumeration
  - each value in an ``enum`` is an enumerator
  - *ex.* ``red`` in ``Color``
  - each ``enum`` is a distince type at compile time, even if underlying values are the same
  - ``Color a { dog };`` illegal
  - *unscoped* here just means that the values within the ``enum`` bracket has the same scope as the ``enum`` itself, which means it is not "creating scope"
    - ```c
      enum Color {
        red,
        green,
        blue,
      }
      ```
   - for this reason, enum enumerators within the same scope cannot have the same name
     - this can be solved by wrapping with ``namespace``s

-  values can be explicitely assigned to enumerators
   -  duplicate values can be given to different enumerators within the same ``enum``
   -  however, this makes the ``enum`` non-distinct and thereby interchangeable

- ``enum``s can be assigned values explicitly
- ```c
  std::ostream& operator<<(std::ostream& out, Color color)
  {
    switch (color)
    {
    case black: out << "black";  break;
    case red:   out << "red";    break;
    case blue:  out << "blue";   break;
    default:    out << "???";    break;
    }

    return out;
  }
  ```
  - overrides the ``<<`` operator between ``std::cout`` and ``color``
  - usually, ``enum`` has the same size as ``int``
  - it is possible to specify underlying type explicitly
  ```c
  enum Color : std::uint8_t
  {
    //...
  };
  ```
- 