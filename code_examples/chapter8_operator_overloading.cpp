// Chapter 8: Operator Overloading - Runnable Examples
// Compile with: g++ -std=c++11 chapter8_operator_overloading.cpp -o chapter8_operator_overloading

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// EXAMPLE 1: Arithmetic Operators
// ============================================================
class Complex {
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    Complex operator+(const Complex& c) const {
        return Complex(real + c.real, imag + c.imag);
    }
    
    Complex operator-(const Complex& c) const {
        return Complex(real - c.real, imag - c.imag);
    }
    
    void display() const {
        cout << real;
        if (imag >= 0) cout << " + ";
        cout << imag << "i" << endl;
    }
};

void example1_arithmetic_operators() {
    cout << "\n=== EXAMPLE 1: Arithmetic Operators ===" << endl;
    
    Complex c1(3, 4), c2(1, 2);
    Complex sum = c1 + c2;
    Complex diff = c1 - c2;
    
    cout << "c1: "; c1.display();
    cout << "c2: "; c2.display();
    cout << "Sum: "; sum.display();
    cout << "Difference: "; diff.display();
}

// ============================================================
// EXAMPLE 2: Comparison Operators
// ============================================================
class Point {
private:
    int x, y;
public:
    Point(int a = 0, int b = 0) : x(a), y(b) {}
    
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
    
    bool operator<(const Point& p) const {
        return (x * x + y * y) < (p.x * p.x + p.y * p.y);
    }
    
    bool operator>(const Point& p) const {
        return (x * x + y * y) > (p.x * p.x + p.y * p.y);
    }
    
    void display() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

void example2_comparison_operators() {
    cout << "\n=== EXAMPLE 2: Comparison Operators ===" << endl;
    
    Point p1(3, 4), p2(3, 4), p3(1, 1);
    
    cout << "p1: "; p1.display();
    cout << "p2: "; p2.display();
    cout << "p3: "; p3.display();
    
    cout << "p1 == p2? " << (p1 == p2 ? "Yes" : "No") << endl;
    cout << "p1 < p3? " << (p1 < p3 ? "Yes" : "No") << endl;
    cout << "p3 < p1? " << (p3 < p1 ? "Yes" : "No") << endl;
}

// ============================================================
// EXAMPLE 3: Subscript Operator
// ============================================================
class Array {
private:
    int* data;
    int size;
public:
    Array(int s) : size(s) {
        data = new int[s];
        for (int i = 0; i < s; i++) data[i] = 0;
    }
    
    ~Array() { delete[] data; }
    
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return data[0];
        }
        return data[index];
    }
    
    const int& operator[](int index) const {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return data[0];
        }
        return data[index];
    }
};

void example3_subscript_operator() {
    cout << "\n=== EXAMPLE 3: Subscript Operator ===" << endl;
    
    Array arr(5);
    
    // Set values
    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
    }
    
    // Get values
    cout << "Array contents: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 4: Increment/Decrement Operators
// ============================================================
class Counter {
private:
    int value;
public:
    Counter(int v = 0) : value(v) {}
    
    Counter& operator++() {        // Pre-increment
        ++value;
        return *this;
    }
    
    Counter operator++(int) {      // Post-increment
        Counter temp(*this);
        ++value;
        return temp;
    }
    
    Counter& operator--() {        // Pre-decrement
        --value;
        return *this;
    }
    
    Counter operator--(int) {      // Post-decrement
        Counter temp(*this);
        --value;
        return temp;
    }
    
    int getValue() const { return value; }
};

void example4_increment_operators() {
    cout << "\n=== EXAMPLE 4: Increment/Decrement Operators ===" << endl;
    
    Counter c(5);
    
    cout << "Initial: " << c.getValue() << endl;
    cout << "Pre-increment: " << (++c).getValue() << endl;
    cout << "Post-increment: " << (c++).getValue() << " (returns old)" << endl;
    cout << "After post-increment: " << c.getValue() << endl;
    cout << "Pre-decrement: " << (--c).getValue() << endl;
}

// ============================================================
// EXAMPLE 5: Stream Operators (<<, >>)
// ============================================================
class Rational {
private:
    int numerator, denominator;
public:
    Rational(int n = 0, int d = 1) : numerator(n), denominator(d) {}
    
    friend ostream& operator<<(ostream& os, const Rational& r);
    friend istream& operator>>(istream& is, Rational& r);
};

ostream& operator<<(ostream& os, const Rational& r) {
    os << r.numerator << "/" << r.denominator;
    return os;
}

istream& operator>>(istream& is, Rational& r) {
    char slash;
    is >> r.numerator >> slash >> r.denominator;
    return is;
}

void example5_stream_operators() {
    cout << "\n=== EXAMPLE 5: Stream Operators ===" << endl;
    
    Rational r1(3, 4), r2(5, 6);
    
    cout << "Rational numbers: " << r1 << " and " << r2 << endl;
    cout << "Custom output format works!" << endl;
}

// ============================================================
// EXAMPLE 6: Assignment Operator
// ============================================================
class String {
private:
    char* str;
    int len;
public:
    String(const char* s = "") {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    
    ~String() { delete[] str; }
    
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] str;
            len = other.len;
            str = new char[len + 1];
            strcpy(str, other.str);
        }
        return *this;
    }
    
    void display() const { cout << str << endl; }
};

void example6_assignment_operator() {
    cout << "\n=== EXAMPLE 6: Assignment Operator ===" << endl;
    
    String s1("Hello");
    String s2("World");
    
    cout << "s1: "; s1.display();
    cout << "s2: "; s2.display();
    
    s1 = s2;
    cout << "After s1 = s2:" << endl;
    cout << "s1: "; s1.display();
}

// ============================================================
// EXAMPLE 7: Function Call Operator ()
// ============================================================
class Multiplier {
private:
    int factor;
public:
    Multiplier(int f) : factor(f) {}
    
    int operator()(int x) const {
        return x * factor;
    }
};

void example7_function_call_operator() {
    cout << "\n=== EXAMPLE 7: Function Call Operator ===" << endl;
    
    Multiplier times3(3);
    Multiplier times5(5);
    
    cout << "times3(10) = " << times3(10) << endl;
    cout << "times5(10) = " << times5(10) << endl;
    
    cout << "Objects can be used like functions!" << endl;
}

// ============================================================
// EXAMPLE 8: Type Conversion Operators
// ============================================================
class Temperature {
private:
    double celsius;
public:
    Temperature(double c = 0) : celsius(c) {}
    
    operator double() const { return celsius; }
    operator int() const { return (int)celsius; }
};

void example8_type_conversion() {
    cout << "\n=== EXAMPLE 8: Type Conversion Operators ===" << endl;
    
    Temperature t(98.6);
    
    double d = (double)t;
    int i = (int)t;
    
    cout << "Temperature: " << d << " degrees" << endl;
    cout << "As integer: " << i << " degrees" << endl;
}

// ============================================================
// EXAMPLE 9: Member Access Operators
// ============================================================
class Vector3D {
private:
    double x, y, z;
public:
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vector3D* operator->() { return this; }
    const Vector3D* operator->() const { return this; }
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

void example9_member_access() {
    cout << "\n=== EXAMPLE 9: Member Access Operators ===" << endl;
    
    Vector3D v(1, 2, 3);
    Vector3D* ptr = &v;
    
    cout << "Using arrow operator:" << endl;
    cout << "X: " << ptr->getX() << endl;
    cout << "Y: " << ptr->getY() << endl;
    cout << "Z: " << ptr->getZ() << endl;
}

// ============================================================
// EXAMPLE 10: Logical Operators
// ============================================================
class Condition {
private:
    bool value;
public:
    Condition(bool v = false) : value(v) {}
    
    Condition operator&&(const Condition& c) const {
        return Condition(value && c.value);
    }
    
    Condition operator||(const Condition& c) const {
        return Condition(value || c.value);
    }
    
    Condition operator!() const {
        return Condition(!value);
    }
    
    bool getValue() const { return value; }
};

void example10_logical_operators() {
    cout << "\n=== EXAMPLE 10: Logical Operators ===" << endl;
    
    Condition c1(true), c2(false), c3(true);
    
    cout << "c1: " << c1.getValue() << ", c2: " << c2.getValue() << endl;
    cout << "c1 && c3: " << (c1 && c3).getValue() << endl;
    cout << "c1 || c2: " << (c1 || c2).getValue() << endl;
    cout << "!c2: " << (!c2).getValue() << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 8: OPERATOR OVERLOADING (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_arithmetic_operators();
    example2_comparison_operators();
    example3_subscript_operator();
    example4_increment_operators();
    example5_stream_operators();
    example6_assignment_operator();
    example7_function_call_operator();
    example8_type_conversion();
    example9_member_access();
    example10_logical_operators();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Arithmetic Operators**: +, -, *, /
   - Binary operators (return new object)
   - Member functions or friend functions

2. **Comparison Operators**: ==, <, >, !=, <=, >=
   - Usually member functions
   - Return bool

3. **Subscript Operator**: []
   - Enables array-like access
   - Return reference for modification

4. **Increment/Decrement**: ++, --
   - Pre-increment: ++x (increment, return reference)
   - Post-increment: x++ (increment, return old value)
   - Different signatures (int dummy parameter)

5. **Stream Operators**: <<, >>
   - Usually friend functions
   - Return stream for chaining

6. **Assignment Operator**: =
   - Only as member function
   - Self-assignment check important

7. **Function Call Operator**: ()
   - Makes objects callable like functions
   - Useful for function objects (functors)

8. **Type Conversion**: Implicit or explicit
   - operator type() for conversion
   - Enables automatic type casting

9. **Member Access**: ->, *
   - Arrow for pointer dereference
   - Useful for smart pointers

10. **Logical Operators**: &&, ||, !
    - Can be overloaded (though carefully)
    - Don't short-circuit like built-in
*/
