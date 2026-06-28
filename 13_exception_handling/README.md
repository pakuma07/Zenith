# Chapter 13: Exception Handling

Exceptions are C++'s mechanism for separating error-handling code from the normal flow of logic. When a function detects a condition it cannot handle, it `throw`s an exception object; the runtime then unwinds the call stack, destroying local objects along the way, until it finds a matching `catch` handler in some calling function. This lets errors propagate up to a level capable of dealing with them without every intermediate function having to check and forward return codes.

This chapter covers the syntax (`try`/`catch`/`throw`), the standard exception hierarchy rooted at `std::exception`, how to write custom exception types, how stack unwinding interacts with object destruction, and the exception-safety guarantees that well-designed code provides. The unifying lesson is that exception safety depends on RAII: because stack unwinding reliably runs destructors, tying every resource to an object's lifetime (smart pointers, containers, lock guards) is what makes code correct in the presence of exceptions. C++11 additions — `noexcept`, `exception_ptr`, and nested exceptions — are covered in the later sections.

## 13.1 Try-Catch Blocks

The `try`/`catch` construct is the foundation of exception handling. Code that might fail goes inside a `try` block; if any statement within it throws, execution immediately jumps to a matching `catch` block, skipping the rest of the `try`. A `catch` clause names the exception type it handles and should almost always take it by `const` reference (e.g. `const std::runtime_error&`) to avoid copying and to preserve polymorphism. If no statement throws, all `catch` blocks are skipped entirely, so there is no cost on the success path in typical implementations.

```cpp
#include <iostream>
using namespace std;

int main() {
  try {
    int x = 10;
    int y = 0;
    
    if (y == 0) {
      throw runtime_error("Division by zero!");
    }
    
    int result = x / y;
  }
  catch (const runtime_error& e) {
    cout << "Error caught: " << e.what() << endl;
  }
  
  return 0;
}
```

---

## 13.2 Standard Exceptions

The standard library defines a hierarchy of exception classes in `<stdexcept>`, all derived from `std::exception`, which provides the virtual `what()` method returning a descriptive C-string. The hierarchy splits into `logic_error` (faults detectable before running, such as `invalid_argument` and `out_of_range` from precondition violations) and `runtime_error` (faults only knowable at run time, such as `overflow_error` or `range_error`). Reusing these types — rather than throwing raw integers or strings — lets callers catch at whatever granularity they need, including a single `catch (const std::exception& e)` that handles the entire family. A trailing `catch (...)` catches absolutely anything, including non-standard throw types, and is useful as a last-resort safety net.

```cpp
#include <stdexcept>
using namespace std;

// Exception hierarchy
// exception
// ├─ logic_error
// │  ├─ invalid_argument
// │  ├─ out_of_range
// │  └─ ...
// └─ runtime_error
//    ├─ range_error
//    ├─ overflow_error
//    └─ ...

// Common standard exceptions
try {
  vector<int> v = {1, 2, 3};
  cout << v.at(10);  // Throws out_of_range
}
catch (const out_of_range& e) {
  cout << e.what() << endl;  // "vector::_M_range_check"
}

// invalid_argument
try {
  int value = stoi("abc");
}
catch (const invalid_argument& e) {
  cout << "Invalid: " << e.what() << endl;
}

// Catch all exceptions
try {
  throw runtime_error("Something failed");
}
catch (const exception& e) {
  cout << e.what() << endl;
}
catch (...) {
  cout << "Unknown exception" << endl;
}
```

---

## 13.3 Custom Exceptions

When the standard exception types do not convey enough domain meaning, define your own by deriving from the most appropriate standard class (commonly `std::runtime_error` or `std::invalid_argument`, which conveniently accept a message string in their constructor). Deriving from the standard hierarchy is important: it means callers can still catch your custom type via a `catch (const std::exception&)` base handler, and your exception participates in the same `what()` interface. Keep exception types lightweight and ensure their copy/move operations cannot themselves throw, since copying an in-flight exception must not fail. Throw the most specific type that fits, and catch derived types before base types (see 13.4).

```cpp
// Derive from standard exception
class DivideByZero : public runtime_error {
public:
  DivideByZero() : runtime_error("Division by zero") { }
};

class NegativeValue : public invalid_argument {
public:
  NegativeValue(const string& msg) 
    : invalid_argument(msg) { }
};

// Usage
try {
  if (divisor == 0) {
    throw DivideByZero();
  }
  if (value < 0) {
    throw NegativeValue("Value cannot be negative");
  }
}
catch (const DivideByZero& e) {
  cout << "Division error: " << e.what() << endl;
}
catch (const NegativeValue& e) {
  cout << "Validation error: " << e.what() << endl;
}
catch (const exception& e) {
  cout << "General error: " << e.what() << endl;
}
```

---

## 13.4 Multiple Catch Blocks

A single `try` block can be followed by several `catch` clauses, and the runtime tries them top to bottom, selecting the first whose type matches the thrown exception. Because a handler for a base class also matches all derived classes, **order is critical**: list the most specific (most derived) types first and the most general (`std::exception`, then `catch (...)`) last. If you put a base handler before a derived one, the derived handler becomes unreachable — many compilers will warn about this. Only one matching handler runs; after it completes, execution continues after the entire `try`/`catch` group.

```cpp
try {
  // Code that might throw
  processData();
}
catch (const invalid_argument& e) {
  cout << "Invalid input: " << e.what() << endl;
}
catch (const runtime_error& e) {
  cout << "Runtime error: " << e.what() << endl;
}
catch (const logic_error& e) {
  cout << "Logic error: " << e.what() << endl;
}
catch (const exception& e) {
  cout << "Unknown error: " << e.what() << endl;
}

// Order matters! Catch specific first, general last
```

---

## 13.5 Stack Unwinding

**Stack unwinding** is the process the runtime performs after a `throw`: it walks back up the call stack from the throw point toward a matching handler, and as it exits each function scope it destroys that scope's automatic (local) objects by calling their destructors. This is the mechanism that makes RAII work — a `unique_ptr`, `lock_guard`, or file wrapper declared as a local will be cleaned up correctly even when control leaves via an exception rather than a normal return. Statements after the throw in each unwound function never execute. One crucial caveat: a destructor that itself throws *during* unwinding causes `std::terminate()`, which is why destructors should never let exceptions escape.

```cpp
void levelThree() {
  cout << "In levelThree" << endl;
  throw runtime_error("Error in level 3");
  cout << "Never reaches here" << endl;
}

void levelTwo() {
  cout << "In levelTwo" << endl;
  levelThree();
  cout << "Never reaches here" << endl;
}

void levelOne() {
  cout << "In levelOne" << endl;
  levelTwo();
  cout << "Never reaches here" << endl;
}

int main() {
  try {
    levelOne();
  }
  catch (const exception& e) {
    cout << "Caught: " << e.what() << endl;
  }
  
  return 0;
}

// Output:
// In levelOne
// In levelTwo
// In levelThree
// Caught: Error in level 3
```

---

## 13.6 Resource Cleanup with Exceptions

Because an exception can fly past any `delete`, `fclose`, or `unlock` statement, manual cleanup placed after the acquiring code is fundamentally unsafe — if a throw occurs in between, the release is skipped and the resource leaks. C++ has no `finally` keyword; the idiomatic substitute is RAII, where cleanup lives in a destructor that stack unwinding is guaranteed to run. The examples below show the leaking anti-pattern alongside the RAII and smart-pointer solutions, plus how to emulate a `try`/`finally` by wrapping the resource in an object whose destructor always frees it. The takeaway: wrap every resource in an RAII type and you get exception-safe cleanup for free.

```cpp
// ❌ Resource leak with exception
void leak() {
  int* ptr = new int(42);
  if (error) throw runtime_error("fail");
  delete ptr;  // Never reached!
}

// ✅ RAII prevents leaks
class Resource {
public:
  Resource() { cout << "Acquired" << endl; }
  ~Resource() { cout << "Freed" << endl; }
};

void safe() {
  Resource r;  // Acquired
  if (error) throw runtime_error("fail");
}  // Freed (always!)

// ✅ Smart pointers
void safe2() {
  unique_ptr<int> ptr(new int(42));
  if (error) throw runtime_error("fail");
}  // Automatically deleted

// Try-catch-finally pattern (using RAII)
void processFile(const string& filename) {
  FILE* f = fopen(filename.c_str(), "r");
  
  try {
    if (!f) throw runtime_error("Cannot open file");
    
    // Process file
    processData(f);
  }
  catch (const exception& e) {
    cout << "Error: " << e.what() << endl;
  }
  
  // Cleanup (always happens)
  if (f) fclose(f);
}
```

---

## 13.7 Exception Safety Levels

**Exception safety** describes the guarantee a function makes about program state if it throws partway through. There are four widely-recognized levels, from strongest to weakest: the **no-throw guarantee** (the operation never throws — marked `noexcept`, required for destructors, swaps, and move operations); the **strong guarantee** (commit-or-rollback: if it throws, the program state is exactly as it was before the call); the **basic guarantee** (no leaks and all invariants hold, but the state may have changed); and **no guarantee** (state may be corrupt — to be avoided). Aim for at least the basic guarantee everywhere and the strong guarantee for operations that modify important state, typically achieved with the "do all the throwing work on a copy, then swap" technique.

```cpp
// No-throw guarantee
void functionNoThrow() noexcept {
  // Must not throw
  int x = 5;
  cout << x;
}

// Strong guarantee
void functionStrong() {
  // Either succeeds completely or has no effect
  int* ptr = new int(42);
  try {
    if (error) throw runtime_error("fail");
    // Success
  }
  catch (...) {
    delete ptr;  // Rollback
    throw;
  }
}

// Basic guarantee
void functionBasic() {
  // If throws, object in valid state (but not original)
  vector<int> v;
  v.push_back(1);
  if (error) throw runtime_error("fail");
}

// No guarantee (avoid!)
void functionUnsafe() {
  int* ptr = new int(42);
  if (error) throw runtime_error("fail");
  delete ptr;  // Might not happen
}
```

---

## 13.8 Best Practices

This section condenses the chapter's lessons into actionable rules. The most important habits are: catch exceptions by `const` reference (never by value, which slices derived types and copies needlessly); throw specific, meaningful exception types derived from `std::exception`; rely on RAII rather than manual cleanup so unwinding handles resources; mark genuinely non-throwing functions `noexcept`; and document the exception contract of every function that can throw. Use exceptions for genuinely exceptional, hard-to-handle-locally errors — not as ordinary control flow.

```cpp
// ✅ Catch by const reference
try { }
catch (const exception& e) {
  cout << e.what();
}

// ❌ Avoid catch by value
// try { }
// catch (exception e) { }  // Inefficient copy

// ✅ Throw specific exceptions
if (value < 0) {
  throw invalid_argument("Value must be positive");
}

// ❌ Avoid bare throw
// throw;  // Outside catch block

// ✅ Use noexcept for no-throw functions
void noThrowFunction() noexcept { }

// ✅ RAII for automatic cleanup
void process() {
  try {
    Resource r;
    // Use r
  }
  catch (...) {
    // r automatically cleaned
    throw;
  }
}

// ✅ Document exception contracts
/// @throws invalid_argument if value < 0
/// @throws runtime_error if file not found
void process(int value);
```

---

## 13.9 noexcept Specifier (C++11)

`noexcept` declares that a function will **never throw** an exception. Violating this causes `std::terminate()` — not stack unwinding. It also enables critical optimizations in the standard library.

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

// Basic noexcept: this function guarantees no exceptions
int add(int a, int b) noexcept {
  return a + b;
}

// Without noexcept: may throw (default assumption)
int divide(int a, int b) {
  if (b == 0) throw runtime_error("Division by zero");
  return a / b;
}

// noexcept on move constructor: critical for vector efficiency
// vector<T> can only use move during reallocation if move is noexcept
// Otherwise it falls back to copy (slower)
class Widget {
  vector<int> data;
public:
  Widget(size_t n) : data(n, 0) {}

  // Mark move noexcept so vector<Widget> uses move during resize
  Widget(Widget&& other) noexcept : data(move(other.data)) {}
  Widget& operator=(Widget&& other) noexcept {
    data = move(other.data);
    return *this;
  }
};

// noexcept on destructors — they are implicitly noexcept in C++11
// Never throw from a destructor

// noexcept(expr) — conditional: noexcept if the expression is noexcept
template<typename T>
void mySwap(T& a, T& b) noexcept(noexcept(T(move(a)))) {
  T tmp = move(a);
  a = move(b);
  b = move(tmp);
}

// noexcept operator: query whether an expression can throw (compile-time bool)
cout << noexcept(add(1, 2));    // 1 (true)
cout << noexcept(divide(4, 2)); // 0 (false — not marked noexcept)
```

| Scenario | With `noexcept` | Without `noexcept` |
|---|---|---|
| Exception thrown | `std::terminate()` | Normal propagation |
| Move in `vector` resize | Used (fast) | Falls back to copy (slow) |
| Destructor default | Implicitly `noexcept` | Implicitly `noexcept` |
| Compile overhead | Slightly smaller binary | Standard |

**Guideline:** Always mark **move constructors, move assignments, swap functions, and destructors** `noexcept`. Do not mark functions `noexcept` if they can legitimately throw.

---

## 13.10 Exception Propagation and Transport (C++11)

C++11 lets you **capture an exception as a first-class object** (`std::exception_ptr`) and move it across function boundaries, store it, or rethrow it later — even on a different thread. This is how `std::async`/`std::future` deliver exceptions from a worker thread back to the caller.

```cpp
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
using namespace std;

// std::exception_ptr: a copyable, shared handle to a captured exception
exception_ptr captureException() {
  exception_ptr eptr;
  try {
    throw runtime_error("something failed deep inside");
  }
  catch (...) {
    eptr = current_exception();   // capture WHATEVER was thrown (type-erased)
  }
  return eptr;                    // the exception now travels as a value
}

int main() {
  exception_ptr eptr = captureException();

  // Rethrow it later, in a different context, to inspect its real type
  if (eptr) {
    try {
      rethrow_exception(eptr);    // re-raises the stored exception
    }
    catch (const runtime_error& e) {
      cout << "Transported exception: " << e.what() << endl;
    }
  }
  return 0;
}
```

**Collecting exceptions from multiple operations** — a common pattern when you must run every task and report failures afterward:

```cpp
vector<exception_ptr> errors;

for (int i = 0; i < 3; ++i) {
  try {
    if (i == 1) throw logic_error("task 1 broke");
    if (i == 2) throw runtime_error("task 2 broke");
    // task i succeeded
  }
  catch (...) {
    errors.push_back(current_exception());  // stash, keep going
  }
}

for (const exception_ptr& e : errors) {
  try { rethrow_exception(e); }
  catch (const exception& ex) { cout << "Deferred: " << ex.what() << endl; }
}
```

**`std::nested_exception` — chaining causes (exception "stack traces").** `throw_with_nested` attaches the currently-handled exception to a new one, so you can report a high-level error *and* its underlying cause.

```cpp
#include <exception>

void readConfig() {
  throw runtime_error("disk read error");          // low-level cause
}

void startup() {
  try {
    readConfig();
  }
  catch (...) {
    // Wrap the low-level error inside a higher-level one
    throw_with_nested(runtime_error("startup failed"));
  }
}

// Recursively unwrap and print the full cause chain
void printChain(const exception& e, int depth = 0) {
  cout << string(depth * 2, ' ') << "- " << e.what() << "\n";
  try {
    rethrow_if_nested(e);   // rethrows the nested exception, if any
  }
  catch (const exception& inner) {
    printChain(inner, depth + 1);
  }
  catch (...) {}
}

// try { startup(); }
// catch (const exception& e) { printChain(e); }
// Output:
//   - startup failed
//     - disk read error
```

| Facility | Purpose |
|---|---|
| `current_exception()` | Inside a `catch`, capture the active exception as `exception_ptr` |
| `exception_ptr` | Copyable, null-able handle that stores a captured exception |
| `rethrow_exception(p)` | Re-raise a stored exception (on any thread) |
| `make_exception_ptr(e)` | Build an `exception_ptr` directly from an exception object |
| `throw_with_nested(e)` | Throw `e` while attaching the currently-handled exception |
| `rethrow_if_nested(e)` | Rethrow the nested cause, if `e` derives from `nested_exception` |

---

## 13.11 Deprecation of Dynamic Exception Specifications

Pre-C++11 code sometimes listed the exceptions a function could throw using `throw(...)` — the **dynamic exception specification**. C++11 **deprecates** this feature (it was removed entirely in C++17). Use `noexcept` (see 13.9) instead.

```cpp
// ❌ Deprecated in C++11 — do not write new code like this
void oldStyle() throw(std::runtime_error);  // "throws only runtime_error"
void oldNoThrow() throw();                   // "throws nothing"

// ✅ C++11 replacement
void newStyle();                  // may throw anything (the normal default)
void newNoThrow() noexcept;       // guaranteed not to throw (replaces throw())
```

- `throw()` (empty spec) → replace with `noexcept`.
- `throw(TypeList)` → there is no modern equivalent; document thrown types instead and rely on `noexcept` only for the no-throw case.
- A violated dynamic spec called `std::unexpected()`; a violated `noexcept` calls `std::terminate()`.

---

## Summary

| Concept | Details |
|---------|---------|
| **try** | Protected code block |
| **catch** | Exception handler |
| **throw** | Raise exception |
| **Stack unwinding** | Unwind to catch handler |
| **Resource** | Use RAII/smart pointers |
| **noexcept** | No-throw guarantee; enables move optimization in STL |
| **Custom** | Derive from exception |
| **exception_ptr** | Capture/transport an exception as a value (C++11) |
| **current_exception / rethrow_exception** | Store the active exception and re-raise it later (C++11) |
| **nested_exception** | Chain a new exception to its underlying cause (C++11) |
| **throw(...) spec** | Deprecated in C++11 — use `noexcept` |

---

## Next Steps
- Use try-catch for error handling
- Create custom exceptions
- Practice stack unwinding
- Move to [Chapter 14: File I/O](../14_file_io/README.md)
