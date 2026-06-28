# Chapter 3: Functions

A **function** is a named, reusable block of code that performs one well-defined task. Functions are the primary tool for managing complexity in C++: they let you write a piece of logic once and call it from many places, give that logic a meaningful name, and hide its implementation behind a simple interface. Breaking a program into small, single-purpose functions makes it easier to read, test, and change.

Every function has a **signature** — its name plus the number and types of its parameters — and a **return type** describing the value it hands back (or `void` if it returns nothing). Understanding three distinctions early will make the rest of this chapter click: the difference between a **declaration** (promising a function exists) and a **definition** (providing its body); the difference between a **parameter** (the variable in the function's header) and an **argument** (the actual value passed at the call site); and the difference between passing data **by value** (a copy) versus **by reference** (the original). This chapter builds from those basics up to recursion, function pointers, lambdas, and the C++11 callable utilities.

---

## 3.1 Function Basics

### Function Declaration & Definition

C++ requires that a function be **declared** before it is called, so the compiler knows its signature. A *declaration* (also called a prototype) states the return type, name, and parameter types but no body — it is the promise that the function exists somewhere. A *definition* supplies the actual body. The two are often combined, but in larger programs declarations live in header files while definitions live in `.cpp` files, letting code that uses a function compile without seeing how it works.
```cpp
// Function declaration (prototype)
int add(int a, int b);

// Function definition
int add(int a, int b) {
  return a + b;
}

// Usage
int main() {
  int result = add(5, 3);  // result = 8
  return 0;
}
```

### Function Components
```cpp
returnType functionName(paramType param1, paramType param2) {
  // Function body
  return result;  // Must match returnType
}

// Example with no return
void printMessage(string message) {
  cout << message << endl;
  // No return needed for void functions
}

// Example with no parameters
int getRandom() {
  return rand() % 100;
}

// Multiple parameters
double calculateArea(double length, double width) {
  return length * width;
}
```

---

## 3.2 Parameters & Arguments

How you declare a parameter controls whether the function sees a copy of the caller's data or the data itself — and this choice affects both correctness and performance. C++ offers three main mechanisms. **Pass by value** copies the argument, so the function works on its own private duplicate and the caller's variable is untouched. **Pass by reference** (`T&`) binds the parameter to the caller's actual object, so changes are visible outside the function — the idiom for "output" parameters or for returning more than one value. **Pass by const reference** (`const T&`) combines the efficiency of a reference (no copy) with the safety of immutability, making it the default choice for passing large objects you only need to read. Finally, **default arguments** let callers omit trailing parameters, with the compiler filling in the declared defaults.

### Pass by Value
```cpp
void increment(int x) {
  x++;  // Modifies local copy only
}

int main() {
  int num = 5;
  increment(num);
  cout << num;  // Still 5
  return 0;
}
```

### Pass by Reference
```cpp
void increment(int& x) {
  x++;  // Modifies original
}

int main() {
  int num = 5;
  increment(num);
  cout << num;  // Now 6
  return 0;
}

// Multiple return values using references
void divideAndRemainder(int dividend, int divisor, int& quotient, int& remainder) {
  quotient = dividend / divisor;
  remainder = dividend % divisor;
}

int main() {
  int q, r;
  divideAndRemainder(17, 5, q, r);
  cout << q << " " << r;  // 3 2
}
```

### Pass by Const Reference
```cpp
// Efficient (no copy) but prevents modification
void printVector(const vector<int>& vec) {
  for (int val : vec) {
    cout << val << " ";
  }
  // vec[0] = 10;  // ERROR: cannot modify
}

int main() {
  vector<int> v = {1, 2, 3};
  printVector(v);  // No copy made
  return 0;
}
```

### Default Arguments

Default arguments make a parameter optional by supplying a fallback value used when the caller omits it. This often replaces a set of overloaded functions with a single flexible one. Two rules govern them: defaults must be supplied for a **contiguous trailing group** of parameters (you cannot give a default to an earlier parameter while leaving a later one without one), and a default should appear in only **one** place — typically the declaration in a header, not repeated in the definition.

```cpp
void greet(string name = "Guest", string greeting = "Hello") {
  cout << greeting << ", " << name << "!" << endl;
}

int main() {
  greet();                      // Hello, Guest!
  greet("Alice");               // Hello, Alice!
  greet("Bob", "Hi");           // Hi, Bob!
  return 0;
}

// Default arguments must be at the end
void func(int a, int b = 5, int c = 10);  // OK
// void func(int a = 1, int b);           // ERROR
```

---

## 3.3 Function Overloading

**Overloading** lets several functions share the same name as long as their parameter lists differ in number or type. When you call the name, the compiler performs *overload resolution* — it examines the arguments and selects the best-matching version, all at compile time with no runtime cost. This is what allows a single conceptual operation (say, `add`) to work naturally across `int`, `double`, or different argument counts. The crucial constraint is that overloads must differ in their **parameters**: the return type alone is *not* enough to distinguish them, because the compiler chooses the overload from the call's arguments, not from what you do with the result.

```cpp
// Same name, different parameter types/count

// Version 1: two integers
int add(int a, int b) {
  return a + b;
}

// Version 2: two doubles
double add(double a, double b) {
  return a + b;
}

// Version 3: three integers
int add(int a, int b, int c) {
  return a + b + c;
}

int main() {
  cout << add(5, 3);           // Calls version 1 -> 8
  cout << add(5.5, 3.2);       // Calls version 2 -> 8.7
  cout << add(5, 3, 2);        // Calls version 3 -> 10
  return 0;
}
```

### Overloading Rules
```cpp
// ✅ Different parameter count
void process(int x);
void process(int x, int y);

// ✅ Different parameter types
void process(int x);
void process(double x);
void process(int x, double y);

// ❌ Return type alone is NOT enough
// int getValue();
// double getValue();     // ERROR: ambiguous

// ✅ Const can affect overloading
void modify(int& x);      // Non-const reference
void modify(const int& x);  // Const reference
```

---

## 3.4 Inline Functions

The `inline` keyword is a *hint* asking the compiler to replace a call with the function's body directly, eliminating the small overhead of a function call. It is best suited to tiny, frequently-called functions. Modern compilers make their own inlining decisions based on optimization settings and often ignore the hint, so `inline`'s more important role today is a *language* one: it permits a function to be **defined in a header** and included in multiple translation units without violating the One Definition Rule. Treat `inline` as a tool for code organization first and a micro-optimization second — and don't inline large functions, which only bloats the binary.

```cpp
// Request compiler to inline (copy) function code
inline int square(int x) {
  return x * x;
}

int main() {
  // Compiler may inline: int result = 5 * 5;
  int result = square(5);
  return 0;
}

// Benefits: No function call overhead
// Drawbacks: Code size increases

// Modern alternative: templates, let compiler decide
```

---

## 3.5 Recursion

**Recursion** is when a function calls itself to solve a problem by reducing it to smaller instances of the same problem. Every recursive function needs two ingredients: one or more **base cases** that stop the recursion by returning directly, and a **recursive case** that does a little work and then calls itself on a smaller input, moving toward a base case. Recursion shines on naturally self-similar structures — factorials, tree traversals, divide-and-conquer algorithms — where it often reads far more clearly than the equivalent loop. The trade-offs to keep in mind are the **call stack**: each pending call consumes stack space, so unbounded recursion causes a *stack overflow*, and naive recursion can repeat work exponentially (as the Fibonacci example shows), which **memoization** fixes by caching results.

### Simple Recursion
```cpp
// Factorial: 5! = 5 * 4 * 3 * 2 * 1
int factorial(int n) {
  // Base case
  if (n <= 1) {
    return 1;
  }
  
  // Recursive case
  return n * factorial(n - 1);
}

int main() {
  cout << factorial(5);  // 120
  return 0;
}
```

### Call Stack

Each recursive call is suspended until the calls it makes return, so the partially-finished calls stack up. The diagram below traces `factorial(3)` unwinding: the calls descend to the base case, then resolve back up, each multiplying its result by the value returned from the level beneath it. Visualizing this "down then back up" shape is the key to reasoning about any recursive function.

```cpp
factorial(3)
├─ 3 * factorial(2)
│  ├─ 2 * factorial(1)
│  │  ├─ return 1
│  │  └─ return 2 * 1 = 2
│  └─ return 3 * 2 = 6
└─ return 6
```

### More Complex Recursion
```cpp
// Fibonacci: 1, 1, 2, 3, 5, 8, 13, ...
int fibonacci(int n) {
  if (n <= 2) {
    return 1;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}

// Binary search
int binarySearch(vector<int>& arr, int target, int left, int right) {
  if (left > right) {
    return -1;  // Not found
  }
  
  int mid = (left + right) / 2;
  
  if (arr[mid] == target) {
    return mid;
  } else if (arr[mid] < target) {
    return binarySearch(arr, target, mid + 1, right);
  } else {
    return binarySearch(arr, target, left, mid - 1);
  }
}

// Tree traversal (pre-order)
void traverseTree(Node* node) {
  if (node == nullptr) return;
  
  cout << node->value << " ";    // Process
  traverseTree(node->left);      // Traverse left
  traverseTree(node->right);     // Traverse right
}
```

### Recursion Considerations
```cpp
// ❌ No base case = infinite recursion = stack overflow
void bad() {
  bad();  // CRASH!
}

// ✅ Always have base case
void good(int n) {
  if (n <= 0) return;  // Base case
  cout << n << " ";
  good(n - 1);
}

// ⚠️ Inefficient recursion
int fib(int n) {
  if (n <= 2) return 1;
  return fib(n-1) + fib(n-2);  // Recalculates many times
}
// fib(5) calls fib(4) and fib(3)
// fib(4) calls fib(3) and fib(2) <- fib(3) recalculated!

// ✅ Memoization (caching results)
map<int, int> memo;
int fibMemo(int n) {
  if (n <= 2) return 1;
  if (memo.find(n) != memo.end()) {
    return memo[n];  // Return cached result
  }
  memo[n] = fibMemo(n-1) + fibMemo(n-2);
  return memo[n];
}
```

---

## 3.6 Function Pointers

A **function pointer** stores the address of a function, letting you treat behaviour as data — pass a function as an argument, store it in a variable, or pick one at runtime from a table. This is the classic mechanism behind **callbacks**: a general routine (a sorter, an event dispatcher) that calls back into code you supply. The syntax is notoriously dense — `returnType (*name)(paramTypes)` — which is one reason a `typedef` or `using` alias is common, and why the modern alternatives `std::function` and lambdas (sections 3.7 and 3.9) have largely replaced raw function pointers in new code.

```cpp
// Pointer to function: returnType (*pointerName)(parameters)

typedef int (*FunctionPtr)(int, int);  // Type alias

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int main() {
  // Get function address
  FunctionPtr func = &add;
  
  // Call through pointer
  int result = func(5, 3);  // Can also use (*func)(5, 3)
  cout << result << endl;   // 8
  
  // Change to different function
  func = multiply;
  result = func(5, 3);
  cout << result << endl;   // 15
  
  return 0;
}

// Callback example
void execute(FunctionPtr operation, int x, int y) {
  cout << operation(x, y) << endl;
}

int main() {
  execute(add, 10, 20);        // 30
  execute(multiply, 10, 20);   // 200
  return 0;
}
```

---

## 3.7 Lambda Functions (C++11)

A **lambda** is an anonymous function you can define inline, right where you use it — perfect for short, one-off callbacks passed to STL algorithms. Its anatomy is `[captures](parameters) -> returnType { body }`, where the return type is usually deduced and can be omitted. What sets lambdas apart from plain function pointers is the **capture list** `[...]`: it lets the lambda use variables from the enclosing scope. Capturing **by value** `[x]` copies the variable into the lambda (a snapshot at creation time), while capturing **by reference** `[&x]` lets the lambda read and modify the original. The catch-all forms `[=]` and `[&]` capture everything used, by value or by reference respectively — convenient, but explicit captures are clearer and safer (a dangling reference capture is a common bug).

```cpp
// [captures](parameters) -> returnType { body }

auto add = [](int a, int b) { return a + b; };
int result = add(5, 3);  // 8

// With explicit return type
auto greet = [](string name) -> string {
  return "Hello, " + name;
};

// Capture variables
int multiplier = 3;
auto multiply = [multiplier](int x) { return x * multiplier; };
cout << multiply(5);  // 15

// Capture by reference
int counter = 0;
auto increment = [&counter]() { counter++; };
increment();
cout << counter;  // 1

// Capture all by value [=] or all by reference [&]
auto printAll = [&]() {
  cout << "Counter: " << counter << endl;
};

// With STL algorithm
vector<int> v = {1, 2, 3, 4, 5};
sort(v.begin(), v.end(), [](int a, int b) {
  return a > b;  // Descending order
});
```

---

## 3.8 Best Practices

Good functions are small, focused, and predictable. The guidelines below distill the chapter's themes: give each function a **single responsibility** so its name fully describes what it does; use `const` references for parameters you only read, both for efficiency and to document that you won't modify them; and make a function's **error behaviour** explicit — whether by returning a success flag, an output parameter, or (as Chapter 13 covers) throwing an exception. Above all, document any **side effects**: a reader should be able to predict what a function changes from its signature and a short comment, without reading its body.

```cpp
// ✅ Single Responsibility Principle
int calculateSum(vector<int>& v) {
  int sum = 0;
  for (int val : v) sum += val;
  return sum;
}

// ✅ Const correctness
void printInfo(const string& name, int age) {
  cout << name << " is " << age << endl;
}

// ✅ Error handling
bool divide(int a, int b, int& result) {
  if (b == 0) {
    return false;  // Signal error
  }
  result = a / b;
  return true;
}

// ✅ Document side effects
// Modifies vector and returns count
int processAndCount(vector<int>& v) {
  int count = 0;
  for (int& val : v) {
    if (isValid(val)) count++;
    else val = 0;
  }
  return count;
}
```

---

## 3.9 std::function and std::bind (C++11)

`std::function` is a general-purpose callable wrapper. It can hold free functions, lambdas, functors, and bound member functions under a single uniform type.

```cpp
#include <functional>
#include <iostream>
using namespace std;

// --- std::function ---
// Signature: function<ReturnType(ArgTypes...)>

int add(int a, int b) { return a + b; }

function<int(int, int)> f1 = add;                             // free function
function<int(int, int)> f2 = [](int a, int b){ return a*b; }; // lambda

struct Multiplier {
  int operator()(int a, int b) { return a * b; }
};
function<int(int, int)> f3 = Multiplier{};                    // functor

cout << f1(3, 4);  // 7
cout << f2(3, 4);  // 12
cout << f3(3, 4);  // 12

// Use as callback parameter — accepts any compatible callable
void applyAndPrint(int x, int y, function<int(int,int)> op) {
  cout << op(x, y) << endl;
}
applyAndPrint(10, 3, add);
applyAndPrint(10, 3, [](int a, int b){ return a - b; });

// Wrap member functions
struct Counter {
  int count = 0;
  void increment(int by) { count += by; }
  int  get() const       { return count; }
};

Counter c;
// Wrap: &Class::method, then supply instance pointer
function<void(int)> inc = bind(&Counter::increment, &c, placeholders::_1);
inc(5);
inc(3);
cout << c.get();  // 8

// --- std::bind ---
// bind(callable, arg_or_placeholder, ...) — partially applies arguments
// Use placeholders::_1, _2, ... for arguments to be supplied later

using namespace placeholders;

function<int(int)> add5 = bind(add, _1, 5);  // fix second arg to 5
cout << add5(10);  // 15
cout << add5(20);  // 25

// Reorder arguments
function<int(int,int)> flip = bind(add, _2, _1);  // swap argument order
cout << flip(3, 7);  // 10 (same as add(7,3))

// Bind to a different fixed value
function<int(int)> triple = bind([](int x, int factor){ return x * factor; },
                                  _1, 3);
cout << triple(4);  // 12
```

| Tool | Purpose |
|------|---------|
| `function<R(A,B)>` | Store any callable matching that signature |
| `bind(f, _1, val)` | Partially apply arguments; reorder them |
| `placeholders::_1` | Marks position for caller-supplied argument |

**Tip:** For simple partial application, lambdas are usually cleaner than `bind`:
```cpp
// bind equivalent (verbose)
function<int(int)> add5 = bind(add, placeholders::_1, 5);
// lambda equivalent (clearer)
auto add5 = [](int x){ return add(x, 5); };
```

---

## 3.10 C++11 Function Syntax: Trailing Return Type, constexpr, noexcept

C++11 adds three modifiers to the function declaration itself.

### Trailing Return Type (`auto f(...) -> Type`)

The return type can be written **after** the parameter list. This is essential when the return type depends on the parameters (via `decltype`), and is also just a cleaner way to write complex return types.

```cpp
// Classic form
int add(int a, int b) { return a + b; }

// Trailing return type — equivalent
auto addTrailing(int a, int b) -> int { return a + b; }

// The real motivation: a return type computed FROM the parameters.
// 'a * b' isn't in scope until after the parameter list, so decltype must
// appear in the trailing position.
template<typename A, typename B>
auto multiply(A a, B b) -> decltype(a * b) {
  return a * b;
}

auto r1 = multiply(3, 4);     // int
auto r2 = multiply(3, 4.5);   // double

// Also tidies up hard-to-read return types (e.g. function pointers)
auto getHandler(int code) -> void(*)(int);
```

### `constexpr` Functions

A `constexpr` function **can** be evaluated at compile time when called with constant arguments — usable as an array size, template argument, etc. In **C++11** a `constexpr` function body is restricted to a single `return` statement (loops and local variables were allowed only from C++14).

```cpp
constexpr int square(int x) { return x * x; }
constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

int buf[square(8)];           // 64 — evaluated at compile time
static_assert(factorial(5) == 120, "compile-time check");

// The same function still works at runtime with non-constant arguments
int n = 6;
int f = factorial(n);         // runtime call — perfectly fine
```

(See [Chapter 17.6](../17_template_metaprogramming/README.md) for `constexpr` as a compile-time computation tool.)

### `noexcept` Specifier

`noexcept` declares that a function will not throw. It enables library optimizations (notably move operations in `std::vector`) and documents intent; violating it calls `std::terminate()`.

```cpp
int add(int a, int b) noexcept { return a + b; }      // promises not to throw

// Conditional: noexcept only if the expression inside is noexcept
template<typename T>
void doSwap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) { /* ... */ }

// noexcept operator: compile-time bool — "can this expression throw?"
bool safe = noexcept(add(1, 2));   // true
```

Always mark move constructors, move assignment, `swap`, and destructors `noexcept`. Full treatment is in [Chapter 13.9](../13_exception_handling/README.md).

| Modifier | Purpose |
|---|---|
| `auto f(...) -> T` | Trailing return type; required when the type depends on parameters |
| `constexpr` | Allow compile-time evaluation (C++11: single-`return` body) |
| `noexcept` | Promise no exceptions; enables STL move optimizations |

---

## Summary

| Concept | Details |
|---------|---------|
| **Declaration** | Function prototype |
| **Pass by value** | Parameter copy |
| **Pass by reference** | Modify original |
| **Const reference** | Efficient & safe |
| **Overloading** | Same name, different signatures |
| **Recursion** | Function calls itself |
| **Pointers** | Function addresses |
| **Lambda** | Anonymous functions (C++11) |
| **std::function** | Type-erased callable wrapper (C++11) |
| **std::bind** | Partial application and argument reordering (C++11) |
| **Trailing return** | `auto f(...) -> T`, enables `decltype` returns (C++11) |
| **constexpr** | Compile-time-evaluable functions (C++11) |
| **noexcept** | No-throw guarantee; enables move optimizations (C++11) |

---

## Next Steps
- Write functions for various tasks
- Practice function overloading
- Use lambdas with algorithms
- Move to [Chapter 4: Pointers & References](../04_pointers_references/README.md)
