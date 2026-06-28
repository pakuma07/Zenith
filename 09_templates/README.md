# Chapter 9: Templates

Templates are C++'s mechanism for **generic programming**: writing code once that works with many different types, with the actual types filled in by the compiler at compile time. Instead of duplicating a function or class for `int`, `double`, `string`, and every other type, you write a single template parameterized over a type (or value), and the compiler generates a concrete *instantiation* for each set of arguments you actually use. This delivers full type safety with zero runtime overhead, since all the work happens during compilation.

This chapter progresses from function and class templates through specialization, the C++11 additions of variadic templates and partial specialization, non-type and default parameters, and SFINAE-based compile-time selection. Later sections (9.9 onward) cover advanced idioms such as CRTP, template template parameters, dependent-name disambiguation, explicit instantiation, alias templates, and perfect forwarding. A key thing to remember is that template code is only fully checked when instantiated, so error messages can be verbose and a template's definition normally must live in a header.

## 9.1 Function Templates

A function template defines a family of functions parameterized over one or more types, declared with `template <typename T>` (the keyword `class` may be used in place of `typename` here). When you call the function, the compiler performs **template argument deduction**, inferring `T` from the types of the arguments, so `add(5, 3)` deduces `T = int` automatically. You can also specify the type explicitly with `add<double>(...)` when deduction is impossible or you want a particular instantiation. The body must compile for whatever type is supplied — for example `add` requires that `T` support `operator+` — otherwise you get an instantiation-time error.

```cpp
// Generic function that works with any type

template <typename T>
T add(T a, T b) {
  return a + b;
}

int main() {
  cout << add(5, 3) << endl;           // 8 (T = int)
  cout << add(3.5, 2.1) << endl;       // 5.6 (T = double)
  cout << add(string("Hello"), 
             string(" World")) << endl;  // "Hello World" (T = string)
  
  return 0;
}
```

### Multiple Template Parameters

A template can declare several type parameters, each deduced or specified independently, as in `template <typename T, typename U>`. This is useful when a function relates two distinct types — here `convertValue` takes a value of type `U` and converts it to a result type `T`. Because `T` appears only in the return type and cannot be deduced from the arguments, the caller must supply it explicitly with `convertValue<int>(...)`; the order matters, since explicit arguments fill the parameter list left to right.

```cpp
template <typename T, typename U>
T convertValue(U value) {
  return static_cast<T>(value);
}

int main() {
  int i = convertValue<int>(3.14);     // 3
  double d = convertValue<double>(42);  // 42.0
  
  return 0;
}
```

---

## 9.2 Class Templates

A class template parameterizes an entire class over one or more types, which is how generic containers like `std::vector` and `std::stack` are built. Unlike function templates, class templates cannot have their arguments deduced (prior to C++17), so you must always specify them explicitly, as in `Stack<int>` or `Stack<string>`. Each distinct instantiation is a separate, unrelated type — `Stack<int>` and `Stack<string>` share no inheritance relationship even though they come from the same template. Member functions of a class template are themselves templates and are only instantiated when actually called, which keeps unused functionality from being compiled.

```cpp
template <typename T>
class Stack {
private:
  vector<T> elements;

public:
  void push(const T& value) {
    elements.push_back(value);
  }
  
  void pop() {
    if (!elements.empty()) {
      elements.pop_back();
    }
  }
  
  T top() const {
    return elements.back();
  }
  
  bool isEmpty() const {
    return elements.empty();
  }
};

int main() {
  Stack<int> intStack;
  intStack.push(10);
  intStack.push(20);
  cout << intStack.top() << endl;  // 20
  
  Stack<string> stringStack;
  stringStack.push("hello");
  cout << stringStack.top() << endl;  // "hello"
  
  return 0;
}
```

---

## 9.3 Template Specialization

Specialization lets you provide a custom implementation of a template for a particular type or family of types, overriding the generic version. **Full (explicit) specialization**, written `template <>` with concrete arguments like `Printer<bool>`, replaces the whole template for one exact type. **Partial specialization** (available for class templates) leaves some parameters open while fixing a pattern, such as `Printer<vector<T>>`, so it applies to any `vector` regardless of element type. The compiler always selects the most specific matching specialization; note that function templates cannot be partially specialized — you use overloading instead.

```cpp
// Generic template
template <typename T>
class Printer {
public:
  void print(T value) {
    cout << "Generic: " << value << endl;
  }
};

// Specialization for bool
template <>
class Printer<bool> {
public:
  void print(bool value) {
    cout << "Boolean: " << (value ? "true" : "false") << endl;
  }
};

// Partial specialization (C++11)
template <typename T>
class Printer<vector<T>> {
public:
  void print(const vector<T>& vec) {
    cout << "Vector: ";
    for (T val : vec) {
      cout << val << " ";
    }
    cout << endl;
  }
};

int main() {
  Printer<int> p1;
  p1.print(42);            // Generic: 42
  
  Printer<bool> p2;
  p2.print(true);          // Boolean: true
  
  Printer<vector<int>> p3;
  vector<int> v = {1, 2, 3};
  p3.print(v);             // Vector: 1 2 3
  
  return 0;
}
```

---

## 9.4 Variadic Templates (C++11)

Variadic templates, introduced in C++11, accept an arbitrary number of template arguments through a **parameter pack**, declared as `typename... Args`. The pack is expanded with the `...` syntax, and the classic way to process it is recursion: a variadic overload peels off the first argument and forwards the rest with `args...`, while a non-variadic base-case overload terminates the recursion. This is how type-safe variadic facilities like `printf`-style logging, `std::tuple`, and `emplace_back` are built — replacing unsafe C-style varargs. The most common pitfall is forgetting the base case, which causes the recursion to fail to compile when the pack becomes empty.

```cpp
// Base case
template <typename T>
void print(T value) {
  cout << value << endl;
}

// Recursive case
template <typename T, typename... Args>
void print(T first, Args... args) {
  cout << first << " ";
  print(args...);
}

int main() {
  print(1, 2.5, "hello", true);
  // Output: 1 2.5 hello 1
  
  return 0;
}
```

### Variadic Function Templates

This example reinforces the recursive pattern by computing a sum over any number of arguments of mixed (but addable) types. Each recursive call adds the first element to the result of summing the remainder, again bottoming out at a single-argument base case. Because the expansion happens at compile time, the compiler effectively unrolls the recursion into a flat chain of additions, so there is no runtime recursion cost.

```cpp
// Sum any number of values
template <typename T>
T sum(T value) {
  return value;
}

template <typename T, typename... Args>
T sum(T first, Args... args) {
  return first + sum(args...);
}

int main() {
  cout << sum(1, 2, 3, 4, 5) << endl;        // 15
  cout << sum(1.5, 2.5, 3.5) << endl;        // 7.5
  
  return 0;
}
```

---

## 9.5 Non-Type Template Parameters

In addition to types, templates can be parameterized over compile-time **values** — most commonly integers, as in `template <typename T, int SIZE>`. The value must be a constant expression known at compile time, which lets the compiler do things like size a fixed array member (`T data[SIZE]`) without any heap allocation. This is exactly how `std::array<T, N>` works. Allowed non-type parameters in C++11 include integral and enumeration types, pointers, and references; you cannot use floating-point values or class-type objects as non-type parameters.

```cpp
// Template with non-type parameter
template <typename T, int SIZE>
class Array {
private:
  T data[SIZE];
  int count = 0;

public:
  void add(T value) {
    if (count < SIZE) {
      data[count++] = value;
    }
  }
  
  int size() const { return count; }
  T get(int index) const { return data[index]; }
};

int main() {
  Array<int, 10> arr;
  arr.add(1);
  arr.add(2);
  arr.add(3);
  
  cout << "Size: " << arr.size() << endl;  // 3
  
  return 0;
}
```

---

## 9.6 Template Default Arguments

Template parameters can be given default arguments, so callers may omit them and fall back to a sensible default — much like default function arguments. Defaults are written in the parameter list (`typename T = int, int SIZE = 100`) and, as with function defaults, must appear from right to left without gaps. When all parameters have defaults you still need the empty angle brackets `Buffer<>` to request the all-defaults instantiation of a class template. This is widely used in the standard library; for example `std::vector`'s allocator parameter defaults to `std::allocator<T>`.

```cpp
// Default template argument
template <typename T = int, int SIZE = 100>
class Buffer {
private:
  T data[SIZE];

public:
  void store(T value) { }
};

int main() {
  Buffer<> b1;                  // T=int, SIZE=100
  Buffer<double> b2;            // T=double, SIZE=100
  Buffer<double, 50> b3;        // T=double, SIZE=50
  
  return 0;
}
```

---

## 9.7 SFINAE (Substitution Failure Is Not An Error)

SFINAE is the rule that when substituting template arguments produces an invalid type or expression in a function's signature, the compiler silently *discards* that candidate rather than reporting an error — as long as some other overload still matches. This is the basis of compile-time overload selection: by making a template's return type or a parameter valid only for certain types, you enable or disable overloads based on type traits. The standard tool is `std::enable_if` from `<type_traits>`, often combined with predicates like `is_arithmetic` or `is_same`, where `enable_if<Cond, T>::type` exists only when `Cond` is true. The technique is powerful but notoriously hard to read; in modern C++ it is the precursor to the cleaner `if constexpr` and concepts, which are beyond C++11.

```cpp
#include <type_traits>
using namespace std;

// Generic implementation
template <typename T>
typename enable_if<is_arithmetic<T>::value, void>::type
process(T value) {
  cout << "Processing arithmetic type: " << value << endl;
}

// Alternative implementation
template <typename T>
typename enable_if<is_same<T, string>::value, void>::type
process(T value) {
  cout << "Processing string: " << value << endl;
}

int main() {
  process(42);              // Calls first overload
  process(3.14);            // Calls first overload
  process(string("hello")); // Calls second overload
  
  return 0;
}
```

---

## 9.8 Best Practices

Writing good templates is mostly about clarity, correctness, and managing compile-time cost. Prefer descriptive parameter names and `typename` for readability, keep template code `const`-correct, and pass containers by `const` reference to avoid expensive copies. Use specialization or `enable_if` only where genuinely needed, and document the implicit requirements a template places on its type parameters (copyable, comparable with `<`, has `operator<<`, and so on), since C++11 has no language-level way to state these constraints. Clear documentation and small, well-named templates go a long way toward taming the verbose error messages templates are infamous for.

```cpp
// ✅ Use typename for clarity
template <typename T>  // Clear intent
void func(T value) { }

// ✅ Const-correctness
template <typename T>
void processArray(const vector<T>& vec) {
  for (const auto& item : vec) {
    cout << item << " ";
  }
}

// ✅ Use template specialization when needed
template <typename T>
void sort(vector<T>& v) {
  // Generic sort
}

template <>
void sort<string>(vector<string>& v) {
  // String-specific sort (case-insensitive?)
}

// ✅ Enable_if for compile-time selection
template <typename T>
typename enable_if<is_integral<T>::value, T>::type
square(T x) { return x * x; }

// ✅ Document template requirements
template <typename T>
class MyContainer {
  // Requires: T must be copyable
  // Requires: T must be comparable with <
};
```

---

## Summary

| Concept | Details |
|---------|---------|
| **Function Template** | Generic functions |
| **Class Template** | Generic classes |
| **Specialization** | Custom behavior for specific types |
| **Variadic** | Variable number of parameters (C++11) |
| **Non-type** | Constants as template parameters |
| **SFINAE** | Compile-time conditional compilation |
| **Constraints** | Document template requirements |
| **CRTP** | Static polymorphism via self-referential inheritance |
| **Template template params** | Templates that accept template types |
| **Dependent names** | `typename` / `template` disambiguators |
| **Alias templates** | Templated `using` aliases (C++11) |
| **Perfect forwarding** | `T&&` + `std::forward` preserve value category (C++11) |

---

## 9.9 CRTP — Curiously Recurring Template Pattern

CRTP is a technique where a class `Derived` inherits from a template base instantiated with `Derived` itself. It enables **static (compile-time) polymorphism** without virtual functions — zero overhead dispatch.

```cpp
#include <iostream>
using namespace std;

// CRTP base: parameterized by the derived type
template<typename Derived>
struct Shape {
  // Static dispatch: no virtual function overhead
  double area() const {
    return static_cast<const Derived*>(this)->areaImpl();
  }

  double perimeter() const {
    return static_cast<const Derived*>(this)->perimeterImpl();
  }

  // Reuse behavior using the derived class's methods
  void describe() const {
    cout << "Area: "      << area()
         << "  Perimeter: " << perimeter() << endl;
  }
};

struct Circle : Shape<Circle> {
  double r;
  explicit Circle(double radius) : r(radius) {}
  double areaImpl()      const { return 3.14159 * r * r; }
  double perimeterImpl() const { return 2 * 3.14159 * r; }
};

struct Square : Shape<Square> {
  double side;
  explicit Square(double s) : side(s) {}
  double areaImpl()      const { return side * side; }
  double perimeterImpl() const { return 4 * side; }
};

Circle c(5.0);
Square s(4.0);
c.describe();    // Area: 78.5  Perimeter: 31.4
s.describe();    // Area: 16    Perimeter: 16

// CRTP for the "Barton-Nackman trick" — inject operators automatically
template<typename T>
struct Comparable {
  // != and > implemented in terms of == and < (which Derived provides)
  bool operator!=(const T& rhs) const { return !(static_cast<const T&>(*this) == rhs); }
  bool operator> (const T& rhs) const { return rhs < static_cast<const T&>(*this); }
  bool operator<=(const T& rhs) const { return !(static_cast<const T&>(*this) > rhs); }
  bool operator>=(const T& rhs) const { return !(static_cast<const T&>(*this) < rhs); }
};

struct Price : Comparable<Price> {
  double value;
  explicit Price(double v) : value(v) {}
  bool operator==(const Price& o) const { return value == o.value; }
  bool operator< (const Price& o) const { return value <  o.value; }
};

Price p1(10.0), p2(20.0);
cout << (p1 < p2);   // true
cout << (p1 > p2);   // false — inherited from Comparable
cout << (p1 != p2);  // true  — inherited from Comparable
```

| | CRTP | Virtual functions |
|---|---|---|
| Dispatch time | Compile-time | Runtime |
| Overhead | Zero | Vtable pointer per object + indirect call |
| Polymorphism container | No (each type is distinct) | Yes (via base pointer) |
| Use case | Performance-critical, mixins | General OOP hierarchy |

---

## 9.10 Template Template Parameters

A **template template parameter** is a template that itself takes a template as a parameter. This lets you write generic code that is parameterized over a container type, not just a value type.

```cpp
#include <vector>
#include <list>
#include <iostream>
using namespace std;

// A Stack generic over both element type T and container type Container
// Container itself is a template: template<typename> class
template<
  typename T,
  template<typename, typename> class Container = vector   // default: vector
>
class Stack {
  Container<T, allocator<T> > data;
public:
  void push(const T& v)  { data.push_back(v); }
  void pop()             { data.pop_back(); }
  const T& top() const   { return data.back(); }
  bool empty()   const   { return data.empty(); }
  int  size()    const   { return static_cast<int>(data.size()); }
};

Stack<int>            intStack;          // uses vector<int>
Stack<int, list>      listStack;         // uses list<int>
Stack<string, vector> strStack;          // uses vector<string>

intStack.push(1);
intStack.push(2);
cout << intStack.top();   // 2

// Template template in a function
template<typename T, template<typename, typename> class C>
void printAll(const C<T, allocator<T> >& container) {
  for (typename C<T, allocator<T> >::const_iterator it = container.begin();
       it != container.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;
}

vector<int> v = {1, 2, 3};
printAll<int, vector>(v);   // 1 2 3
```

---

## 9.11 Dependent Names — typename and template Disambiguators

Inside a template, names that depend on a template parameter are called **dependent names**. Because the compiler hasn't seen the actual type yet, it cannot tell if a dependent name is a type or a value. Two keywords resolve the ambiguity.

```cpp
#include <vector>
using namespace std;

// --- typename: tell the compiler a dependent name is a TYPE ---
template<typename Container>
void processFirst(const Container& c) {
  // Without typename: compiler might parse this as multiplication
  // Container::iterator it;      // ERROR: compiler doesn't know it's a type
  typename Container::iterator it = c.begin();  // OK: explicitly a type

  typename Container::value_type first = *it;   // OK: value_type is a type
}

vector<int> v = {1, 2, 3};
processFirst(v);   // OK

// --- template: tell the compiler a dependent name is a TEMPLATE ---
struct MyAllocator {
  template<typename T>
  T* allocate(size_t n) { return new T[n]; }
};

template<typename Alloc>
void allocateInts(Alloc& a, size_t n) {
  // Without template keyword: compiler parses allocate<int> as (allocate < int) > n
  // int* p = a.allocate<int>(n);    // ERROR: ambiguous
  int* p = a.template allocate<int>(n);  // OK: explicitly a template call
  delete[] p;
}

MyAllocator ma;
allocateInts(ma, 10);  // OK

// --- Dependent base class: access members with this-> ---
template<typename T>
struct Base {
  void hello() { }
};

template<typename T>
struct Derived : Base<T> {
  void greet() {
    // hello();         // ERROR in some compilers: name lookup doesn't look in Base<T>
    this->hello();      // OK: forces lookup in the dependent base class
    Base<T>::hello();   // Also OK: explicit qualification
  }
};
```

| Keyword | When to use |
|---|---|
| `typename` | Before a dependent type: `typename T::iterator`, `typename T::value_type` |
| `template` | Before a dependent template call: `obj.template method<X>()` |
| `this->` | Before a member of a dependent base: `this->baseMethod()` |

---

## Summary

| Concept | Details |
|---------|---------|
| **Function Template** | Generic functions |
| **Class Template** | Generic classes |
| **Specialization** | Custom behavior for specific types |
| **Variadic** | Variable number of parameters (C++11) |
| **Non-type** | Constants as template parameters |
| **SFINAE** | Compile-time conditional compilation |
| **Constraints** | Document template requirements |
| **CRTP** | Static polymorphism via self-referential inheritance |
| **Template template params** | Templates that accept template types |
| **Dependent names** | `typename` / `template` disambiguators |
| **Alias templates** | Templated `using` aliases (C++11) |
| **Perfect forwarding** | `T&&` + `std::forward` preserve value category (C++11) |

---

## 9.12 Explicit Instantiation and `extern template` (C++11)

By default the compiler instantiates a template whenever it is used, producing
object code in every translation unit that includes the header. For large
projects this can cause significant code bloat and slower link times.

**Explicit instantiation definition** (in one `.cpp` file):

```cpp
// math.h
template<typename T>
T add(T a, T b) { return a + b; }

// math.cpp  -- instantiate once, in exactly one translation unit
template int    add<int>(int, int);
template double add<double>(double, double);
```

**`extern template` declaration** (in the header or other `.cpp` files) — tells
the compiler "do not instantiate this here; the definition exists elsewhere":

```cpp
// math.h (after the function template definition)
extern template int    add<int>(int, int);      // C++11
extern template double add<double>(double, double);
```

Now every translation unit that includes `math.h` skips instantiation of those
specializations and links against the one produced in `math.cpp`.

```cpp
// Example with a class template
template<typename T>
struct Stack {
    void push(T val);
    T    pop();
};

// In stack.cpp: one explicit instantiation
template struct Stack<int>;
template struct Stack<std::string>;

// In every other header/cpp that includes Stack: suppress re-instantiation
extern template struct Stack<int>;
extern template struct Stack<std::string>;
```

**When to use:**
- Large codebases with many translation units including the same template headers
- Templates with expensive instantiation bodies (matrix math, serialization)
- When link time or binary size is noticeably affected by template bloat

---

## 9.13 Template Aliases with `using` (C++11)

Before C++11, the only way to "rename" a type was `typedef`, which **cannot be templated**. C++11's alias declaration (`using`) can take template parameters, producing an **alias template**.

```cpp
#include <vector>
#include <map>
#include <string>
using namespace std;

// --- Plain alias (equivalent to typedef, but reads left-to-right) ---
using Int32   = int;                 // same as: typedef int Int32;
using IntVec  = vector<int>;         // same as: typedef vector<int> IntVec;
using FuncPtr = int(*)(int, int);    // far clearer than the typedef form

// --- Alias TEMPLATE: this is impossible with typedef ---
template<typename T>
using Vec = vector<T>;               // Vec<T> means vector<T>

Vec<int>    vi = {1, 2, 3};          // vector<int>
Vec<string> vs = {"a", "b"};         // vector<string>

// Partially bind a multi-parameter template
template<typename Value>
using StringMap = map<string, Value>;   // fix the key type to string

StringMap<int>    ages;     // map<string, int>
StringMap<double> prices;   // map<string, double>

// Alias templates are the idiomatic way to simplify trait access
template<typename T>
using RemoveConst = typename remove_const<T>::type;

RemoveConst<const int> x = 5;   // int x  — no 'typename ...::type' noise
```

| | `typedef` | `using` alias |
|---|---|---|
| Plain type alias | Yes | Yes |
| Templated alias | **No** | **Yes** (`template<...> using ...`) |
| Reads naturally | `typedef vector<int> V;` | `using V = vector<int>;` |

---

## 9.14 Perfect Forwarding — `T&&` and `std::forward` (C++11)

When a template forwards its arguments to another function, you want to preserve each argument's **value category**: lvalues stay lvalues, rvalues (temporaries) stay rvalues so they can be moved. C++11 solves this with **forwarding references** (`T&&` on a deduced template parameter) plus `std::forward`.

```cpp
#include <iostream>
#include <utility>   // std::forward, std::move
#include <string>
using namespace std;

void overloaded(int& x)       { cout << "lvalue\n"; }
void overloaded(const int& x) { cout << "const lvalue\n"; }
void overloaded(int&& x)      { cout << "rvalue\n"; }

// T&& on a DEDUCED parameter is a "forwarding reference" (a.k.a. universal
// reference), NOT a plain rvalue reference. It binds to anything.
template<typename T>
void relay(T&& arg) {
  // std::forward<T> restores the original value category of 'arg'
  overloaded(std::forward<T>(arg));
}

int main() {
  int a = 10;
  relay(a);              // lvalue       — 'a' is an lvalue
  relay(20);             // rvalue       — 20 is a temporary
  const int c = 5;
  relay(c);              // const lvalue
  return 0;
}
```

**Why `std::forward` and not `std::move`?** `std::move` *always* casts to rvalue; `std::forward` casts to rvalue **only if the original argument was an rvalue**. Using `move` on a forwarding reference would wrongly steal from an lvalue the caller still owns.

**The canonical use — a perfectly-forwarding factory (how `make_shared`/`emplace` work):**

```cpp
#include <memory>
#include <utility>

// Forward ANY number of arguments, each with its correct value category,
// straight into T's constructor.
template<typename T, typename... Args>
unique_ptr<T> make_unique_cpp11(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Widget {
  Widget(const string& name, int id) { /* ... */ }
};

auto w = make_unique_cpp11<Widget>("gadget", 42);  // args forwarded, no extra copies
```

| Tool | Effect | Use on |
|---|---|---|
| `T&&` (deduced `T`) | Forwarding reference — binds to lvalue or rvalue | Generic forwarding functions |
| `std::move(x)` | Unconditional cast to rvalue | A named value you are done with |
| `std::forward<T>(x)` | Conditional cast — rvalue only if original was rvalue | A forwarding-reference parameter |

**Rule:** `std::move` for "I own this and I'm finished with it"; `std::forward<T>` for "pass this on exactly as I received it." (`std::move`/`std::forward` are covered from the value-semantics angle in [Chapter 4.11](../04_pointers_references/README.md) and [Chapter 12.6](../12_memory_management/README.md).)

---

## Next Steps
- Use function templates for generic code
- Create template classes like containers
- Specialize templates for specific types
- Move to [Chapter 10: STL Containers](../10_stl_containers/README.md)
