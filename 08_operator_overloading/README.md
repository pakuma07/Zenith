# Chapter 8: Operator Overloading

Operator overloading lets you define how built-in operators such as `+`, `==`, `<<`, and `[]` behave when applied to objects of your own classes. The goal is to make user-defined types feel as natural to use as fundamental types, so that `a + b` or `cout << obj` reads exactly as it would for an `int`. Each overload is just a specially-named function — `operator+`, `operator==`, and so on — that the compiler invokes whenever it sees the corresponding symbol applied to your type.

This chapter covers unary and binary operators, comparison and stream operators, the assignment, subscript, and function-call operators, and the C++11 additions of `explicit` conversion operators and move assignment. The guiding principle throughout is to overload operators only when their meaning is intuitive and consistent with built-in semantics; surprising or "clever" overloads make code harder to read, not easier.

## 8.1 Operator Overloading Basics

An overloaded operator is written as a member function named `operator` followed by the symbol, for example `operator+`. When defined as a member, the left-hand operand is the implicit `this` object and the right-hand operand is the function parameter, so `c1 + c2` becomes `c1.operator+(c2)`. Arithmetic operators like `+` and `-` should return a brand-new object by value (leaving both operands unchanged) and are typically marked `const` since they do not modify the calling object. Start with these simple value-returning operators before tackling the trickier ones.

```cpp
class Complex {
private:
  double real, imag;

public:
  Complex(double r = 0, double i = 0) : real(r), imag(i) { }
  
  // Overload + operator
  Complex operator+(const Complex& other) {
    return Complex(real + other.real, imag + other.imag);
  }
  
  // Overload - operator
  Complex operator-(const Complex& other) {
    return Complex(real - other.real, imag - other.imag);
  }
  
  void display() {
    cout << real << " + " << imag << "i" << endl;
  }
};

int main() {
  Complex c1(3, 4);
  Complex c2(1, 2);
  
  Complex c3 = c1 + c2;  // Calls operator+
  c3.display();          // 4 + 6i
  
  return 0;
}
```

---

## 8.2 Unary Operators

Unary operators act on a single operand: examples include `++`, `--`, unary `-`, and `!`. The increment and decrement operators have two forms, and C++ distinguishes them with a dummy `int` parameter: `operator++()` is the prefix form (`++n`), while `operator++(int)` is the postfix form (`n++`). Prefix should modify the object and return a reference to it (`*this`) for efficiency, whereas postfix must return a copy of the object's *old* value, making it slightly more expensive. A common pitfall is forgetting to save the original value in the postfix version or returning the wrong form.

```cpp
class Number {
private:
  int value;

public:
  Number(int v = 0) : value(v) { }
  
  // Prefix increment (++n)
  Number& operator++() {
    ++value;
    return *this;
  }
  
  // Postfix increment (n++)
  Number operator++(int) {  // int parameter (dummy)
    Number temp(value);
    value++;
    return temp;
  }
  
  // Unary minus (-n)
  Number operator-() {
    return Number(-value);
  }
  
  // Logical NOT (!n)
  bool operator!() {
    return value == 0;
  }
  
  int getValue() { return value; }
};

int main() {
  Number n(5);
  
  ++n;
  cout << n.getValue() << endl;  // 6
  
  Number m = n++;  // Postfix
  cout << n.getValue() << endl;  // 7
  cout << m.getValue() << endl;  // 6
  
  Number neg = -n;
  cout << neg.getValue() << endl;  // -7
  
  return 0;
}
```

---

## 8.3 Binary Operators

Binary operators take two operands, such as `a + b`, `a * scalar`, or `a == b`. When implemented as a member function, only the right-hand operand is passed as a parameter; pass it by `const` reference to avoid copying and to allow `const` operands. A useful technique is to implement related operators in terms of each other — for example, defining `operator!=` as `!(*this == v)` — so the logic lives in one place and stays consistent. Note that member operators force the left operand to be your class type, which is why mixed-type expressions like `scalar * vector` often require a non-member (free function) overload instead.

```cpp
class Vector {
private:
  double x, y;

public:
  Vector(double x = 0, double y = 0) : x(x), y(y) { }
  
  // + (vector addition)
  Vector operator+(const Vector& v) {
    return Vector(x + v.x, y + v.y);
  }
  
  // * (scalar multiplication)
  Vector operator*(double scalar) {
    return Vector(x * scalar, y * scalar);
  }
  
  // == (equality)
  bool operator==(const Vector& v) {
    return x == v.x && y == v.y;
  }
  
  // != (inequality)
  bool operator!=(const Vector& v) {
    return !(*this == v);
  }
  
  // < (comparison)
  bool operator<(const Vector& v) {
    return (x*x + y*y) < (v.x*v.x + v.y*v.y);
  }
};
```

---

## 8.4 Comparison Operators

Comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`) return `bool` and let your objects be tested for equality and ordering. They should always be `const` member functions because comparing two objects must never modify them. To avoid bugs and duplication, define a small set of primitives — typically `==` and `<` — and derive the rest from them (`!=` is `!(a == b)`, `>` is `b < a`, and so on). Providing a correct `operator<` is also what makes your type usable as a key in ordered containers like `std::map` and `std::set` or with `std::sort`.

```cpp
class Person {
private:
  string name;
  int age;

public:
  Person(string n, int a) : name(n), age(a) { }
  
  // ==
  bool operator==(const Person& other) const {
    return name == other.name && age == other.age;
  }
  
  // !=
  bool operator!=(const Person& other) const {
    return !(*this == other);
  }
  
  // <
  bool operator<(const Person& other) const {
    return age < other.age;
  }
  
  // >
  bool operator>(const Person& other) const {
    return age > other.age;
  }
  
  // <=, >= (derived from above)
};

int main() {
  Person p1("Alice", 25);
  Person p2("Bob", 30);
  
  if (p1 < p2) {
    cout << "p1 is younger" << endl;
  }
  
  return 0;
}
```

---

## 8.5 Stream Operators

The stream insertion (`<<`) and extraction (`>>`) operators let your objects work directly with `cout`, `cin`, and other streams. These must be implemented as **non-member** functions because the left operand is a stream (`ostream`/`istream`), not your class — you cannot add a member to `std::ostream`. They are usually declared `friend` so they can access private data, take the stream by non-`const` reference, and crucially must **return that same stream reference** so calls can be chained, as in `cout << a << b << endl`. By convention `operator<<` takes the object by `const` reference (it only reads it) while `operator>>` takes it by non-`const` reference (it writes into it).

```cpp
class Point {
private:
  int x, y;

public:
  Point(int x = 0, int y = 0) : x(x), y(y) { }
  
  // Get x, y
  int getX() const { return x; }
  int getY() const { return y; }
  
  friend ostream& operator<<(ostream& os, const Point& p);
  friend istream& operator>>(istream& is, Point& p);
};

// Output stream operator
ostream& operator<<(ostream& os, const Point& p) {
  os << "(" << p.x << "," << p.y << ")";
  return os;
}

// Input stream operator
istream& operator>>(istream& is, Point& p) {
  is >> p.x >> p.y;
  return is;
}

int main() {
  Point p(3, 4);
  cout << p << endl;      // (3,4)
  
  Point p2;
  cin >> p2;
  cout << p2 << endl;
  
  return 0;
}
```

---

## 8.6 Assignment Operator

The copy assignment operator, `operator=`, defines what happens when one existing object is assigned to another (`a = b`). For classes that manage raw resources like heap memory, the compiler-generated version performs a shallow copy that duplicates the pointer rather than the data, leading to double-deletes and aliasing bugs — so you must write your own. A correct implementation does three things: guard against self-assignment (`if (this != &other)`), release the object's existing resources, and then deep-copy the source's data. It should return `*this` by reference to support chained assignment (`a = b = c`). This is closely tied to the copy constructor and destructor under the Rule of Three.

```cpp
class String {
private:
  char* str;
  int length;

public:
  String(const char* s = "") {
    length = strlen(s);
    str = new char[length + 1];
    strcpy(str, s);
  }
  
  ~String() {
    delete[] str;
  }
  
  // Assignment operator
  String& operator=(const String& other) {
    if (this != &other) {  // Self-assignment check
      delete[] str;        // Delete old data
      length = other.length;
      str = new char[length + 1];
      strcpy(str, other.str);
    }
    return *this;
  }
  
  // Copy constructor (default-generated without this)
  String(const String& other) : length(other.length) {
    str = new char[length + 1];
    strcpy(str, other.str);
  }
};
```

---

## 8.7 Subscript Operator

The subscript operator `operator[]` gives your class array-like indexing syntax, as in `m[i]`. It must be a member function and takes a single index argument (any type, not just an integer — map-like classes use keys). To allow both reading and writing through the subscript, return the element by reference (`T&`) so expressions like `m[0][0] = 1` work. In practice you usually provide two overloads — a non-`const` version returning `T&` and a `const` version returning `const T&` — so the operator can be used on both mutable and `const` objects; note that the operator itself does no bounds checking unless you add it.

```cpp
class Matrix {
private:
  vector<vector<int>> data;
  int rows, cols;

public:
  Matrix(int r, int c) : rows(r), cols(c) {
    data.resize(r, vector<int>(c, 0));
  }
  
  // Subscript operator
  vector<int>& operator[](int row) {
    return data[row];
  }
};

int main() {
  Matrix m(3, 3);
  m[0][0] = 1;
  m[1][2] = 5;
  
  cout << m[1][2] << endl;  // 5
  
  return 0;
}
```

---

## 8.8 Function Call Operator

Overloading `operator()` turns objects of your class into **function objects**, or *functors* — objects that can be called like functions using `obj(args)`. Unlike a plain function, a functor can carry state in its member variables (such as the `factor` below), which it remembers between calls. This makes functors ideal as predicates and operations passed to STL algorithms like `std::transform`, `std::sort`, and `std::for_each`, where the algorithm calls your object once per element. The operator can take any number of parameters and return any type, and you may even provide several overloads.

```cpp
// Functor (function object)
class Multiplier {
private:
  int factor;

public:
  Multiplier(int f) : factor(f) { }
  
  // () operator makes object callable
  int operator()(int x) {
    return x * factor;
  }
};

int main() {
  Multiplier times3(3);
  cout << times3(5) << endl;  // 15
  cout << times3(10) << endl;  // 30
  
  // Use with algorithms
  vector<int> v = {1, 2, 3, 4};
  transform(v.begin(), v.end(), v.begin(), times3);
  
  return 0;
}
```

---

## 8.9 Restrictions & Best Practices

Not every operator can be overloaded, and a few rules constrain those that can. The operators `::` (scope resolution), `.` (member access), `.*` (pointer-to-member), `?:` (ternary), and `#` (preprocessor) can never be overloaded. You also cannot invent new operators or change an operator's arity, precedence, or associativity — only its meaning for your types. The most important guideline is the principle of least surprise: overload an operator only when the operation is genuinely analogous to its built-in behavior, and follow the conventional return types (value for arithmetic, reference for compound assignment, stream reference for `<<`/`>>`).

```cpp
// ✅ Overloadable operators
+  -  *  /  %
== != <  >  <= >=
++  --  !  ~  &  |  ^
<<  >>  []  ()
new delete  =

// ❌ Cannot be overloaded
::  .  .*  ?:  #

// ✅ Use operator overloading for intuitive interfaces
class Vector { };
Vector operator+(const Vector& a, const Vector& b);

// ❌ Avoid changing operator semantics
// Don't overload + to do something not addition-like

// ✅ Return types
// For compound assignments, return reference
Complex& operator+=(const Complex& other);

// For arithmetic, return value or const reference
Complex operator+(const Complex& other) const;

// For stream operators, return stream reference
ostream& operator<<(ostream& os, const Object& obj);
```

---

## Summary

| Operator | Type | Example |
|----------|------|---------|
| **+** | Binary | a + b |
| **++** | Unary | ++a, a++ |
| **==** | Comparison | a == b |
| **[]** | Subscript | a[i] |
| **()** | Function call | a() |
| **<<** | Stream | cout << a |
| **=** | Assignment | a = b |

---

## 8.10 Conversion Operators

A **conversion operator** lets objects of your class implicitly (or explicitly) convert to another type. The syntax is `operator TargetType() const`. In C++11, you can prefix it with `explicit` to require a cast — this is almost always preferable.

```cpp
#include <iostream>
#include <string>
using namespace std;

// --- Implicit conversion operator (dangerous) ---
class Temperature {
  double celsius;
public:
  Temperature(double c) : celsius(c) {}

  // Implicit: Temperature t(25); double d = t; // silently converts
  operator double() const { return celsius; }
};

Temperature t(100.0);
double d = t;              // OK: calls operator double()
cout << d;                 // 100

// Problem: allows nonsensical implicit conversions
// bool result = (t == 100.0);  // works but easy to misread

// --- explicit conversion operator (C++11, preferred) ---
class Fraction {
  int num, den;
public:
  Fraction(int n, int d) : num(n), den(d) {}

  // explicit: requires a cast or contextual conversion
  explicit operator double() const {
    return static_cast<double>(num) / den;
  }

  // Contextual conversions (if, while, bool) are allowed even with explicit
  explicit operator bool() const { return den != 0 && num != 0; }
};

Fraction f(3, 4);
// double d2 = f;        // ERROR: explicit conversion op requires cast
double d2 = static_cast<double>(f);    // OK: explicit cast
double d3 = (double)f;                 // OK: C-style cast also works

if (f) {                               // OK: contextual bool conversion
  cout << "non-zero fraction" << endl;
}
// bool b = f;           // ERROR: explicit prevents this

// --- Common pattern: safe bool idiom via explicit operator bool ---
class SmartHandle {
  void* ptr;
public:
  explicit SmartHandle(void* p) : ptr(p) {}
  explicit operator bool() const { return ptr != nullptr; }
};

SmartHandle h(nullptr);
if (!h) cout << "null handle" << endl;   // OK — contextual conversion
// int n = h + 1;                        // ERROR — explicit prevents arithmetic

// --- operator string for printability ---
class Version {
  int major_, minor_;
public:
  Version(int ma, int mi) : major_(ma), minor_(mi) {}
  explicit operator string() const {
    return to_string(major_) + "." + to_string(minor_);
  }
};

Version v(2, 1);
string s = static_cast<string>(v);     // "2.1"
cout << s << endl;
```

| | Implicit `operator T()` | `explicit operator T()` |
|---|---|---|
| Implicit assignment `T x = obj;` | Allowed | Error |
| Explicit cast `static_cast<T>(obj)` | Allowed | Allowed |
| Contextual conversion (`if`, `while`, `!`) | Allowed | Allowed |
| Risk | Silent unexpected conversions | Controlled, intentional |

**Rule:** Always use `explicit` on conversion operators unless you specifically intend silent implicit conversion (rare). In particular, `explicit operator bool()` is the standard idiom for "has value" types.

---

## Summary

| Operator | Type | Example |
|----------|------|---------|
| **+** | Binary | a + b |
| **++** | Unary | ++a, a++ |
| **==** | Comparison | a == b |
| **[]** | Subscript | a[i] |
| **()** | Function call | a() |
| **<<** | Stream | cout << a |
| **=** | Assignment | a = b |
| **operator T()** | Conversion | `static_cast<T>(obj)` |

---

## 8.11 Move Assignment Operator (C++11)

C++11 adds a second form of `operator=` that takes an **rvalue reference** (`T&&`). When the right-hand side is a temporary (or an explicit `std::move`), the move assignment **steals** its resources instead of deep-copying them — far cheaper for classes that own heap memory.

```cpp
#include <cstring>
#include <utility>   // std::move
#include <iostream>
using namespace std;

class Buffer {
  char* data;
  size_t size;
public:
  Buffer(const char* s = "") {
    size = strlen(s);
    data = new char[size + 1];
    strcpy(data, s);
  }
  ~Buffer() { delete[] data; }

  // Copy assignment — deep copy (RHS is an lvalue we must not disturb)
  Buffer& operator=(const Buffer& other) {
    if (this != &other) {
      delete[] data;
      size = other.size;
      data = new char[size + 1];
      strcpy(data, other.data);
    }
    return *this;
  }

  // Move assignment (C++11) — steal the RHS's buffer, no allocation
  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {
      delete[] data;          // release our own resource
      data = other.data;      // take ownership of other's buffer
      size = other.size;
      other.data = nullptr;   // leave 'other' in a valid, destructible state
      other.size = 0;
    }
    return *this;
  }

  const char* c_str() const { return data ? data : ""; }
};

int main() {
  Buffer a("hello");
  Buffer b;

  b = a;                    // COPY assignment — 'a' still owns "hello"
  b = Buffer("temporary");  // MOVE assignment — temporary's buffer is stolen
  b = std::move(a);         // MOVE assignment — forced; 'a' is now empty

  cout << b.c_str();        // hello
  return 0;
}
```

**Why `noexcept`?** STL containers (e.g. `vector`) only use your move operations during reallocation if they are marked `noexcept`; otherwise they fall back to copying. Always mark move constructors and move assignment `noexcept`.

| RHS expression | Overload chosen | Effect |
|---|---|---|
| `b = a;` (lvalue) | `operator=(const T&)` | Deep copy — both objects keep valid data |
| `b = T(...);` (temporary) | `operator=(T&&)` | Steal resources from the expiring temporary |
| `b = std::move(a);` | `operator=(T&&)` | Steal from `a`; `a` left valid-but-empty |

This complements the **copy** assignment of 8.6 and is one of the five special members of the [Rule of Five](../12_memory_management/README.md). The value-category mechanics behind `T&&` and `std::move` are covered in [Chapter 4.11](../04_pointers_references/README.md).

---

## Next Steps
- Overload operators for custom classes
- Create intuitive interfaces with operators
- Understand assignment vs copy
- Move to [Chapter 9: Templates](../09_templates/README.md)
