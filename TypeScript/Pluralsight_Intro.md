# Intro
- superset of JS
  - static typing
  - organization support (classes, namespaces, modules, interfaces)
  - sourcemap from JS back to TS during debug
- JS has problems scaling and hinting
- TS compiles to JS
- a valid JS program is automatically a valid TS program
- [documentation](typescriptalang.org)

# Environment Setup
- install `typescript` with NPM
- the `tsc` or `npx tsc` command compiles the argument `.ts` file to a same named `.js` file in the same directory
- the compiler takes flags
## Config File
- by default, `tsconfig.json` under the working directory
- can be created with `tsc --init`

# Language Basics
## Types
- type inference upon initialization
  - ```typescript
    let a = 1;
    a = 'a'; // causes error
    ```
- explicit typing
  - `let a: number;`
- typed parameters
  - `function foo(bar: string): number { ... }`
  - return type can be deduced
  - return type can be `void`
## Class
```typescript
class Customer {
    name: string; // typed member variable
    private isActive: boolean;

    constructor(name: string) {
        this.name = name; // this

        this.isActive = true; // note that all member variables need to be instantiated upon construction, or else error
    }

    // public method
    announce() {
        return this.name;
    }

    protected foo() {

    }
}

class SuperCustomer extends Customer {
    // ...
}
```

## Module
- can choose between different export/import syntax, like the `export` `import` or `module.exports` `require`
- set relevant fields in the config file