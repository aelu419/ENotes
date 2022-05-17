- [The Need For Move Semantics](#the-need-for-move-semantics)
- [R-Value References](#r-value-references)
  - [Move Constructor & Assignment](#move-constructor--assignment)
  - [`Std::Move`](#stdmove)
- [`Std::Unique_Ptr`](#stdunique_ptr)
- [`Std::Shared_Ptr`](#stdshared_ptr)
  - [Circular Dependency Issues](#circular-dependency-issues)

# The Need For Move Semantics
- handles the case where dynamically allocated pointers do not clean up after themselves
  - uses the destructor to clean up the referenced object
- however the destructor is not enough, as copy assiugnment and copy constructor can require change in destrruction behavior as well
  - instead, use **move semanntics** to transfer ownership
  - `a = b` will set 
    - `b`'s backing pointeer to `nullptr`
    - `a`'s backing pointer to `b`'s original backing pointer
    - delete `a`'s original pointed resource
  - note that this functionality is similar to `std::auto_ptr` which is removed in `C++17`
    - this is due to how `std::auto_ptr` defaults to non-array `delete` 
    - also, passing the `std::auto_ptr` by value will cause an ownership transfer to the parameter
- since `C++11`, the language adds more support for moves, including the addition of `std::smart_ptr` and etc.

# R-Value References
- to recap, r-values are defined to be values that can appear at the right side of assignments and are not l-values
- a r-value reference can only be initialized by an r-value
  - `int &&rref{5};` for example is a r-value reference referencing thet literal `5`
- non-const r-value references allow modification of the r-value
- r-value ferences extend the lifespan of the r-value object
- r-value references are often used as parameters to distinguish from l-value references
  - ```c++
    void foo(const int& lref);
    void foo(int&& rref);
    
    // ...
    foo(x); // resolves to lref implementation
    foo(5); // resolves to rref implementation

    int&& ref{5};
    foo(ref); // resolves to lref implementation, because the rref is stored in a lref
    ```

## Move Constructor & Assignment
- move ownership of resources from one object to another
- take non-`const` r-value reference instead of `const` l-value reference
  - ```c++
    class Foo {
      // ...
      Foo(Foo&& f) noexcept;
      Foo& operator=(Foo&& f) noexcept;
    };
    // ...
    f2 = generateFoo(); // the return value is a r-value, which resolves operator= to the move assignment operator
                        // within generateFoo, the return value is returned by value, meaning that it invokes the move constructor to construct a temporary object which is returned
    ```
    - note that the move assignment overload still returns a l-value reference, even though it takes a r-value reference
- move constructor and assignment will not be provided by default
- note that the temporary object should still be left in an operable state at the end of move construction/assignment
  - it may not directly go out of scope
  - it may be an l-value used forcefully as an r-value
  - consider the `auto_ptr` implementation above, if implemented with move constructors and assignments, the "stolen" object (`a`)'s pointer should be pointed to `nullptr`
- note that return values will prioritize move than copy even if the returned value is a local l-value
- copy construction/assignment can be marked as `delete` to complete disable copying
- with move implemented and copy disabled, the `auto_ptr` implementation becomes similar to `std::unique_ptr`

## `Std::Move`
- used to forcefully apply move assignment and construction on l-value references
  - located in `<utility>`
  - ```c++
    template <typename T>
    void swap(T& a, T& b) {
      T t { std::move(a) }; // move constructor
      a = std::move(b); // move assignment
      b = std::move(t); // move assignment
    }
    ```
  - ```c++
    vector.push_back(std::move(curr)); // moves the current object into vector element by calling the r-value reference version of std::vector::push_back
    ```
  - note that ``std::move`` signifies that the parameter is a temporary value and is not actually needed (semantically, not syntactically) for the rest of the scope
    - only use `std::move` on l-value references when they no longer need to be referenced directly
    - however, it is still good practice to leave the moved l-value at a well-defined, "empty" state
- a variant of `std::move` is `std::move_if_noexcept` which provides strong exception guarantee
  - `noexcept` promises that the move will complete
  - without `noexcept`, if the move fails midway the moved object is damaged
    - this is not desirable for l-value references
  - instead, `std::move_if_noexcept` will not start the move unless the movement is guaranteed to succeed
    - if `noexcept` fails, a copyable l-value referencce is returned instead of a movable r-value reference
  - note that if the type has a potentially-throwing move assignment/constructor and *no* copy assignment/constructors, then `std::move_if_noexcept` will still prefer move over copy, since the latter is impossible

# `Std::Unique_Ptr`
- located in `<memory>`
- completely owns the object it manages
- will manage resources upon deletion
- implements move semantics
- implements `operator*` and `operator->` for managed resource access
- can tell between arrays and non-arrays and use `delete` and `delete[]` properly
  - however it is still better to favor *actual* containers than `std::unique_ptr<T[size]>`
- `C++14` provides `std::make_unique()` which constructs a `T` object and initializes it with arguments
  - ```c++
    auto f{ std::make_unique<Foo>(1, 2, 3) }; // calls the (int, int, int) constructor of Foo to initialize a r-value, then creates a std::unique_ptr<Foo> out of the initialized object
    ```
- `std::unique_ptr` can be returned by function
  - the value only goes out of scope if it is not assigned to anything
  - do not return `std::unique_ptr` by pointer or reference since those will likely be dangling
- note that `std::unique_ptr` disables copy semantics, so `std::move` is needed when passing it as an argument
  - ```c++
    // ...
    void takeOwnership(std::unique_ptr<Foo> f) {
      if (f) std::cout << *res << std::endl;
    }

    auto p{ std::make_unique<Foo>() };
    takeOwnership(std::move(p)); // the function takes the ownership of p and destroys the pointed object upon termination
    ```
- the alternative without making the function take the ownership
  - ```c++
    void doNotTakeOwnership(Foo* f) {
      if (f) std::cout << *res << std::endl;
    }

    auto p{ std::make_unique<Foo>() };
    doNotTakeOwnership(p.get());
    ```
- when using smart pointers such as `std::unique_ptr` as member variables, they still need to be deallocated manually when the class object is dynamically allocated

> the tutorial lists two error-causing ways to initialize `std::unique_ptr`, both are avoidable when using `std::make_unique`

# `Std::Shared_Ptr`
- multiple smart pointers co-owning a resource
- located also in `<memory>`
- internally keeps track of other `std::shared_pointer`s pointing to the same resource, and will not deallocate unless the last of them goes out of scope
  - ```c++
    Foo *f = new Foo;
    std::shared_ptr<Foo> p{f};
    {
      std::shared_ptr<foo> q{p};
    } // q going out of scope here doesn't deallocate f
    // p going out of scope here deallocates f
    ```
  - note that the same thing does not happen if q were to be created from the resource
    - in that case, p and q have no way of tracking each other, both will try to `delete` f upon going out of scope
- since `C++11`, `std::make_shared` can be used
  - this method provides added performance
    - when creating resource and `std::shard_ptr` separately, there are 2 separate memory allocations
    - when creating from `std::make_shared`, it optimizes the whole thing to 1 larger memory allocation which is usually faster
- shared pointers can be created from unique pointers (by stealing its resource), but not otherwise
- `std::shared_ptr` adds additional complexity upon deallocation
  - a dynamically generated shared pointer also needs to have itself (and its resource) deallocated, and the process is more complicated than for `std::unique_ptr`
- `std::shared_ptr` does not support arrays until `C++20`
## Circular Dependency Issues
- Consider the class
  ```c++
  class Reference {
    private:
    std::shared_ptr<Reference> r;
    public:
    // ...
    friend bool eachOther(std::shared_ptr<Reference>& p, std::shared_ptr<Reference>& q) {
      if (!(p && q)) return false;
      
      // point to each other
      p->r = q;
      q->r = p;
      
      return true;
    }
  };

  // ...
  // create p and q from different resources f1 and f2
  eachOther(p, q);
  // here, p and q goes out of scope but f1 and f2 are not deallocated
  ```
  - `std::shared_ptr` needs to check whether it is the last pointer in order to deallocate
    - `p` knows that `q->r` also tracks `f1`, so it doesn't deallocate
    - `q` knows that `p->r` also tracks `f2`, so it doesn't deallocate
  - the similar happens as long as there is a cycle
    - every pointer keeps the next one alive
    - the last thing keeps the first thing alive
  - cycles can happen with one pointer
    - it points to a resource with a member assigned as the pointer itself
- A way to solve the issue is `std::weak_ptr`
  - `std::weak_ptr` works similarly to a `std::shared_ptr`, just that it doesn't check for deallocation and will not be check by the `std::shared_ptr`
  - `std::weak_ptr` cannot be accessed directly
    - must use the `std::weak_ptr<T>::lock()` function which returns a `std::shared_ptr<T>`, which could then be accessed
    - the locked shared pointer is guaranteed to go out of scope because it is always a local variable