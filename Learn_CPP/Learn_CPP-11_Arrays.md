- [Arrays](#arrays)
  - [Fixed Sized Array](#fixed-sized-array)
    - [Arrays as Parameters](#arrays-as-parameters)
  - [Iterating Arrays](#iterating-arrays)

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