- [MVVM](#mvvm)
- [Knockout](#knockout)
- [Observables](#observables)
- [Observable arrays](#observable-arrays)

# MVVM
- *def.* **model**: objects in the domain
- *def.* **view**: UI
- *def.* **view model**: code representing data/operations *on* the UI, but not necessarily in the domain

# Knockout
- the goal is to be complementary to other JS frameworks
- declarative bindings and UI refresh
- dependency
- templating

# Observables
- are *functions*
  - this is to account for certain browsers not supporting getter/setters
- types
  - normal observable
  - observable arrays
  - dependent observables (aka. computed)

# Observable arrays
- 