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
- 