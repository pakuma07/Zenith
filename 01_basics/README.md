# Chapter 1: Basics & Fundamentals

## 1.1 Introduction to C++

C++ is a general-purpose programming language that extends C with object-oriented and generic programming features. It provides:
- **High Performance**: Direct memory access, minimal overhead
- **Flexibility**: Low-level and high-level abstractions
- **Efficiency**: Compile-time polymorphism via templates
- **Portability**: Code runs on multiple platforms

### Historical Context
- **1983**: Bjarne Stroustrup begins work on C with Classes
- **1985**: First commercial release
- **1998**: C++98 standardized
- **2003**: C++03 (minor update)
- **2011**: C++11 (major update - **our focus**)
- **2014**: C++14 (incremental improvements)
- **2017**: C++17 (significant additions)

---

## 1.2 Hello World & Your First Program

### Basic Structure
```cpp
#include <iostream>  // Include input/output library

using namespace std;  // Use standard namespace

int main() {
  std::cout << "Hello, World!" << std::endl;
  return 0;           // Return 0 indicates success
}
```

### Key Components
1. **`#include`**: Preprocessor directive that includes headers
2. **`using namespace std`**: Avoids writing `std::` prefix repeatedly
3. **`main()`**: Entry point of every C++ program
4. **`cout`**: Standard output stream
5. **`endl`**: Endline and flush output
6. **`return 0`**: Indicates successful execution

### Compilation & Execution
```bash
# Compile
g++ -std=c++11 hello.cpp -o hello

# Run
./hello
# Output: Hello, World!
```

---

## 1.3 Data Types

### Fundamental Data Types

#### Integer Types

Integer types store whole numbers, and each comes in a `signed` (default) and `unsigned` variant; the `unsigned` form cannot hold negatives but doubles the positive range for the same number of bytes. C++ only guarantees minimum sizes and a size ordering (`char` ≤ `short` ≤ `int` ≤ `long` ≤ `long long`), so exact widths are platform-dependent — use `<cstdint>` fixed-width types when you need an exact size. The biggest pitfall is overflow and unsigned wrap-around (e.g. subtracting past zero wraps to a huge positive value), so choose a type whose range comfortably covers your data.

```cpp
// Signed integers
signed char       c = -128;        // 1 byte, range: -128 to 127
short int        s = -32768;       // 2 bytes, range: -32,768 to 32,767
int              i = -2147483648;  // 4 bytes (typically)
long int         l = -9223372036;  // 4 or 8 bytes depending on platform
long long int   ll = -9223372036854775808LL;  // 8 bytes (C++11)

// Unsigned integers (always positive)
unsigned char      uc = 255;
unsigned short     us = 65535;
unsigned int       ui = 4294967295U;
unsigned long      ul = 4294967295UL;
unsigned long long ull = 18446744073709551615ULL;  // C++11
```

#### Floating-Point Types

Floating-point types represent real numbers using a binary mantissa and exponent, trading exactness for a huge dynamic range. `double` is the sensible default (about 15-17 significant digits); use `float` only when memory or bandwidth is tight, and `long double` when you need extra precision. Because most decimal fractions cannot be represented exactly in binary, never compare floating-point values with `==` — compare within a small tolerance (epsilon) instead.

```cpp
float       f = 3.14f;      // 4 bytes, ~6-7 decimal digits precision
double      d = 3.14159;    // 8 bytes, ~15-17 decimal digits
long double ld = 3.1415926535897932L;  // 10+ bytes, extended precision
```

#### Boolean Type

The `bool` type holds exactly one of two values, `true` or `false`, and is the natural result type of comparisons and logical operators. It converts implicitly to `int` (`true` becomes `1`, `false` becomes `0`) and any nonzero numeric or pointer value converts to `true`. Although a `bool` represents a single bit of information, it typically occupies one byte of storage.

```cpp
bool flag = true;   // true or false
bool result = false;

// Can be implicitly converted to int (true = 1, false = 0)
int value = flag;  // value = 1
```

#### Character Type

A `char` stores a single character and is really a small (usually 1-byte) integer holding its character-set code, so it can be used in arithmetic and compared like a number. Plain `char` may be signed or unsigned depending on the platform; use `signed char`/`unsigned char` when you specifically need a tiny integer. For text beyond ASCII, C++11 provides `wchar_t` plus `char16_t` and `char32_t` for fixed-width Unicode code units.

```cpp
char c = 'A';           // Single character
unsigned char uc = 'Z';
wchar_t wc = L'Ω';      // Wide character for Unicode
```

### Type Sizes

The `sizeof` operator reports the number of bytes a type or object occupies, evaluated entirely at compile time. Because the standard fixes only minimum sizes, `sizeof` is the reliable way to discover the actual width on your target platform rather than assuming it. It is invaluable for buffer calculations and for understanding memory layout, but remember that `sizeof` of a pointer gives the pointer's own size, not the size of what it points to.

```cpp
#include <iostream>
using namespace std;

int main() {
  cout << "Size of int: " << sizeof(int) << " bytes" << endl;
  cout << "Size of double: " << sizeof(double) << " bytes" << endl;
  cout << "Size of char: " << sizeof(char) << " bytes" << endl;
  cout << "Size of bool: " << sizeof(bool) << " bytes" << endl;
  
  return 0;
}

/* Output (typical):
Size of int: 4 bytes
Size of double: 8 bytes
Size of char: 1 byte
Size of bool: 1 byte
*/
```

---

## 1.4 Variables & Initialization

### Variable Declaration & Initialization

#### Traditional Method

The classic C-style approach uses `=` (copy initialization) or assignment after declaration. A variable declared without an initializer has an *indeterminate* value if it is a built-in type with automatic (local) storage — reading it before assignment is undefined behavior, the source of many subtle bugs. You can declare several variables of the same type in one statement, but note that an initializer applies only to the variable it directly follows.

```cpp
int age = 25;              // Initialize at declaration
int height;                // Uninitialized (contains garbage)
height = 180;              // Assign value later
int weight, count = 50;    // Multiple declarations
```

#### C++11 Uniform Initialization

C++11 introduced brace `{}` initialization that works consistently across built-in types, arrays, and class types, replacing the patchwork of older syntaxes. Its key safety feature is that it forbids *narrowing* conversions (such as `double` to `int`) at compile time, catching data-loss bugs that copy initialization silently allows. Prefer braces for new code, but be aware that for class types a `{}` list will prefer a constructor taking `std::initializer_list` if one exists.

```cpp
// Braced initialization - prevents implicit narrowing
int x{10};                 // OK
int y{3.14};               // ERROR - implicit narrowing not allowed
int z = {20};              // OK
int array[] = {1, 2, 3};   // Initialize array
```

#### Auto Type Deduction (C++11)

The `auto` keyword tells the compiler to deduce a variable's type from its initializer, which is why an `auto` variable must always be initialized. It removes verbose, error-prone type names — especially for iterators and other complex template types — and guarantees the variable's type exactly matches the initializer. Keep in mind that `auto` drops top-level `const` and references and that a brace-enclosed list deduces to `std::initializer_list`, so spell out the type when those distinctions matter.

```cpp
auto x = 10;              // int
auto y = 3.14;            // double
auto z = "hello";         // const char*
auto arr = {1, 2, 3};     // std::initializer_list<int>

// Very useful with complex types
std::vector<int> v = {1, 2, 3};
auto it = v.begin();      // std::vector<int>::iterator
```

### Scope

#### Local Variables

A local variable is declared inside a function or block and exists only from its declaration to the closing brace of that scope (automatic storage duration). It is created each time control enters the scope and destroyed on exit, so it cannot be accessed from outside. A variable declared in an inner block can *shadow* one of the same name in an outer scope, where the inner name hides the outer until the block ends.

```cpp
int main() {
  int x = 10;             // Local to main()
  {
    int x = 20;           // Local to this block, shadows outer x
    cout << x;            // Prints 20
  }
  cout << x;              // Prints 10
  return 0;
}
```

#### Global Variables

A global variable is declared outside any function and has static storage duration, so it lives for the entire program and is accessible from any function in the file (or other files via `extern`). Globals are zero-initialized before `main` begins, unlike uninitialized locals. Use them sparingly: shared mutable global state makes code hard to reason about, complicates testing, and is unsafe across threads without synchronization.

```cpp
int globalVar = 100;      // Accessible everywhere

int main() {
  cout << globalVar;      // Can access global
  return 0;
}
```

#### Static Variables

A `static` local variable retains its value between calls to the function: it is initialized only once, the first time control reaches its declaration, and persists until the program ends. This gives you function-scoped state without a global, useful for counters, caches, or lazily created singletons. Since C++11 this first-time initialization is guaranteed thread-safe, but subsequent concurrent reads and writes still require your own synchronization.

```cpp
void function() {
  static int count = 0;   // Initialized only once
  count++;
  cout << count;          // Persists between calls
}

int main() {
  function();             // Prints 1
  function();             // Prints 2
  function();             // Prints 3
  return 0;
}
```

---

## 1.5 Constants

### Const Keyword

The `const` qualifier makes a variable read-only after initialization, so any attempt to modify it is a compile-time error. This documents intent, prevents accidental changes, and enables compiler optimizations; "const-correctness" — marking everything that should not change as `const` — is a core C++ habit. With pointers, placement matters: `const char*` is a pointer to constant data, while `char* const` is a constant pointer to mutable data.

```cpp
// Compile-time constant
const int MAX_SIZE = 100;
const double PI = 3.14159;
const char* name = "Alice";  // Pointer to const char

// Cannot be modified
// MAX_SIZE = 200;  // ERROR

// Benefits: compiler optimization, prevents accidental changes
```

### Constexpr (C++11)

`constexpr` is a C++11 keyword that asks the compiler to evaluate a value or function *at compile time*, producing a true constant expression usable for array sizes, template arguments, and other compile-time contexts. It is stronger than `const`: `const` only means "won't change," whereas `constexpr` means "known at compile time." In C++11 a `constexpr` function is restricted to essentially a single `return` statement, a limitation that later standards relaxed.

```cpp
// Guaranteed to be evaluated at compile time
constexpr int MAX = 100;
constexpr double calculate() { return 3.14159; }

// Can be used in constant expressions
int arr[MAX];  // OK
int arr2[calculate()];  // Error - can't convert double to int
```

### Literals

A literal is a fixed value written directly in source code, and its form determines both its type and its value. Integer literals can be expressed in decimal, octal (`0` prefix), or hexadecimal (`0x` prefix), and suffixes like `U`, `L`, or `LL` adjust their type. C++11 added Unicode and UTF-8 string-literal prefixes (`u8`, `u`, `U`) and the `char16_t`/`char32_t` types; escape sequences such as `\n` or `\x41` let you embed special characters inside character and string literals.

```cpp
// Integer literals
int decimal = 42;         // Decimal
int octal = 052;          // Octal (0 prefix)
int hex = 0x2A;           // Hexadecimal (0x prefix)

// Floating-point literals
double x = 3.14;
double y = 1.5e2;         // Scientific notation (150.0)
double z = 42.0;          // Standard decimal floating-point

// String literals
const char* str1 = "hello";
const wchar_t* str2 = L"wide";
const char* str3 = u8"UTF-8";
const char16_t* str4 = u"UTF-16";
const char32_t* str5 = U"UTF-32";

// Character literals
char c1 = 'A';
char c2 = '\n';            // Newline escape sequence
char c3 = '\x41';          // Hexadecimal (65 = 'A')
```

---

## 1.6 Input & Output

### Output with cout

`std::cout` is the standard output stream, and the insertion operator `<<` writes data to it. The operator can be chained to print several values in one statement and is overloaded for all built-in types, so it formats numbers, characters, and strings automatically. Note the difference between `endl` and `'\n'`: both emit a newline, but `endl` also flushes the stream's buffer, which is slower if done in a tight loop.

```cpp
#include <iostream>
using namespace std;

int main() {
  cout << "Integer: " << 42 << endl;
  cout << "Double: " << 3.14 << endl;
  cout << "Multiple: " << 1 << ", " << 2 << ", " << 3 << endl;
  
  // No endl (no automatic flush)
  cout << "Line 1\n";
  cout << "Line 2\n";
  
  return 0;
}
```

### Input with cin

`std::cin` is the standard input stream, and the extraction operator `>>` reads formatted data into a variable, automatically converting text to the target type. It skips leading whitespace and stops at the next whitespace, so `>>` reads a single word, not a whole line. Always consider error handling: if the input cannot be parsed into the requested type, the stream enters a failed state and the variable is left unchanged (zero-initialized in C++11), so check the stream before trusting the value.

```cpp
#include <iostream>
using namespace std;

int main() {
  int age;
  double height;
  string name;
  
  cout << "Enter name: ";
  cin >> name;              // Single word
  
  cout << "Enter age: ";
  cin >> age;               // Integer
  
  cout << "Enter height: ";
  cin >> height;            // Floating point
  
  cout << "Name: " << name << ", Age: " << age << endl;
  
  return 0;
}
```

### Advanced I/O Formatting

Stream manipulators, mostly from `<iomanip>`, control how values are formatted on output. They let you set decimal precision (`setprecision` with `fixed`), change the numeric base (`hex`, `oct`, `dec`), and pad or align fields (`setw`, `setfill`, `left`/`right`). A key subtlety is that most manipulators are *sticky* — settings like `hex` or `setprecision` persist until you change them — whereas `setw` applies only to the very next item written.

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
  double pi = 3.14159265;
  int x = 255;
  
  // Set precision
  cout << fixed << setprecision(2);
  cout << pi << endl;               // 3.14
  
  // Hexadecimal, octal, binary
  cout << hex << x << endl;         // ff (255 in hex)
  cout << oct << x << endl;         // 377 (255 in octal)
  cout << dec << x << endl;         // 255 (decimal)
  
  // Width and alignment
  cout << setw(10) << "right";      // Right-aligned in 10 chars
  cout << setw(10) << left << "left";  // Left-aligned
  
  // Padding
  cout << setfill('*') << setw(10) << "pad" << endl;  // *******pad
  
  return 0;
}
```

### getline for String Input

`std::getline` reads an entire line of input — including embedded spaces — into a `std::string`, stopping at the newline (which it consumes but does not store). Use it instead of `cin >> str` whenever the input may contain spaces, such as a full name or a sentence. A classic pitfall is mixing `cin >> x` with `getline`: the `>>` leaves the newline in the buffer, so the next `getline` reads an empty line — clear it first with `cin.ignore()`.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
  string fullName;
  
  cout << "Enter your full name: ";
  getline(cin, fullName);           // Reads entire line including spaces
  
  cout << "Hello, " << fullName << endl;
  
  return 0;
}
```

---

## 1.7 Type Conversion & Casting

### Implicit Conversion

An implicit conversion is performed automatically by the compiler when a value of one type is used where a different but compatible type is expected. Widening conversions (such as `int` to `double`) are safe and lossless, but narrowing ones (such as `double` to `int`, or to a smaller integer) silently discard data — the fractional part or high-order bits are simply dropped. Because these happen without any visible syntax, they are an easy source of subtle bugs; uniform `{}` initialization deliberately rejects narrowing to catch them.

```cpp
int x = 10;
double y = x;      // Implicit conversion: int to double (safe)

double z = 3.14;
int w = z;         // Implicit conversion: double to int (data loss!)
                   // w = 3 (fractional part lost)
```

### Explicit Casting

#### C-Style Cast

The C-style cast `(type)value` forces a conversion using a single terse syntax inherited from C. It is discouraged in C++ because it is a blunt instrument: it will silently attempt whatever combination of `static_cast`, `const_cast`, and `reinterpret_cast` is needed to make the conversion compile, hiding potentially dangerous operations. Prefer the named C++ casts below, which express your exact intent and are far easier to find and audit.

```cpp
double d = 3.14;
int x = (int)d;    // C-style cast (less safe, generic)
```

#### C++ Style Casts

C++ provides four named cast operators, each expressing a specific, searchable intent. `static_cast` performs well-defined compile-time conversions (numeric conversions, related pointer types); `reinterpret_cast` reinterprets a bit pattern as an unrelated type and is inherently unsafe; `const_cast` adds or removes `const` (modifying a truly `const` object through it is undefined behavior); and `dynamic_cast` performs a checked downcast in a polymorphic class hierarchy at runtime. Prefer the narrowest cast that does the job, and reach for `static_cast` for the vast majority of everyday conversions.

```cpp
// static_cast: Safe, compile-time checked
int x = 10;
double y = static_cast<double>(x);

// reinterpret_cast: Dangerous, interprets bits differently
int* ptr = &x;
char* cptr = reinterpret_cast<char*>(ptr);

// const_cast: Removes const qualifier (rare, dangerous)
const int* cptr2 = &x;
int* ptr2 = const_cast<int*>(cptr2);

// dynamic_cast: Runtime type checking (covered in inheritance)
Base* base = new Derived();
Derived* derived = dynamic_cast<Derived*>(base);
```

### Promotion & Narrowing

Promotion converts a smaller type to a larger compatible one (for example `int` to `double`, or `short` to `int`) and is always safe because the larger type can represent every value of the smaller one. Narrowing goes the other way and may lose information: a fractional part, magnitude, or sign can be discarded without warning under copy initialization. C++11's brace initialization turns narrowing into a compile-time error, which is the recommended way to make these dangerous conversions explicit and visible.

```cpp
// Promotion: smaller type -> larger type (safe)
int x = 10;
double y = x;      // Promotion (int to double)

// Narrowing: larger type -> smaller type (can lose data)
double z = 3.14;
int w = z;         // Narrowing (double to int, loses fractional part)

// C++11 prevents accidental narrowing with braces
int a{10};         // OK
int b{3.14};       // ERROR: narrowing conversion not allowed
int c = 3.14;      // OK but dangerous (implicit narrowing)
```

---

## 1.8 Common Mistakes & Best Practices

### Common Mistakes
```cpp
// ❌ Uninitialized variables
int x;              // Contains garbage value
cout << x;          // Unpredictable output

// ✅ Always initialize
int x = 0;

// ❌ Overflow
unsigned char c = 300;  // 300 doesn't fit in 1 byte (0-255)
                        // c = 44 (300 % 256)

// ✅ Use appropriate types
int x = 300;

// ❌ Loss of precision
float f = 3.141592653589793238;
// Only stores ~6-7 decimal places

// ✅ Use double for precision
double d = 3.141592653589793238;
```

### Best Practices
```cpp
// ✅ Const-correctness
const int MAX_SIZE = 100;

// ✅ Meaningful names
int studentAge = 20;  // Not: int x = 20;

// ✅ Explicit types with auto
auto count = 0;       // Type is clear from context

// ✅ Use nullptr instead of NULL
int* ptr = nullptr;   // C++11 way

// ✅ Minimize scope
{
  int temp = calculate();
  process(temp);
}  // temp destroyed here

// ✅ Use constexpr for compile-time constants
constexpr int BUFFER_SIZE = 256;
```

---

## 1.9 More C++11 Type & Compile-Time Features

Alongside `auto`, `constexpr`, and uniform initialization (covered above), C++11 adds three more building blocks you'll use constantly.

### `decltype` — deduce the type of an expression

While `auto` deduces a type from an **initializer**, `decltype` yields the exact type of any **expression** without evaluating it. The two are often used together.

```cpp
int x = 10;
double d = 3.14;

decltype(x)     a = 5;       // a is int
decltype(x + d) b = 1;       // b is double (type of int + double)
decltype(d)     c = d;       // c is double

const int& ref = x;
decltype(ref)   r = x;       // r is const int& — decltype preserves const/ref

// Combined with auto: deduce a return type from the parameters
// (see Chapter 3.10 for the trailing-return-type form)
auto multiply(int p, int q) -> decltype(p * q) {
  return p * q;
}
```

`auto` strips top-level `const` and references; `decltype` keeps them — choose based on whether you want the exact declared type.

### `static_assert` — compile-time assertions

`static_assert` checks a constant expression **at compile time** and aborts compilation with your message if it is false. There is zero runtime cost.

```cpp
static_assert(sizeof(int) >= 4, "This code needs at least 32-bit ints");
static_assert(sizeof(void*) == 8, "64-bit build required");

constexpr int BUFFER = 256;
static_assert(BUFFER % 8 == 0, "BUFFER must be a multiple of 8");

// Frequently used inside templates to validate type requirements
template<typename T>
struct OnlyIntegral {
  static_assert(sizeof(T) <= 8, "T is too large");
  // ...
};
```

Unlike the runtime `assert` macro from `<cassert>`, `static_assert` fires during compilation and needs no header.

### User-Defined Literals (UDLs)

C++11 lets you define a suffix that converts a literal into a custom type by writing an `operator""`. This makes units and domain types read naturally.

```cpp
#include <iostream>
using namespace std;

// Define a suffix _km that produces a distance in meters (double)
constexpr double operator"" _km(long double v) { return v * 1000.0; }
constexpr double operator"" _m (long double v) { return v; }

double trip = 2.5_km + 300.0_m;   // 2800.0  (meters)
cout << trip << "\n";

// Integer and string UDLs
constexpr unsigned long long operator"" _KB(unsigned long long n) {
  return n * 1024;
}
auto bufSize = 4_KB;   // 4096

// The standard library itself uses UDLs (e.g. <chrono>'s 10ms, 5s — see Ch15)
```

| Literal kind | Required parameter type |
|---|---|
| Floating-point (`2.5_km`) | `long double` |
| Integer (`4_KB`) | `unsigned long long` |
| Character (`'a'_x`) | `char` |
| String (`"hi"_s`) | `const char*, size_t` |

> **Tip:** Standard-library UDL suffixes (without a leading underscore) are reserved; always begin your own suffixes with `_`.

(Raw string and Unicode literals are covered in [Chapter 5.8](../05_arrays_strings/README.md).)

---

## Summary

| Concept | Key Points |
|---------|-----------|
| **Data Types** | char, int, float, double, bool |
| **Variables** | Local, global, static scopes |
| **Const** | Compile-time constant, const-correctness |
| **Initialization** | C++11 uniform init prevents narrowing |
| **I/O** | cout for output, cin for input |
| **Casting** | Use static_cast for safe conversions |
| **Auto** | Type deduction for cleaner code |
| **decltype** | Type of an expression, preserving const/ref (C++11) |
| **static_assert** | Compile-time assertion (C++11) |
| **User-defined literals** | Custom suffixes via `operator""` (C++11) |

---

## Next Steps
- Practice declaring and initializing variables of different types
- Write programs that accept user input and display output
- Experiment with type casting and precision
- Move to [Chapter 2: Control Flow](../02_control_flow/README.md)
