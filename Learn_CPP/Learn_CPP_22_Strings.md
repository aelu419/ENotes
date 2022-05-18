
# `Std` String Classes
> use of `std::string` was already explained in section 4.
> please refer missing notes to that section
## Constructor
- constructors
  - copy constructor
  - from another string, with starting index (and optionally max length)
  - from C-style string, *up to but not including* `\0`
  - from misc types
    - use `std::ostringstream` objects (from `<sstream>`):
      ```c++
      template <typename T>
      inline std::string ToString(T t) {
          std::ostringstream oStream;
          oStream << t;
          return oStream.str();
      }
      ```
    - to misc types
      - use `sted::istringstream` objects (also from `<sstream>`):
        ```c++
        template <typename T>
        inline bool FromString(const std::string& sString, T &tX) {
            std::istringstream iStream(sString);
            return !(iStream >> t).fail();
        }
        ```
- length and capacity
  - `std::string::size()` and `std::string::length()` both return length
  - `std::string::capacity` behaves like other sequential containers' capacities
- assignment
  - `std::string::assign` works similarly to `operator=`, but in a member function form
  - `std::string::swap` swaps current and another string
    - the global `std::swap` swaps two arbitrary strings
- appending
  - `operator+=`, `std::string::push_back`, `std::string::append`
- inserting
  - `std::string::insert`