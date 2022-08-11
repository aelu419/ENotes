- [JS To React](#js-to-react)
  - [React Components](#react-components)
  - [React Props](#react-props)
  - [React States](#react-states)
- [React to NextJS](#react-to-nextjs)
  - [Optimizations](#optimizations)
  - [Rendering](#rendering)
  - [Networking](#networking)

# JS To React
- *def.* DOM: object representation of HTML elements, has tree-like structure where the root is `html`
  - js manipulates DOM [example](0.js2React/index.html)
  - the html code is the initial page content, the DOM is the current page content
- React as a declarative UI library
  - imperative vs. declarative
    - imperative focuses more on low-level implementation (per-step manipulation of DOM)
    - declarative focuses more on declaring high-level goals (UI end result)
  - `react`: core react library
  - `react-dom`: DOM-specific methods
    - *ex.* [adding element to DOM](0.js2React/1.react_intro.html)

## React Components
- components are functions returning UI elements
  - *ex.*
    ``` jsx
    function Header() {
        return (<h1>Hello React Component</h1>);
    }

    ReactDOM.render(<Header />, app)
    ```
  - note the capitalization and usage as tag
- component nesting
  - *ex.*
    ``` jsx
    
    // ... Header function

    function HomePage() {
        return (
            <div>
            <Header />
            </div>
        )
    }
    
    ReactDOM.render(<HomePage />, app);
    ```

## React Props
- passing props to a component
  - *ex.*
    ``` jsx

    // example: props as an entire object
    // function Header(props) {
    //   props.title
    // }

    // deconstructing props
    function Header({ title }) {
        return <h1>{title}</h1>;
    }

    // reusing the component
    function HomePage() {
        return (
            <div>
            <Header title="Hello Title" />
            <Header title="Hellow Another Title" />
            </div>
        )
    }
    ```
- handle arrays
  - *ex.*
    ``` jsx
    <ul>
    {
        [1, 2, 3].map(i => (<li key={i}>{i}</li>))
    }
    </ul>
    ```
  - here `key` is needed so React can keep track of individual elements

## React States
- event listeners
  - *ex.*
    ``` jsx
    function handleClick(){
        // ..
    }

    // ...
    <button onClick={handleClick}>Submit</button>
    ```
  - arrow functions are also usable
- *def.* **states** are internal states of some UI component which changes over time
  - registering a state and its setter: `const [foo, setFoo] React.useState()`
  - to provide initial value, do `useState(val)` instead
  - to use a state within some element
    - *ex.*
      ``` jsx
      function HomePage() {
        const [n, setN] = React.useState();
        function handleClick() { setN(n+1); }
        return (
            <button onClick={handleClick}>{n}</button>
        );
      }
      ```

# React to NextJS
- switching to NPM for package management
  - `next`, `react`, `react-dom`
- in `pages`, use `index.jsx` where the module default export is the homepage component (*i.e.* function)
- deploy development server locally with `npx next dev`
  - the compiled site is saved at `.next`
- [example project](1.react2Next/pages/index.jsx)

## Optimizations
- development stage optimizations
  - optimizes developer experience (TS, linting, fast-refresh, etc.)
- production stage optimizations
  - compiling: going from easy to read code to more low-level code
  - minifying: basically automated golfing
  - bundling: resolving dependencies and merging modules into optimized bundles for the browser
    - reduce number of requests (and therefore request waterfalls, *i.e.* download A, then B, then C, ... each with latency to/from the server)
  - code splitting: splitting the application's code bundles into different chunks, depending on different entry points to the app
    - prevents duplicate downloading of the same content

## Rendering
- build vs. run time
  - build time: series of steps preparing code for production, including
    - static HTML
    - server-side js code (for rendering)
    - client-side js code (for interactivity)
    - CSS
  - runtime is when the page is served as response to request
- *def.* **rendering** is the process from React code to HTML representation of the UI
  - pre-rendering
    - server-side rendering
      - specified via `getServerSideProps`
      - **hydration** non-interactive HTML + reactive JS (originally React code)
      - note that new React/Next uses **server side components** to further reduce client workload
    - static site generation
      - specified via `getStaticProps`
      - there is no server at runtime, only static pages are served
  - client-side rendering
    - client sees blank page when rendering work is being done
  - NextJS allows choosing from these methods per component

## Networking
- origin server
  - authoritative actions
  - results cached in CDN servers
- content delivery network (CDN) servers
  - static content in multiple locations to serve clients in proximity
- edge servers
  - distributed to multiple locations, but runs code as well as serving static content
  - reduce load on origin server