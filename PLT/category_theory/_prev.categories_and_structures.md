- [Introduction](#introduction)
- [Categories](#categories)
  - [Definitions](#definitions)
  - [Basic Descriptions of Categories](#basic-descriptions-of-categories)
  - [Category Diagrams](#category-diagrams)
  - [Basic Category Constructions](#basic-category-constructions)
  - [Types of Morhpisms](#types-of-morhpisms)

# Introduction
- book: Andrea Asperti, Giuseppe Longo, *Categories Types and Structures*
- methodological connection between category theory in math and programming language theory in computer science
- establish connection between types/programs and objects/morphisms

# Categories

## Definitions
- category theory is a theory of functions, where the basic operation is composition
  - $g \circ f$ "g of f"
  - always associative
  - identity always exists ($a \to a$)
- the abstract definition of a category
  - contains a set of **objects** $Ob_C$
  - contains a set of **morphisms(arrows)** $Mor_C$
  - two operations $dom$ and $cod$ that assign to each arrow its domain (or source) and codomain (or target) respectively
  - the operation $id$ that assigns each object $b$ a morphism $id_b$ (in $Mor_C$) that maps from $b$ to $b$
    - $dom(id_b)=cod(id_b)=b$
    - the *identity law* must be satisifed:
      - $id_b \circ f = f$
      - $g \circ id_b = g$
  - an operation $\circ$ (composition) that...
    - to each pair of morhpisms $f, g$ where $dom(f)=cod(g)$
    - assigns an arrow $f \circ g$, where
      - $dom(f \circ g) = dom(g)$, $cod(f \circ g) = cod(f)$
- the textbook uses the word "collection" instead of set, while some other sources use "class" or "set"
  - another formulation for morphisms is that for every pair or objects $a, b$ the category $C$ contains a set of morphisms from $a$ to $b$ called $Hom(a, b)$
  - the book does not include this in the definition, but it does provide the notation $C[a,b]$ for such a set
    - $f \in C[a, b]$ implies $f: a \to b$, or in other words $dom(f)=a \land cod(f)=b$
- declaration of a morphism looks similar to a function, except for two objects instead of two sets
  - $f: a \to b$
- a morphism is *one* object to *one* object
  - those objects themselves *can* be sets, but it's not required
  - a morphism does not map from multiple object / to multiple objects
  - there can be more than one morphism from the same source to the same destination, with the exception of the identity
- for any object $b$ in a category, there is only one identity $id_b$
  - for some "different" identity $id_b'$, $id_b' = id_b' \circ id_b = id_b$ which brings it back to the original one

## Basic Descriptions of Categories
- example categories
  - **Set**: objects are sets, morphisms are functions
  - **Vect**: objects are vector spaces, morphisms are linear transforms
  - note that these cases adhere to the definition where $Ob_C$ is a *class*
    - namely, there is some defining characteristic that is shared by all objects
- simplext category
  - one object
  - one arrow (the identity)
  - this categories is called $\bold{1}$ in the context of category theory
- discrete category
  - $Mor_C$ only contains identities
- a **preorder** is a category where
  - $|C[a, b]| \leq 1$ for all $a, b$
  - a preorder can be completely defined by a relation $R_C$, where $aRb \iff C[a, b] = \{ f: a \to b \}$
  - note that $R_C$ is by itself a preorder (i.e. reflexive and transitive)
    - the identity property of categories imply that $R$ must be reflexive
    - the compisition property of categories imply that $R$ must be transitive
  - every discrete category is a preorder
  - the smallest non-discrete preorder is the $\bold{2}$, with $0R0$, $0R1$, $1R1$
- **monoids** as categories
  - a monoid is a set with one identity element and one associative binary operation
    - *ex.* $(A, \cdot)$ defines the monoid with identity element $A$ and operation $\cdot$
    - the category $\bold{1}$ is the monoid $(a, \circ)$
- **deductive systems** as categories
  - objects are propositions, morphisms are proofs for some "a **entails** b"

## Category Diagrams
![](../res/cts_for_programmers/1.1.png)
- arrows represent morphisms
- diagram "commutes" when different paths from the same source ($a$ for the left diagram, $b$ for the right diagram) evaluates to the exact same destination ($d$ for the left diagram, $c$ for the right diagram)
- while the book will use equations (such as the identity law and the composition operator), diagrams are a equivalent way of expressing category information

## Basic Category Constructions
- lists a number of simple but slightly more structured categories than the general category
- **subcategory** D of category C
  1. has $Ob_D \subseteq Ob_C$
  2. has $D[a, b] \subseteq C[a, b]$
  3. with the *coinciding* compositions and identities as C
  - a *full* subcategory has $D[a, b]=C[a, b]$
    - such a subcategory is determined completely by which objects it include (and of course which category it is derived from)
    - other subcategories can still choose which $f \in D[a, b]$ to ignore
- the **dual** category $C^{op}$ of $C$
  - same objects, same morhpisms, but flipped operations
  - $id^{op}_b = id_b$
  - $dom^{op}(f)=cod(f)$
  - $cod^{op}(g)=dom(g)$
  - $f \circ^{OP} g = g \circ f$
  - basically, the arrows are still there but are interpreted in the different direction
  - properties:
    - $C^{op}[a, b] = C[b, a]$
    - $(C^{op})^{op}=C$
  - **duality**
    - if $P$ is true for a category $C$, then $P^{op}$ is true for $C^{op}$
    - if $P$ is true for all categories, so is $P^{op}$, since every category has its dual
    - the dual of a diagram commutes iff the original diagram commutes (the dual is just flipping all the arrows)
- for two categories $C$ and $D$, the **product category** $C \times D$
  - has objects $(c, d)$ for all $c$ and all $d$
  - has morphisms $(f, g)$ for all morphisms $f:c \to c'$, $g: d \to d'$
    - $(f, g): (c, d) \to (c', d')$
    - $f$ and $g$ must both exist for $(f, g)$ to be included in the product, if there's only one of them it doesn't form a pair
  - has identities $id_{(c, d)}=(id_c, id_d)$
  - has compositions $(f, g) \circ (f', g') = (f \circ f', g \circ g')$
- for category $C$ with object $c$, the **slice category over** $c$, $C_{/c}$
  - has objects $\{f \in Mor_C | cod(f)=c\}$ (note: morphisms in the original category are treated as *objects* in the over category)
    - *ex.* $f: X \to c$, $f': X' \to c$
  - has morphisms $g: f \to f'$, where each $g$ is an arrow in $C$ such that $f' \circ g = f$
    - *ex.* $g: X \to X', \exists f, \exists f' \iff C_{/c}[f, f']$
  - has same identities and compositions as $C$
  - ![](../res/cts_for_programmers/1.2.png)
    - essentially maps between the inverse images of $c$ in the slice
  - the textbook uses the notation $C_{\downarrow a}$ with the name "**category over** a" when talking about general objects in category $C$, and $C_{/I}$ with the name "**slice category** over I$ when talking about object $I$ as some "index set"
    - this is used in the context that $C$ is $\bold{Set}$, which means each $f$ can be thought of as the the collection $\{\{i\} \times f^{-1}(i)\}$
    - an object in a *slice* $f: X \to I$ is a **generalized object** of $C$
    - a **section** of that generalized object is $s: I \to X$ such that $f \circ s = id_I$
      - for each element $i$, the section function returns some $s(i)\in f^{-1}(I)$
  - the dual of the over category is the **under category**, where $(f: c \to X) \in Ob_{C_{\uparrow c}}$, and $g: f \to f'$ satisfies $g \circ f' = f$

## Types of Morhpisms
- epics, monics, and isos are category-theory analogies of what injective functions and etc. in set theory
  - namely, for an injective (one to one) function $f: A \to B$, and two other functions $g, h: C \to A$, if $f \circ g = f \circ h$ then $g = h$
  - $f$ serves as a "left identity" because when it is left-composed with $g$ and $h$, the equality of $g$ and $h$ preserves "between" the equations
- let $C$ and $a, b \in Ob_C$
  - $h: a \to b$ is an **epic** (epimorphism) iff $g \circ h = f \circ h$ implies $g = f$
    - $h$ serves as a "right identity"
  - $h: a \to b$ is a **monic** (monomorphism) iff $h \circ g = h \circ f$ implies $g=f$
    - $h$ serves as a "left identity"
  - $h: a \to b$ is an **iso** (isomorphism) iff $\exists g: b \to a$ where $g \circ h = id$ and $h \circ g = id$
    - objects are isomorphic $\cong$ iff $\exists h$ (iso)
    - isos are both epics and monics
- **split**
  - a monic $h$ is "split" if $\exists g:b \to a, g \circ h = id_a$
    - $g$ is a **retraction** of the monic/epic $h$
    - $h$ is a **section** of $g$
    - $b$ is a **retract** of $a$
  - a epic $h$ is "split" if $\exists g:b \to a, h \circ g = id_b$
    - $a$ is a **retract** of $b$, etc.

- for "most of the time", such as when $C = \bold{Set}$
  - monos are injective maps
  - epis are surjective maps
  - isos are bijective maps
- monics and epics are duals
  - a monic in $C$ is an epic in $C^{op}$
  - an epic in $C$ is a monic in $C^{op}$

> dropped off, switched to Category Theory for Programmers (newer book, starts focused on programming at the beginning)