- [Syntax and Semantic Errors](#syntax-and-semantic-errors)
- [Strategy for Debugging](#strategy-for-debugging)
- [Integrated Debugger Commands](#integrated-debugger-commands)
  - [Step In](#step-in)
  - [Step Over](#step-over)
  - [Step Out](#step-out)
  - [Step Back](#step-back)
  - [Flow Change](#flow-change)
  - [Variable Tracking](#variable-tracking)
  - [Call Stack](#call-stack)
- [Defensive Programming](#defensive-programming)
- [Other Coding Practices](#other-coding-practices)
# Syntax and Semantic Errors
- *def.* **syntax error**: compile time error violating the language's grammar rules
- *def.* **semanti error**: runtime error violating the programmer's intent

# Strategy for Debugging
1. Find the root cause
   * Reproducing the problem using a minimalistic setting: reduce the method calls and passed data down to the absolute minimal amount. Sometimes the reproduction steps are not precise and the error only manifests in a percentage of runs. This is acceptible, but the lower that percentage is, the more time it takes to diagnose the source.
   * **hi-lo** strategy: the erroneous code must be
     * *after ($\ge$) the last observed bit of healthy state*
     * *before ($\le$) the first observed bit of unhealthy state*
2. Understand the problem
   1. Commenting: comment a segment of code and see if the error persists
   2. Validate Flow: print something upon certain function calls (or breakpointing at the function)
   3. Print Values: verify that variables have correct values at correct points in the execution
3. Determine a fix
4. Repair the issue
5. Retest

- use ```std:cerr``` instead of ```std:cout``` when printing errors, because the former is not buffered while the latter is
- do not indent temporary debug lines. this makes them easily spottable later when needing to remove them
- prevent the need for removing debug lines
  1. use directives to guard debug lines
    - *ex.*
        ```c++
        #define ENABLE_DEBUG

        //...

        #ifdef ENABLE_DEBUG
        std::cerr << "...";
        #endif
        ```
  2. use debug logger
    * *ex.* ```plog``` library
    * prevents the program from clutteriing the terminal output
  3. use integrated debugger

# Integrated Debugger Commands
## Step In
- pause to examine after executing the next line in normal program flow

## Step Over
- pause to examine after evaluating the next statement (skipping all called functions unless other breakpoints are specified inside them)

## Step Out
- finishes all code in *the current function* and then pause

## Step Back
- available in a few debuggers, rewinds to the last step (what exactly counts a step varies by debuggers since it is not yet a standardized feature, due to the complexity of implementation)

## Flow Change
- Jumping, Continue, Starting, etc.

## Variable Tracking
- different tracking window options for global, local, etc.

## Call Stack
- trace method calls

# Defensive Programming
- anticipate misuse when coding by
  - end users
  - fellow developers

# Other Coding Practices
- unit testing
- constraints (assert)
- using *static analysis tools* (linters)
  - most C++ compilers already act as such