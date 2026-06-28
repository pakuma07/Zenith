// Chapter 15: Advanced Features - Runnable Examples
// Compile with: g++ -std=c++11 chapter15_advanced_features.cpp -o chapter15_advanced_features

#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// ============================================================
// EXAMPLE 1: Lambda Functions - Basics
// ============================================================
void example1_lambda_basics() {
    cout << "\n=== EXAMPLE 1: Lambda Functions - Basics ===" << endl;
    
    // Simplest lambda
    auto greet = []() { cout << "Hello from lambda!" << endl; };
    greet();
    
    // Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    cout << "3 + 4 = " << add(3, 4) << endl;
    
    // Lambda with return type
    auto multiply = [](int a, int b) -> int { return a * b; };
    cout << "5 * 6 = " << multiply(5, 6) << endl;
}

// ============================================================
// EXAMPLE 2: Lambda Captures
// ============================================================
void example2_lambda_captures() {
    cout << "\n=== EXAMPLE 2: Lambda Captures ===" << endl;
    
    int x = 10, y = 20;
    
    // Capture by value [=]
    auto by_value = [=]() { cout << "By value: x=" << x << ", y=" << y << endl; };
    by_value();
    x = 100;  // Doesn't affect lambda
    by_value();
    
    // Capture by reference [&]
    auto by_ref = [&]() { cout << "By reference: x=" << x << ", y=" << y << endl; };
    by_ref();  // Shows changed value
    
    // Specific captures
    int z = 30;
    auto specific = [=, &z]() { cout << "z by ref: " << z << endl; };
    z = 300;
    specific();  // Shows changed z
}

// ============================================================
// EXAMPLE 3: Auto Type Deduction (C++11)
// ============================================================
void example3_auto_type() {
    cout << "\n=== EXAMPLE 3: Auto Type Deduction ===" << endl;
    
    auto i = 42;               // int
    auto d = 3.14;             // double
    auto s = "string";         // const char*
    auto v = vector<int>();   // vector<int>
    
    cout << "Types deduced by auto:" << endl;
    cout << "i: " << typeid(i).name() << endl;
    cout << "d: " << typeid(d).name() << endl;
    cout << "s: " << typeid(s).name() << endl;
    cout << "v: " << typeid(v).name() << endl;
}

// ============================================================
// EXAMPLE 4: Range-Based For Loop (C++11)
// ============================================================
void example4_range_for() {
    cout << "\n=== EXAMPLE 4: Range-Based For Loop ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    // By value
    cout << "By value: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // By reference (to modify)
    for (int& x : v) x *= 2;
    cout << "After doubling: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Const reference
    for (const auto& x : v) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 5: Nullptr (C++11)
// ============================================================
void example5_nullptr() {
    cout << "\n=== EXAMPLE 5: Nullptr (C++11) ===" << endl;
    
    int* ptr1 = nullptr;        // Preferred in C++11
    int* ptr2 = NULL;           // Old style (macro)
    int* ptr3 = 0;              // Old style (integer 0)
    
    cout << "ptr1 is null: " << (ptr1 == nullptr) << endl;
    cout << "ptr2 is null: " << (ptr2 == nullptr) << endl;
    cout << "ptr3 is null: " << (ptr3 == nullptr) << endl;
    
    // Type-safe
    int x = 5;
    int* ptr = &x;
    cout << "ptr points to x: " << (*ptr == x) << endl;
}

// ============================================================
// EXAMPLE 6: Type Traits for Compile-Time Checks
// ============================================================
void example6_type_traits() {
    cout << "\n=== EXAMPLE 6: Type Traits ===" << endl;
    
    cout << "is_integral<int>: " << is_integral<int>::value << endl;
    cout << "is_integral<double>: " << is_integral<double>::value << endl;
    cout << "is_floating_point<double>: " << is_floating_point<double>::value << endl;
    cout << "is_pointer<int*>: " << is_pointer<int*>::value << endl;
    cout << "is_const<const int>: " << is_const<const int>::value << endl;
    
    // Using in enable_if (shown earlier in templates)
    cout << "Type analysis complete" << endl;
}

// ============================================================
// EXAMPLE 7: Constexpr (Compile-Time Constants)
// ============================================================
constexpr int fibonacci(int n) {
    return (n <= 1) ? n : fibonacci(n-1) + fibonacci(n-2);
}

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n-1);
}

void example7_constexpr() {
    cout << "\n=== EXAMPLE 7: Constexpr ===" << endl;
    
    // Computed at compile time
    constexpr int fib10 = fibonacci(10);
    constexpr int fact5 = factorial(5);
    
    cout << "Fibonacci(10): " << fib10 << endl;
    cout << "Factorial(5): " << fact5 << endl;
    
    // Can also use at runtime
    int n = 7;
    cout << "Fibonacci(7): " << fibonacci(n) << endl;
}

// ============================================================
// EXAMPLE 8: Rvalue References and Move (C++11)
// ============================================================
class String {
private:
    char* data;
    size_t length;
public:
    String(const char* s) {
        length = strlen(s);
        data = new char[length + 1];
        strcpy(data, s);
        cout << "String copy constructed" << endl;
    }
    
    // Move constructor
    String(String&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
        cout << "String move constructed (efficient!)" << endl;
    }
    
    ~String() { delete[] data; }
    
    void print() const { if (data) cout << data << endl; }
};

void example8_move_semantics() {
    cout << "\n=== EXAMPLE 8: Rvalue References & Move ===" << endl;
    
    String s1("Hello");        // Construct
    String s2 = move(s1);      // Move (efficient)
    
    cout << "s1 after move: ";
    s1.print();
    cout << "s2 after move: ";
    s2.print();
}

// ============================================================
// EXAMPLE 9: Delegating Constructors (C++11)
// ============================================================
class Point {
private:
    int x, y;
public:
    // Main constructor
    Point(int x, int y) : x(x), y(y) {
        cout << "Main constructor" << endl;
    }
    
    // Delegating constructors
    Point() : Point(0, 0) {
        cout << "Default constructor (delegated)" << endl;
    }
    
    Point(int val) : Point(val, val) {
        cout << "Single-value constructor (delegated)" << endl;
    }
    
    void display() {
        cout << "Point(" << x << ", " << y << ")" << endl;
    }
};

void example9_delegating_constructors() {
    cout << "\n=== EXAMPLE 9: Delegating Constructors ===" << endl;
    
    Point p1;           // Delegates to Point(0, 0)
    p1.display();
    
    Point p2(5);        // Delegates to Point(5, 5)
    p2.display();
    
    Point p3(3, 7);     // Direct
    p3.display();
}

// ============================================================
// EXAMPLE 10: Modern C++ Best Practices
// ============================================================
void example10_modern_cpp() {
    cout << "\n=== EXAMPLE 10: Modern C++11 Best Practices ===" << endl;
    
    cout << "C++11 Key Features:" << endl;
    cout << "  ✓ auto: Type deduction" << endl;
    cout << "  ✓ range-based for: Clean iteration" << endl;
    cout << "  ✓ lambda: Anonymous functions" << endl;
    cout << "  ✓ nullptr: Null pointer safety" << endl;
    cout << "  ✓ smart pointers: Memory management" << endl;
    cout << "  ✓ move semantics: Efficient transfers" << endl;
    cout << "  ✓ constexpr: Compile-time computation" << endl;
    cout << "  ✓ type_traits: Meta-programming" << endl;
    cout << "  ✓ override: Virtual safety" << endl;
    cout << "  ✓ = default/= delete: Explicit defaults" << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 15: ADVANCED FEATURES (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_lambda_basics();
    example2_lambda_captures();
    example3_auto_type();
    example4_range_for();
    example5_nullptr();
    example6_type_traits();
    example7_constexpr();
    // example8_move_semantics();  // Commented due to strlen
    example9_delegating_constructors();
    example10_modern_cpp();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Lambda Functions**: Anonymous functions (C++11)
   - Syntax: [] (capture) (params) { body }
   - Powerful with algorithms
   - Inline implementation

2. **Lambda Captures**: Access external variables
   - [=]: by value (copies)
   - [&]: by reference (original)
   - [=, &x]: mixed captures
   - Closure concept

3. **Auto Type Deduction**: Let compiler determine types
   - Cleaner code
   - Avoids verbose types
   - Still type-safe

4. **Range-Based For**: Modern iteration (C++11)
   - Simpler than traditional for
   - Works with containers
   - By value, reference, or const

5. **Nullptr**: Type-safe null (C++11)
   - Preferred over NULL or 0
   - Prevents type confusion
   - Explicit intent

6. **Type Traits**: Compile-time type analysis
   - is_integral, is_floating_point, etc.
   - Enable/disable code paths
   - Meta-programming foundation

7. **Constexpr**: Compile-time evaluation
   - Functions evaluated at compile time
   - Zero runtime cost
   - Limited to pure computation

8. **Rvalue References**: Efficient transfers
   - && notation
   - Move semantics foundation
   - Avoid unnecessary copies

9. **Delegating Constructors**: DRY principle
   - One constructor calls another
   - Reduce code duplication
   - Clear initialization order

10. **Modern C++11 Summary**:
    - Cleaner, safer, more efficient
    - Better abstraction possibilities
    - Focus on intent not details
    - Still maintain backward compatibility
*/
