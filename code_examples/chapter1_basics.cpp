// Chapter 1: Basics & Fundamentals - Code Examples
// Compile: g++ -std=c++11 chapter1_basics.cpp -o chapter1_basics
// Run: ./chapter1_basics

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ============================================================
// EXAMPLE 1: Data Types & Variables
// ============================================================
void example1_data_types() {
    cout << "\n=== EXAMPLE 1: Data Types & Variables ===" << endl;
    
    // Integer types
    signed char sc = -128;
    short s = -32768;
    int i = 2147483647;
    long l = 9223372036L;
    long long ll = 9223372036854775807LL;
    
    // Floating point
    float f = 3.14f;
    double d = 3.14159265;
    
    // Boolean
    bool flag = true;
    
    // Character
    char c = 'A';
    
    cout << "Int: " << i << " (size: " << sizeof(i) << " bytes)" << endl;
    cout << "Float: " << f << " (size: " << sizeof(f) << " bytes)" << endl;
    cout << "Double: " << d << " (size: " << sizeof(d) << " bytes)" << endl;
    cout << "Bool: " << flag << " (size: " << sizeof(flag) << " bytes)" << endl;
    cout << "Char: " << c << " (size: " << sizeof(c) << " bytes)" << endl;
}

// ============================================================
// EXAMPLE 2: C++11 Uniform Initialization
// ============================================================
void example2_uniform_init() {
    cout << "\n=== EXAMPLE 2: C++11 Uniform Initialization ===" << endl;
    
    int x{10};
    int y{20};
    // int z{3.14};  // ERROR: narrowing not allowed
    
    int arr[] = {1, 2, 3, 4, 5};
    
    cout << "x = " << x << ", y = " << y << endl;
    cout << "Array: ";
    for (int val : arr) cout << val << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 3: Auto Type Deduction (C++11)
// ============================================================
void example3_auto() {
    cout << "\n=== EXAMPLE 3: Auto Type Deduction ===" << endl;
    
    auto a = 10;              // int
    auto b = 3.14;            // double
    auto c = "hello";         // const char*
    auto d = true;            // bool
    
    cout << "a = " << a << " (likely int)" << endl;
    cout << "b = " << b << " (likely double)" << endl;
    cout << "c = " << c << " (likely const char*)" << endl;
    cout << "d = " << d << " (bool)" << endl;
}

// ============================================================
// EXAMPLE 4: Constants & Constexpr (C++11)
// ============================================================
void example4_constants() {
    cout << "\n=== EXAMPLE 4: Constants & Constexpr ===" << endl;
    
    const int MAX_SIZE = 100;
    constexpr double PI = 3.14159;
    
    cout << "MAX_SIZE = " << MAX_SIZE << endl;
    cout << "PI = " << PI << endl;
    
    // Constexpr can be used at compile-time
    int arr[MAX_SIZE];
    cout << "Created array of size: " << MAX_SIZE << endl;
}

// ============================================================
// EXAMPLE 5: Basic Input & Output
// ============================================================
void example5_io() {
    cout << "\n=== EXAMPLE 5: Input & Output ===" << endl;
    
    cout << "Simple output with cout" << endl;
    cout << "Value: " << 42 << ", Float: " << 3.14 << endl;
    
    // Formatted output
    cout << "\nFormatted Output:" << endl;
    cout << fixed << setprecision(2);
    cout << "PI with 2 decimals: " << 3.14159 << endl;
    
    cout << hex << "42 in hex: " << 42 << endl;
    cout << dec << "Back to decimal: " << 42 << endl;
    
    // Width and alignment
    cout << "\nAlignment:" << endl;
    cout << setw(10) << right << "Right";
    cout << setw(10) << left << "Left" << endl;
    cout << setfill('*') << setw(10) << "Padded" << endl;
}

// ============================================================
// EXAMPLE 6: Type Conversion
// ============================================================
void example6_casting() {
    cout << "\n=== EXAMPLE 6: Type Conversion ===" << endl;
    
    // Implicit conversion
    int x = 10;
    double y = x;  // int to double (safe)
    cout << "Implicit: int " << x << " -> double " << y << endl;
    
    // Narrowing (data loss)
    double z = 3.14;
    int w = z;  // double to int (loses fractional part)
    cout << "Narrowing: double " << z << " -> int " << w << endl;
    
    // C++ style casting
    int a = 5;
    double b = static_cast<double>(a);
    cout << "Static cast: " << a << " -> " << b << endl;
}

// ============================================================
// EXAMPLE 7: Scope Demonstration
// ============================================================
void example7_scope() {
    cout << "\n=== EXAMPLE 7: Variable Scope ===" << endl;
    
    int x = 10;
    cout << "Outer scope x = " << x << endl;
    
    {
        int x = 20;  // Shadows outer x
        cout << "Inner scope x = " << x << endl;
    }
    
    cout << "Back to outer scope x = " << x << endl;
}

// ============================================================
// EXAMPLE 8: Literals
// ============================================================
void example8_literals() {
    cout << "\n=== EXAMPLE 8: Literals ===" << endl;
    
    int decimal = 42;
    int octal = 052;          // 0 prefix
    int hex = 0x2A;           // 0x prefix
    int binary = 0b101010;    // 0b prefix (C++14)
    
    cout << "Decimal: " << decimal << endl;
    cout << "Octal: " << octal << endl;
    cout << "Hex: " << hex << endl;
    cout << "Binary: " << binary << endl;
    
    // String literals
    const char* str1 = "C-string";
    string str2 = "C++ string";
    
    cout << "C-string: " << str1 << endl;
    cout << "C++ string: " << str2 << endl;
}

// ============================================================
// EXAMPLE 9: Compound Types
// ============================================================
void example9_compound() {
    cout << "\n=== EXAMPLE 9: Compound Types ===" << endl;
    
    int arr[5] = {1, 2, 3, 4, 5};
    cout << "Array: ";
    for (int val : arr) cout << val << " ";
    cout << endl;
    
    // Pointer
    int x = 42;
    int* ptr = &x;
    cout << "Pointer to x: " << ptr << " (value: " << *ptr << ")" << endl;
    
    // Reference
    int& ref = x;
    ref = 100;
    cout << "After reference assignment: x = " << x << endl;
}

// ============================================================
// EXAMPLE 10: Size of operator
// ============================================================
void example10_sizeof() {
    cout << "\n=== EXAMPLE 10: sizeof Operator ===" << endl;
    
    cout << "sizeof(char): " << sizeof(char) << endl;
    cout << "sizeof(short): " << sizeof(short) << endl;
    cout << "sizeof(int): " << sizeof(int) << endl;
    cout << "sizeof(long): " << sizeof(long) << endl;
    cout << "sizeof(float): " << sizeof(float) << endl;
    cout << "sizeof(double): " << sizeof(double) << endl;
    cout << "sizeof(bool): " << sizeof(bool) << endl;
    
    int arr[10];
    cout << "sizeof(int[10]): " << sizeof(arr) << " bytes" << endl;
}

// ============================================================
// Main function
// ============================================================
int main() {
    cout << "=======================================" << endl;
    cout << "    Chapter 1: Basics & Fundamentals   " << endl;
    cout << "=======================================" << endl;
    
    example1_data_types();
    example2_uniform_init();
    example3_auto();
    example4_constants();
    example5_io();
    example6_casting();
    example7_scope();
    example8_literals();
    example9_compound();
    example10_sizeof();
    
    cout << "\n=======================================" << endl;
    cout << "       All Examples Completed!" << endl;
    cout << "=======================================" << endl;
    
    return 0;
}
