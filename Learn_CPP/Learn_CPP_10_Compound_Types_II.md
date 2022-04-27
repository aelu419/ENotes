- [Program Defined Types](#program-defined-types)
- [Enums](#enums)
  - [Unscoped Enum](#unscoped-enum)
    - [IO](#io)
  - [Scoped Enums](#scoped-enums)
- [Structs](#structs)
  - [Member Selection](#member-selection)
  - [Aggregate Initialization:](#aggregate-initialization)
  - [Designated Aggregate Initialization](#designated-aggregate-initialization)
  - [Default Member Initialization](#default-member-initialization)
  - [Structs as Arguments](#structs-as-arguments)
  - [Structs as Return Values](#structs-as-return-values)
  - [Struct Nesting](#struct-nesting)
  - [Struct Size & Alignment](#struct-size--alignment)
  - [Member Selection with Pointers](#member-selection-with-pointers)
- [Class Templates](#class-templates)
  - [Class Template Argument Deduction (CTAD)](#class-template-argument-deduction-ctad)
- [Using Language Reference](#using-language-reference)

# Program Defined Types
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

# Enums
- can define *enumerated types*
- *enumerable types* can be enumerated by *enumerators*

## Unscoped Enum
- ``enum`` marks an unscoped enumeration
  - each value in an ``enum`` is an enumerator
  - *ex.* ``red`` in ``Color``
  - each ``enum`` is a distince type at compile time, even if underlying values are the same
  - ``Color a { dog };`` illegal
  - *unscoped* here just means that the values within the ``enum`` bracket has the same scope as the ``enum`` itself, which means it is not "creating scope"
    - ```c++
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
  ```c++
    enum Color {
        red = -5,
        green = 77,
        blue = 0
    }
    ```
    - note that this allows for duplicate ``enum`` values, which is not recommended
- there is no straightforward way to print ``enum`` names
  ```c++
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
    ```c++
    enum Color : std::uint8_t
    {
        //...
    };
    ```
- unscoped ``enum``s are in global namespace by default
  - the same ``enum`` values collide within the same namespace
  - this happens at compile time within the ``enum``s, so whether the use write ``Enum::val`` or ``val`` does not matter
  - a way to avoid this issue is to wrap the enum in a namespace
### IO
- to input, first store the input in a ``int`` and then use ``statc_cast<Enum_T>``

## Scoped Enums
- ``enum class``
- strongly typed (don't implicitely convert to integers)
  - *ex.*  cannot do ``red + 5``
  - integer valued initialization is allowed since ``C++ 17``
  - this can be abbreviated using operator overload
    ```c++

    enum class Enum_T {
        element
    };

    constexpr auto operator+(Enum_T e) noexcept {
        return static_cast<std::underlying_type_t<Enum_T>>(e);
    }

    // ...
    std::cout << +Enum_T::element; // converts element to int
    ```
- strongly scoped (only placed in the scope region of the ``enum`` itself)

# Structs
- convention: capitalize struct type name, lower cammel case member name
- struct object definition: ``Struct_T foo;``
- note that struct members are not initialized by default, and will be set to whatever is currently in the memory
- *def.* aggregate
  - any type that contains multiple data members (struct,  array, etc.)
  - a ``C++ aggregates`` must satisfy
    - is a class or array type
    - no private or protected non-static data members
    - no user-declared or inherited constructors
    - has no base class
    - has no virtual member functions

## Member Selection
- ``operator.`` for member selection
## Aggregate Initialization:
  ```c++
  struct Player {
      int health;
      int id;
  };

  // ...
  Player bob { 100, 12345 };
  ```
  - within an aggregate initialization, all the filled-out members are initialized member-wise
    - all missing members are value-initialized (filled w/ initial value)
  - *ex.* ``Player p { 100 };`` will leave ``id`` as ``0``
- ``struct`` objects can be made constant
- the assignment equivalence of aggregate init. is a *list assignment*
  - *ex.* ``p = { 99 };``
  - similarly, members left outside of a list assignent is value-initialized
  - the above operation clears ``id`` to ``0``
  - to avoid, assign its value back to itself ``p = { 99,  p.id };``
- note that a simple way to member-wise, value-initialize is to have ``Player p{};``
- ``struct``s can be initialized with ``struct``s
  - ``Player p2{ p1 };``

## Designated Aggregate Initialization
- ``C++ 20`` feature
- associate member names in agg. init.
  - *ex.* ``Player p { .id{100}, .health{0}};``
- note that list assignment can also use the designated format

## Default Member Initialization
- *ex.*
```c++
struct Player {
    int health {100};   // explicit default value
    int money {};       // default follows value init
    int id;             // unhandled
};

// ...
Player p1; // 100, 0, unitialized
Player p2 { 50, 20, 1 }; // 50, 20, 1
Player p3 {}; // 100, 0, 0
```
- note that even without explicit value-initializer for the struct object ``p1``, the default initializers for ``money`` and ``health`` still run
- best practice
  - try to provide default values whenever possible
  - prefer value-initializing struct object over simply defining it

## Structs as Arguments
- by default, the entire struct is copied as argument, which is expensive
- usually, struct objects are passed as ``const`` reference to avoid copying
    ```c++
    void printPlayerStats(const Player& player) {
        std::cout << player.health << std::endl;
        // ...
    }
    ```

## Structs as Return Values
- returning an unnamed, temporary struct object
    ```c++
    return Player { 1, 2, 3 };
    ```
    - the ``Player`` is actually not necessary when the struct type could be inferred

## Struct Nesting
- nesting as member value
    ```c++
    struct Player {
        int id{};
    };

    struct Game {
        Player p1{};
        Player p2{};
    }
    ```
- nesting also within the namespace
    ```c++
    struct Game {
        struct Player {
            int id{};
        }
        Player p1{};
        Player p2{};
    }
    //...
    Game::Player p3{}; // access Player within Game's namespace
    ```
## Struct Size & Alignment
- usually,  structs are padded and aligned to fit to system word width
- this has impacts on both the struct size and overall performance for accessing members

## Member Selection with Pointers
- member selection does not work on pointers
- can be dereferenced first and then reach members ``(*playerPtr).id``
- member selection from pointer operator ``->``
  - ``playerPtr->id``
  - implicitely deferences
- a mix of both member accessing methods
  - ``gamePtr->player.id``
  - game pointer dereferences to a ``Game`` which yields a ``Player`` object
  - the ``id`` member of the ``Player`` object can be accessed normally

# Class Templates
- *def.* **class type**L a ``struct``, ``class``, or ``union``
- for instantiating class types
  ```c++
  template <typename T>
  struct Vector3 {
    T x{};
    T y{};
    T z{};
  };

  //...
  Vector3<float> p { .5f, 1, 0 };
  Vector3<int> q { 0, 1, 2 };
  ```
  - similar to the function template, the compiler generates necessary struct definitions
- class template can be used in functions as parameters
  ```c++
  constexpr float norm(Vector3<float> v) {
    // v.x, v.y, v.z
    // ...
  }
  ```
- function template can be used to take class templates
  ```c++
  template <typename T>
  constexpr T getX(Vector3<T> v) {
    // ...
  }
  ```
  - the function template type can be explicit or deduced upon compile
    - ```getX<int>(p)``` vs. ```getX(p)```
- class templates can have multiple tempalte types: ``template <typename T, typename U>``, ``tuple<int, float>{ 0, 0f }``
- class templates are typically defined in header files and then included upon use
## Class Template Argument Deduction (CTAD)
- ``C++ 17`` feature
- the compiler deduces class template type upon initialization, when all types are omitted
  - *ex.* ``Vector3 v{ 0, 1, 2 };`` $\to$ ``int``
  - this is done by providing a deduction guide to the compiler
    - *ex.*
      ```c++
      template <typename T, typename U>
      pair(T, U) -> pair<T, U>;
      ```
  - deduction guides are no longer needed in ``C++ 20``

# Using Language Reference
- ``C`` reference is not always the same with ``C++`` reference for the same search
- official references may be different from technical specifications, the latter details upcoming features instead of implemented ones