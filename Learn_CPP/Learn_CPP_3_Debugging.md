- [Syntax and Semantic Errors](#syntax-and-semantic-errors)
- [Strategy for Debugging](#strategy-for-debugging)
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
        ```c
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
    * 