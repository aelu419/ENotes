# Creational
## SOLID Design Principles
- single responsibility
  - benefits testing
- open for extension, closed for modification
  - *ex.* abstract fixed settings to enums and service classes that are exchangeable
- liskov substitution (behavioral subtype)
  - childtype always substitutes the parent type
- interface segregation
  - modularity, reduce compile time
- dependency inversion
  - shared abstraction

## Singleton
- shared state holder that is only instantiated once
- ```c++
  class MySingleton {
    private:
      MySingleton(){}; // private constructor
    public:
      MySingleton(MySingleton const&) = delete; // remove copy construtor
      MySingleton &operator=(MySingleton const&) = delete; // remove assignment

      static MySingleton& get() {
        static MySingleton instance{};
        return instance;
      }

      // ... public member methods
  };
  ```

## Builder
- customization of object construction
- declarative, step by step
- ```c++
  class MyClass {
  private:
      std::string name;
  public:
      bool settingA{};
      bool settingB{};
      bool settingC{};

      MyClass(std::string name) : name{name} {};

      friend class MyClassBuilder; // for exposure
      static MyClassBuilder create(std::string name) {
          return MyClassBuilder{name};
      }
  };

  class MyClassBuilder {
  private:
      MyClass myClass;
  public:
      MyClassBuilder(std::string name) : myClass{name} {}

      // type conversion back to the custom type
      operator MyClass() const { return std::move(myClass); }

      MyClassBuilder& setA() {
          myClass.settingA = true;
          return *this; // for chaining
      }

      MyClassBuilder& setB() {
          myClass.settingB = true;
          return *this; // for chaining
      }

      MyClassBuilder& setC() {
          myClass.settingC = true;
          return *this; // for chaining
      }
  };

  int main() {
      MyClass m = MyClass::create("bob").setA().setC();
  }
  ```

## Prototype
- when copying is cheaper than creating from scratch
- when there are several base configurations
- implementation
  - the base class' manager class holds
    - an `static` array of `new` subtype instances (of type `std::unique_ptr<Base>`)
    - a `static createInstance(Type)` method, where `Type` is an enum
      - `return prototypes[type]->clone()`
  - the base class defines a `virtual clone` method of type `(std::unique_ptr<Base>)()`
    - eacy derived class (prototype) implements the clone, usually using the `std::make_unique` with the copy constructor
  - to use, call `BaseManager::createInstance(type)`
> the tutorial uses raw pointers and `new`/`delete` instead

## Factory
- flexibility
- generic, extensible code
- decoupling
- implementation 1
  - put `static virtual` factory method on `Base` of type `std::unique_ptr<Base>(Config)`
  - implement factory method on subtypes
- implementation 2
  - use separate `BaseFactory` class with a `static` factory method of type `std::unique_ptr<Base>(Type, Config)`, where `Type` is an `enum`
  - the factory class may also use a non-static factory method

## Abstract Factory
- base and concrete factories
- for creating different families of the same set of objects
- note that for this pattern the factory method cannot be `static` as it has to be `virtual`
  - this means the user would need to instantiate factories (unless singletons exist) to use the factory method

## Dependency Injection
- simpler testing
- proper decoupling of dependencies
- instantiate services outside the user class and pass them in the constructor

# Behavioral
## Observers
- subject/publisher: generates events of interest
- observer: subscribes to subjects/publishers and listens for events of interest
- note that the `subscribe`/`unsubscribe` functionality exists in the publisher, since it edits the subscriber list
- it is common to have an abstract base subscriber class with an abstract update method
- note that it is appropriate to use `std::list<Observer*>` rather than any smart pointers, since there is no ownership to be conveyed
- pro
  - introduce new subscribers dynamically
  - subscribe silently
  - reuse subscribers
- con
  - arbitrary notifications
  - explicit registration/unregistration\ may cause memory leak

## State
- alter behavior when state changes
- finite state machine
- implementation
  - a `Context` class holding 
    - a private `State` instance
    - a `changeState` method of type `void(State)`
    - a constructor of type `Context(State)`
    - several behavior meethods
  - a `State` class with several `virtual` behaviors matching that of `Context`, as well as a `protected` context variable
    - concrete `State` subclasses with
      - implementations of `State` behaviors
      - a `setContext` method of type `void(Context)`
- pro
  - reduce duplicate code (for conditionals checking states)
  - easier code expansion
- con
  - overhead
  - complications with memory handling when transitioning between states

## Strategy
- similar to State, but each strategy is not aware of the context
- selectable algorithm to accomplish the same task
- implementation
  - a `Context` class holding
    - a strategy or set of strategies
    - setters for strategies
    - business logic
  - an abstract `Strategy` class for each business logic
    - with abstract method `execute` of any type required by the caller
    - concrete `Strategy` subclasses implementing the `execute` method
    - there is no need for a unified `Strategy` class between different business logics, since each may require different forms of `execute`
      - *ex.* `UnaryCalculatorStrategy` with execute of type `double execute(double)` while `BinaryCalculatorStategy` with execute of type `double execute(double, double)`
    - one base strategy may have more than one execute-like methods
      - *ex.* `VehicleStrategy` with `signalLeft`, `signalRight`, `signalStop`
        - `BicycleStrategy`, `CarStrategy`
- pro
  - can add new algorithms without changing the caller
- con
  - overcomplication
  - client needs to be aware of the different strategies to use them

## Template Method
- an abstract base class with a template method containing several steps (`protected virtual` methods)
- subclasses override the steps but not the template method itself
- "skeleton method"
  
## Visitor
- add featuress without modifying the original base class (outside the components for the visitor pattern itself)
- **double dispatch**
  - special case of multiple dispatch
  - single dispatch is just virtual methods (i.e. method implementation depends on the dynamic type of a single object)
    - double dispatch depends the implementation on two dynamically resolved types
- implemenattion
  - `Element` interface with `accept` method of type `void(Visitor)`
    - subclasses derived from `Element`, say `A` and `B`, each implementing `accept`
      - the `accept` method may be different, but is generally just `visitA(this)`, `visitB(this)`, etc.
      - there may be a `MultipleElement` class derived from `Element` storing a list of `Element`s, and the `accept` method evokes `accept` on each element
  - `Visitor` interface with overloads of `visit`, say of types `void(A)` and `void(B)`
    - a concrete implementation of `Visitor` that implements `visit` for both `A` and `B`
- con
  - need to update all visitors when a new subclass is derived for `Element`
  - coupling

## Chain of Responsibility
- loose coupling + 1 unified point of access to functionality (the entrance of the chain)
- chain of priority
  - every handler consumes certain categories of data, and passing unconsumed data to the next part of the chain
- implementation
  - `Handler` implements a singly linked list node and a `consume` method
    - ```c++
      if (dataCanBeConsumed) {
        // ... logic
        return processed;
      } else if (next) {
        return next.consume(data);
      } else return someDefault;
      ```
    - `setNext`
- further decoupling
  - note that the SLL setup is still done in the base class
  - instead, make the `Handler` a non-node class storing an actual container of all nodes
    - refactor nodes to derive from `Receiver` instead, and no longer handle `next`
    - ```c++
      if(dataCanBeConsumed) {
        // ... logic
        return processed;
      } else return NULL;
      ```
      - the `Handler` handles the `NULL` case and rewires that to the default value if there is no next `Receiver` in the container
- issues
  - does not guarantee handling
  - introduces overhead
  - for multiple valid handlers/receivers, order matters

## Command
- separation of concerns, particularly with reusable UI
- implementation
  - a `Command` abstract base class with method `execute` of type `void()`
    - implementations of `Command` class with parameters injected in their constructors
    - the caller (the original base class, say the UI button) will instantiate the command with appropriate data and 
- sometimes failures need to be notified, which can be done through exceptions
  - the exception needs to be generic enough (or derived from some common exception) so that the caller `Command` instances can understand it
- implementation of macros
  - a `MacroCommand` class storing a list of commands in `std::list`
    - with `virtual` methods `add`, `remove`, and `execute`
    - the `execute` method iterates through the list to evoke entry executes until reaching the end
      - upon catching an exception, rethrow it
    - note that this does not have strong exception guarantee as executed subcommands are not rolled back
  - the alternative to the list is a *composite* pattern
- implementation of undo
  - each `Command` has an `undo` method alongside `execute`, which reverses the effect of `execute`
  - the caller needs to hold a `std::stack` of `Command`s
    - each `Command` is pushed to the stack upon execution
  - the caller needs to implement an `undo` method of type `void()`
    - pops the `Command` from the stack and calls its `undo` method
  - an alternative to the stack is a *memento* pattern

## Interpreter
- interpret some structured expression with grammar
- usually with a tree structure that can be *parsed*
- terminal vs. non-terminal
  - *ex.* 1+2, 1 and 2 are terminal, + is non-terminal
- implementation
  - an abstract base `Expression` class
    - with reference to a `Context` class global to the interpreter (and all its child nodes)
    - an `interpret` function of type `void(Context)`
    - a `TerminalExpression` subclass
      - cannot have child nodes
      - the `interpret` function processes its own data
    - a `NonTerminalExpression` subclass
      - can have child nodes (in some kind of container)
      - the `interpret` function processes each child first, and then combines them according to the node's semantics
  - to evaluate the entire expression, use `interpret` on the root node
- issues
  - complex gramamrs are hard to implement
  - lexing and parsing are difficult, especially for fuzzy cases

## Iterator Pattern
- traversing containers
- there are built-in iterators
- to wrap a built-in iterator
  - provide functionality for `first`, `next`, `done`, `current`
  - *ex.* add skips in `next` for some skipping predicate
    - parameters can be injected from the constructor
  - for custom containers, the iterator wrapper often needs to be a `friend` class


## Mediator
- point of communication between different sections
- reduce coupling and multiple dependencies on the same thing
- centralizes control and complexity
- implementation
  - `Mediator` abstract base class defining types of interactions between colleagues
    - `ConcreteMediator` subclass implementing the interactions
    - `Colleague` concrete instances are managed by the `ConcreteMediator`
  - `Colleague` abstract base class
    - with reference to the `Mediator`
    - implemented by `ConcreteColleague`s that communicate with the `Mediator` by calling functions
  - the pattern resembles a chatroom
- issue
  - might become bloated
    - can divide into several different mediators

## Memento
- undoing tasks of high complexity
- components
  - a `Memento` that stores the internal state of the `Originator`
  - a `Originator` that creates the memento
  - a `Caretaker` that safeguards the memento
- implementation
  - a `Memento` class 
    - that is a `friend class` of the `Originator`
    - getter(s) for stored internal data
  - an `Originator` class
    - with `createMemento` method of type `Memento*()`
    - with stack of `Memento`s
    - with an `undo` function that pops the `Memento` and restores the last one
    - some operations that pushes to the `Memento`
- implementing redo
  - in `Originator`, add another stack to store undone `Memento`s
  - upon undo, push poped `Memnto` to the undone stack
  - upon non-undo operation, clear undone stack
- issues
  - snapshot frequency & lifetime leading to large stack
  - other techniques to minimize stack size (storing only changes, etc.)

# Structural
## Adapter
- wrapper/translator pattern
- convert existing class to needed interface
  - commonly used for legacy code
  - sometimes used to unify different API
- implementation of an object adapter (wraps an object)
  - create a concrete `Adapter` class that
    - holds the wrapped class instance which is often binded through constructor
    - defines certain points of interaction from the public
  - do NOT write business logic in `Adapter`
    - keep it as lightweight as possible
- implementation of a class adaptor
  - create a concrete `Adapter` class that
    - inherits both the target API interface and the wrapped class

## Bridge
- decouple abstraction from implementation
  - isolate clients from changes in abstraction and implementation
  - handle duplicated inheritance
    - for example, one action is done with different API, leading to different subclasses for each API
      - `A` becomes `AWithToolA`, `AWithToolB`, ...
      - `B` becomes `BWithToolA`, `BWithToolB`, ...
      - this leads to MxN classes in the same hierarchy
- implementation
  - an abstract base `Abstraction` class for entities with some operation that
    - holds an implementator instance
    - is derived into concrete entities
  - an abstract base `Implementator` that
    - defines a implementation for the operation named in `Abstraction`
    - is derived into concrete implementors for each API
- the result is M+N classes instead
  - both `Abstraction` and `Implementor` can be extended
- implementation of PImpl (Pointer to Implementation) idiom
  - `C++` specific, in order to speed up compilation
  - in some class
    - declare another class with private visibility called `impl`
    - create a unique pointer of type `impl` called pimpl
  - in another file, define functionality in the `impl` class
    - for each method in the custom class itself, forward every function to the appropriate `pimpl`'s functionality
    - similar to regular bridge, `impl` can be subclassed
  - pro
    - faster compilation
    - hide internal data & dependencies
  - con
    - memory overhead
    - maintenance overhead
    - complicates inheritance

## Composite
- one or more types of items that needs to be treated uniformly
  - unifying interface between collections and single items
- uses a tree-based structure
  - avoids complex loops over data
- implementation
  - a abstract base `Component` class that
    - defines some business operations
    - defines `add` of type `void(Component)`
    - defines `remove` of type `void(Component)` or `Component(Component)`
    - defines `getChild` of type `Component(int)`
    - is derived into
      - `Leaf` with only the operations implemented (other operations just voided)
      - `Composite` with everything implemented
- pro
  - simplify client code
- con
  - the client must know the difference between `Leaf` and `Composite`
  - added complexity

## Decorator
- wrapper
  - adds functionality through owning instead of inheriting
  - dynamic
- useful when
  - the decorated class cannot be changed
  - some features are optional
  - logic is not part of the core business features
- implementation
  - a `Decoratorated` class that
    - defines a `virtual` destructor
    - defines some business operations
  - a `Decorator` class that
    - inherits from `Decorated`, and
      - implements the destructor
      - implements a constructor that injects a `Decorated` instance
      - holds an instance of the `Decorated` class
      - defines added functionality to aid business operations
      - overrides the busines operations referencing those added functionalities
      - is derived into several `ConcreteDecorator` classes that
        - implements the additional functionality
- use
  - note that a `Decorator`/`ConcreteDecorator` instance can be injected into another because they all inherit from `Decorated`
- static decorators using `template`
  - gives the benefit from accessing `static` functions from the immediate `ConcreteDecorator` and not necessarily the base `Decorated`
    - `T instance`, `T::foo()`
  - the drawback is that the constructors of `ConcreteDecorator`s must agree
    - alternatively, use variable parameters and pass them to the held instance
      - `template <typename T, typename... Args>`
      - `ConcreteDecoratorB(Args... args) : T(std::forward<Args>(args)...) {}`
      - however this disables hinting
- functional decorators using `std::function`
  - a generic implementation would just be overloading `operator()`
  - another implementation is with `template`
    - `typename R`, `typename... Args`
    - constructor containing `std::function<R(Args ...)>`
      - this is then stored instead of the base `Decorated` object
      - it is common to wrap the constructor in a `template` method to avoid having to type in the type arguments
    - `R operator()(Args ... args);`
    - note that `std::function` cannot reference member functions, so instead wrap it with a free function that takes the `Decorated` (or some specific `ConcreteDecorator`) instance and invokes the method
- issues
  - creates a lot of similar, small classes
  - the base class needs to be lightweight
  - limitations with each implementation

## Facade
- avoid tight coupling with client and subsystems
- implementation
  - a concrete `Facade` that
    - holds reference to certain subsystems, injected through constructor
    - defines business operations that draws upon subsystem features
  - note the subsystems still needs to be constructed manually
- issues
  - subsystem dependency on the `Facade` class
  - overall larger codebase
- note that this is not the same with the Mediator pattern
  - subsystems are not aware of the `Facade` itself or need to communicate with eachother

## Flyweight
- share common parts of state between multiple objects
- intrinsic states are immutable, extrinsic states are mutable
- implementation
  - a `FlyweightFactory` managing correct sharing and creation of flyweights
    - uses a hashmap to keep track of `Flyweight` instances
  - `Flyweight` storing shared intrinsic states and functionalities of a certain entity
  - entities with reference to some `Flyweight` instance
- issues
  - added complexity
  - most can just be done with `static` immutable members

## Null Object
- provide default behavior for `NULL` cases
  - prevent need of repeated null checks
- implementation
  - for a certain `Entity`, create a subclass `NullEntity` that overloads functionality
  - instead of returning `NULL`, return a `NullEntity` instance
  - it is commonly to define a `Entity` method named `isNull` of type `bool()` and implement in both `Entity` and `NullEntity` classes

## Proxy
- substitue for actual object
- implementation
  - both the real `Service` and its `Proxy` inherits from the same interface
  - the `Proxy`
    - holds a reference to the real `Service`
    - forwards calls to business operations to the real `Service`
    - adds certain preprocessing and postprocessing operations
  - the `Service`
    - may contain additional operations and members not visible to `Proxy`, as they are sibling classes
- variants
  - remote proxy: local object as representation of a remote object (ex. ATM is a remote proxy of a bank)
    - it is common to perform preprocessing in remote proxy
  - virtual proxy: send dummy result first, and then send correct result once the service consumes the task (ex. lazy loading)
  - protection proxy: mainly to control access
  - cache proxy: stores operation results of the real service
  - smart proxy: additional functionality that is invisible to the client
    - counting, logger, etc.
- 