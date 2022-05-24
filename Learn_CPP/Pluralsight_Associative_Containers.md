> the following notes add to notes for Learn CPP, and is incomplete when read independenly

# `Std::Set`
- `O(log N)` insertion
  - `std::set::insert(T)`
    - returns `(iterator, bool)`: inserted position - success
  - `auto [it, inserted] = set.insert(t)`
    - see *structure binding* in `C++ 17`
- iterate with range based `for`
  - only `T const&` or `T` acceptible (or `auto`), no `T&`
- `std::set::erase(T)`
- `std::erase_if(container, predicate)`
- `std::set::find(T)`
  - on fail, returns the iterator equalling `set.end()`
  - `O(log N)`
- internally sorted
  - `T` must implement `operator<`

# `Std::Map`
- `for(auto [key, value] : map);`
  - use structure binding in `for` loop
- alternatively, `for(auto const& [key, value] : map)`
- internally sorted by key
  - backed by balanced BST
- when passed as a `const&` argument, `operator[]` produces a bug because it's non-`const`
  - use the `std::map<K, V>::find(K)` instead
    - returns iterator to `std::pair<const K, V>`
- note that `operator[]` creates a new key if the key is not found
  - `std::map::find` is safer because it just returns the end pointer
- similarly, `find` is `O(log N)`

# `Std::Unordered_Map`
- exact same interface with `std::map`
- the implementation is different
  - not sorted
  - hash table with buckets
    - `O(1)` query best case
    - `O(N)` worst case
    - hashing already provided in library
- custom `K`
  - need to implement hashing and `operator==`
- 