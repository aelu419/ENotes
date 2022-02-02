# $\textrm{Intro to KnockoutJS}$

- [$\textrm{Intro to KnockoutJS}$](#textrmintro-to-knockoutjs)
- [Introduction](#introduction)
- [Lists and Collections](#lists-and-collections)

# Introduction
- *def.* **data bind**: associate html view with properties in the view model
  - in JS:
    ```js
    // This is a simple *viewmodel* - JavaScript that defines the data and behavior of your UI
    function AppViewModel() {
        this.firstName = "Bert";
        this.lastName = "Bertington";
    }

    // Activates knockout.js
    ko.applyBindings(new AppViewModel());
    ```
  - in html:
    ```html
    <!-- This is a *view* - HTML markup that defines the appearance of your UI -->

    <p>First name: <strong data-bind="text: firstName">todo</strong></p>
    <p>Last name: <strong data-bind="text: lastName">todo</strong></p>
    ```
  - ``todo`` is the default data
  - ``data-bind="<scheme>: <field>"``
- JS constructor functions
  - ``function foo() { this.param = 'bar' };``
  - ``new foo()`` creates ``Object { param: 'bar' }``
  - within ``foo``, ``this`` refers to the object to be created
- In KO, we specify the constructor object of the view model and bind it to the view with ``ko.applyBindings``
  - use **observables** to communicate between views and view models
  - use **computed properties** to represent real-time computed values
    - ``ko.computed( function() { return this.firstName() + ' ' + this.lastName() }, this) ``
    - the first parameter is how to compute the property *w.r.t. the object context*
    - the second parameter specifies the object context (namely, the view model object created by the constructor function)
  - other bindings include ``click``, etc.
    - *ex.* ``<button data-bind="click: capitalize">capitalize</button>``
      - where 
        ```js
        this.capitalize = function() {
            var temp = this.lastName();
            this.lastName(temp.toUpperCase());
        }
        ```
  - note that ``observable()`` returns the observable field
  - ``observable(val)`` sets the observable to val while also returning the field

# Lists and Collections
