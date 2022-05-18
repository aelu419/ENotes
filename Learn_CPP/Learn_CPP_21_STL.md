- [Containers](#containers)
  - [Sequence](#sequence)
  - [Associative](#associative)
  - [Container Adapter](#container-adapter)
- [Iterators](#iterators)
- [Algorithms](#algorithms)

# Containers
## Sequence
- maintains the order of elements in the container
  - vector, deque, array, list (doubly linked), forward_list, basic_string

## Associative
- automatically sort inputs upon insertion
- compares elements using `operator<`
- set, multiset, map, multimap (dictionary)

## Container Adapter
- adapts sequence containers to other forms of data structures
- can specify which sequence container to back the adapter
- stack, queue, priority queue

# Iterators
- operators
  - `operator*` dereferences to the current pointed object
  - `operator++` next, `operator--` prev
  - `operator==` and `operator!=` compares if two iterators point to the same element
  - `operator=` assigns the iterator to another position
- container member iterators
  - `begin()`, `end()`, `cbegin()`, `cend()`
  - the c prefix marks for `const`
- priviledges
  - `container_type::iterator` allows for R/W
  - `container_type::const_iterator` allows for R
- uses
  - `vector` and `list`: add element with `push_back(t)`, dereferencing the iterator returns `T` object
  - `set`: add element with `insert(t)`, dereferencing the iterator returns `T` object
  - `map`:
    - add element with `insert(std::make_pair(k, v))`
    - iterator returns `std::pair<K, V>`
      - access `k` with `iterator->first`
      - access `v` with `iterator->second`
  
# Algorithms
- located in `<algorithm>
- `std::min_element` and `std::max_element`
- `std::iota` (from `<numeric>`) fills from beginning to ending with incrementing values
- `std::find` returns an iterator pointing to the first occurrence
- `std::sort` and `std::reverse` (mentioned previously)
  - note that `std::sort` doesn't apply to `std::list` since swapping isn't intuitive
  - `std::list` provides its own `std::list::sort` functionality
