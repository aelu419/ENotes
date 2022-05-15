- [Analogies](#analogies)
- [Composition](#composition)
- [Aggregation](#aggregation)
- [Association](#association)
  - [Reflexive Association](#reflexive-association)
  - [Indirect Association](#indirect-association)
- [Dependency](#dependency)
- [Container Classes](#container-classes)
  - [`Std::Initializer_List`](#stdinitializer_list)

# Analogies
- part-of, has-a, uses-a, depends-on, member-of

# Composition
- an object consists of several parts
  - part-of
  - if the owner is destroyed, the part is destroyed as well
- `struct`s and `class`es are called composite types for including members
- qualities
  - the member is part of the `class`
  - the member can only belong to one `class` at a time
  - the member has existence managed by the `class`
  - the member does not know about the existence of the `class`
    - *def.* **unidirectional**
- composition does not limit the transferability of parts, but upon transfer the part must act like a part
  - that is, it must start to satisfy the 4 qualities above
- best implemented as normal data members of `struct`s and `class`es
- practically, implementations deviate from the principle
  - owners can delay creation of parts
  - owners may *accept* instead of *create* parts
  - owners may delegate part desruction to other management like garbage collection

# Aggregation
- subtype of composition
  - the part can belong to more than one owner at a time
  - the part does not know about the existence of its owners
  - the part does not have its existence managed by the owners
- has-a
- commonly implemented as reference/pointer members
  - beware of creation/destruction for parts
  - in `C++` composition should be favored over aggregation for this reason
- note that aggregate is not aggregation
  - aggregates are `class`es that act as data collection
  - aggregation is this part-owner(s) relationship
- `std::reference_wrapper`
  - must be a named object
  - safer alternative to pointers
  - example use is to have a `std::vector` of `std::ref`s
  - can make reference `const` by doing `std::ref<const T>` instead of `std::ref<T>`

# Association
- more loose relation than part-whole
  - not exactly a "part" of the object
  - can belong to more than o e object a a time
  - does not have existence managed by object
  - may or may not know the object
- for example, server/client is associated with each other, but both can persist without the other
- usually implemented with pointers that are nullable
- both unidirectional and bidirectional are permitted, but only the minimum should be implemented to prevent unnecessary information
## Reflexive Association
- relationship with other objects of the same type
  - *ex.* a course has pre-requisite courses
## Indirect Association
- having a member that "points to" the asociated object in some sense
  - *ex.* holding an id/unique name to the associated object

# Dependency
- an object invokes another object's functionality to accomplish some specific task
  - weaker relationship than association
  - always unidirectional
- in `C++`, associations are between two classes at the class level
  - one class keeps a direct/indirect asociation to the other class as a member
- dependencies are usually *not* at class level
  - *ex.* referenced in methods

# Container Classes
- object holds multiple instances of another type
- commonly implement the following
  - create empty container
  - insert
  - remove
  - size
  - remove all
  - access to stored objects
  - sort
- value containers store a copy of the values, reference containers store references to the values
  - value containers are compositions
  - reference containers are aggregates
- typically, containers hold one type of data

## `Std::Initializer_List`
- the compiler converts a list initializer and its contents to `std::initializer_list`
  - located in `<initializer_list>`
  - custom containers can make use of a constructor that accepts `std::initializer_list<T>`
    - this allows custom containers to be initialized with the list initializer
    - *ex.* `IntArray arr{1, 2, 3, 4, 5};`
    - *ex.* `IntArray arr{ new int[5]{1, 2, 3, 4, 5}};`
  - using the `{}` initializer will prefer any `std::initializer_list` constructor over other types of constructors
    - *ex.*
      ```c++
      class IntArray {
        // ...
        IntArray(int i) {
          // ...
        }
        IntArray(std::initializer<int> list) : IntArray(static_cast<int>(list.size())) { // list size can be used in the other constructor
          // std::initializer can be iterated
          for (int element : list) {
            // ...
          }
        }
      }

      // ...
      IntArray a{0}; // prefers the std::initializer_list constructor
      IntArray b(1); // prefers the int constructor
      ```
- the `operator=` can be overloaded similar to the constructor
  - without doing so, the `std::initializer_list` constructor can also be used, since it can be used to do implicit type conversion (unless marked `explicit`)