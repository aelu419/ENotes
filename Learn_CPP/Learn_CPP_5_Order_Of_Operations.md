- [Operator Precedence](#operator-precedence)
  - [5.1 Quiz Incorrect](#51-quiz-incorrect)
- [Special Arithmetics](#special-arithmetics)
  - [Modulo](#modulo)
  - [Exponent](#exponent)
- [Increment/Decrement](#incrementdecrement)
- [Comma](#comma)
- [Conditional](#conditional)
- [Comparisons](#comparisons)
- [Logical Operators](#logical-operators)

# Operator Precedence
- order of operation is determined by the compiler
  - each operator has an assigned *precedence level*
- at the same level of precedence, the compiler determines the order of evaluation is determined by the *associativity*
  - an operator can have ```L->R``` or ```R->L``` associativity
- a good practice is to use parenthesis on any complex and not-obvious expressions
- the variables themselves are not evaluated in order
  - *ex.*
    ```c++
    bar = a() + b(); // a and b are not guaranteed to be called in order
    ```
  - make sure not to assume any order, and write expressions in such a way that does not depend on order
## 5.1 Quiz Incorrect
- ```=``` is just another operator with ```R->L``` associativity. ```x=y=z``` evaluates to ```x=(y=z)```
  - all three variables will be set to ``z``

# Special Arithmetics
## Modulo
- ```x % y``` takes the sign of x and ignores the sign of y
  - the modulo of a negative number is just the negative of the modulo of the positive
## Exponent
```c++
#include <cmath>
// ...
std::pow(base, exp); // returns a double
```

# Increment/Decrement
- generally prefer the prefix version (```++x```) because it performs better and is more robust
- *def.* **side effect**: effects beyond expression evaluation
  - *ex.* ```a = ++x;``` has the side effect of incrementing ```x```
  - a good practice is not to depend on side effects *within* an expression
    - *ex.* ```y = x + ++x;```, increment could be evaluated earlier or later

# Comma
- ```(<expr1>, ..., <exprn>)``` evaluates from left to right, then returns ```exprn```
  - for some warning levels, the compiler may warn that the first n-1 expressions are unused, *if they definitely don't have side effects*
  - *ex.1* ```(1, 2, 3)``` $\to$ warning
  - *ex.2* ```(a++, b++, 3)``` $\to$ no warning

# Conditional
- ``` <cond> ? <expr1> : <expr2> ```
- make sure to put the entire expression inside a parenthesis because ```?``` has very low precedence
  - *ex.* ```std::cout << (x>y) ? x : y``` prints ```0``` or ```1``` depending on ```x>y```
- ```<expr1>``` and ```<expr2>``` must be *derived from* the same base type

# Comparisons
- floating point comparisons are fine as long as not for equality
- use epsilon for equality
    ```c++
    #include <cmath> // for std::abs()

    // epsilon is an absolute value
    bool isAlmostEqual(double a, double b, double epsilon)
    {
        // if the distance between a and b is less than epsilon, then a and b are "close enough"
        return std::abs(a - b) <= epsilon;
    }
    ```
  - absolute episilons are bad for large numbers, since significant figures do not always evaluate to the same scale
- Knuth's method
    ```c++
    // a common choice for epsilon is 1e-8
    return (std::abs(a-b) <= std::max(std::abs(a), std::abs(b)) * epsilon);
    ```
- Knuth's with optimization:
    - use absolute error first, if absolute doesn't pass, use relative

# Logical Operators
- C++ supports short circuit evaluations
- make sure to parenthesize, since different logical operations have different levels of precedence
