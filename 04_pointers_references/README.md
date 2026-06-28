# Chapter 4: Pointers & References

Pointers and references are the two ways C++ lets you refer to an object indirectly rather than copying it. A pointer is a variable that holds the memory address of another object; a reference is an alias — another name for an existing object. Both let functions operate on a caller's data, build linked data structures, and manage memory allocated on the heap. Understanding them is essential because C++ exposes the machine's memory model directly, giving you precise control at the cost of having to manage that memory correctly.

This chapter covers raw pointers and their arithmetic, the close relationship between pointers and arrays, dynamic allocation with `new`/`delete`, and the C++11 smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`) that automate cleanup. It then contrasts pointers with references, surveys special cases like void and function pointers, and ends with C++11 additions such as `nullptr` and rvalue references that underpin move semantics.

## 4.1 Pointer Basics

A pointer stores the address of another object rather than a value directly. Before working with pointers it helps to understand that every variable lives at some address in memory and occupies a number of bytes determined by its type. The address-of operator `&` yields a variable's address, and `sizeof` reports how many bytes it occupies.

### Understanding Memory
```cpp
#include <iostream>
using namespace std;

int main() {
  int x = 10;
  
  // Address-of operator &
  cout << "Value: " << x << endl;           // 10
  cout << "Address: " << &x << endl;        // 0x7ffee76b4a4c (varies)
  cout << "Size: " << sizeof(x) << endl;    // 4 (bytes)
  
  return 0;
}
```

### Declaring Pointers

A pointer is declared by placing `*` between the pointed-to type and the pointer's name, as in `int* ptr`. The type matters: an `int*` and a `double*` are distinct types because the compiler needs the element size for dereferencing and arithmetic. A common pitfall is `int* ptr1, ptr2;` — only `ptr1` is a pointer, while `ptr2` is a plain `int`, because the `*` binds to the declarator, not the type. Write `int *ptr1, *ptr2;` (or declare one per line) to avoid the surprise.

```cpp
// Pointer declaration: type *pointerName;

int* ptr;              // Pointer to int
double* dptr;          // Pointer to double
char* cptr;            // Pointer to char
int** pptr;            // Pointer to pointer

// Initialize with address
int x = 5;
int* ptr = &x;         // ptr points to x

// Multiple pointers
int* ptr1, ptr2;       // ptr1 is pointer, ptr2 is int!
int *ptr3, *ptr4;      // Both are pointers
```

### Dereferencing

Dereferencing — applying the `*` operator to a pointer — gives you access to the object the pointer points at, so you can read or modify it. Writing through a dereferenced pointer changes the original object, which is exactly how functions modify their caller's data. The cardinal rule is that you may only dereference a pointer that actually points to a valid, live object; dereferencing a null, uninitialized, or dangling pointer is undefined behavior and a frequent source of crashes.

```cpp
int x = 42;
int* ptr = &x;

// Dereference operator *
cout << *ptr << endl;  // 42 (get value)
cout << ptr << endl;   // 0x... (address)

// Modify through pointer
*ptr = 100;
cout << x << endl;     // 100 (x is modified!)

// Pointer arithmetic
int arr[] = {10, 20, 30};
int* p = arr;
cout << *p << endl;        // 10
cout << *(p + 1) << endl;  // 20
cout << *(p + 2) << endl;  // 30
```

---

## 4.2 Pointer Arithmetic

Pointer arithmetic is scaled by the size of the pointed-to type: adding `1` to an `int*` advances the address by `sizeof(int)` bytes (typically 4), not by one byte, so `ptr + n` lands on the n-th element. This is what makes pointers natural for walking through arrays. Subtracting two pointers into the same array yields the number of elements between them. Arithmetic is only well defined within a single array (or one past its end); computing or dereferencing a pointer outside those bounds is undefined behavior.

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* ptr = arr;

// Pointer increments by type size (int = 4 bytes)
cout << ptr << endl;         // 0x...
cout << ptr + 1 << endl;     // 0x... + 4
cout << ptr + 2 << endl;     // 0x... + 8

// Pointer arithmetic
ptr++;                       // Points to next element
ptr--;                       // Points to previous element
ptr += 3;                    // Points 3 elements ahead

// Pointer difference
int* p1 = &arr[0];
int* p2 = &arr[4];
cout << p2 - p1 << endl;     // 4 (elements apart)

// Traversing array
for (int* p = arr; p < arr + 5; p++) {
  cout << *p << " ";        // 10 20 30 40 50
}
```

---

## 4.3 Pointers & Arrays

Arrays and pointers are closely related: in most expressions an array name "decays" into a pointer to its first element, which is why `arr`, `&arr[0]`, and a pointer initialized from `arr` behave alike. As a result `arr[i]` is defined as `*(arr + i)`, and because addition commutes, the curious `i[arr]` is also valid (though you should never write it). Keep in mind the differences too: an array knows its full size for `sizeof`, while a decayed pointer does not, and a pointer-to-array type like `int (*)[5]` is distinct from a plain `int*`.

```cpp
// Array and pointer relationship

int arr[5] = {10, 20, 30, 40, 50};
int* ptr = arr;              // arr decays to pointer

// These are equivalent:
cout << arr[2] << endl;      // 30
cout << *(arr + 2) << endl;  // 30
cout << *(ptr + 2) << endl;  // 30
cout << 2[arr] << endl;      // 30 (unusual but valid!)

// Array of pointers
int a = 10, b = 20, c = 30;
int* ptrs[] = {&a, &b, &c};

for (int i = 0; i < 3; i++) {
  cout << *ptrs[i] << " ";   // 10 20 30
}

// Pointer to array
int (*ptr2arr)[5] = &arr;    // Pointer to array of 5 ints
cout << (*ptr2arr)[0] << endl;  // 10
```

---

## 4.4 Dynamic Memory

Dynamic memory lets you allocate objects whose size or lifetime is not known at compile time, drawing from a region called the heap (or free store) rather than the automatically managed stack. You request memory explicitly and, with raw pointers, are responsible for releasing it just as explicitly. This section introduces the `new`/`delete` operators and the leaks and dangling pointers that arise when ownership is mismanaged — problems the C++11 smart pointers in 4.5 are designed to eliminate.

### New & Delete

`new` allocates an object on the heap and returns a pointer to it; `delete` frees that memory and runs the object's destructor. The pairing must match exactly: memory from `new` is released with `delete`, and an array from `new[]` must be released with `delete[]` — mixing them is undefined behavior. Every `new` needs exactly one corresponding `delete`; forgetting it leaks memory, while deleting twice or using a pointer after deletion corrupts the heap. Setting a pointer to `nullptr` after deleting it is a cheap habit that prevents accidental reuse of a dangling pointer.

```cpp
// Stack allocation (fixed size, automatic cleanup)
int x = 5;
int arr[10];

// Heap allocation (dynamic size, manual cleanup)
int* ptr = new int;          // Allocate 1 int
*ptr = 42;
delete ptr;                  // Must delete!
ptr = nullptr;               // Avoid dangling pointer

// Allocate array
int* arr = new int[10];      // Allocate array
arr[0] = 5;
delete[] arr;                // Delete array with []
arr = nullptr;

// Objects
class Person {
public:
  Person(string name) : name(name) {}
private:
  string name;
};

Person* p = new Person("Alice");
delete p;
p = nullptr;
```

### Memory Leaks

A memory leak occurs when heap memory is allocated but never freed, so the program holds onto memory it can no longer reach or use; over time this can exhaust available memory. The most common causes are simply forgetting to call `delete`, overwriting the only pointer to an allocation, and — subtly — an exception thrown between `new` and `delete` that skips the cleanup. Manual `delete` is hard to get right in the presence of exceptions and multiple return paths, which is the central motivation for the RAII-based smart pointers covered next.

```cpp
// ❌ Memory leak: forget to delete
void leak() {
  int* ptr = new int(10);
  // Function exits, ptr local variable destroyed
  // But allocated memory NOT freed!
}

// ✅ Always delete
void noLeak() {
  int* ptr = new int(10);
  delete ptr;
  ptr = nullptr;
}

// ❌ Exception causes leak
void exceptionLeak() {
  int* ptr = new int(10);
  // Some code
  if (error) throw runtime_error("fail");
  // ptr never deleted!
  delete ptr;
}

// ✅ Smart pointers prevent leaks (next section)
```

---

## 4.5 Smart Pointers (C++11)

Smart pointers are class templates in `<memory>` that own a heap allocation and automatically `delete` it when they go out of scope, applying the RAII idiom so cleanup happens even when exceptions unwind the stack. They replace error-prone manual `new`/`delete` and make ownership explicit in the type system: `unique_ptr` for sole ownership and `shared_ptr` for shared ownership via reference counting, with `weak_ptr` to break ownership cycles. Reaching for smart pointers by default is the modern C++ approach to dynamic memory.

### Unique Pointer

`unique_ptr` models exclusive ownership: exactly one `unique_ptr` owns the object, and it deletes the object automatically when destroyed or reset. Because two unique pointers cannot both own the same object, it is move-only — you cannot copy it, but you can transfer ownership with `std::move`, after which the source becomes null. It has essentially zero overhead compared to a raw pointer, so prefer it whenever a single owner suffices. Note the array form `unique_ptr<T[]>`, which correctly uses `delete[]`.

```cpp
#include <memory>
using namespace std;

// Only one owner
unique_ptr<int> ptr(new int(42));

cout << *ptr << endl;      // 42
ptr = nullptr;             // Automatically deletes

// With arrays
unique_ptr<int[]> arr(new int[10]);
arr[0] = 5;

// Move semantics
unique_ptr<int> p1(new int(10));
unique_ptr<int> p2 = move(p1);  // p1 becomes null
// p1 is now nullptr, p2 owns the memory

// Cannot copy
// unique_ptr<int> p3 = p2;  // ERROR
```

### Shared Pointer

`shared_ptr` models shared ownership: it keeps a reference count of how many shared pointers point to the same object, and the object is deleted only when the last one is destroyed or reset. Use it when ownership genuinely must be shared across several parts of a program with no single clear owner. Its main hazard is the reference cycle — two objects that hold `shared_ptr`s to each other will never reach a count of zero and so leak; the fix is to make one of the links a `weak_ptr`, which observes the object without contributing to its count. `shared_ptr` also carries more overhead than `unique_ptr` because of the atomic reference count, so prefer unique ownership when you can.

```cpp
#include <memory>

// Multiple owners, reference counting
shared_ptr<int> ptr1(new int(42));
shared_ptr<int> ptr2 = ptr1;

cout << ptr1.use_count() << endl;  // 2 (two owners)

ptr1 = nullptr;
cout << ptr2.use_count() << endl;  // 1

ptr2 = nullptr;                    // Memory freed (no owners)

// Cycle prevention with weak_ptr
class Node {
public:
  shared_ptr<Node> next;
  weak_ptr<Node> prev;  // Avoid circular reference
};
```

### Smart Pointer Construction in C++11

In C++11 you construct a `unique_ptr` by passing it a raw `new` expression directly, while a `shared_ptr` is best created with the helper `std::make_shared`, which allocates the object and its control block in a single allocation for better efficiency and exception safety. (`std::make_unique` does not exist in C++11 — it was added in C++14 — so for `unique_ptr` you use the `new` form shown here.) Combining `make_shared` with `auto` keeps the code concise and avoids repeating the type.

```cpp
// Direct construction in C++11
unique_ptr<int> ptr1(new int(42));
auto ptr2 = make_shared<string>("hello");

// Equivalent to:
// unique_ptr<int> ptr1(new int(42));
// shared_ptr<string> ptr2(new string("hello"));
```

---

## 4.6 References

A reference is an alias — an alternative name for an existing object — and once bound it behaves as the object itself. References are the preferred way to let a function operate on a caller's variable or to pass large objects without copying, and they are safer than pointers because they cannot be null and need no dereferencing syntax. This section covers plain references, const references (which can also bind to temporaries), and how references are used as function parameters.

### Reference Basics

A reference must be initialized when it is declared and is permanently bound to that object for its lifetime — there is no way to "reseat" it to refer to a different object afterward. A frequent point of confusion is that assigning to a reference (`ref = y`) does not rebind it; it writes `y`'s value into the original object the reference already names. Because a reference has no independent existence, taking its address gives the address of the referred-to object.

```cpp
// Reference: alias to existing variable
int x = 10;
int& ref = x;           // ref is reference to x

ref = 20;
cout << x << endl;      // 20 (x changed)

cout << &ref << endl;   // Same address as &x

// ❌ Reference must be initialized
// int& ref2;           // ERROR

// ❌ Reference cannot be reassigned
int y = 30;
ref = y;                // Sets ref value to 30, not to y!
cout << &ref << endl;   // Still points to x
```

### Const References

A const reference (`const T&`) provides read-only access to an object: you can observe it but not modify it through the reference. This is the standard way to pass large objects to functions efficiently — you avoid the copy of pass-by-value while promising not to change the argument. A special property is that a const reference may bind to a temporary (an rvalue) and extends that temporary's lifetime to match the reference, whereas a non-const reference cannot bind to a temporary at all.

```cpp
int x = 10;
const int& cref = x;    // Const reference

cout << *cref << endl;  // ERROR: must dereference pointer
cout << cref << endl;   // 10

// cref = 20;           // ERROR: cannot modify

// Temporary can bind to const ref
const int& ref2 = 42;   // OK with const
// int& ref3 = 42;      // ERROR with non-const
```

### References in Functions

Passing parameters by reference lets a function read and modify the caller's actual variable rather than a copy, which is essential for output parameters and for swapping or mutating values in place. For inputs you do not intend to change, pass by `const` reference: it avoids an expensive copy while documenting and enforcing that the argument is left untouched. The example also shows the related const-pointer forms, where `const` can qualify what is pointed to, the pointer itself, or both.

```cpp
// Pass by reference
void increment(int& x) {
  x++;
}

int main() {
  int num = 5;
  increment(num);
  cout << num << endl;   // 6
  return 0;
}

// Const reference (efficient, prevents modification)
void printLargeVector(const vector<int>& v) {
  for (int val : v) {
    cout << val << " ";
  }
}

// Reference to const
const int* ptr = &x;    // Pointer to const int
int* const ptr2 = &x;   // Const pointer to int
const int* const ptr3 = &x;  // Const pointer to const int
```

---

## 4.7 Pointers vs References

Pointers and references both provide indirection, but they make different trade-offs. A pointer is a distinct object that can be null, reassigned to point elsewhere, and used in arithmetic, which makes it flexible but requires explicit null checks and dereferencing. A reference is an alias that must be bound at initialization, can never be null or reseated, and reads like the object itself, which makes it safer and clearer but less flexible. The rule of thumb: use a reference when you always have a valid object to refer to, and a pointer when the target may be absent (null), needs to be rebound, or must support arithmetic over an array.

```cpp
// Pointer characteristics
int x = 10;
int* ptr = &x;

ptr = nullptr;          // Can be null
ptr = nullptr;          // Can be reassigned
(*ptr)++;               // Must dereference
ptr = new int;          // Can point to dynamic memory
delete ptr;             // Manual cleanup needed

// Reference characteristics
int& ref = x;

// ref = nullptr;       // ERROR: cannot be null
// ref = y;             // ERROR: cannot be reassigned
ref++;                  // No dereferencing needed
// Cannot point to dynamic memory alone
// Automatic cleanup (goes out of scope)

// Comparison table
/*
                 Pointer           Reference
Null            Can be            Cannot be
Reassign        Yes               No
Dereference     Needed            Not needed
Arithmetic      Supported         Not supported
Dynamic memory  Yes               No (use ptr)
Efficiency      Same after        Slightly better (no null check)
Safety          Less              More
Flexibility     More              Less
*/
```

---

## 4.8 Void Pointers

A `void*` is a generic pointer that holds an address without recording the type of the object stored there. Because the type and therefore the size are unknown, you cannot dereference a `void*` or do arithmetic on it; you must first cast it back to the correct concrete pointer type. Void pointers appear mostly in low-level C-style interfaces (such as `malloc` or callback APIs); in modern C++ they are rarely needed and templates usually provide a type-safe alternative. The danger is that the cast is unchecked — casting to the wrong type and dereferencing is undefined behavior.

```cpp
// Generic pointer, doesn't know type

int x = 10;
double y = 3.14;

void* ptr = &x;         // Point to int
ptr = &y;               // Point to double
// ptr = ptr + 1;       // ERROR: don't know size

// Must cast before use
int* iptr = (int*)&x;
cout << *iptr << endl;  // 10

double* dptr = (double*)&y;
cout << *dptr << endl;  // 3.14
```

---

## 4.9 Function Pointers

A function pointer stores the address of a function, letting you call it indirectly and pass behavior around as data — the basis of callbacks, plugin tables, and simple strategy selection. Its type encodes the function's return type and parameter list, with the slightly awkward syntax `returnType (*name)(paramTypes)`; the parentheses around `*name` are required to distinguish it from a function returning a pointer. The pointer can only be assigned functions with a matching signature, and you can build arrays of them to dispatch on an index. In modern C++, `std::function` and lambdas often provide a more flexible alternative.

```cpp
// Pointer to function: returnType (*name)(parameters)

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int main() {
  int (*operation)(int, int) = add;
  
  cout << operation(5, 3) << endl;  // 8
  
  operation = multiply;
  cout << operation(5, 3) << endl;  // 15
  
  return 0;
}

// Array of function pointers
int (*ops[])(int, int) = {add, multiply};
cout << ops[0](5, 3) << endl;  // 8
cout << ops[1](5, 3) << endl;  // 15
```

---

## 4.10 Nullptr (C++11)

`nullptr` is a C++11 keyword representing a null pointer of a distinct type (`std::nullptr_t`) that converts to any pointer type but not to an integer. It replaces the older `NULL` macro and the literal `0`, both of which are really integers and can cause ambiguous overload resolution — for example, calling `f(int)` instead of `f(char*)`. Using `nullptr` makes the intent unambiguous, works correctly with templates and function pointers, and is the recommended way to denote and test for null in modern C++.

```cpp
// C++11 introduced nullptr for null pointers

// Old way (bad)
int* ptr = NULL;       // NULL is 0
int* ptr2 = 0;         // Works but confusing

// New way (good)
int* ptr3 = nullptr;   // Clear intent, type-safe

// nullptr can bind to function pointers
void (*fp)() = nullptr;  // OK
// NULL might not work with templates

// nullptr in conditions
if (ptr == nullptr) {  // Clear
  // Handle null
}

if (ptr != nullptr) {  // Clear
  // Use pointer
}
```

---

## 4.11 Rvalue References and `std::move` (C++11)

A normal reference (`int&`) is now called an **lvalue reference** — it binds to a named object you can take the address of. C++11 adds the **rvalue reference** (`int&&`), which binds to **temporaries** (values about to expire). This distinction is what makes move semantics possible.

```cpp
#include <iostream>
#include <utility>   // std::move
#include <string>
using namespace std;

int  x = 10;
int& lref = x;        // lvalue reference — binds to a named object
// int& bad = 10;     // ERROR: can't bind lvalue ref to a temporary

int&& rref = 10;      // rvalue reference — binds to the temporary 10
int&& rref2 = x + 5;  // OK: x+5 is a temporary (an rvalue)
// int&& bad2 = x;    // ERROR: x is an lvalue, not a temporary
```

### lvalues vs rvalues

| Category | Has a name / address? | Examples |
|---|---|---|
| **lvalue** | Yes | `x`, `arr[2]`, `*ptr`, a returned reference |
| **rvalue** | No (temporary) | `10`, `x + 5`, `string("hi")`, a function returning by value |

### Overloading on value category

A function can detect whether its argument is a temporary by overloading on `&` vs `&&`:

```cpp
void handle(const string& s) { cout << "lvalue: copy " << s << "\n"; }
void handle(string&& s)      { cout << "rvalue: can steal " << s << "\n"; }

string name = "Alice";
handle(name);                 // lvalue: copy Alice   — name is reused later
handle(string("Bob"));        // rvalue: can steal Bob — temporary, safe to gut
handle(name + "!");           // rvalue: can steal Alice!
```

### `std::move` — turn an lvalue into an rvalue

`std::move` does **not** move anything by itself. It is just a cast: it tells the compiler "treat this lvalue as a temporary, you may steal from it." The actual stealing happens in a move constructor / move assignment.

```cpp
string a = "hello";
string b = std::move(a);   // selects string's MOVE constructor: b takes a's buffer
// 'a' is now in a valid but unspecified state — assign to it before reusing

cout << b << "\n";         // hello
a = "fresh";               // OK: assigning a new value is always safe
```

**Caution:** after `std::move(a)`, do not assume anything about `a`'s contents — only that it is destructible and assignable. Never `std::move` a variable you still need.

| Tool | Meaning |
|---|---|
| `T&` | Lvalue reference — binds to named objects |
| `T&&` | Rvalue reference — binds to temporaries (enables moving) |
| `std::move(x)` | Cast `x` to an rvalue so move operations can steal from it |

Move **constructors** and **move assignment** (which consume these rvalue references) are covered in [Chapter 12.6](../12_memory_management/README.md) and [Chapter 8.11](../08_operator_overloading/README.md); generic forwarding of value categories is in [Chapter 9.14](../09_templates/README.md).

---

## 4.12 Best Practices

The guiding principle for indirection in modern C++ is to make ownership clear and to avoid manual memory management wherever possible. Prefer smart pointers over raw `new`/`delete`, pass non-owning read-only arguments by `const` reference, and reach for references over pointers when the target is always present. Always guard a dereference with a null check when a pointer might be null, avoid returning the address of a local variable (which dangles the moment the function returns), and prefer indexing or range-based loops over hand-written pointer arithmetic for clarity.

```cpp
// ✅ Use smart pointers by default
unique_ptr<int> ptr(new int(10));  // Automatic cleanup

// ✅ Use const references for efficiency and safety
void process(const vector<int>& v) { }

// ✅ Avoid raw pointers for ownership
// ❌ int* ptr = new int(10);  // Who deletes?
// ✅ unique_ptr<int> ptr(new int(10));  // Clear ownership in C++11

// ✅ Check for null before dereferencing
if (ptr != nullptr) {
  cout << *ptr;
}

// ✅ Avoid pointer arithmetic
// ❌ ptr += (ptr - start) * 2;  // Confusing
// ✅ arr[index]  // Clearer

// ✅ Use references when appropriate
void swap(int& a, int& b) {  // Clearer than pointer
  int temp = a;
  a = b;
  b = temp;
}

// ❌ Dangling pointer
int* getDanglingPtr() {
  int local = 10;
  return &local;  // ERROR: local destroyed!
}

// ✅ Return by value or pointer to heap
int getValue() {
  return 10;
}

unique_ptr<int> getPointer() {
  return unique_ptr<int>(new int(10));
}
```

---

## Summary

| Topic | Key Points |
|-------|-----------|
| **Pointers** | Store addresses, dereference with * |
| **References** | Aliases, cannot be null, no reassignment |
| **Arithmetic** | pointer + n advances by n*sizeof(type) |
| **Dynamic** | new/delete, smart pointers better |
| **Smart Ptrs** | unique_ptr, shared_ptr (C++11) |
| **Null Safety** | Use nullptr instead of NULL |
| **Best Practice** | Prefer references, then smart pointers |

---

## Next Steps
- Practice pointer manipulation and arithmetic
- Create objects on the heap and manage them
- Use smart pointers instead of raw new/delete
- Move to [Chapter 5: Arrays & Strings](../05_arrays_strings/README.md)
