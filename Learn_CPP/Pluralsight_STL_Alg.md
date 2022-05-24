# Intro
- free functions: container functions are usually free functions that accept a container, rather than the container's member function
  - the language doesn't focus much on inheritance
  - instead, focus on iterators that each container produces
  - cuts down the number of methods needing to be written
    - from M * N to M + N, for N functions and M containers
- there are usually better implementations than raw loops
  - `count`, `find`, etc.
  - using library functions are safer and accounts for potential edge cases

# Counting & Finding
- `std::count(begin, end, target)`
- for other more specific cases, there are specific functions like
  - `std::allof`, `std::none_of`, `std::any_of`
  - the parameters are all `begin, end, predicate`
- similarly, `find` also has variations
  - `find_if_not`
  - `find_first_of`
  - `search` searches for a subsequence
  - `find_end` search but returns the end of the subsequence instead
  - `search_n` searches for repetitions
  - `adjacent_find` all instances where adjacent entries are equal
  - each of the above accepts `target` or `predicate`

# Sorting
- uses `operator<` or a `lessthan` predicate
- `stable_sort` is the variation of `sort` with higher cost
  - stable-sort can be used to prioritize different sorting indices
  - *ex.* sort by price, then stable sort by name, then within the same name the price will be sorted as well
- `is_sorted`
- `binary_search` for sorted collections
- `max_element`, `min_element`
- `lower_bound` for first >=, `upper_bound` for first <=
  - on sorted containers
- `partial_sort`
  - find the first `n` smallest and have them sorted
  - the next `m-n` elements are all larger than the first `n`, but without ordering guarantee
  - commonly used with pagination
  - `partial_sort_copy` copies out the sorted part to another container
  - `is_sorted_until` returns the first element breaking order
- `nth_element(begin, piv, end)`
  - the correct value will be put to `piv`
  - everything before and after `piv` will be partitioned correctly

# Comparing & Accumulating
- `equal` member equality
- `mismatch` first diverge
  - parameters are `A_begin, A_end, B_begin, optional_predicate`
  - with mismatching elements `a` and `b`, returns the pair of iterators pointing to each
  - with reaching the end of `A` or `B`, returns the pair of the end iterator of whichever is depleted, and then the matching iterator of the other collection (since `C++ 14`)
    - before `C++ 14` `A` must not be longer than `B`
- `accumulate`
  - defaults to using `operator+`
  - can use lambda instead
    - the parameters are `total` of some type `S` and `current` of type `T`
    - the lambda can decide if `current` should be added to `total` or not
    - the lambda can decide which part of `current` is added to `total`
  - returns the sum of type `S`
  - note that this `accumulate` is iterative, not logarithmic
- `for_each`

# Generating & Manipulating Collections
- `operator=`, `copy`, `copy_if`, `copy_n`, `copy_backward`
  - common parameters are `A_begin, A_end, B_begin`
  - `copy_n` takes `A_begin, n, B_begin` instead
  - `copy_backward` takes `A_begin, A_end, B_end` instead
- for non-copyable types, use `move` and `move_backward` instead
- removing
  - note that `vector` keeps entries consecutive, moving subsequent entries forward if gaps form in the middle
  - trailing elements stay in their place and are not erased
    - needs to call `erase` manually
      - `remove(A_begin, A_end, targetOrPredicate)` returns the new end pointer of A after the operation
      - `erase(E_begin, E_end)` needs to set parameters as that new end pointer and the original end (the fixed array's end)
    - a common way is to use the erase-remove idiom
      - `erase(remove(b, e, t), e)`
- creating and filling collections
  - `fill`, `fill_n`
  - `iota` (see learn cpp notes) for incrementing
  - `generate`, `generate_n` for creating elements with lambda (no parameter, returns `T`)
    - it is common to capture a index variable by reference and use that in the lambda
- replacing values
  - `replace` for target, `replace_if` for lambda
- mapping
  - `transform(i_begin, i_end, o_begin, mapping_function)` with mapping function from `I` to `O`
  - `transform(i1_begin, i1_end, i2_begin, o_begin, mapping)` with mapping function from `(I1, I2)` to `O`
- eliminating duplicates
  - `unique`, `unique_copy` eliminates consecutive duplicates
    - a similar erase-remove idiom exists
- reversing
  - `reverse`, `reverse_copy`
  - for single elements, `itr_swap`

# Iterators
- preallocation: not really practical for dynamic-sized data
- usually containers support iterators for insertion
  - `back_inserter` and `front_inserter` (no guarantee that specific container will support)
  - both are free `std` functions taking the container as the parameter
  - common use is to use `back_inserter(v)` in functions like `generate_n` instead of the beginning pointer
  - using `front_inserter` will always insert at the first element, leading to a reversed order
    - front-inserting iota on a `deque` will actually create decrementing elements at the front
- reverse iterators
  - `rbegin` to `rend` is the reverse of `begin` to `end`
  - reverse iterators are differently typed from normal iterators
- `operator+` is defined for `int`
- `operator-` is defined for `int` and other iterators
  - subtracting the other iterator means to find the signed distance
- `const` iterators (see learn cpp notes for `cbegin` and `cend`)
  - `crbegin` and `crend`
  - most containers don't support `const` objects since they need to move them around in memory
    - in that case, explicitly ask for `const` via the `c` prefixed methods above

# Misc
- `swap`, `itr_swap`
- `swap_ranges`, works within or between collections
  - `A_begin, A_end, B_begin`
- `rotate`
  - if partial, it moves `n` elements move up for certain levels
- `partition`
  - everything satisfying a lambda moves to the top, the rest moves to the bottom
  - variant `stable_partition`