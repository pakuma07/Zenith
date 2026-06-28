# Chapter 17: Template Metaprogramming (TMP)

Template Metaprogramming is programming that executes **at compile time** using the C++ template system. The compiler becomes the interpreter; template instantiation is the execution mechanism. TMP produces zero-runtime-overhead abstractions that are impossible to achieve with runtime polymorphism.

---

## 17.1 What is Template Metaprogramming?

TMP exploits the fact that C++ template instantiation is **Turing-complete**. Any computation expressible as a recursive algorithm can be performed at compile time.

```cpp
#include <iostream>
using namespace std;

// Runtime version: computed every call
int factorial_runtime(int n) {
  return (n <= 1) ? 1 : n * factorial_runtime(n - 1);
}

// Compile-time version: computed ONCE by the compiler, result baked into binary
template<int N>
struct Factorial {
  static const int value = N * Factorial<N - 1>::value;
};

// Base case: full specialization
template<>
struct Factorial<0> {
  static const int value = 1;
};

// Usage: all four are compile-time constants — no runtime computation
const int f5  = Factorial<5>::value;    // 120
const int f10 = Factorial<10>::value;   // 3628800
const int f0  = Factorial<0>::value;    // 1

cout << f5  << "\n";   // 120
cout << f10 << "\n";   // 3628800

// sizeof() with TMP: compute array sizes at compile time
int data[Factorial<5>::value];  // int data[120] — size is a compile-time constant

// Fibonacci at compile time
template<int N>
struct Fib {
  static const int value = Fib<N-1>::value + Fib<N-2>::value;
};
template<> struct Fib<0> { static const int value = 0; };
template<> struct Fib<1> { static const int value = 1; };

const int fib10 = Fib<10>::value;  // 55 — computed entirely at compile time
```

**Why TMP?**
- Zero-cost abstractions: no runtime overhead
- Type-safe compile-time checks
- Domain-specific language embedding (units, matrices)
- Auto-generated code specialised for each type

---

## 17.2 Compile-Time Type Traits from Scratch

The standard `<type_traits>` header is itself built from TMP. Understanding how to write traits manually reveals the mechanism.

```cpp
#include <iostream>
using namespace std;

// --- is_same: true if two types are identical ---
template<typename T, typename U>
struct is_same {
  static const bool value = false;
};

// Specialization: T and U are the same type
template<typename T>
struct is_same<T, T> {
  static const bool value = true;
};

cout << is_same<int, int>::value;     // 1
cout << is_same<int, float>::value;   // 0

// --- is_pointer ---
template<typename T>
struct is_pointer {
  static const bool value = false;
};
template<typename T>
struct is_pointer<T*> {              // partial specialization matches any pointer
  static const bool value = true;
};

cout << is_pointer<int*>::value;     // 1
cout << is_pointer<int>::value;      // 0
cout << is_pointer<const int*>::value; // 1

// --- remove_const ---
template<typename T>
struct remove_const {
  typedef T type;                    // default: unchanged
};
template<typename T>
struct remove_const<const T> {
  typedef T type;                    // strip the const
};

remove_const<const int>::type x = 42;    // int x = 42
remove_const<int>::type       y = 42;    // int y = 42 (already non-const)

// --- add_pointer ---
template<typename T>
struct add_pointer {
  typedef T* type;
};

add_pointer<int>::type  p = nullptr;     // int* p
add_pointer<void>::type vp = nullptr;    // void* vp

// --- Conditional: pick one type or another based on a bool ---
template<bool Condition, typename IfTrue, typename IfFalse>
struct conditional {
  typedef IfTrue type;              // default: condition is true
};
template<typename IfTrue, typename IfFalse>
struct conditional<false, IfTrue, IfFalse> {
  typedef IfFalse type;             // specialization: condition is false
};

conditional<true,  int, double>::type a = 1;    // int a
conditional<false, int, double>::type b = 2.0;  // double b
```

---

## 17.3 Tag Dispatch

**Tag dispatch** uses overloading on empty "tag" types to select an algorithm implementation at compile time. It is more readable than SFINAE for dispatch based on type categories.

```cpp
#include <iterator>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

// Tags are empty types — their only role is to carry type information
struct slow_tag  {};   // for slow (non-random-access) advance
struct fast_tag  {};   // for fast (random-access) advance

// --- Iterator category dispatch ---
// Implementation for non-random-access iterators (forward, bidirectional)
template<typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, slow_tag) {
  cout << "[slow] stepping " << n << " times\n";
  while (n > 0) { ++it; --n; }
}

// Implementation for random-access iterators — O(1)
template<typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, fast_tag) {
  cout << "[fast] jumping " << n << " positions\n";
  it += n;
}

// Trait to map an iterator's category to our dispatch tag
template<typename Iterator>
struct dispatch_tag {
  typedef slow_tag type;   // default: slow
};
template<>
struct dispatch_tag<vector<int>::iterator> {
  typedef fast_tag type;   // vector iterators are random access
};

// Public interface: deduces tag, dispatches
template<typename Iterator, typename Distance>
void my_advance(Iterator& it, Distance n) {
  typedef typename dispatch_tag<Iterator>::type tag;
  advance_impl(it, n, tag{});
}

vector<int> v = {1, 2, 3, 4, 5};
list<int>   l = {1, 2, 3, 4, 5};

vector<int>::iterator vit = v.begin();
list<int>::iterator   lit = l.begin();

my_advance(vit, 3);   // [fast] jumping 3 positions
my_advance(lit, 3);   // [slow] stepping 3 times

cout << *vit << "\n";  // 4
cout << *lit << "\n";  // 4

// --- Overloading on tag hierarchy (how std::advance works internally) ---
struct input_iterator_tag {};
struct forward_iterator_tag  : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

template<typename It, typename D>
void stl_advance_impl(It& i, D n, random_access_iterator_tag) { i += n; }

template<typename It, typename D>
void stl_advance_impl(It& i, D n, bidirectional_iterator_tag) {
  if (n >= 0) while (n--) ++i; else while (n++) --i;
}

template<typename It, typename D>
void stl_advance_impl(It& i, D n, input_iterator_tag) {
  while (n-- > 0) ++i;
}
```

| Technique | Dispatch on | Readability | Overhead |
|---|---|---|---|
| Tag dispatch | Type category via empty struct | High | Zero |
| SFINAE | Enable/disable via enable_if | Medium | Zero |
| virtual | Runtime type | Low | Vtable indirect call |

---

## 17.4 Policy-Based Design

**Policies** are template parameters that inject behaviour into a class. Instead of one monolithic class or a deep virtual hierarchy, you assemble behaviour from swappable, zero-overhead policy components.

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

// --- Sorting policy ---
struct AscendingPolicy {
  static bool compare(int a, int b) { return a < b; }
  static const char* name()         { return "Ascending"; }
};
struct DescendingPolicy {
  static bool compare(int a, int b) { return a > b; }
  static const char* name()         { return "Descending"; }
};

// --- Bounds-checking policy ---
struct CheckedPolicy {
  static void checkBounds(int idx, int size) {
    if (idx < 0 || idx >= size)
      throw out_of_range("Index out of range");
  }
};
struct UncheckedPolicy {
  static void checkBounds(int /*idx*/, int /*size*/) {}  // no-op, zero cost
};

// --- Assembling the policies into one class ---
template<
  typename SortPolicy     = AscendingPolicy,
  typename BoundsPolicy   = UncheckedPolicy
>
class NumberArray {
  int   data[10];
  int   size_;
public:
  NumberArray() : size_(0) {}

  void push(int v) {
    BoundsPolicy::checkBounds(size_, 10);
    data[size_++] = v;
  }

  int get(int i) const {
    BoundsPolicy::checkBounds(i, size_);
    return data[i];
  }

  void sort() {
    // Insertion sort using the sort policy's compare
    for (int i = 1; i < size_; ++i) {
      int key = data[i];
      int j = i - 1;
      while (j >= 0 && SortPolicy::compare(key, data[j])) {
        data[j+1] = data[j];
        --j;
      }
      data[j+1] = key;
    }
  }

  void print() const {
    cout << SortPolicy::name() << ": ";
    for (int i = 0; i < size_; ++i) cout << data[i] << " ";
    cout << "\n";
  }
};

// Different policy combinations — all compile-time, zero overhead
NumberArray<AscendingPolicy,  UncheckedPolicy> fast_asc;
NumberArray<DescendingPolicy, CheckedPolicy>   safe_desc;

fast_asc.push(5); fast_asc.push(2); fast_asc.push(8); fast_asc.push(1);
fast_asc.sort();
fast_asc.print();   // Ascending: 1 2 5 8

safe_desc.push(5); safe_desc.push(2); safe_desc.push(8); safe_desc.push(1);
safe_desc.sort();
safe_desc.print();  // Descending: 8 5 2 1

try { safe_desc.get(99); }           // throws: checked
catch (const out_of_range& e) { cout << "Caught: " << e.what() << "\n"; }

// --- Policy inheritance (CRTP variant) ---
// Policies can be mixed in via inheritance for more complex behaviour
template<typename Derived, typename LogPolicy>
struct Loggable {
  void log(const char* msg) const {
    LogPolicy::write(msg);
  }
};

struct ConsoleLog  { static void write(const char* m) { cout << "[LOG] " << m << "\n"; } };
struct SilentLog   { static void write(const char* /*m*/) {} };

template<typename Log = ConsoleLog>
class Service : public Loggable<Service<Log>, Log> {
public:
  void run() {
    this->log("Service starting");
    // ...
    this->log("Service done");
  }
};

Service<ConsoleLog> verbose;
Service<SilentLog>  quiet;

verbose.run();  // prints log messages
quiet.run();    // silent — SilentLog::write is a no-op, optimised away
```

---

## 17.5 Expression Templates

**Expression templates** delay the evaluation of arithmetic expressions until the final assignment, eliminating temporary objects. This is the technique behind high-performance linear algebra libraries (Eigen, Blitz++).

```cpp
#include <iostream>
#include <cassert>
using namespace std;

// --- Problem: naive vector addition creates temporaries ---
// a + b + c creates: tmp1 = a+b; result = tmp1+c — two full passes, two allocations

// --- Expression template solution: represent the expression as a type ---

// Leaf node: references an actual vector
template<typename T, int N>
struct VecRef {
  const T* data;
  explicit VecRef(const T* d) : data(d) {}
  T operator[](int i) const { return data[i]; }
};

// Expression node: represents A + B (both can be expressions)
template<typename L, typename R>
struct VecAdd {
  L left;
  R right;
  VecAdd(L l, R r) : left(l), right(r) {}
  // Evaluation is LAZY — only computes one element at a time when asked
  double operator[](int i) const { return left[i] + right[i]; }
};

// The actual vector class
template<int N>
class Vec {
public:
  double data[N];

  Vec() { for (int i = 0; i < N; ++i) data[i] = 0.0; }

  // Constructor from any expression template
  template<typename Expr>
  Vec(const Expr& expr) {
    for (int i = 0; i < N; ++i)
      data[i] = expr[i];   // single loop — computes all additions in one pass
  }

  // Assign from any expression template
  template<typename Expr>
  Vec& operator=(const Expr& expr) {
    for (int i = 0; i < N; ++i)
      data[i] = expr[i];
    return *this;
  }

  VecRef<double, N> ref() const { return VecRef<double, N>(data); }

  double operator[](int i) const { return data[i]; }
  double& operator[](int i)      { return data[i]; }
};

// operator+ returns an expression template node, not a Vec
template<int N>
VecAdd<VecRef<double,N>, VecRef<double,N>>
operator+(const Vec<N>& a, const Vec<N>& b) {
  return VecAdd<VecRef<double,N>, VecRef<double,N>>(a.ref(), b.ref());
}

// operator+ for expression + Vec
template<typename L, int N>
VecAdd<L, VecRef<double,N>>
operator+(const L& left, const Vec<N>& right) {
  return VecAdd<L, VecRef<double,N>>(left, right.ref());
}

// --- Usage ---
const int SIZE = 4;
Vec<SIZE> a, b, c, result;
for (int i = 0; i < SIZE; ++i) { a[i] = i; b[i] = i*2; c[i] = 1; }

// a + b + c: NO temporaries created — single loop over all additions
result = a + b + c;

for (int i = 0; i < SIZE; ++i)
  cout << result[i] << " ";   // 1 4 7 10
// (0+0+1), (1+2+1), (2+4+1), (3+6+1)
```

**Performance impact:** For a vector of N elements and K operands, naive chaining does K-1 passes + K-1 temporaries. Expression templates do 1 pass, 0 temporaries — O(N) memory reads instead of O(K*N).

---

## 17.6 Advanced constexpr Evaluation (C++11)

`constexpr` in C++11 is the type-safe replacement for TMP integer computation. Functions must have a single `return` statement in C++11; iteration must use recursion.

```cpp
#include <iostream>
using namespace std;

// C++11 constexpr: must be a single return expression (no loops, no locals)
constexpr int factorial(int n) {
  return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
  return n <= 1 ? n : fibonacci(n-1) + fibonacci(n-2);
}

constexpr int power(int base, int exp) {
  return exp == 0 ? 1 : base * power(base, exp - 1);
}

// Compile-time constants
constexpr int f5   = factorial(5);    // 120
constexpr int fib8 = fibonacci(8);    // 21
constexpr int p2_8 = power(2, 8);    // 256

// Use as array size (must be compile-time constant)
int buf[factorial(5)];    // int buf[120]
int lut[power(2, 4)];     // int lut[16]

// constexpr with classes (C++11: constructor + single-valued members)
struct Point {
  int x, y;
  constexpr Point(int x, int y) : x(x), y(y) {}
  constexpr int distSq() const { return x*x + y*y; }
};

constexpr Point origin(0, 0);
constexpr Point p(3, 4);
constexpr int   d = p.distSq();   // 25 — computed at compile time
static_assert(d == 25, "Distance check failed");

// Compile-time GCD (Euclidean algorithm via recursion)
constexpr int gcd(int a, int b) {
  return b == 0 ? a : gcd(b, a % b);
}
constexpr int lcm(int a, int b) {
  return (a / gcd(a, b)) * b;
}

static_assert(gcd(12, 8) == 4,   "gcd check");
static_assert(lcm(4, 6)  == 12,  "lcm check");

// Mixing runtime and compile time — constexpr can be called at runtime too
int n;
cin >> n;
cout << factorial(n);   // runtime call — same function

cout << f5 << " " << fib8 << " " << p2_8 << "\n";  // 120 21 256
```

---

## 17.7 SFINAE with Void-Type Trick

Beyond `enable_if`, the **void-t trick** detects whether a type has a particular member or nested type — a foundation of modern type-trait machinery.

```cpp
#include <iostream>
using namespace std;

// Primary template: default to false
template<typename T, typename = void>
struct has_begin {
  static const bool value = false;
};

// Specialization: tries to form void from T::begin — succeeds only if it exists
template<typename T>
struct has_begin<T, typename enable_if<
    is_same<
      typename T::iterator,
      decltype(((T*)nullptr)->begin())
    >::value
  >::type
> {
  static const bool value = true;
};

// Simpler version using a direct member-type check
template<typename, typename = void>
struct has_size_method {
  static const bool value = false;
};

// This specialization compiles only if sizeof(decltype(t.size())) works
template<typename T>
struct has_size_method<T,
  typename enable_if<sizeof(decltype(((T*)nullptr)->size())) >= 0>::type
> {
  static const bool value = true;
};

#include <vector>
#include <string>
cout << has_size_method<vector<int> >::value;  // 1
cout << has_size_method<int>::value;            // 0
cout << has_size_method<string>::value;         // 1

// Using traits for conditional behaviour
template<typename T>
void printSize(const T& container,
               typename enable_if<has_size_method<T>::value>::type* = 0) {
  cout << "Size: " << container.size() << "\n";
}

template<typename T>
void printSize(const T& /*val*/,
               typename enable_if<!has_size_method<T>::value>::type* = 0) {
  cout << "No size() available\n";
}

vector<int> v = {1,2,3};
printSize(v);    // Size: 3
printSize(42);   // No size() available
```

---

## 17.8 Variadic Templates and Parameter Packs (C++11)

**Variadic templates** accept any number of template arguments of any types. They are the foundation of modern TMP — `std::tuple`, `std::make_shared`, `printf`-style type-safe formatting, and `emplace` all rely on them. A **parameter pack** (`typename... Ts`) is expanded with the `...` syntax.

```cpp
#include <iostream>
using namespace std;

// --- Recursive unpacking: process one argument, recurse on the rest ---
void print() { cout << "\n"; }   // base case: empty pack

template<typename First, typename... Rest>
void print(First first, Rest... rest) {
  cout << first << " ";
  print(rest...);                // peel off 'first', recurse on the rest
}

print(1, 2.5, "hello", 'x');     // 1 2.5 hello x

// --- sizeof...(): count the elements in a pack at compile time ---
template<typename... Args>
constexpr int countArgs(Args... args) {
  return sizeof...(Args);         // number of types in the pack
}
static_assert(countArgs(1, 2, 3) == 3, "three args");

// --- Compile-time recursive sum over a pack ---
template<typename T>
T sum(T v) { return v; }          // base case

template<typename T, typename... Rest>
T sum(T first, Rest... rest) {
  return first + sum(rest...);    // fold by recursion (C++11 has no fold expressions)
}
cout << sum(1, 2, 3, 4, 5);       // 15

// --- Variadic class template: compute total sizeof a type list ---
template<typename... Ts>
struct TotalSize;

template<>
struct TotalSize<> {              // base: empty list
  static const int value = 0;
};

template<typename Head, typename... Tail>
struct TotalSize<Head, Tail...> { // recurse on the type pack
  static const int value = sizeof(Head) + TotalSize<Tail...>::value;
};

static_assert(TotalSize<char, int, double>::value == 1 + 4 + 8, "sizes");
```

**Pack expansion patterns.** The `...` can expand a pattern applied to every element, not just the bare pack:

```cpp
#include <iostream>
using namespace std;

// Expand a pattern: forward each argument doubled into another variadic call
template<typename... Args>
void callPrint(Args... args) {
  print(args...);          // expands to print(a0, a1, a2, ...)
}

// Expansion inside a braced-init list — a common C++11 trick to invoke a
// function on every pack element left-to-right (the array is discarded)
template<typename... Args>
void printEach(Args... args) {
  int dummy[] = { 0, ( (cout << args << " "), 0 )... };  // pattern: (expr, 0)
  (void)dummy;
  cout << "\n";
}
printEach(10, "abc", 3.14);   // 10 abc 3.14

// Perfect-forwarding a variadic pack (see also Chapter 9.13) — the basis of
// emplace_back and make_shared
template<typename T, typename... Args>
T* construct(Args&&... args) {
  return new T(std::forward<Args>(args)...);  // forward each arg with its value category
}
```

| Construct | Meaning |
|---|---|
| `typename... Ts` | Template type parameter pack |
| `Ts... args` | Function parameter pack |
| `args...` | Pack expansion (one element per `,`) |
| `pattern...` | Expand a pattern over each element, e.g. `f(args)...` |
| `sizeof...(Ts)` | Number of elements in the pack (compile-time) |

**Note (C++11 vs later):** C++11 unpacks variadic packs via **recursion** (a base case + a recursive case) or the braced-init-list trick above. *Fold expressions* (`(args + ...)`) are a **C++17** feature and are not available in C++11.

---

## 17.9 TMP Best Practices

```cpp
// ✅ Use static_assert to document TMP requirements
template<typename T>
struct MyTrait {
  static_assert(sizeof(T) > 0, "T must be a complete type");
  static const bool value = true;
};

// ✅ Prefer constexpr over struct-value TMP for scalar computations (C++11)
constexpr int compileTimeMax(int a, int b) { return a > b ? a : b; }

// ✅ Use tag dispatch for clean, readable dispatch (prefer over complex SFINAE)

// ✅ Keep policies simple and orthogonal — one responsibility each

// ✅ Wrap TMP in user-friendly interfaces
// The user calls: sortArray<Descending>(arr);
// not: sortArray_impl<Descending, CheckedPolicy, AllocatorPolicy>(arr);

// ✅ Test compile-time results with static_assert
static_assert(Factorial<5>::value == 120,   "Factorial<5> failed");
static_assert(Fib<10>::value == 55,         "Fib<10> failed");

// ⚠️ Avoid deep TMP that takes minutes to compile — prefer constexpr
// ⚠️ Limit instantiation depth — deep recursion slows compile and produces
//    massive error messages
// ⚠️ Write a runtime fallback for every TMP facility — easier to debug
```

---

## Summary

| Technique | Purpose | C++ Mechanism |
|---|---|---|
| **Struct-value TMP** | Compile-time integer computation | Template specialization |
| **Type traits from scratch** | Compile-time type introspection | Partial specialization |
| **Tag dispatch** | Algorithm selection by type category | Function overloading on tag types |
| **Policy-based design** | Composable, zero-overhead behaviour | Template parameters as policies |
| **Expression templates** | Eliminate temporaries in arithmetic | Lazy evaluation via type system |
| **`constexpr` functions** | Type-safe compile-time computation | `constexpr` recursion |
| **Variadic templates** | Operate on arbitrary type/value lists | Parameter packs + recursion |
| **SFINAE / void-t** | Detect type members/methods | enable_if + substitution |

---

## Next Steps

- Study the `<type_traits>` standard header — it is built from these techniques
- Apply policies in your own library code to make classes configurable without virtual functions
- Explore Boost.MPL for a full TMP library built on these foundations
- Move to [Chapter 18: Memory Model](../18_memory_model/README.md)
