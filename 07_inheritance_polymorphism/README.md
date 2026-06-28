# Chapter 7: Inheritance & Polymorphism

This chapter covers the two pillars of object-oriented design in C++: inheritance, which lets one class reuse and extend another, and polymorphism, which lets code call the right behavior for an object through a common base-class interface. You will see how `virtual` functions enable runtime (dynamic) dispatch, how abstract classes define interfaces with pure virtual functions, and how to handle the complications of multiple inheritance and the diamond problem.

A consistent theme is safety and intent: always give a polymorphic base class a `virtual` destructor, use the C++11 `override` keyword to catch signature mistakes, and remember that polymorphism only works through pointers or references — copying a derived object into a base-class value "slices" it. The later sections (7.6 onward) dig into these subtleties, and the guiding principle throughout is to prefer composition over deep inheritance hierarchies.

## 7.1 Inheritance Basics

Inheritance models an "is-a" relationship: a derived class automatically gains the members of its base class and can add its own. The base class holds the shared state and behavior (here `name`, `age`, `eat()`, `sleep()`), and the derived class extends it (`Dog` adds `bark()`). The derived constructor must initialize the base part by calling the base constructor in its member-initializer list (`: Animal(n, a)`). Members marked `protected` are accessible to derived classes but not to outside code, which is how a base shares internals with its subclasses while keeping them hidden from the public.

```cpp
// Base class
class Animal {
protected:
  string name;
  int age;

public:
  Animal(string n, int a) : name(n), age(a) { }
  
  void eat() { cout << name << " is eating" << endl; }
  void sleep() { cout << name << " is sleeping" << endl; }
};

// Derived class (inherits from Animal)
class Dog : public Animal {
public:
  Dog(string n, int a) : Animal(n, a) { }
  
  void bark() { cout << name << " says: Woof!" << endl; }
};

int main() {
  Dog d("Rex", 5);
  d.eat();           // From Animal
  d.bark();          // From Dog
  
  return 0;
}
```

### Inheritance Types

The access specifier before the base class (`public`, `protected`, or `private`) controls how the base's members are exposed through the derived class — it does not change what the derived class itself can access. `public` inheritance preserves the base's access levels and is the only form that models a true "is-a" relationship usable polymorphically; it is what you want the vast majority of the time. `protected` and `private` inheritance progressively restrict the inherited members and express "implemented-in-terms-of" relationships, but they are rarely needed and are usually better replaced by composition (making the base a member instead).

```cpp
// Public inheritance (most common)
class Derived : public Base { };
// Public members stay public, protected stay protected

// Protected inheritance
class Derived : protected Base { };
// Public/protected members become protected

// Private inheritance
class Derived : private Base { };
// Public/protected members become private
```

---

## 7.2 Virtual Functions & Polymorphism

A `virtual` function enables runtime polymorphism: when you call it through a base-class pointer or reference, C++ dispatches to the override matching the object's *actual* type rather than the static type of the pointer. This is what lets a single loop over `Animal*` call `Dog::sound` for dogs and `Cat::sound` for cats. The C++11 `override` keyword on the derived function asks the compiler to verify it really overrides a base virtual, catching subtle signature typos. Critically, any class used polymorphically must declare a `virtual` destructor — otherwise `delete` through a base pointer fails to call the derived destructor, leaking resources and causing undefined behavior.

```cpp
class Animal {
public:
  // Virtual function (can be overridden)
  virtual void sound() {
    cout << "Generic animal sound" << endl;
  }
  
  virtual ~Animal() { }  // Virtual destructor
};

class Dog : public Animal {
public:
  void sound() override {  // C++11: override keyword
    cout << "Woof!" << endl;
  }
};

class Cat : public Animal {
public:
  void sound() override {
    cout << "Meow!" << endl;
  }
};

int main() {
  // Polymorphism in action
  Animal* animals[] = {
    new Dog(),
    new Cat(),
    new Animal()
  };
  
  for (int i = 0; i < 3; i++) {
    animals[i]->sound();  // Calls appropriate function
  }
  
  // Output:
  // Woof!
  // Meow!
  // Generic animal sound
  
  delete animals[0];
  delete animals[1];
  delete animals[2];
  
  return 0;
}
```

### Virtual Functions Mechanism

Under the hood, virtual dispatch is typically implemented with a vtable: each polymorphic class has a hidden table of function pointers, and each object carries a hidden pointer to its class's table. A virtual call looks up the function through that pointer at runtime, so the correct override is chosen based on the real object type — a small, constant overhead per call. A *pure* virtual function, declared with `= 0`, has no (required) implementation and makes its class abstract, forcing every concrete derived class to supply an override. This is the standard way to define an interface that subclasses must implement.

```cpp
// When you call a virtual function:
// 1. Check object's actual type
// 2. Find appropriate implementation
// 3. Call that function

class Shape {
public:
  virtual void draw() = 0;  // Pure virtual
};

class Circle : public Shape {
public:
  void draw() override { cout << "Drawing circle" << endl; }
};

class Square : public Shape {
public:
  void draw() override { cout << "Drawing square" << endl; }
};

void renderShape(Shape* shape) {
  shape->draw();  // Calls correct implementation
}
```

---

## 7.3 Abstract Classes & Interfaces

An abstract class is one that has at least one pure virtual function (`= 0`) and therefore cannot be instantiated directly — you can only create objects of concrete derived classes that override every pure virtual. This lets you define a contract: `Shape` declares that every shape must provide `area()` and `perimeter()` without saying how, and `Rectangle` and `Circle` fill in the details. A class consisting solely of pure virtual functions acts as a pure interface, the C++ equivalent of an interface in languages like Java. Always give such a base a `virtual` destructor so objects can be safely deleted through a `Shape*`.

```cpp
// Abstract base class (cannot instantiate)
class Shape {
public:
  virtual double area() const = 0;      // Pure virtual
  virtual double perimeter() const = 0;
  virtual ~Shape() { }
};

class Rectangle : public Shape {
private:
  double width, height;

public:
  Rectangle(double w, double h) : width(w), height(h) { }
  
  double area() const override { return width * height; }
  double perimeter() const override { return 2 * (width + height); }
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(double r) : radius(r) { }
  
  double area() const override { return 3.14159 * radius * radius; }
  double perimeter() const override { return 2 * 3.14159 * radius; }
};

int main() {
  // Shape s;  // ERROR: cannot instantiate abstract class
  
  Rectangle r(10, 20);
  Circle c(5);
  
  cout << "Rectangle area: " << r.area() << endl;
  cout << "Circle perimeter: " << c.perimeter() << endl;
  
  return 0;
}
```

---

## 7.4 Multiple Inheritance

C++ allows a class to inherit from more than one base at once, combining their interfaces — here `ColoredCircle` is both a `Shape` and `Colored`. Each base must be initialized in the derived constructor's member-initializer list. Multiple inheritance is powerful for mixing in independent capabilities (especially when the bases are pure interfaces), but it adds complexity: name clashes between bases must be disambiguated, and inheriting the same base along two paths creates the "diamond problem" described below. A common guideline is to limit multiple inheritance to combining interfaces rather than concrete, state-carrying classes.

```cpp
class Shape {
public:
  virtual void draw() = 0;
};

class Colored {
protected:
  string color;
public:
  Colored(string c) : color(c) { }
  string getColor() const { return color; }
};

// Multiple inheritance
class ColoredCircle : public Shape, public Colored {
private:
  double radius;

public:
  ColoredCircle(double r, string c) 
    : Colored(c), radius(r) { }
  
  void draw() override {
    cout << "Drawing " << color << " circle" << endl;
  }
};

int main() {
  ColoredCircle cc(5, "red");
  cc.draw();
  cout << cc.getColor() << endl;  // "red"
  
  return 0;
}
```

### Diamond Problem

The diamond problem arises when a class inherits from two classes that both derive from the same base: by default the most-derived class ends up with two separate copies of that base sub-object, making its members ambiguous and wasteful. The fix is *virtual inheritance* — declaring the intermediate classes with `virtual public Animal` tells the compiler to share a single common base sub-object among all paths. The trade-off is that the most-derived class then becomes responsible for initializing the shared virtual base directly (covered in detail in section 7.10).

```cpp
//      Animal
//     /      \
//   Dog      Cat  
//     \      /
//   DogCat (ERROR without virtual)

// ✅ Solution: Virtual inheritance
class Animal { };
class Dog : virtual public Animal { };
class Cat : virtual public Animal { };
class DogCat : public Dog, public Cat { };
```

---

## 7.5 Best Practices

This section gathers the chapter's key recommendations. Use `virtual` functions to express polymorphic behavior, mark every override with the C++11 `override` keyword so the compiler verifies it, and always declare a `virtual` destructor in any class meant to be used as a polymorphic base. Use `protected` to share state with derived classes, avoid multiple inheritance of concrete (state-carrying) classes, and most importantly prefer composition (holding an object as a member) over inheritance when there is no genuine "is-a" relationship — composition gives looser coupling and more flexibility.

```cpp
// ✅ Use virtual functions for polymorphism
class Base {
public:
  virtual void process() = 0;
  virtual ~Base() { }
};

// ✅ Use override keyword (C++11)
class Derived : public Base {
public:
  void process() override { }
};

// ✅ Virtual destructor in base classes
class Base {
public:
  virtual ~Base() { }
};

// ✅ Use protected for derived class access
class Base {
protected:
  int data;
};

// ✅ Avoid multiple inheritance of concrete classes
// ❌ class Multiple : public ClassA, public ClassB { };

// ✅ Prefer composition over inheritance
class Engine { };
class Car {
private:
  Engine engine;  // Composition
};
```

---

## Summary

| Concept | Details |
|---------|---------|
| **Inheritance** | Derive from base class |
| **Virtual** | Dynamic dispatch, polymorphism |
| **Override** | Replace base implementation |
| **Abstract** | Pure virtual, cannot instantiate |
| **Polymorphism** | Same interface, different implementations |
| **Destructor** | Must be virtual in base |

---

## 7.6 Object Slicing

**Object slicing** occurs when a derived class object is copied or assigned into a **base class variable** (by value). The derived-class data members are silently discarded — "sliced off" — and only the base part is copied. This is one of the most insidious OOP bugs in C++.

```cpp
#include <iostream>
using namespace std;

struct Animal {
  string name;
  virtual string sound() const { return "..."; }
  Animal(const string& n) : name(n) {}
};

struct Dog : Animal {
  string breed;
  Dog(const string& n, const string& b) : Animal(n), breed(b) {}
  string sound() const override { return "Woof"; }
};

// ❌ Slicing via value copy — Dog becomes Animal
Animal a = Dog("Rex", "Labrador");  // Dog is SLICED: breed is lost
cout << a.sound();    // "..." — virtual dispatch fails: vtable points to Animal
cout << a.name;       // "Rex" — base part survived
// a.breed             // doesn't exist — Dog part is gone

// ❌ Slicing via function parameter by value
void makeNoise(Animal a) {         // takes Animal by value
  cout << a.sound() << endl;       // always calls Animal::sound
}
makeNoise(Dog("Buddy", "Poodle")); // Dog sliced at the function call

// ✅ Solutions: pass/store by pointer or reference
void makeNoiseRef(const Animal& a) { cout << a.sound() << endl; }
void makeNoisePtr(const Animal* a)  { cout << a->sound() << endl; }

Dog d("Max", "Husky");
makeNoiseRef(d);   // "Woof" — no slicing, virtual dispatch works
makeNoisePtr(&d);  // "Woof" — no slicing, virtual dispatch works

// ✅ Store polymorphic objects via smart pointer
#include <memory>
#include <vector>

vector<unique_ptr<Animal> > zoo;
zoo.push_back(unique_ptr<Animal>(new Dog("Rex", "Lab")));
for (size_t i = 0; i < zoo.size(); ++i) {
  cout << zoo[i]->sound() << endl;  // "Woof" — works correctly
}
```

| Scenario | Slicing? | Fix |
|---|---|---|
| `Animal a = dog;` | Yes | Use `Animal& a = dog;` or `Animal* a = &dog;` |
| `void f(Animal a)` | Yes | Change to `void f(const Animal& a)` or `void f(const Animal* a)` |
| `vector<Animal>` | Yes | Change to `vector<Animal*>` or `vector<unique_ptr<Animal>>` |
| `Animal& a = dog;` | No | Reference — no copy made |
| `unique_ptr<Animal>` | No | Pointer — original object preserved |

**Rule:** Never store polymorphic objects by value. Always use references, pointers, or smart pointers. If you need value semantics for a polymorphic type, implement a virtual `clone()` method.

---

## Summary

| Concept | Details |
|---------|---------|
| **Inheritance** | Derive from base class |
| **Virtual** | Dynamic dispatch, polymorphism |
| **Override** | Replace base implementation |
| **Abstract** | Pure virtual, cannot instantiate |
| **Polymorphism** | Same interface, different implementations |
| **Destructor** | Must be virtual in base |
| **Object slicing** | Value copy of derived discards derived-only members |

---

## 7.7 `override` and `final` (C++11)

Without `override`, a typo in a virtual function name silently creates a new
function instead of overriding the base. `override` turns that into a compile
error. `final` prevents further overriding or inheritance.

```cpp
struct Base {
    virtual void draw() const {}
    virtual void update(int dt) {}
};

struct Derived : Base {
    void draw() const override {}      // OK: matches Base::draw
    // void drwa() const override {}   // ERROR: no virtual drwa() in Base
    void update(int dt) override {}    // OK
};

struct Leaf final : Derived {         // no class may inherit from Leaf
    void draw() const override final {} // no class may override draw() again
};

// struct Bad : Leaf {};              // ERROR: Leaf is final
```

**Rules:**
- `override` must be placed after the parameter list, before `{` or `= 0`
- `final` on a class prevents any inheritance; `final` on a method prevents override
- Both are context-sensitive identifiers, not reserved keywords — you can still use them as variable names (avoid doing so)

---

## 7.8 Covariant Return Types

An overriding virtual function may return a pointer/reference to a *more derived*
type than the base version — the return type is called covariant.

```cpp
struct Animal {
    virtual Animal* clone() const { return new Animal(*this); }
};

struct Dog : Animal {
    Dog* clone() const override { return new Dog(*this); } // covariant: Dog* <- Animal*
};

Animal* a = new Dog;
Dog* d = static_cast<Dog*>(a->clone()); // clone() correctly returns Dog*
delete a; delete d;
```

Covariant return types work with pointers and references but **not** plain values.

---

## 7.9 Pure Virtual Destructor

A class can be made abstract via a pure virtual destructor. This is useful when
you want an abstract base but have no other natural pure virtual method to use.
You must still provide a definition, because destructors are always called.

```cpp
struct AbstractBase {
    virtual ~AbstractBase() = 0;  // pure virtual destructor
};
AbstractBase::~AbstractBase() {}  // definition required

struct Concrete : AbstractBase {
    ~Concrete() override {}
};

// AbstractBase ab;  // ERROR: abstract
Concrete c;         // OK
AbstractBase* p = new Concrete;
delete p;           // calls ~Concrete(), then ~AbstractBase()
```

---

## 7.10 Virtual Base Classes — Fixing the Diamond Properly

`virtual` inheritance makes the compiler share a single copy of the base class
in a diamond hierarchy instead of creating two separate sub-objects.

```cpp
struct A {
    int x = 0;
    virtual void hello() { std::cout << "A\n"; }
};

struct B : virtual A {};   // virtual inheritance
struct C : virtual A {};   // virtual inheritance

struct D : B, C {};        // one shared A sub-object

D d;
d.x = 42;        // unambiguous: only one A::x
d.hello();       // unambiguous: only one A::hello

// Without virtual inheritance D would have two A sub-objects
// and d.x would be ambiguous.
```

**Rules for virtual base initialization:**
- The most-derived class (`D`) is responsible for initializing the virtual base (`A`)
- `B` and `C` initializer-list calls to `A`'s constructor are ignored when constructing `D`

```cpp
struct A { A(int v) : x(v) {} int x; };
struct B : virtual A { B() : A(1) {} };   // A(1) ignored when D is constructed
struct C : virtual A { C() : A(2) {} };   // A(2) ignored when D is constructed
struct D : B, C    { D() : A(99) {} };    // D initializes A directly

D d; // d.x == 99
```

---

## 7.11 Inheriting Constructors (C++11)

By default a derived class does **not** inherit its base's constructors — you must re-declare each one and forward the arguments by hand. C++11's `using Base::Base;` declaration inherits all of the base constructors at once.

```cpp
#include <string>
#include <iostream>
using namespace std;

struct Base {
  Base(int x)                 { cout << "Base(int)\n"; }
  Base(const string& s)       { cout << "Base(string)\n"; }
  Base(int x, double y)       { cout << "Base(int,double)\n"; }
};

// --- Pre-C++11: tedious manual forwarding ---
struct OldDerived : Base {
  OldDerived(int x)           : Base(x) {}
  OldDerived(const string& s) : Base(s) {}
  OldDerived(int x, double y) : Base(x, y) {}   // repeat for every constructor
};

// --- C++11: inherit them all in one line ---
struct Derived : Base {
  using Base::Base;           // inherits Base(int), Base(string), Base(int,double)
};

Derived d1(42);          // Base(int)
Derived d2("hello");     // Base(string)
Derived d3(1, 2.0);      // Base(int,double)
```

**Rules and caveats:**
- `using Base::Base;` brings in **all** of the base's constructors (except the default, copy, and move constructors, which are handled separately).
- If `Derived` adds its own data members, prefer in-class member initializers so inherited constructors still leave them initialized:

```cpp
struct Derived2 : Base {
  using Base::Base;
  int extra = 0;          // initialized even when an inherited constructor runs
};
```

- A constructor you declare in `Derived` with the same signature **hides** the corresponding inherited one.

| Approach | Effort | When |
|---|---|---|
| Manual forwarding constructors | One per base constructor | Need to add logic in the derived constructor |
| `using Base::Base;` | One line, inherits all | Derived just extends the base with no extra construction logic |

---

## Next Steps
- Design class hierarchies
- Use virtual functions for polymorphism
- Create abstract base classes
- Move to [Chapter 8: Operator Overloading](../08_operator_overloading/README.md))
