# Chapter 5: Arrays & Strings

This chapter covers how C++ stores collections of values and sequences of characters. It begins with raw fixed-size arrays — the low-level primitive that C++ inherits from C — and the way they decay into pointers, then moves on to text handling with both C-style `char` arrays and the much safer `std::string` class. Finally it introduces the standard library containers `std::vector` (a dynamic, resizable array) and `std::array` (a fixed-size, STL-friendly wrapper), along with string streams for parsing and formatting.

The overarching lesson is to prefer the standard library types over raw arrays and C-strings: `std::string`, `std::vector`, and `std::array` manage their own memory, know their own size, and offer bounds-checked access, eliminating whole categories of buffer-overflow and out-of-bounds bugs. Raw arrays and C-strings are still worth understanding because you will encounter them in legacy code and at API boundaries.

## 5.1 Arrays

An array is a contiguous, fixed-size block of elements of the same type, allocated in one chunk and indexed from `0` to `N-1`. The size must be a compile-time constant, and C++ performs no bounds checking — reading or writing past the end is undefined behavior, not an error. Arrays are the foundational building block behind strings and many containers, but for everyday code prefer `std::vector` or `std::array`, which carry their size with them and avoid silent pointer decay.

### 1D Arrays

### 1D Arrays
```cpp
// Fixed-size array
int arr[5];                    // Uninitialized
int arr2[5] = {1, 2, 3, 4, 5};  // Initialized
int arr3[] = {10, 20, 30};     // Size deduced (3)

// Accessing elements
cout << arr2[0] << endl;       // 1
cout << arr2[4] << endl;       // 5
// cout << arr2[5];            // Out of bounds (undefined!)

// Array length (remember for loops)
int size = sizeof(arr2) / sizeof(arr2[0]);  // 5

// Iterate
for (int i = 0; i < 5; i++) {
  cout << arr2[i] << " ";
}

// Range-based for (C++11)
for (int val : arr2) {
  cout << val << " ";
}

// Modify elements
arr2[0] = 100;
arr2[3] += 10;
```

### 2D Arrays

A two-dimensional array is really an "array of arrays" stored as one contiguous block in row-major order, indexed with two subscripts as `matrix[row][col]`. They are natural for fixed-size grids and matrices, and all dimensions except possibly the first must be specified so the compiler knows the stride of each row. As with 1D arrays the size is fixed at compile time; for grids whose dimensions are only known at runtime, use a `std::vector<std::vector<int>>` instead (see 5.5).

```cpp
// 2D array (matrix)
int matrix[3][3] = {
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9}
};

// Access elements
cout << matrix[0][0] << endl;  // 1
cout << matrix[2][1] << endl;  // 8

// Modify
matrix[1][1] = 0;

// Nested loops
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    cout << matrix[i][j] << " ";
  }
  cout << endl;
}

// Partial initialization
int partial[2][3] = {
  {1, 2},      // Second element defaults to 0
  {4}          // Other elements default to 0
};
```

### Multidimensional Arrays

The same nesting principle extends to three or more dimensions: each extra `[N]` adds another level of nested arrays, accessed with one subscript per dimension. These are useful for volumetric data or tensors, but their total element count (and therefore memory use) grows multiplicatively, so keep the dimensions modest. Deeply nested raw arrays quickly become unwieldy to pass around, which is another reason real code often reaches for containers or a flat 1D array with computed indices.

```cpp
// 3D array
int cube[2][3][4];

// 4D array
int hypercube[2][2][2][2];

// Initialization
int tensor[2][2][2] = {
  {{1, 2}, {3, 4}},
  {{5, 6}, {7, 8}}
};

// Access
cout << tensor[1][1][0] << endl;  // 7
```

### Arrays & Pointers

Arrays and pointers are closely related: in most expressions an array name "decays" into a pointer to its first element, which is why `int* ptr = arr;` works and why a function receiving an array actually receives a pointer (losing the size). Pointer arithmetic mirrors indexing — `*(ptr + i)` is equivalent to `ptr[i]` — because adding `i` to a pointer advances it by `i` elements, not bytes. Be careful to distinguish an array *of* pointers (`int* ptrs[]`) from a pointer *to* an array (`int (*p)[3]`); the parentheses change the meaning entirely, and confusing the two is a common source of compiler errors.

```cpp
int arr[] = {10, 20, 30};
int* ptr = arr;                // arr decays to pointer

cout << *ptr << endl;          // 10
cout << *(ptr + 1) << endl;    // 20

// Array of pointers
int a = 10, b = 20, c = 30;
int* ptrs[] = {&a, &b, &c};

for (int i = 0; i < 3; i++) {
  cout << *ptrs[i] << " ";     // 10 20 30
}

// Pointer to array
int (*ptr2arr)[3] = &arr;
cout << (*ptr2arr)[0] << endl;  // 10
```

---

## 5.2 Strings (C-Style)

A C-style string is simply a `char` array whose end is marked by a null terminator (`'\0'`); there is no separate length stored, so functions like `strlen` must scan until they find the null. This is the original string representation inherited from C, and you will still meet it in legacy code and operating-system APIs. The classic library functions (`strcpy`, `strcat`, `strcmp`, in `<cstring>`) do no bounds checking, so writing more characters than the destination can hold causes a buffer overflow — a serious correctness and security bug. Whenever you have the choice, prefer `std::string` (5.3), which manages its own length and memory automatically.

### C-Style Strings (char arrays)
```cpp
// String as char array
char str[] = "Hello";           // null-terminated
char str2[] = {'H','e','l','l','o','\0'};  // Explicit null

cout << str << endl;            // Hello
cout << strlen(str) << endl;    // 5 (excludes null terminator)
cout << sizeof(str) << endl;    // 6 (includes null terminator)

// Reading strings
char input[50];
cin >> input;                   // Read word (no spaces)

char line[100];
cin.getline(line, 100);         // Read entire line

// String functions
strcpy(str, "World");           // Copy (dangerous, use strncpy)
strcat(str, " C++");            // Concatenate
strcmp(str, "World C++");       // Compare (0 if equal)

// Dangers of C-style strings
char buffer[5];
strcpy(buffer, "Hello World");  // BUFFER OVERFLOW!
```

---

## 5.3 Strings (C++ String Class)

`std::string` (from `<string>`) is the standard, safe way to handle text in C++. Unlike a `char` array it owns and automatically resizes its buffer, tracks its own length, and frees its memory when it goes out of scope, so overflow and manual-termination bugs largely disappear. It supports natural operations — concatenation with `+`, comparison with `<`/`==`, indexing with `[]`, and a rich set of member functions for searching, substrings, and editing. Use `std::string` by default for any text; reach for C-strings only when interfacing with C APIs.

### String Basics

Constructing a `std::string` is flexible: you can default-construct an empty one, copy from a string literal, repeat a character a fixed number of times, or copy another string. Two members report length — `length()` and `size()` are exact synonyms. Element access comes in two forms: `operator[]` is fast but unchecked, while `at()` performs bounds checking and throws `std::out_of_range` on an invalid index, making it the safer choice when the index might be wrong.

```cpp
#include <string>
using namespace std;

// Creation
string s1;                      // Empty
string s2 = "Hello";            // Initialized
string s3(5, 'A');              // 5 'A's = "AAAAA"
string s4(s2);                  // Copy

// Length
cout << s2.length() << endl;    // 5
cout << s2.size() << endl;      // 5 (same as length)

// Access
cout << s2[0] << endl;          // 'H'
cout << s2.at(1) << endl;       // 'e' (with bounds checking)

// Modification
s2[0] = 'h';                    // s2 = "hello"
s2.at(0) = 'H';                 // s2 = "Hello" (safe)
```

### String Operations

`std::string` offers a full toolkit for transforming text. You can build strings with `+` and `+=`, extract pieces with `substr(pos, len)`, and locate content with `find`, which returns the special value `std::string::npos` when nothing is found — always test against `npos` rather than assuming a hit. Editing members such as `replace`, `erase`, and `insert` modify the string in place using position/length arguments. Note there is no built-in case conversion: you transform character by character with `toupper`/`tolower` (from `<cctype>`), typically over a range-based `for` loop using a `char&` reference so the changes stick.

```cpp
string s = "Hello";

// Concatenation
s = s + " World";               // s = "Hello World"
s += " C++";                    // s = "Hello World C++"

// Substring
string sub = s.substr(0, 5);    // "Hello"
string sub2 = s.substr(6);      // "World C++"

// Find
size_t pos = s.find("World");   // 6
if (pos != string::npos) {
  cout << "Found at " << pos << endl;
}

// Replace
s.replace(6, 5, "C++");         // Replace 5 chars at pos 6

// Erase
s.erase(0, 6);                  // Erase first 6 chars
s.erase(s.find("C++"), 3);      // Erase "C++"

// Insert
s.insert(0, "Intro: ");         // Insert at beginning

// Compare
string s1 = "apple", s2 = "banana";
if (s1 < s2) {
  cout << "apple comes first" << endl;
}

// Case conversion
string upper = "hello";
for (char& c : upper) c = toupper(c);  // "HELLO"

for (char& c : upper) c = tolower(c);  // "hello"
```

### String I/O

Reading strings has a subtle trap: `cin >> word` stops at the first whitespace, so it reads only a single word and leaves the rest (including the newline) in the buffer. To read a full line including spaces, use `getline(cin, line)`. Mixing the two is a classic pitfall — a leftover newline from a previous `>>` makes the next `getline` return an empty string. This section also covers numeric conversions: `to_string` turns numbers into text, while the C++11 `stoi`/`stod`/`stol` functions parse text back into numbers (throwing on invalid input).

```cpp
// Read word
string word;
cin >> word;

// Read line
string line;
getline(cin, line);             // Read entire line

// Read with extraction
cout << "Enter name: ";
string name;
getline(cin, name);

// Formatting
string formatted = "The answer is " + to_string(42);

// Convert string to number
string numStr = "123";
int num = stoi(numStr);         // String to integer
double d = stod("3.14");        // String to double
long l = stol("9999999");       // String to long

// Convert number to string
int x = 42;
string str = to_string(x);      // "42"
```

### Useful String Methods

Beyond the core operations, `std::string` provides several convenience members: `empty()` to test for zero length (preferred over comparing `size() == 0`), `find`/`rfind` to search from the left or right, and `front()`/`back()` to access the first and last characters. Calling `front()` or `back()` on an empty string is undefined behavior, so guard with `empty()` first. Some common tasks like trimming whitespace are not built in, but are easily composed from `find_first_not_of`/`find_last_not_of` together with `erase`, as the helper below shows.

```cpp
string s = "Hello World";

// Query
s.empty();                      // false
s.find("World");                // 6
s.rfind("o");                   // 7 (rightmost)

// Extract
s.front();                      // 'H'
s.back();                       // 'd'

// Trim (not built-in, but common)
string trim(string s) {
  // Remove leading/trailing whitespace
  s.erase(0, s.find_first_not_of(" \t\n\r"));
  s.erase(s.find_last_not_of(" \t\n\r") + 1);
  return s;
}
```

---

## 5.4 Array Container: Vector

`std::vector` (from `<vector>`) is the workhorse container of C++: a dynamic array that grows and shrinks at runtime while storing its elements contiguously in memory. It manages its own storage, so you never call `new`/`delete` for the elements, and it tracks both `size()` (how many elements it holds) and `capacity()` (how many it can hold before reallocating). Because the data is contiguous, indexing is as fast as a raw array, yet the container can resize itself — making `vector` the default choice for sequences whose length is not known at compile time.

### Vector Basics

A `vector` can be constructed empty, sized with a default value, sized with a repeated fill value, or from a brace-enclosed initializer list. Element access mirrors `std::string`: `operator[]` is fast but unchecked, `at()` is bounds-checked and throws `std::out_of_range`, and `front()`/`back()` retrieve the ends. Be aware that `capacity()` is generally larger than `size()` because the vector over-allocates to make repeated growth efficient, so do not rely on its exact value.

```cpp
#include <vector>
using namespace std;

// Creation
vector<int> v;                  // Empty
vector<int> v2(5);              // 5 elements, value 0
vector<int> v3(5, 10);          // 5 elements, value 10
vector<int> v4 = {1, 2, 3};     // Initialized

// Size and capacity
cout << v4.size() << endl;      // 3
cout << v4.capacity() << endl;  // At least 3

// Access
cout << v4[0] << endl;          // 1
cout << v4.at(1) << endl;       // 2 (with bounds check)
cout << v4.front() << endl;     // 1 (first)
cout << v4.back() << endl;      // 3 (last)

// Modify
v4[0] = 10;
v4.at(0) = 20;
```

### Vector Operations

Vectors support a rich set of modifying operations. `push_back` appends to the end (the cheapest insertion), while `insert` and `erase` add or remove at an arbitrary position given by an iterator — both are O(n) because following elements must shift. A key pitfall: inserting or erasing can reallocate the buffer and thereby invalidate any existing iterators, pointers, or references into the vector, so do not hold onto them across a modification. Because the elements are contiguous, vectors also work directly with `<algorithm>` functions like `sort`, using the `begin()`/`end()` iterator pair.

```cpp
vector<int> v = {1, 2, 3};

// Add elements
v.push_back(4);                 // v = {1, 2, 3, 4}
v.insert(v.begin() + 1, 99);    // Insert 99 at position 1

// Remove elements
v.pop_back();                   // Remove last
v.erase(v.begin() + 1);         // Erase at position 1
v.clear();                      // Remove all

// Iterate
for (int val : v) {
  cout << val << " ";
}

// Sort
sort(v.begin(), v.end());       // Ascending
sort(v.begin(), v.end(), greater<int>());  // Descending
```

---

## 5.5 Multidimensional Vector

A `vector<vector<int>>` builds a dynamic 2D grid where each row is itself an independently sized vector, giving you a "jagged" array whose dimensions can be decided and changed at runtime. Access is the familiar `grid[row][col]`, and rows can be added or resized individually. Unlike a true 2D array the rows are not stored in one contiguous block (each inner vector owns its own buffer), which costs some cache locality; for large numeric grids a single flat `vector` with manual index arithmetic (`v[row * cols + col]`) is often faster. Note the C++11 caveat that pre-C++11 compilers needed a space in `vector<vector<int> >` to avoid the `>>` being parsed as a shift operator.

```cpp
// Vector of vectors (dynamic 2D array)
vector<vector<int>> matrix = {
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9}
};

// Access
cout << matrix[0][0] << endl;   // 1
cout << matrix[2][2] << endl;   // 9

// Modify
matrix[1][1] = 0;

// Grow dynamically
vector<vector<int>> grid(3);    // 3 rows
grid[0] = {1, 2, 3};
grid[1] = {4, 5, 6};
grid[2] = {7, 8, 9};

// Iterate
for (auto& row : matrix) {
  for (int val : row) {
    cout << val << " ";
  }
  cout << endl;
}
```

---

## 5.6 String Stream

A `std::stringstream` (from `<sstream>`) is an in-memory stream that lets you use the familiar `<<` and `>>` operators on a string instead of the console or a file. This makes it ideal for two jobs: building a formatted string piece by piece (then retrieving it with `.str()`), and parsing — feeding it a string and extracting typed values, which is the easy way to split text like `"10 20 30"` into integers. You can apply stream manipulators such as `hex` for formatting. One important gotcha when reusing a stream: call both `ss.str("")` to clear its contents and `ss.clear()` to reset its error/EOF state flags, since extraction failures otherwise leave the stream stuck.

```cpp
#include <sstream>
using namespace std;

// String stream for string manipulation
stringstream ss;

// Write to stream
ss << "Hello " << 42 << " " << 3.14;

// Extract as string
string result = ss.str();       // "Hello 42 3.14"

// Parse from stream
stringstream parse("10 20 30");
int a, b, c;
parse >> a >> b >> c;           // a=10, b=20, c=30

// Format numbers
stringstream format;
format << hex << 255;           // Hexadecimal
format.str();                   // "ff"

// Clear for reuse
ss.str("");
ss.clear();
ss << "new data";
```

---

## 5.7 Best Practices

This section distills the chapter's central guidance into a checklist. The recurring theme is to prefer the self-managing standard library types — `std::string` over `char` arrays and `std::vector` over raw arrays — because they track their own size, manage memory automatically, and offer bounds-checked access. Favor range-based `for` loops and `<algorithm>` functions for clear, correct iteration, validate indices before subscripting, and avoid unchecked C-string functions like `strcpy` that invite buffer overflows.

```cpp
// ✅ Prefer string over char arrays
string s = "Hello";             // Safe, convenient
// char c[] = "Hello";           // C-style, error-prone

// ✅ Prefer vector over raw arrays
vector<int> v = {1, 2, 3};      // Dynamic, safe
// int arr[] = {1, 2, 3};        // Fixed size, manual

// ✅ Use iterators or range-for
for (auto val : vec) {          // Clean
  process(val);
}

// ✅ Check bounds
if (vec.size() > index) {
  vec[index];
}

// ✅ Use algorithm with containers
vector<int> v = {3, 1, 4, 1, 5};
sort(v.begin(), v.end());

// ❌ Don't use buffer overflow with strcpy
// char buf[5];
// strcpy(buf, "Hello World");   // OVERFLOW!

// ✅ Use safe alternatives
string buf = "Hello World";     // Safe
```

---

## Summary

| Concept | Key Points |
|---------|-----------|
| **Arrays** | Fixed-size, decay to pointers |
| **C-String** | char array, null-terminated |
| **String class** | Dynamic, safe, many methods |
| **Vector** | Dynamic array container |
| **std::array** | Fixed-size, no decay, STL-compatible (C++11) |
| **2D Array** | For matrices and grids |
| **Iteration** | Range-for, iterators |

---

## 5.8 Unicode and Raw String Literals (C++11)

### Unicode String Literals

C++11 provides four new string literal prefixes for Unicode text:

```cpp
// u8"..." -- UTF-8 encoded, type const char[]
const char* utf8 = u8"Hello \u4e16\u754c";   // "Hello World" in Chinese

// u"..."  -- UTF-16 encoded, type const char16_t[]
const char16_t* utf16 = u"Hello \u4e16\u754c";

// U"..."  -- UTF-32 encoded, type const char32_t[]
const char32_t* utf32 = U"Hello \u4e16\u754c";

// L"..."  -- Wide string (platform-dependent width), type const wchar_t[]
const wchar_t* wide = L"Hello World";

// Character literals
char16_t c16 = u'\u0041';  // 'A'
char32_t c32 = U'\u0001F600';  // emoji codepoint
```

**`std::u16string` and `std::u32string` (C++11):**

```cpp
#include <string>
std::u16string s16 = u"hello";
std::u32string s32 = U"world";
std::wstring   ws  = L"wide";
// All support the same interface as std::string
```

### Raw String Literals `R"(...)"` (C++11)

Raw string literals disable all escape processing. Useful for regex, file paths, JSON.

```cpp
// Normal string: must escape backslashes and quotes
std::string path1 = "C:\\Users\\user\\file.txt";
std::string regex1 = "\\d{3}-\\d{4}";

// Raw string: no escaping needed
std::string path2 = R"(C:\Users\user\file.txt)";
std::string regex2 = R"(\d{3}-\d{4})";

// Custom delimiter to include ) in the string: R"delim(...)delim"
std::string json = R"json({"key": "value", "arr": [1,2,3]})json";

// Multiline raw string
std::string html = R"html(
<html>
  <body>Hello</body>
</html>
)html";
```

**Syntax:** `R"delimiter(content)delimiter"` — delimiter can be any sequence of
up to 16 characters that does not appear in the content.

---

## 5.9 std::array — A Safer Fixed-Size Array (C++11)

A raw array (`int arr[5]`) has two long-standing problems: it silently **decays to a pointer** (losing its size), and it has no member functions. C++11's `std::array<T, N>` is a zero-overhead wrapper that keeps the fixed size and stack allocation of a raw array while behaving like an STL container.

```cpp
#include <array>
#include <algorithm>
#include <iostream>
using namespace std;

// Type is array<element_type, size> — size is part of the type
array<int, 5> a = {10, 20, 30, 40, 50};

// Knows its own size (a raw array does not, once passed to a function)
cout << a.size() << "\n";   // 5

// Bounds-checked and unchecked access
cout << a[2] << "\n";       // 30 (no check)
cout << a.at(2) << "\n";    // 30 (throws out_of_range if invalid)
cout << a.front() << " " << a.back() << "\n";  // 10 50

// Works directly with STL algorithms and range-for
sort(a.begin(), a.end());
for (int x : a) cout << x << " ";

// Fill and whole-object assignment (raw arrays can't be assigned)
array<int, 5> b = a;        // OK — copies all elements
b.fill(0);                  // every element set to 0

// Passing to a function keeps the size — no decay
void printAll(const array<int, 5>& arr);   // size travels with the type
```

| | Raw array `int[5]` | `std::array<int,5>` | `std::vector<int>` |
|---|---|---|---|
| Size known after passing | No (decays to pointer) | Yes — `.size()` | Yes — `.size()` |
| Bounds-checked access | No | Yes — `.at()` | Yes — `.at()` |
| Assignable / copyable | No | Yes | Yes |
| Storage | Stack | Stack | Heap |
| Resizable | No | No | Yes |

**Use `std::array` when** the size is known at compile time and you want raw-array performance without the pointer-decay and missing-size pitfalls. Reach for `std::vector` only when the size must change at runtime. (See also [Chapter 10.10](../10_stl_containers/README.md) for `std::array` among the other containers.)

---

## Next Steps
- Work with arrays and vectors
- Practice string manipulation
- Use containers for data storage
- Move to [Chapter 6: OOP Basics](../06_oop_basics/README.md)
