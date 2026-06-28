# Chapter 6: Object-Oriented Programming Basics

Object-oriented programming organizes software around classes — user-defined types that bundle data (member variables) together with the functions that operate on that data (member functions). A class is a blueprint; an object is a concrete instance created from it. The central idea is encapsulation: hide the internal representation behind a controlled public interface so that the rest of the program depends on what an object does, not how it stores its state. This makes large programs easier to reason about, change, and protect from invalid use.

This chapter covers the foundations: defining classes and access control, constructing and destroying objects (including initializer lists and the destructor), static and `const` members, the `this` pointer, getters/setters, `friend` functions, and the object lifecycle. It closes with several C++11 features — `= default`/`= delete`, delegating constructors, `std::initializer_list`, `explicit`, and `mutable` — plus lower-level building blocks like unions, bit fields, and nested classes.

## 6.1 Classes & Objects

A class groups related data and behavior into a single type, and an object is an instance of that type with its own copy of the data members. Member access is governed by access specifiers — `public` members form the interface usable from outside, while `private` members are the hidden implementation details accessible only within the class. Choosing what to expose versus hide is the essence of encapsulation and is what keeps an object's internal state consistent.

### Basic Class Definition

A class definition lists its members under access specifiers and ends with a semicolon. Conventionally the data is kept `private` and accessed through `public` member functions, so the class controls every change to its own state. In the example below, `name` and `age` are public for simplicity while `ssn` is private and reachable only through the class's own functions — attempting to touch a private member from outside is a compile error.

```cpp
#include <iostream>
#include <string>
using namespace std;

// Class definition
class Person {
  // Access modifiers define visibility
public:
  // Public: accessible from outside
  string name;
  int age;
  
  // Member function
  void display() {
    cout << name << " is " << age << " years old" << endl;
  }

private:
  // Private: only accessible within class
  string ssn;  // Social Security Number
  
public:
  void setSsn(string s) { ssn = s; }
  string getSsn() { return ssn; }
};

int main() {
  // Create object
  Person p;
  p.name = "Alice";
  p.age = 25;
  p.display();
  
  p.setSsn("123-45-6789");
  // cout << p.ssn;  // ERROR: private
  
  return 0;
}
```

### Access Modifiers

C++ provides three access levels. `public` members are accessible from anywhere, forming the class's external interface. `private` members are accessible only from within the class's own member functions (and its friends), protecting implementation details. `protected` sits in between: like `private` for outside code, but also accessible from derived classes, which matters for inheritance (Chapter 7). Members of a `class` are `private` by default, whereas members of a `struct` are `public` by default — the only real difference between the two keywords.

```cpp
class MyClass {
public:
  // Accessible from anywhere
  void publicMethod() { }
  int publicData = 10;

protected:
  // Accessible from class and derived classes
  void protectedMethod() { }
  int protectedData = 20;

private:
  // Only accessible within the class
  void privateMethod() { }
  int privateData = 30;
};

int main() {
  MyClass obj;
  obj.publicMethod();        // OK
  obj.publicData = 50;       // OK
  
  // obj.protectedMethod();  // ERROR
  // obj.privateMethod();    // ERROR
  
  return 0;
}
```

---

## 6.2 Constructors & Destructors

Constructors and destructors are special member functions that run automatically at the start and end of an object's life. A constructor initializes a new object — putting its members into a valid state — and runs as soon as the object is created; a destructor performs cleanup (such as freeing resources the object acquired) and runs automatically when the object goes out of scope or is `delete`d. Together they make the RAII idiom possible, tying resource ownership to object lifetime so cleanup is never forgotten.

### Constructors

A constructor has the same name as the class and no return type. A class can have several constructors that differ by parameters: a default constructor takes no arguments, a parameterized constructor sets members from supplied values, and a copy constructor (taking `const ClassName&`) initializes a new object as a duplicate of an existing one. If you declare no constructors at all, the compiler supplies a default one; once you declare any, you get only the ones you write (plus a compiler-generated copy constructor unless you suppress it).

```cpp
class Student {
private:
  string name;
  int id;
  double gpa;

public:
  // Default constructor (no arguments)
  Student() : name("Unknown"), id(0), gpa(0.0) {
    cout << "Default constructor called" << endl;
  }
  
  // Parameterized constructor
  Student(string n, int i, double g) {
    name = n;
    id = i;
    gpa = g;
    cout << "Parameterized constructor called" << endl;
  }
  
  // Copy constructor
  Student(const Student& other) {
    name = other.name;
    id = other.id;
    gpa = other.gpa;
    cout << "Copy constructor called" << endl;
  }
  
  // Display
  void display() {
    cout << name << " (" << id << "): " << gpa << endl;
  }
};

int main() {
  Student s1;                           // Default
  Student s2("Alice", 101, 3.8);       // Parameterized
  Student s3 = s2;                      // Copy constructor
  
  return 0;
}
```

### Initializer List

A constructor initializer list — the `: member(value), ...` clause between the parameter list and the body — initializes members directly, before the constructor body runs. This is more efficient than assigning inside the body, because assignment first default-constructs the member and then overwrites it, whereas the initializer list constructs it once with the right value. It is also mandatory for members that cannot be assigned after construction: `const` members, reference members, and members of types lacking a default constructor. Note that members are initialized in the order they are declared in the class, not the order they appear in the list.

```cpp
class Person {
private:
  string name;
  int age;

public:
  // Using initializer list (preferred)
  Person(string n, int a) : name(n), age(a) {
    cout << "Initialized: " << name << endl;
  }
  
  // Equivalent but less efficient
  // Person(string n, int a) {
  //   name = n;
  //   age = a;
  // }
};
```

### Destructor

A destructor is named `~ClassName`, takes no parameters, and is called automatically when an object's lifetime ends — at the closing brace for a stack object, or at `delete` for a heap object. Its job is to release whatever the object owns, most importantly memory allocated with `new` (which must be freed with the matching `delete`/`delete[]`). A class that manages a raw resource needs a destructor; in modern C++, wrapping the resource in a smart pointer or container often removes the need to write one at all. (When a class needs a custom destructor, it usually needs a custom copy constructor and copy assignment too — the "Rule of Three.")

```cpp
class Resource {
private:
  int* data;
  int size;

public:
  // Constructor: allocate
  Resource(int s) : size(s) {
    data = new int[size];
    cout << "Resource allocated" << endl;
  }
  
  // Destructor: cleanup
  ~Resource() {
    delete[] data;
    cout << "Resource deallocated" << endl;
  }
};

int main() {
  {
    Resource r(100);
    // Use resource
  }  // Destructor called here
  
  return 0;
}
```

---

## 6.3 Member Variables & Functions

Beyond ordinary per-object members, classes support members with special semantics. A `static` member belongs to the class as a whole rather than to any individual object, and a `const` member function promises not to modify the object it is called on. Understanding these is key to writing classes that share state appropriately and that are usable in `const` contexts.

### Static Members

A `static` data member is shared by every object of the class — there is exactly one copy regardless of how many objects exist — making it ideal for things like an instance counter or a shared configuration value. Because it is not tied to any object, a `static` member must be defined once outside the class (as `int Counter::count = 0;`) in addition to being declared inside it. A `static` member function likewise has no `this` pointer and can only touch static members; it is called on the class itself, as in `Counter::getCount()`.

```cpp
class Counter {
private:
  static int count;  // Shared by all instances
  int id;

public:
  Counter() : id(++count) { }
  
  int getId() { return id; }
  
  static int getCount() { return count; }
};

// Initialize static member (outside class)
int Counter::count = 0;

int main() {
  Counter c1;
  Counter c2;
  Counter c3;
  
  cout << "Total created: " << Counter::getCount() << endl;  // 3
  cout << "c1 ID: " << c1.getId() << endl;                   // 1
  cout << "c3 ID: " << c3.getId() << endl;                   // 3
  
  return 0;
}
```

### Const Member Functions

Appending `const` to a member function's signature declares that the function will not modify the object's (non-`mutable`) data members, letting the compiler enforce that promise. This matters because a `const` object — including one passed as a `const` reference — can only have its `const` member functions called on it. As a rule, mark every member function that does not change observable state (getters, display routines, queries) as `const`; doing so makes your class usable in the many places that pass objects by `const` reference.

```cpp
class Point {
private:
  int x, y;

public:
  Point(int a, int b) : x(a), y(b) { }
  
  // Const member function: cannot modify members
  int getX() const { return x; }
  int getY() const { return y; }
  
  // Non-const: can modify
  void move(int dx, int dy) {
    x += dx;
    y += dy;
  }
  
  // Display (should be const)
  void display() const {
    cout << "(" << x << "," << y << ")" << endl;
  }
};

int main() {
  const Point p(3, 4);
  p.display();         // OK
  cout << p.getX();    // OK
  
  // p.move(1, 1);     // ERROR: const object
  
  return 0;
}
```

---

## 6.4 This Pointer

Inside a non-static member function, `this` is a pointer to the object the function was called on, so `this->member` refers to that object's data. It is mostly implicit — you usually write `member` directly — but it becomes useful for disambiguating a member from a parameter of the same name, for checking self-assignment (`if (this != &other)`), and for returning the current object. Returning `*this` by reference is the standard technique for enabling method chaining, where one call's result is the receiver of the next.

```cpp
class Number {
private:
  int value;

public:
  Number(int v) : value(v) { }
  
  // Using this pointer
  Number add(const Number& other) {
    this->value += other.value;
    return *this;  // Return reference to this object
  }
  
  // Assignment operator
  Number& operator=(const Number& other) {
    if (this != &other) {  // Avoid self-assignment
      this->value = other.value;
    }
    return *this;
  }
  
  int getValue() { return value; }
};

int main() {
  Number n1(10);
  Number n2(5);
  
  n1.add(n2).add(n2);  // Chain calls
  
  cout << n1.getValue() << endl;  // 20
  
  return 0;
}
```

---

## 6.5 Getters & Setters

Getters and setters are public member functions that provide controlled access to private data. A getter (typically `const`) returns a member's value or a computed view of it, while a setter writes a member after validating the input, so the object can reject illegal states. This indirection is the practical payoff of encapsulation: callers cannot corrupt the object's invariants, and you can change the internal representation or add validation later without altering the public interface. The example computes a Fahrenheit view from a Celsius field and refuses temperatures below absolute zero.

```cpp
class Temperature {
private:
  double celsius;

public:
  Temperature() : celsius(0) { }
  
  // Getter
  double getCelsius() const {
    return celsius;
  }
  
  double getFahrenheit() const {
    return (celsius * 9/5) + 32;
  }
  
  // Setter with validation
  void setCelsius(double c) {
    if (c >= -273.15) {  // Absolute zero
      celsius = c;
    } else {
      cout << "Invalid temperature!" << endl;
    }
  }
  
  void setFahrenheit(double f) {
    celsius = (f - 32) * 5/9;
  }
};

int main() {
  Temperature t;
  t.setCelsius(25);
  cout << t.getCelsius() << "Â°C" << endl;      // 25Â°C
  cout << t.getFahrenheit() << "Â°F" << endl;   // 77Â°F
  
  return 0;
}
```

---

## 6.6 Friend Functions

A `friend` declaration grants a specific external function or another class access to the private and protected members of the class that names it. Friendship is granted by the class, not taken — it is declared inside the class body — and it is neither inherited nor reciprocal. The classic use is a non-member function (such as an overloaded `operator<<` or a binary operator that should treat both operands symmetrically) that needs to reach into a class's internals. Use it sparingly: each friend widens the class's encapsulation boundary, so prefer the public interface when it suffices.

```cpp
class Point {
private:
  int x, y;

public:
  Point(int a = 0, int b = 0) : x(a), y(b) { }
  
  friend void printPoint(const Point& p);
  friend class Rectangle;
};

// Friend function (can access private members)
void printPoint(const Point& p) {
  cout << "(" << p.x << "," << p.y << ")" << endl;
}

class Rectangle {
public:
  int area(Point p1, Point p2) {
    return abs(p1.x - p2.x) * abs(p1.y - p2.y);
  }
};

int main() {
  Point p(3, 4);
  printPoint(p);  // Accesses private x, y
  
  return 0;
}
```

---

## 6.7 Encapsulation Example

This example pulls the chapter's ideas together into a realistic class. `BankAccount` keeps its balance and identifying fields `private`, exposes only `const` getters for reading, and routes every change through `deposit` and `withdraw`, which validate the amount before touching the balance. Because outside code cannot modify `balance` directly, the class can guarantee its own invariants — no negative deposits, no overdrafts — which is exactly the protection encapsulation is meant to provide.

```cpp
class BankAccount {
private:
  string accountNumber;
  string ownerName;
  double balance;
  
  // Helper function
  bool isValidAmount(double amount) {
    return amount > 0 && amount <= 1e9;
  }

public:
  // Constructor
  BankAccount(string num, string name, double initial)
    : accountNumber(num), ownerName(name), balance(initial) { }
  
  // Safe getters
  string getAccountNumber() const { return accountNumber; }
  string getOwnerName() const { return ownerName; }
  double getBalance() const { return balance; }
  
  // Deposit with validation
  bool deposit(double amount) {
    if (isValidAmount(amount)) {
      balance += amount;
      return true;
    }
    return false;
  }
  
  // Withdraw with validation
  bool withdraw(double amount) {
    if (isValidAmount(amount) && amount <= balance) {
      balance -= amount;
      return true;
    }
    return false;
  }
};

int main() {
  BankAccount acc("12345", "Alice", 1000);
  
  acc.deposit(500);
  cout << "Balance: $" << acc.getBalance() << endl;
  
  if (acc.withdraw(200)) {
    cout << "Withdrawal successful" << endl;
  }
  
  return 0;
}
```

---

## 6.8 Object Lifecycle

An object's lifetime depends on where it is created. A stack (automatic) object is constructed when control reaches its declaration and destroyed automatically at the end of its enclosing scope, in reverse order of construction. A heap object created with `new` lives until you explicitly `delete` it, and its destructor runs only then — forgetting that `delete` leaks the object. The example interleaves a heap `Logger` with a scoped stack `Logger` so the output makes the construction and destruction order visible.

```cpp
class Logger {
public:
  string name;
  
  Logger(string n) : name(n) {
    cout << "Logger " << name << " created" << endl;
  }
  
  ~Logger() {
    cout << "Logger " << name << " destroyed" << endl;
  }
};

int main() {
  Logger* l1 = new Logger("Heap");      // Created
  
  {
    Logger l2("Stack");                 // Created
  }  // l2 destroyed here
  
  delete l1;                            // l1 destroyed
  
  return 0;
}

/* Output:
Logger Heap created
Logger Stack created
Logger Stack destroyed
Logger Heap destroyed
*/
```

---

## 6.9 Best Practices

Good class design follows a few consistent habits. Keep data members `private` and expose behavior through a deliberate public interface rather than raw fields. Mark member functions `const` whenever they do not change observable state, and return large members by `const` reference to avoid needless copies. Prefer initializer lists over body assignment in constructors, and when copying does not make sense for a type, use C++11's `= delete` to forbid the copy constructor and copy assignment outright. The snippets below contrast each good pattern with the mistake it prevents.

```cpp
// âœ… Private data, public interface
class Good {
private:
  int value;
public:
  int getValue() const { return value; }
  void setValue(int v) { value = v; }
};

// âŒ Public data (breaks encapsulation)
class Bad {
public:
  int value;  // Direct access
};

// âœ… Use const correctly
class Container {
public:
  const vector<int>& getData() const { return data; }
  void addData(int x) { data.push_back(x); }
private:
  vector<int> data;
};

// âœ… Use initializer list
class Good2 {
public:
  Good2(int a, int b) : x(a), y(b) { }
private:
  int x, y;
};

// âœ… Prevent copying where appropriate
class NoCopy {
public:
  NoCopy(const NoCopy&) = delete;           // No copy
  NoCopy& operator=(const NoCopy&) = delete;
};
```

---

## 6.10 = default and = delete (C++11)

`= default` asks the compiler to generate the standard implementation. `= delete` permanently prevents a function from being called â€” at compile time, not runtime.

```cpp
#include <iostream>
using namespace std;

// = default: explicitly request the compiler-generated version
struct Point {
  int x, y;

  Point() = default;                        // default constructor
  Point(const Point&) = default;            // copy constructor
  Point& operator=(const Point&) = default; // copy assignment

  Point(int x, int y) : x(x), y(y) {}     // custom constructor still works
};

// = delete: hard-prevent specific operations
class UniqueHandle {
  int fd;
public:
  explicit UniqueHandle(int f) : fd(f) {}

  UniqueHandle(const UniqueHandle&)            = delete;  // no copy ctor
  UniqueHandle& operator=(const UniqueHandle&) = delete;  // no copy assign
  // move constructor is still allowed (not deleted)

  int get() const { return fd; }
};

// UniqueHandle a(1);
// UniqueHandle b = a;   // ERROR: use of deleted function

// = delete on free functions: block implicit conversions
void process(int x) { cout << x << endl; }
void process(double) = delete;  // forbid: process(3.14)
// process(3.14);   // compile error: deleted function
process(42);        // OK

// Prevent heap allocation
class StackOnly {
public:
  void* operator new(size_t) = delete;      // must live on stack
};
```

| Keyword | Effect | Use case |
|---------|--------|---------|
| `= default` | Compiler generates the body | Restore defaulted member after declaring another constructor |
| `= delete` | Compile error if called | Non-copyable types, block implicit conversions |

---

## 6.11 Delegating Constructors (C++11)

A constructor can call another constructor of the **same class** in its member initializer list. This eliminates duplicated validation and initialization logic.

```cpp
#include <stdexcept>
#include <string>
using namespace std;

class Config {
  string host;
  int    port;
  bool   secure;
public:
  // Primary constructor â€” contains all real work
  Config(const string& h, int p, bool s)
      : host(h), port(p), secure(s) {
    if (port < 1 || port > 65535)
      throw invalid_argument("Invalid port");
  }

  // Delegate to primary â€” no duplicated validation
  Config(const string& h, int p)
      : Config(h, p, false) {}          // delegates

  Config(const string& h)
      : Config(h, 80) {}               // chains through

  Config()
      : Config("localhost") {}          // full chain

  string info() const { return host + ":" + to_string(port); }
};

Config c1;                            // localhost:80
Config c2("api.example.com");         // api.example.com:80
Config c3("db.example.com", 5432, false);

// Key rules:
// - Delegation happens in the member-init list, not the body
// - A delegating constructor cannot also initialize members directly
// - The delegated-to constructor runs first, then the delegating body runs
```

**Benefit:** Validation logic lives in one place. Adding a new default only needs a one-line delegating constructor.

---

## 6.12 std::initializer_list (C++11)

`std::initializer_list<T>` is the mechanism behind brace-initialization (`{1, 2, 3}`) for standard containers. You can use it in your own classes to support the same syntax.

```cpp
#include <initializer_list>
#include <vector>
#include <iostream>
using namespace std;

class NumberBag {
  vector<int> data;
public:
  // Constructor accepting brace-enclosed list
  NumberBag(initializer_list<int> init) : data(init) {}

  // Method accepting brace-enclosed list
  void addAll(initializer_list<int> more) {
    for (int x : more)
      data.push_back(x);
  }

  int  sum()  const { int t = 0; for (int x : data) t += x; return t; }
  int  size() const { return static_cast<int>(data.size()); }
};

NumberBag bag = {1, 2, 3, 4, 5};   // calls initializer_list constructor
bag.addAll({6, 7, 8});
cout << bag.sum();   // 36
cout << bag.size();  // 8

// initializer_list is lightweight: a pointer + size, no element copies
void printAll(initializer_list<int> items) {
  for (int x : items) cout << x << " ";
  cout << endl;
}
printAll({10, 20, 30, 40});   // 10 20 30 40

// Standard containers use this internally
vector<int>          v = {1, 2, 3};          // vector's initializer_list ctor
map<string, int>     m = {{"a", 1}, {"b", 2}};

// Member access
initializer_list<int> il = {5, 6, 7};
cout << il.size();   // 3
// Iterate with range-for or begin()/end()
for (int x : il) cout << x << " ";
```

| Feature | Details |
|---------|---------|
| Header | `<initializer_list>` |
| Element access | Iterate only (begin/end/range-for) â€” no `[]` |
| Lifetime | Valid for the duration of the brace-init expression |
| Cost | Pointer + size â€” no heap allocation, no element copies |

---

## Summary

| Concept | Details |
|---------|---------|
| **Class** | Blueprint for objects |
| **Constructor** | Initialize object |
| **Destructor** | Clean up resources |
| **Access** | public, private, protected |
| **Const** | const members, const functions |
| **Static** | Shared by all instances |
| **Encapsulation** | Hide implementation, expose interface |
| **This pointer** | Points to current object |
| **= default / = delete** | Control special member generation |
| **Delegating constructors** | Chain constructors to eliminate duplication |
| **initializer_list** | Enable brace-init syntax for custom types |
| **explicit** | Prevent silent implicit single-argument conversions |
| **mutable** | Allow mutation of one field inside a const method |

---

## 6.13 The explicit Keyword

Without `explicit`, a single-argument constructor is an **implicit conversion constructor** â€” the compiler can call it silently. This leads to subtle bugs. `explicit` prevents that.

```cpp
#include <iostream>
using namespace std;

// âŒ Implicit conversion constructor â€” risky
class Meters {
  double value;
public:
  Meters(double v) : value(v) {}    // implicit: Meters m = 3.5; works
  double get() const { return value; }
};

// âœ… Explicit constructor â€” conversion requires a cast
class Kilograms {
  double value;
public:
  explicit Kilograms(double v) : value(v) {}
  double get() const { return value; }
};

void shipCargo(Meters distance) {
  cout << "Shipping " << distance.get() << " m" << endl;
}

Meters m1 = 3.5;          // OK: Meters allows implicit construction
shipCargo(5.0);            // Dangerous! 5.0 silently becomes Meters(5.0)

// Kilograms kg1 = 3.5;   // ERROR: explicit prevents this
Kilograms kg2(3.5);        // OK: direct initialization
Kilograms kg3 = Kilograms(3.5);  // OK: explicit cast

// --- explicit on multi-argument constructors (C++11) ---
class Point {
public:
  explicit Point(int x, int y) : x_(x), y_(y) {}
  int x_, y_;
};
// Point p = {1, 2};  // ERROR with explicit in C++11
Point p(1, 2);         // OK

// --- explicit operator (C++11) â€” prevents implicit conversion FROM the class ---
class SafeBool {
  bool value;
public:
  explicit SafeBool(bool b) : value(b) {}
  explicit operator bool() const { return value; }  // explicit conversion op
};

SafeBool sb(true);
if (sb) { }               // OK: contextual conversion (if-statement)
// bool b = sb;           // ERROR: explicit prevents implicit copy init
bool b = static_cast<bool>(sb);  // OK: explicit cast
```

| Without `explicit` | With `explicit` |
|---|---|
| Any double silently becomes the class | Only direct init works |
| Compiler calls ctor implicitly in assignments, function calls | Must use `T(val)` or `static_cast<T>(val)` |

**Rule:** Mark every single-argument constructor `explicit` unless implicit conversion is intentionally designed.

---

## 6.14 The mutable Keyword

`mutable` allows one specific member variable to be modified inside a `const` member function. It is the correct tool for **cached computations, call counters, and mutex members** that are logically read-only but physically modified.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Document {
  string content;
  mutable int    accessCount;  // mutable: can change even in const functions
  mutable string cachedUpper;  // mutable: cache of upper-case version
  mutable bool   cacheValid;
public:
  Document(const string& c) : content(c), accessCount(0), cacheValid(false) {}

  // const function â€” logically read-only, but physically updates cache & counter
  const string& upperCase() const {
    ++accessCount;             // OK: mutable
    if (!cacheValid) {
      cachedUpper = content;
      for (char& ch : cachedUpper) {
        if (ch >= 'a' && ch <= 'z') ch -= 32;
      }
      cacheValid = true;
    }
    return cachedUpper;
  }

  int reads() const { return accessCount; }  // check access counter
};

const Document doc("hello world");   // const object
cout << doc.upperCase();             // HELLO WORLD â€” computes and caches
cout << doc.upperCase();             // HELLO WORLD â€” returns cached
cout << doc.reads();                 // 2

// Common use: mutex inside a const method (thread-safe const access)
// (shown conceptually â€” mutex itself is mutable)
#include <mutex>

class ThreadSafeCounter {
  mutable mutex mtx;       // mutable: locked in const read methods
  int count = 0;
public:
  int get() const {
    lock_guard<mutex> lock(mtx);  // OK: mtx is mutable
    return count;
  }
  void increment() { lock_guard<mutex> lock(mtx); ++count; }
};
```

| | `mutable` member | Regular member |
|---|---|---|
| Modifiable in `const` method | Yes | No |
| Modifiable in non-`const` method | Yes | Yes |
| Use case | Cache, counter, mutex | All regular state |

**Rule:** Use `mutable` only for members that are truly an implementation detail (cache or synchronisation primitive), never for observable state.


---

## 6.15 Unions and Anonymous Unions

A `union` stores all members at the same memory address. Only one member may
be "active" (written and read) at a time. Size equals the largest member.

```cpp
#include <iostream>
union Data {
    int   i;
    float f;
    char  c;
};

Data d;
d.i = 42;
std::cout << d.i << "\n";   // 42 -- i is active
d.f = 3.14f;                // now f is active; reading d.i is undefined behavior

// Tagged union pattern (discriminated union)
struct Variant {
    enum class Type { Int, Float, Char } tag;
    union { int i; float f; char c; };  // anonymous union: members accessible directly
};

Variant v;
v.tag = Variant::Type::Float;
v.f = 2.718f;
if(v.tag == Variant::Type::Float)
    std::cout << v.f << "\n";  // 2.718
```

**Anonymous union** (no tag name): members are directly in the enclosing scope.

```cpp
struct Packet {
    int type;
    union {          // anonymous union -- no name needed
        int   intVal;
        float floatVal;
    };
};
Packet p; p.type = 1; p.intVal = 100;  // intVal accessed directly
```

**Restrictions:** unions cannot have members with non-trivial constructors,
destructors, or copy-assignment operators by default. Prefer `std::variant`
(C++17) for type-safe tagged unions in modern code.

---

## 6.16 Bit Fields

Bit fields pack multiple small integer values into one struct, saving memory.
Useful for hardware registers, protocol headers, flags.

```cpp
#include <iostream>
struct Flags {
    unsigned int ready    : 1;   // 1 bit
    unsigned int error    : 1;   // 1 bit
    unsigned int mode     : 3;   // 3 bits (values 0-7)
    unsigned int reserved : 27;  // padding to 32 bits
};

Flags f = {};
f.ready = 1;
f.mode  = 5;
std::cout << sizeof(f) << "\n";    // typically 4 bytes
std::cout << f.mode << "\n";       // 5

// Compact boolean flags
struct PermissionBits {
    bool canRead    : 1;
    bool canWrite   : 1;
    bool canExecute : 1;
};
```

**Rules:**
- Only integral or `bool` types allowed
- Cannot take the address of a bit field (`&f.ready` is illegal)
- Layout is implementation-defined (padding, byte order varies)
- Unnamed bit fields (`: 3`) create padding with no name

---

## 6.17 Nested and Local Classes

A class defined **inside another class** is a nested class. A class defined
**inside a function** is a local class.

```cpp
class Outer {
public:
    int x = 10;

    class Inner {
    public:
        // Access Outer members only via an explicit reference/pointer
        void show(const Outer& o) { std::cout << o.x << "\n"; }
    };

    void useInner() { Inner in; in.show(*this); }
};

Outer::Inner obj;  // fully qualified from outside

void process() {
    struct Helper {      // local class -- not visible outside this function
        void run() { std::cout << "running\n"; }
    };
    Helper h;
    h.run();
}
```

**Local class rules:**
- Cannot access non-static local variables of the enclosing function
- Can be used as a template argument (C++11 lifted the old restriction)
- Useful for one-off helpers without polluting namespace scope

---

## Next Steps
- Design classes with proper encapsulation
- Implement constructors and destructors
- Practice getters and setters
- Move to [Chapter 7: Inheritance & Polymorphism](../07_inheritance_polymorphism/README.md)