# Syntactic Sugar
- digit separator `'`
  - `long x { 123'345'456 };`
- binary values
  - `0b`
- `auto` return type for template/non-template functions

# Improvements on `C++ 11`
- relaxed `constexpr`
  - `constexpr` functions can have conditions, loops, and local variables
    - the demo shows a recursive fibonacci at compile time using `constexpr`
- variable templates
  - ```c++
    template <typename T>
    constexpr T c = T(1317.129312903712093712937L);

    template<>
    constexpr char c = '\0'; // c specialized for type char

    template <typename T>
    T calculate(T t) {
        return c<T> + t; // instantiate templated variable with the apparopriate precision
    }
    ```
- `[[deprecated]]` attribute above entity declaration
  - additionally, `[[deprecated(msg)]]`
  - compiling with references to deprecated code will yield warnings, but succeed

# Standard Library Improvement
- smart pointers (detailed in learn cpp notes)
- defined literals
  - chronos library and the time literals
    - `std::chrono::seconds` as a class, etc.
    - that class is associated with literals
    - time classes can cast automatically with eachother
  - `auto s = 10s;` 10 seconds
    - equal to `std::chrono::seconds s{10};`
- string deduction
  - by default, string literals are deduced to `const char*` when used with `auto`
  - can use `s` suffix to specify deduction to `std::string`
  - `auto str = "foo"s;`
  - the same suffix can be used in different contexts (see same use of `s` for seconds)
- `std::tuple`
  - located in `<tuple>`
  - heterogeneous data, generalized `std::pair` to `n` elements
  - fetch element by type, if that type only occurs once
    - `std::get<double> student` to get `double score` (while everything else in the tuple is non-`double`)

# Lambda Improvements
- generic/polymorphic lambdas: use `auto` in parameter list
- captures can be used with `std::move` to move uncopyable data
  - `[ p{std::move(q)} ] () {}`

# C++ 17
- nested namespace
  - `A::B::C{}`
  - no need for nested brackets
- variable definition in `if`
- `if constexpr` evaluates at compile time
  - a common use case is to doing typechecks within template function
    - `if constexpr (checkThatTIsSomeType);`
    - note that this is evaluable because template functions are instantiated at compile time
- structured bindings
  - 