- [Arrays](#arrays)
  - [Fixed Sized Array](#fixed-sized-array)
    - [Arrays as Parameters](#arrays-as-parameters)
  - [Iterating Arrays](#iterating-arrays)
  - [Multi-dimensional Arrays](#multi-dimensional-arrays)
- [String Implementations](#string-implementations)
  - [C-Style](#c-style)
  - [Std::String](#stdstring)
  - [Std::String_View](#stdstring_view)
- [Array-related Pointers](#array-related-pointers)
- [Symbolic C-Style String Constants](#symbolic-c-style-string-constants)
- [Dynamic Memory Allocation](#dynamic-memory-allocation)
- [Foreach](#foreach)
- [Void Pointers](#void-pointers)
- [Dynamic Multi-directional Arrays](#dynamic-multi-directional-arrays)
- [``Std::Array``](#stdarray)
  - [``std::array``s as parameters](#stdarrays-as-parameters)
  - [Array of Structs](#array-of-structs)
- [``Std::Vector``](#stdvector)
  - [Initialization](#initialization)
  - [Indexing](#indexing)
  - [Scoping](#scoping)
- [Iterators](#iterators)
  - [Pointers as Iterators](#pointers-as-iterators)
- [``Std`` algorithms](#std-algorithms)
  - [Additionals](#additionals)

# Arrays
- subscript operator ``[]``

## Fixed Sized Array
- initializing a fixed sized array: ``T arr[SIZE]{};`` where ``SIZE`` is a ``constexpr``
  - if not, compile time error
  - note that the initializer ``{}`` can contain any number of values up to ``SIZE``
    - all missing values will be value-initialize (to 0, etc.)
  - omission of initializer will not clear the memory
  - omission of ``SIZE`` will force the compiler to rely on the number of values specified in the initializer ``{}``
- accessing an array member: ``arr[IDX]`` where ``IDX`` must be an integral type
  - ``C++`` does not perform boundary checks
  - this means it is possible to index an array with ``enum``, ``bool``, etc.
  - a trick for ``enum``-indexed arrays is to add a tail item at the end that can be used as ``SIZE``
    ```c++
    enum Colors {
        red, // 0
        green, 
        blue,
        num_colors // 3
    }

    //...
    int colors[num_colors]; // initialize a 3-element array 
    ```
### Arrays as Parameters
- enforce array size
  - *ex.* 
    ```c++
    int sum(int arr[5]);
    ```
- don't enforce array size
  - *ex.*
    ```c++
    int sum(int arr[5]);
    ```
- note that arrays are effectively passed by reference, meaning that altering the array within the function will alter the origional array as well
- since ``C++ 17``, array size can be determined dynamically with ``std::size(array)`` in the ``<iterator>`` library
  - however, this must be used within the same scope
- another way is to divide ``sizeof(arr)`` by ``sizeof(T)``
  - this only works for fixed-size arrays
  - the scope limitation does not apply for this method, however it will almost guarantee an error since the function treats the array as a pointer and ``sizeof(arr)`` will just return word length

## Iterating Arrays
- typically using a ``for`` loop with index from ``0`` to ``SIZE-1``
  - specifically, ``static_cast<int>(std::size(arr))``

## Multi-dimensional Arrays
- for initialization, only the first element can be omitted
  - ``int arr[][2][2]``, etc.
  - note that this forbids jagged arrays
- when an array is more than 2 dimensional, it is common to use a loop (or value init) instead of manually assigning
- manually inputting the initial values can be done via nested ``{}``s

# String Implementations
## C-Style
- effectively a ``char`` array ending with ``\0`` (null terminator)
  - this means that assignment is not allowed after initialization
  - however, members can be changed individually through indexing
    - ``str[1] = 's'``
    - note that this can replace the ``\0`` and cause issues
- backing arrays can be defined longer than the string itself
  - any access will only read up to ``\0``
  - in input, do ``std::cin.getline(str, std::size(str))``, which reads up to the defined length of the array
- string functionality located in ``<cstring>`` library
  - there is no universal way to copy a c-style string
    - ``strcpy`` is not particularly safe
    - ``strncpy`` enforces size, but not null termination within that size
    - ``strcpy_s`` is preferred in ``C++``, but is not always supported
  - for this reason, ``std::string`` and ``std::string_view`` should be preferred

## Std::String
- located in ``<string>``
- designed to be modifiable
- when initiailized from another string, the ``std::string`` instance always copies the original data
  - this is true even for ``const`` instances
- ``string`` can be casted to C-style string by ``str.c_str()``
- 
## Std::String_View
- located in ``<string_view>``
- not modifiable
- can modify the view of the underlying string without changing the actual string
  - *ex.* ``str.remove_prefix(2)``: remove 2 letters from the left
- note that these changes are not reversible
- usually ``string_view`` is faster than ``string`` and should be preferred when modification is not needed
- ``string_view`` does not use null terminator
- ``string`` implicitely converts to ``string_view``
- ``string_view`` can be ``static_cast``ed to a ``string``
- it is recommended to use ``string_view`` as function parameters, and also do so by value instead of reference
- if the function requires null termination, then it is better to take ``const std::string&`` instead because it requires null termination and can accept c-style strings
- when a ``string_view`` is initialized based on a ``string``, and that ``string`` goes out of scope, accessing the ``string_view`` caused undefined behavior
- the underlying ``string`` can be returned with ``str.data()`` in the form of a c-style string
  - this is usually used on functionality for c-styled strings or ``string``s, like ``std::strlen``
  - ``data`` produces undefined behavior when the ``string_view`` is modified

# Array-related Pointers
- *def.* **array decay**: arrays are impliciltly converted to a ``T*`` pointing to the first element of the array
  - *ex.* ``&arr[0]`` will return the address of that data, instead of the data itself
  - the type of the array itself is ``T[SIZE]`` while the decayed pointer is of type ``T*``
  - note that  the decayed array will not provide information about its size
  - dereferencing the decayed array gives the first element (``*arr`` is the same as ``arr[0]``)
  - a special case is ``sizeof`` where arrays and pointers are treated differently
  - note that ``&arr`` will yield ``T*[SIZE]`` instead of ``T**``
- revisiting the fixed array as parameter issue
  - the sizing issue is caused by the argument decaying to a pointer and then passed to the function
  - for this reason, it is often better to use ``T*`` instead of ``T[]`` in the parameter specifications since the behavior aligns more
  - arrays in structs and classes don't decay
- pointer arithmetics
  - *def.* **scaling** additions to a pointer is scaled based on the size of the pointer's target type ``T``
  - this means ``*(array+n)`` is the same as ``array[n]``
    - note the parenthesis
  - ``std::begin(arr)`` points to ``arr[0]`` and ``std::end(arr)`` points to ``arr[SIZE]``
    - note that ``std::end(arr)`` will not yield anything when dereferenced because it is purely a marker
    - there are iteration functions like ``std::count_if`` that take these markers
      - *ex.* ``std::count_if(std::begin(nums), std::end(nums), isPrime)``

# Symbolic C-Style String Constants
- when defined to ``const char*``, compiler places the value in read-only memory, the specific location depends on implementation
  - when duplicated, these constants are occasionally consolidated to the same memory location
  - constants defined this way have static duration rather than automatic
- ``std::cout`` assumes that ``char *`` is a string, which could lead to inconsistencies when the actual intent is to print the address

# Dynamic Memory Allocation
- *def.*
  - **static memory allocation**: static and global variables, allocated at program run and presists until program finish
  - **automatic memory allocation**: function parameters and local variables, when the relevant block is encountered, freed when the relevant block is exited
  - for the above two, the size must be known at compile time
    - both occur on the stack, and stack size if usually small since is fixed size per program
  - **dynamic memory allocation**:
    - occurs on the heap
- ``new T``
  - returns ``T*`` associating with the allocated memory
  - *ex.*
    ```c++
    int* ptr { new int };
    ```
  - *ex.* with initialization
    ```c++
    int* p { new int (6) };
    int* q { new int {5} };
    ```
  - by default, ``new`` throws an exception when allocation could not be completed
    - adding ``std::nothrow`` will change the behavior to returning a ``nullptr`` instead
        ```c++
        int* val { new (std::nothrow) int{} };
        ```
- deletion
  - ``delete ptr``
  - ``ptr = nullptr``
    - this method does not actually free up memory
  - note that both methods result in a dangling pointer within the same scope
    - dereferencing or deleting the dangling pointer will result in undefined behavior
  - when pointers reference each other, deleting one pointer converts the other to dangling pointer as well
  - a good practice is to set the deleted pointer to ``nullptr``
  - ``delete`` implicitely checks for ``nullptr``
    - it does nothing when ``nullptr`` is encountered
- memory leaks happen when allocated memory goes untracked by the system
  - *ex.* pointer reassigned without first deleted
- ``new`` and ``delete`` both have an array form that accepts a subscript ``[SIZE]``
  - ``SIZE`` has to be convertible to ``size_t``
  - ``delete[]`` without ``SIZE`` works on its own because the operator keeps track of the array size since ``new[SIZE]``
  - dynamic arrays also does not keep track of length after decay, and the programmer is responsible for deallocation
  - dynamic arrays can be initialized by
    - zero initialization
        ```c++
        int* array = { new int[length]{} };
        ```
    - since ``C++11``, values can be used as well
        ```c++
        int* array = { new int[length]{1, 2, 3, 4, 5} };
        ```
- ``auto`` can be used instead of ``T`` when ``T`` is already specified after the new operator

# Foreach
- ``for(T t: arr)`` with some *un-decayed* ``T[] arr``
  - can use ``auto`` instead
- by default, foreach values are copied, which could be expensive
  - to use references instead, use ``auto&`` or ``T&``
- as for-each is based on iterators, array index will not be provided directly since many iterable data structures are not directly indexable
  - however, since ``C++ 20``, it is possible to compute the index by using the generalized form of for-each ``for(init_statement; T t : arr)``
  - *ex.*
    ```c++
    for(int i{}; auto s : students) {
        i++;
    }
    ```
- element declaration can be used with ``const``

# Void Pointers
- also called a **generic pointer**, can point to any datatype
  - ``void* ptr``
  - cannot be dereferenced directly
  - casting to typed pointer
    ```c++
    static_cast<T*>(ptr)
    ```
- deleting a void pointer generally leads to undefined behavoir and should be avoided
- void references ``void&`` are illegal
- ``C++`` does not allow pointer arithmetic on void pointers
- in general, other tools provided by ``C++`` should be favored over void pointers

# Dynamic Multi-directional Arrays
- array of pointers
    ```c++
    int** arr {new int*[length]{} };
    ```
- array of arrays
  ```c++
  int x{row};
  constexpr y{column};
  int (*array)[y] = { new int[x][y] };
  ```
  - the rightmost dimension must  be constant
  - otherwise, do row-wise allocation
    - note that this allows for jagged arrays
    - ```c++
      int X = 6; // non-constexpr
      constexpr int Y{10};
      int** arr { *int[Y] };
      for(int i = 0; i < Y; i++) {
        arr[i] = new int[X]{};  // or some function X(i)
      }
      for(int i = 0; i < Y; i++) {
        delete[] arr[i]; // delete all rows first
      }
      delete[] arr; // delete the array itself at last
      ```
    - due to complexity, multi-dimensional arrays are usually flattened and then indexed using another function
      - ```c++
        int getFlattenedIdx(int y, int x, int width) {
          return y * width + x;
        }
        ```
- usually pointers to pointers are avoided due to confusion and difficult memory management

# ``Std::Array``
- located in ``<array>``
- different initialization forms
  - ```c++
    std::array<T, l> arr { regular array initializer };
    ```
  - ``C++ 17`` allows for omission of ``T`` and ``L``
  - ``C++ 20`` allows for the omission of ``std::array`` as well if using ``auto``
    - ```c++
      // without deduction
      auto arr{ std::to_array<T, l>({ regular array initializer}) };
      // with deduction
      auto arr{ std::to_array({ regular array initializer}) };
      ```
- ``std::array`` instances can be accessed with subscript ``[]`` without bounds checking
  - note that arrays have size in the form ``std::size_t`` (unsigned), so the loop index should use this type instead of ``int``
  - more accurately,  the type is ``std::array<T, l>::size_type``, which ``std::size_t`` is an alias of
    - ``std::size_t`` is located in ``<cstddef>``
  - note that unsigned values wrap around
  - 
- alternatively, do ``.at()`` with bounds checking
  - *ex.*
    ```c++
    std::array arr { 1, 2, 3 };
    arr.at(0);
    arr.at(100); // runtime error
    ```
- ``std::array::size()``
- the good practice is to pass ``std::array`` instances through references or ``const`` references
- sorting an (sub)array
  - *ex.* 
    ```c++
    std::sort(arr.begin(), arr.end()); // in order sort
    std::sort(arr.rbegin(), arr.rend()); // reversed order sort
    ```
## ``std::array``s as parameters
- fixed size: 
  ```c++
  void foo(const std::array<int, 3>& arr) {}
  ```
- dynamic size:
  ```c++
  template <std::size_t size>
  void foo(const std::array<int, size>& arr) {}
  ```
- generic type:
  ```c++
  template <typename T>
  void foo(const std::array<T, 3>& arr){}
  ```
## Array of Structs
- tricky syntax...
- inner braces can be omitted during aggregate initialization
  - *ex.*
    ```c++
    struct Array {
      int[3] vals;
    };

    struct Array2 {
      int[3] p,
      int[2] q
    };

    Array array{
      1, 2, 3 // directly put values into vals
    };
    Array2 array2 {
      1, 2, 3, 4, 5 // same with {1, 2, 3}, {4, 5}
    };
    
    struct Array3 {
      Array[2] arr;
    }

    Array3 arr3 {
      {1, 2, 3}, {4, 5, 6} // error!
    };

    Array3 arr4 {
      { 1, 2, 3, 4, 5, 6 } // correct but might be warned or not clear
    };

    Array3 arr5 {
      { {1, 2, 3}, {4, 5, 6} } // correct
    };
    ```
    - the compiler will try to instantiate the one member per set of inner brackets, so ``{1, 2, 3}`` will be used to instantiate ``arr3.arr`` instead of ``arr3.arr[0]``

# ``Std::Vector``
- located in ``<vector>``
- dynamic array functionality, including managing its own memory
- some types are optimized for memory
  - *ex.* ``std::vector<bool>`` compresses things into bytes
## Initialization
- ``std::vector<T> arr;``
- ``std::vector<T> arr { vals };``
  - before ``C++ 11``: ``std::vector<T> arr = { vals };``
- ``std::vector<T> arr(initial_size)``
  - the compiler separates the two parametrized initializations by the notation
  - typically, it tries to initialize something as a list if encoutered ``{}``, otherwise it looks for non-list-like initalization methods
- note that length is always omitted because it is dynamic
## Indexing
- ``arr[i]`` unbounded
- ``arr.at(i)`` bounded
## Scoping
- ``std::vector`` instances self-deallocate as soon as they go out of scope
- ``std::vector`` instances keep track of length without issues of decay
  - can be accessed via the ``std::vector<T>::size`` function
  - the type returned is ``std::vector<T>::size_type()``, similar to ``std::size_t`` with arrays
- ``std::vector`` can be resized manually via ``std::vector<T>::resize(int s)``
  - if expanding, extra elements are initialized to default value
  - can also be shrank

# Iterators
- *def.* an object designed to traverse some container
  - implicitely used by for-each
## Pointers as Iterators
- ``T*`` iterates through array of type ``T`` via incrementation
  - *ex.* doing this manually...
    ```c++
    for(auto p{std::begin(arr)}; p != std::end(arr); p++);
    ```
- implicitely, for-each calls ``begin`` and ``end`` as member functions if class, and ``std::begin`` and ``std::end`` for c-style fixed arrays
  - this means that C-style dynamic arrays cannot be iterated with for-each since its end marker is not preserved at all
- iterators can be invalidated
  - *ex.* ``std::vector<T>::erase( iterator )`` invalidates the iterator, meaning that accessing ``iterator``'s pointed element will cause undefined behavior starting from this erasure

# ``Std`` algorithms
- *def.* categories
  - **inspectors**: view but not modify data in a container
  - **mutators**: modify data in a container
  - **facilitators**: generate results based on values in a container
- ``std::find``, ``std::count``
  - begin iterator, end iterator, val
- ``std::find_if``, ``std::count_if``
  - begin iterator, end iterator, predicate
- ``std::sort``
  - begin iterator, end iterator (default order)
  - begin iterator, end iterator, isBefore(a, b) (custom order)
    - note that ``std::greater`` provides the reverse of the default order
    - *ex.* prior to ``C++ 17``, do
      ```c++
      std::sort(arr.begin(), arr.end(), std::greater<T>{});
      ```
    - type can be omitted since ``C++ 17``
    - ``std::greater<T>`` is a type, not a templated function, so it needs initialization
      - (will expand on later) the type is ``std::function<bool(T, T)>``
- ``std::for_each``
  - begin, end, ``void(T&)``
  - note that the changes reflect on the original array, not to create a new array storing the function's returns per element like what a map/select function typically does
- for all the above functions, ``C++ 20`` adds the abbreviation where the container itself can be used to replace the begin and end iterators
  
## Additionals
- ``std::reduce`` applies ``operator+`` to all elements in an out of order manner, allowing parallelization
  - begin iterator, end iterator
  - an alternative is ``std::accumulate``
- ``std::shuffle`` shuffles the container in-place
  - begin iterator, end iterator
  - optional random number source
    - *ex.* 
      ```c++
      std::mt19937 mt{ static_cast<std::mt19937::reseult_type>(std::time(nullptr)) };
      ```
      - mt19937 is a pseudo-random number generator (PRNG)