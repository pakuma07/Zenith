# Chapter 12: Memory Management

Memory management is one of C++'s defining responsibilities: unlike garbage-collected languages, C++ gives you direct control over when and where objects are allocated and freed. This chapter covers the two memory regions a program uses (the stack and the heap), the raw `new`/`delete` operators that manage heap memory manually, and the bugs that result from mismanaging them — chiefly memory leaks and dangling pointers. It then moves to the modern C++11 tools that make memory safety the default: smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`), the RAII idiom, and move semantics.

The guiding principle throughout is RAII (Resource Acquisition Is Initialization): tie every resource to the lifetime of an object so that cleanup happens automatically, even when exceptions are thrown. With smart pointers and RAII you should almost never need to write a bare `new`/`delete` pair in modern C++ — understanding the raw mechanics first, however, is what lets you use the high-level tools correctly and debug them when they misbehave.

## 12.1 Stack vs Heap

Every running C++ program divides its automatic memory into two regions. The **stack** holds local variables and function call frames; allocation is just a pointer bump, so it is extremely fast, but space is limited (typically a few megabytes) and an object's lifetime is bound to its enclosing scope — it is destroyed automatically when the scope exits. The **heap** (free store) is a large pool from which you request memory explicitly with `new` and must release with `delete`; it supports large and dynamically-sized allocations whose lifetime you control, at the cost of slower allocation and the burden of manual cleanup. Prefer stack allocation whenever an object's size is known and its lifetime fits a scope; reach for the heap only when you need a large buffer, a size determined at runtime, or an object that must outlive the function that created it.

```cpp
// Stack: automatic, fast, limited size
void stackExample() {
  int x = 10;                // Stack
  int arr[100];              // Stack (fixed size)
  vector<int> v;             // Vector object on stack
}  // x, arr automatically destroyed

// Heap: manual, slower, unlimited size
void heapExample() {
  int* ptr = new int(10);    // Heap
  delete ptr;                // Must manually delete
  
  int* arr = new int[100];   // Dynamic array
  delete[] arr;              // Delete array with []
}
```

---

## 12.2 New & Delete

`new` allocates an object on the heap, runs its constructor, and returns a typed pointer to it; `delete` runs the destructor and returns the memory to the free store. The cardinal rule is that allocation and deallocation forms must match: memory from `new` is freed with `delete`, while memory from `new[]` (an array) must be freed with `delete[]` — mixing them is undefined behavior. After deleting, set the pointer to `nullptr` so a later accidental use or double-`delete` is caught rather than corrupting memory. In modern C++ you rarely call these operators directly, but they remain the foundation that smart pointers and containers are built on.

```cpp
// Single object
int* ptr = new int;         // Allocate
*ptr = 42;                  // Use
delete ptr;                 // Deallocate
ptr = nullptr;              // Avoid dangling pointer

// With initialization
int* ptr2 = new int(10);    // Allocate and initialize
delete ptr2;

// Array
int* arr = new int[10];     // Allocate array
arr[0] = 5;
delete[] arr;               // Delete with []

// Object
class Person {
public:
  Person(string n) : name(n) { cout << "Created" << endl; }
  ~Person() { cout << "Destroyed" << endl; }
private:
  string name;
};

Person* p = new Person("Alice");  // Constructor called
delete p;                          // Destructor called
```

---

## 12.3 Memory Leaks

A **memory leak** occurs when heap memory is allocated but never freed, so it remains reserved until the program exits — in a long-running program, repeated leaks steadily consume memory until the process is starved or killed. Leaks commonly arise from forgetting to `delete`, from losing the only pointer to an allocation (by overwriting or returning early), and especially from an exception that bypasses the `delete` statement during stack unwinding. Because the leaked memory has no remaining pointer, it can never be recovered, and tools like Valgrind or AddressSanitizer are usually needed to find leaks after the fact. The reliable fix is not vigilance but design: use smart pointers or RAII so deallocation is automatic and cannot be skipped.

### Examples

The examples below contrast the common ways leaks happen with the disciplined manual pattern that avoids them; note that even the "correct" version is fragile in the presence of exceptions, which is exactly why smart pointers exist.

```cpp
// ❌ Memory leak: forget to delete
void leak() {
  int* ptr = new int(42);
}  // ptr destroyed, memory lost!

// ❌ Exception causes leak
void exceptionLeak() {
  int* ptr = new int(42);
  if (error) throw runtime_error("fail");
  delete ptr;  // Never reached!
}

// ❌ Overwrite pointer
int* ptr = new int(42);
ptr = new int(100);  // First memory leaked!
delete ptr;

// ✅ Always delete
void noLeak() {
  int* ptr = new int(42);
  // Use...
  delete ptr;
  ptr = nullptr;
}
```

---

## 12.4 Smart Pointers (C++11)

Smart pointers are class templates in `<memory>` that wrap a raw pointer and apply RAII to it: they own the heap object and automatically `delete` it when the smart pointer is destroyed, eliminating manual cleanup and the leaks that come with it. C++11 introduced three: `unique_ptr` for exclusive single ownership, `shared_ptr` for shared ownership via reference counting, and `weak_ptr` for non-owning observation that breaks reference cycles. As a rule, make `unique_ptr` your default and only escalate to `shared_ptr` when ownership genuinely must be shared, since reference counting adds overhead and shared mutable state is harder to reason about.

### Unique Pointer

A `unique_ptr` models **exclusive ownership**: exactly one `unique_ptr` owns the object at a time, and when it goes out of scope the object is deleted. It is non-copyable (copying would create two owners) but movable — transferring ownership with `std::move` leaves the source empty (`nullptr`). It has essentially zero overhead compared to a raw pointer, making it the preferred default smart pointer; use the `unique_ptr<T[]>` specialization for arrays so the correct `delete[]` is applied. Note that `std::make_unique` is C++14, so in strict C++11 you construct directly from `new`.

```cpp
#include <memory>
using namespace std;

// Single owner
unique_ptr<int> ptr(new int(42));
cout << *ptr << endl;      // 42

// Automatic deletion
ptr = nullptr;             // Deleted automatically

// No copy
// unique_ptr<int> p2 = ptr;  // ERROR

// Move
unique_ptr<int> p1(new int(10));
unique_ptr<int> p2 = move(p1);  // p1 = nullptr
if (!p1) {
  cout << "p1 no longer owns the object" << endl;
}

// Array specialization
unique_ptr<int[]> arr(new int[10]);
arr[0] = 5;
// Automatically uses delete[]

// In C++11, construct unique_ptr directly
unique_ptr<int> ptr2(new int(42));
```

> **Note — `make_unique` is C++14, not C++11.** `std::make_shared` exists in
> C++11, but `std::make_unique` was added in **C++14**. In strict C++11 you must
> write `unique_ptr<T>(new T(...))` as shown above. If you want the factory
> idiom under C++11, you can define it yourself in one line using perfect
> forwarding (see [Chapter 9.14](../09_templates/README.md)):
> ```cpp
> template<typename T, typename... Args>
> std::unique_ptr<T> make_unique(Args&&... args) {
>   return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
> }
> ```

### Shared Pointer

A `shared_ptr` models **shared ownership** through a reference count: each copy increments the count, each destruction decrements it, and the managed object is deleted only when the count reaches zero. Use it when an object's lifetime genuinely depends on several independent owners and you cannot determine in advance which will finish last. Prefer `make_shared<T>(...)` to construct one — it allocates the object and its control block in a single allocation, which is faster and more cache-friendly than separate `new`. Be aware of the cost (atomic reference-count updates) and the danger of cycles: two `shared_ptr`s that point at each other will never reach zero and leak, which is what `weak_ptr` solves.

```cpp
#include <memory>
using namespace std;

// Multiple owners (reference counting)
shared_ptr<int> ptr1(new int(42));
shared_ptr<int> ptr2 = ptr1;  // Reference count = 2

cout << ptr1.use_count() << endl;  // 2

ptr1 = nullptr;
cout << ptr2.use_count() << endl;  // 1 (can still use)

ptr2 = nullptr;  // Memory deleted (count = 0)

// make_shared (efficient)
auto ptr3 = make_shared<int>(42);
auto ptr4 = ptr3;
```

### Weak Pointer

A `weak_ptr` is a **non-owning** reference to an object managed by a `shared_ptr`: it does not contribute to the reference count, so it never keeps the object alive. Its primary use is to break circular references — for instance a parent/child or doubly-linked structure where one direction owns (`shared_ptr`) and the back-reference observes (`weak_ptr`), preventing a leak-causing cycle. Because the object may already have been destroyed, you cannot dereference a `weak_ptr` directly; you must call `.lock()`, which returns a `shared_ptr` that is either valid (object still alive, count temporarily bumped) or empty (object gone). This makes `weak_ptr` also useful for safe caches and observer patterns where you want to access an object only if it still exists.

```cpp
// Prevent circular references
class Node {
public:
  shared_ptr<Node> next;
  weak_ptr<Node> prev;  // Doesn't increase reference count
};

// Without weak_ptr:
// Node* a = new Node();
// Node* b = new Node();
// a->next = shared_ptr<Node>(b);
// b->next = shared_ptr<Node>(a);  // CYCLE! Memory leak!

// With weak_ptr:
auto a = make_shared<Node>();
auto b = make_shared<Node>();
a->next = b;
b->prev = a;  // Doesn't prevent deletion
```

---

## 12.5 RAII (Resource Acquisition Is Initialization)

**RAII** is the central resource-management idiom of C++: acquire a resource (memory, a file handle, a lock, a socket) in a constructor and release it in the destructor, so that ownership is tied to an object's lifetime. Because the destructor runs automatically when the object leaves scope — including when an exception unwinds the stack — cleanup is guaranteed without any explicit `try`/`finally` and cannot be forgotten. RAII types typically delete or carefully define their copy operations (the Rule of Five, see 12.8) so two objects never claim ownership of the same resource. Smart pointers, `std::vector`, `std::lock_guard`, and `std::fstream` are all RAII wrappers; the example below shows the pattern applied to a C `FILE*`.

```cpp
// RAII principle: resource tied to object lifetime

class File {
private:
  FILE* fp;

public:
  // Constructor: acquire resource
  File(const char* name) {
    fp = fopen(name, "r");
    if (!fp) throw runtime_error("Cannot open file");
  }
  
  // Destructor: release resource
  ~File() {
    if (fp) fclose(fp);
  }
  
  // Prevent copying
  File(const File&) = delete;
  File& operator=(const File&) = delete;
};

// Usage
void processFile() {
  File f("data.txt");  // Resource acquired
  // Use file...
}  // Resource released automatically
```

---

## 12.6 Move Semantics (C++11)

**Move semantics**, introduced in C++11, let you transfer ownership of a resource from one object to another instead of making an expensive deep copy. When the source is a temporary (an rvalue) or is explicitly cast with `std::move`, the compiler can invoke a **move constructor** or **move assignment operator** that "steals" the source's internal pointer and leaves the source in a valid but empty state — turning an O(n) copy into an O(1) pointer swap. This is why returning large objects by value and storing them in containers became cheap in C++11. Two rules matter: always leave the moved-from object in a safe, destructible state (typically null its pointers), and mark move operations `noexcept` so standard containers like `vector` will actually use them during reallocation.

```cpp
class String {
private:
  char* data;
  int length;

public:
  // Copy constructor
  String(const String& other) {
    length = other.length;
    data = new char[length + 1];
    strcpy(data, other.data);
  }
  
  // Move constructor
  String(String&& other) noexcept {
    data = other.data;      // Steal data
    length = other.length;
    other.data = nullptr;   // Leave source empty
    other.length = 0;
  }
  
  // Assignment
  String& operator=(const String& other) {
    if (this != &other) {
      delete[] data;
      length = other.length;
      data = new char[length + 1];
      strcpy(data, other.data);
    }
    return *this;
  }
  
  // Move assignment
  String& operator=(String&& other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      length = other.length;
      other.data = nullptr;
      other.length = 0;
    }
    return *this;
  }
  
  ~String() { delete[] data; }
};

int main() {
  String s1("hello");
  String s2 = s1;           // Copy
  String s3 = move(s1);     // Move (s1 becomes empty)
  
  return 0;
}
```

---

## 12.7 Best Practices

This section gathers the practical guidelines that follow from the concepts above into a quick checklist. The recurring themes are: prefer automatic lifetime management (stack objects, RAII, and smart pointers) over manual `new`/`delete`; never return or store a pointer or reference to a local that has gone out of scope (a dangling reference); pass large objects by `const` reference to avoid needless copies; and observe the Rule of Five — if you define a destructor, copy/move operations, you almost certainly need to address all of them.

```cpp
// ✅ Use smart pointers (not raw new/delete)
unique_ptr<Object> ptr(new Object(args));

// ✅ RAII for resource management
class Resource {
public:
  Resource() { acquire(); }
  ~Resource() { release(); }
private:
  void acquire() { }
  void release() { }
};

// ✅ Stack allocation when possible
Object obj;  // Stack
vector<int> v;  // Dynamic but managed

// ✅ Avoid dangling pointers
int* getDanglingPtr() {
  int local = 10;
  return &local;  // ERROR!
}

// ✅ Use const references for efficiency
void process(const vector<int>& v) { }

// ✅ Rule of five (if you define dtor)
class MyClass {
public:
  ~MyClass();                          // Destructor
  MyClass(const MyClass&);             // Copy constructor
  MyClass& operator=(const MyClass&);  // Copy assignment
  MyClass(MyClass&&) noexcept;         // Move constructor
  MyClass& operator=(MyClass&&) noexcept;  // Move assignment
};
```

---

## 12.8 Rule of Five (C++11)

If you define **any one** of the five special member functions, you should explicitly define (or `= delete`) **all five**. The compiler's generated defaults break down once you manage raw resources.

The Five: **destructor, copy constructor, copy assignment, move constructor, move assignment**.

```cpp
#include <algorithm>
#include <iostream>
using namespace std;

class Buffer {
  int*   data;
  size_t size;
public:
  // Regular constructor
  explicit Buffer(size_t n) : data(new int[n]()), size(n) {}

  // 1. Destructor
  ~Buffer() {
    delete[] data;
  }

  // 2. Copy constructor — deep copy (do NOT steal; source stays valid)
  Buffer(const Buffer& other)
      : data(new int[other.size]), size(other.size) {
    copy(other.data, other.data + other.size, data);
  }

  // 3. Copy assignment — copy-and-swap idiom
  Buffer& operator=(Buffer other) {    // pass by value: copy already made
    swap(data, other.data);
    swap(size, other.size);
    return *this;                      // old data freed when other is destroyed
  }

  // 4. Move constructor — steal resources, leave source valid-but-empty
  Buffer(Buffer&& other) noexcept
      : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
  }

  // 5. Move assignment — same steal pattern
  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      size = other.size;
      other.data = nullptr;
      other.size = 0;
    }
    return *this;
  }

  int     operator[](size_t i) const { return data[i]; }
  int&    operator[](size_t i)       { return data[i]; }
  size_t  getSize()            const { return size; }
};

Buffer a(5);              // regular constructor
a[0] = 42;
Buffer b = a;             // copy constructor — deep copy, a unchanged
Buffer c = move(a);       // move constructor — c owns data, a is empty
b = Buffer(3);            // move assignment — temporary moved into b
```

| Special Member | Compiler generates when | Write manually when |
|---|---|---|
| Destructor | Always | You own raw resources |
| Copy constructor | No user-defined destructor or move | Deep copy needed |
| Copy assignment | No user-defined destructor or move | Deep copy needed |
| Move constructor | No user-defined copy, destructor | Steal resources |
| Move assignment | No user-defined copy, destructor | Steal resources |

**Rule of thumb:**
- Own raw pointers → write all five explicitly
- Use smart pointers / STL members → compiler-generated defaults are correct, use `= default`

---

## Summary

| Topic | Key Points |
|-------|-----------|
| **Stack** | Automatic, fast, limited |
| **Heap** | Manual, larger, slower |
| **Smart Pointers** | Automatic cleanup |
| **unique_ptr** | Single owner |
| **shared_ptr** | Multiple owners |
| **RAII** | Resource tied to lifetime |
| **Move** | Efficient transfer of ownership |
| **Rule of Five** | Define all five special members if you define any one |

---

## 12.9 Copy Elision and Return Value Optimization (RVO)

**Copy elision** is a compiler optimization — permitted by the C++ standard — where the compiler constructs an object directly in its destination instead of constructing-then-copying/moving. **RVO (Return Value Optimization)** is the most important form: eliding the copy when returning a local variable. In C++11 the compiler is allowed (not required) to apply RVO; when it does, not even the move constructor is called.

```cpp
#include <iostream>
using namespace std;

struct Widget {
  int id;
  explicit Widget(int n) : id(n)  { cout << "Construct("  << n << ")\n"; }
  Widget(const Widget& o)         { cout << "Copy("        << o.id << ")\n"; id = o.id; }
  Widget(Widget&& o)    noexcept  { cout << "Move("        << o.id << ")\n"; id = o.id; }
  ~Widget()                       { cout << "Destroy("     << id   << ")\n"; }
};

// --- RVO: Named Return Value Optimization ---
// The compiler can construct 'w' directly into the return slot of the caller
Widget makeWidget() {
  Widget w(42);   // constructed IN the caller's stack space — no copy at return
  return w;       // typically: no copy, no move — just "Construct(42)" is printed
}

Widget a = makeWidget();
// With RVO (typical output): Construct(42) Destroy(42)
// Without RVO (worst case):  Construct(42) Move(42) Destroy(42) Destroy(42)

// --- NRVO: Named RVO — same idea, named local variable ---
Widget makeWidgetNRVO(int n) {
  Widget result(n);
  // ... work with result ...
  return result;    // compiler typically elides the copy/move
}

// --- When elision does NOT apply ---
Widget makeConditional(bool b) {
  Widget w1(1);
  Widget w2(2);
  return b ? w1 : w2;  // can't elide: two candidates for return
}
// This will move-construct the returned Widget

// --- copy elision with temporaries (prvalue elision) ---
// Temporary passed directly to function: no copy at all
void consume(Widget w) { (void)w; }
consume(Widget(10));   // Widget(10) is constructed directly in consume's frame
                       // Output: Construct(10) Destroy(10) — no copy

// --- std::move can PREVENT RVO ---
Widget bad() {
  Widget w(99);
  return move(w);   // ❌ disables RVO: forces a move instead of elision
}
// ✅ Just return w; — let the compiler apply NRVO

// Guideline: Never std::move a return value of local variables
Widget good() {
  Widget w(99);
  return w;         // ✅ Compiler applies NRVO — zero overhead
}
```

| Optimization | What is elided | When it applies |
|---|---|---|
| **RVO** | Copy/move on unnamed temporary return | `return SomeType(args);` |
| **NRVO** | Copy/move on named local return | `T x; ...; return x;` (one return path) |
| **Prvalue elision** | Copy/move of temporaries | `T t = T(args);`, function arguments |

**Key rules:**
- Do not `return std::move(localVar)` — this disables NRVO and forces a move
- Do not `return std::move(*this)` unless you intend rvalue-ref-qualified member functions
- Trust the compiler: modern compilers apply RVO consistently even in debug builds

---

## 12.10 `alignas` and `alignof` (C++11)

Alignment controls where in memory an object may be placed. Misaligned access
can crash (some architectures), or silently slow down code (x86). C++11 provides
two operators for explicit alignment control.

```cpp
#include <iostream>

// alignof: query alignment requirement of a type
std::cout << alignof(char)   << "\n";  // typically 1
std::cout << alignof(int)    << "\n";  // typically 4
std::cout << alignof(double) << "\n";  // typically 8

// alignas: request a specific alignment for a variable or type
alignas(16) char buffer[64];   // buffer starts on a 16-byte boundary (e.g. for SIMD)
alignas(8)  int  x;            // x aligned to 8 bytes (stricter than default 4)

struct Normal  { char c; int i; };           // sizeof = 8 (3 bytes padding)
struct alignas(16) Aligned { char c; int i; }; // sizeof = 16 (padded to 16)

std::cout << alignof(Aligned) << "\n";  // 16
std::cout << sizeof(Aligned)  << "\n";  // 16
```

**`std::aligned_storage` (C++11) — pre-allocated aligned raw storage:**

```cpp
#include <type_traits>
// Storage large enough and aligned for type T, without constructing T
std::aligned_storage<sizeof(double), alignof(double)>::type raw;
double* p = new (&raw) double(3.14);  // placement new into aligned storage
p->~double();                          // explicit destructor call
```

**Rules:**
- `alignas(N)` requires N to be a power of 2
- You cannot reduce alignment below a type's natural requirement (`alignas(1)` on `double` is ill-formed)
- Used for SIMD data, lock-free data structures, and custom allocators

---

## Next Steps
- Use smart pointers by default
- Understand move semantics
- Apply RAII principle
- Move to [Chapter 13: Exception Handling](../13_exception_handling/README.md)
