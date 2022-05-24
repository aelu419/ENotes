# Misc
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
- `transform(i_begin, i_end, o_begin, mapping_function)`