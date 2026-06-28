// Chapter 3: Functions - Code Examples
// Compile: g++ -std=c++11 chapter3_functions.cpp -o chapter3_functions
// Run: ./chapter3_functions

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ============================================================
// EXAMPLE 1: Basic Functions
// ============================================================
int add(int a, int b) {
    return a + b;
}

double divide(double a, double b) {
    if (b == 0) return 0;
    return a / b;
}

void printMessage(string msg) {
    cout << "Message: " << msg << endl;
}

void example1_basic() {
    cout << "\n=== EXAMPLE 1: Basic Functions ===" << endl;
    
    cout << "5 + 3 = " << add(5, 3) << endl;
    cout << "10.0 / 2.0 = " << divide(10.0, 2.0) << endl;
    printMessage("Hello from function!");
}

// ============================================================
// EXAMPLE 2: Pass by Value vs Reference
// ============================================================
void incrementByValue(int x) {
    x++;  // Modifies local copy only
}

void incrementByReference(int& x) {
    x++;  // Modifies original
}

void example2_passing() {
    cout << "\n=== EXAMPLE 2: Pass by Value vs Reference ===" << endl;
    
    int num = 5;
    cout << "Original: " << num << endl;
    
    incrementByValue(num);
    cout << "After pass by value: " << num << " (unchanged)" << endl;
    
    incrementByReference(num);
    cout << "After pass by reference: " << num << " (changed)" << endl;
}

// ============================================================
// EXAMPLE 3: Function Overloading
// ============================================================
void display(int x) {
    cout << "Integer: " << x << endl;
}

void display(double x) {
    cout << "Double: " << x << endl;
}

void display(string x) {
    cout << "String: " << x << endl;
}

int multiply(int a, int b) {
    return a * b;
}

double multiply(double a, double b) {
    return a * b;
}

void example3_overloading() {
    cout << "\n=== EXAMPLE 3: Function Overloading ===" << endl;
    
    display(42);
    display(3.14);
    display("hello");
    
    cout << "int multiply: " << multiply(5, 3) << endl;
    cout << "double multiply: " << multiply(5.5, 2.0) << endl;
}

// ============================================================
// EXAMPLE 4: Default Arguments
// ============================================================
void greet(string name = "Guest", string greeting = "Hello") {
    cout << greeting << ", " << name << "!" << endl;
}

void printRange(int start = 0, int end = 10) {
    for (int i = start; i <= end; i++) {
        cout << i << " ";
    }
    cout << endl;
}

void example4_defaults() {
    cout << "\n=== EXAMPLE 4: Default Arguments ===" << endl;
    
    greet();
    greet("Alice");
    greet("Bob", "Hi");
    
    printRange();
    printRange(1);
    printRange(1, 5);
}

// ============================================================
// EXAMPLE 5: Recursion
// ============================================================
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 2) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

void example5_recursion() {
    cout << "\n=== EXAMPLE 5: Recursion ===" << endl;
    
    cout << "Factorial(5) = " << factorial(5) << endl;
    cout << "Fibonacci(8) = " << fibonacci(8) << endl;
    cout << "2^5 = " << power(2, 5) << endl;
}

// ============================================================
// EXAMPLE 6: Function Pointers
// ============================================================
int operation(int a, int b, int (*func)(int, int)) {
    return func(a, b);
}

int sum(int a, int b) { return a + b; }
int diff(int a, int b) { return a - b; }

void example6_function_pointers() {
    cout << "\n=== EXAMPLE 6: Function Pointers ===" << endl;
    
    cout << "10 + 5 = " << operation(10, 5, sum) << endl;
    cout << "10 - 5 = " << operation(10, 5, diff) << endl;
    
    // Array of function pointers
    int (*ops[])(int, int) = {sum, diff};
    cout << "ops[0](3, 2) = " << ops[0](3, 2) << endl;
    cout << "ops[1](3, 2) = " << ops[1](3, 2) << endl;
}

// ============================================================
// EXAMPLE 7: Lambdas (C++11)
// ============================================================
void example7_lambdas() {
    cout << "\n=== EXAMPLE 7: Lambda Functions (C++11) ===" << endl;
    
    // Basic lambda
    auto add = [](int a, int b) { return a + b; };
    cout << "Lambda add: 5 + 3 = " << add(5, 3) << endl;
    
    // Lambda with capture by value
    int multiplier = 3;
    auto multiply = [multiplier](int x) { return x * multiplier; };
    cout << "Lambda multiply (factor=3): 5 * 3 = " << multiply(5) << endl;
    
    // Lambda with capture by reference
    int counter = 0;
    auto increment = [&counter]() { counter++; };
    increment();
    increment();
    increment();
    cout << "Counter after 3 increments: " << counter << endl;
    
    // Using lambdas with algorithms
    vector<int> v = {1, 2, 3, 4, 5};
    vector<int> squares;
    transform(v.begin(), v.end(), back_inserter(squares),
              [](int x) { return x * x; });
    
    cout << "Original: ";
    for (int x : v) cout << x << " ";
    cout << "\nSquares: ";
    for (int x : squares) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 8: Return by Reference
// ============================================================
string& getLargerString(string& a, string& b) {
    return a.length() > b.length() ? a : b;
}

void example8_return_reference() {
    cout << "\n=== EXAMPLE 8: Return by Reference ===" << endl;
    
    string str1 = "Hello";
    string str2 = "World!";
    
    string& larger = getLargerString(str1, str2);
    cout << "Larger string: " << larger << endl;
    
    larger = "Modified";
    cout << "After modification: " << str2 << endl;
}

// ============================================================
// EXAMPLE 9: Scope of Variables in Functions
// ============================================================
void example9_function_scope() {
    cout << "\n=== EXAMPLE 9: Function Scope ===" << endl;
    
    int x = 10;
    cout << "Outside block: x = " << x << endl;
    
    {
        int x = 20;
        cout << "Inside block: x = " << x << endl;
    }
    
    cout << "Back outside: x = " << x << endl;
    
    // Static variable
    static int staticVar = 0;
    staticVar++;
    cout << "Static var (call 1): " << staticVar << endl;
    staticVar++;
    cout << "Static var (call 2): " << staticVar << endl;
}

// ============================================================
// EXAMPLE 10: Recursion with Memoization
// ============================================================
int fibMemo(int n, vector<int>& memo) {
    if (n <= 2) return 1;
    if (memo[n] != -1) return memo[n];
    
    memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    return memo[n];
}

void example10_memoization() {
    cout << "\n=== EXAMPLE 10: Recursion with Memoization ===" << endl;
    
    int n = 10;
    vector<int> memo(n + 1, -1);
    
    cout << "Fibonacci(" << n << ") with memoization = " 
         << fibMemo(n, memo) << endl;
    
    cout << "Memo array: ";
    for (int val : memo) {
        if (val != -1) cout << val << " ";
    }
    cout << endl;
}

// ============================================================
// Main function
// ============================================================
int main() {
    cout << "=======================================" << endl;
    cout << "        Chapter 3: Functions           " << endl;
    cout << "=======================================" << endl;
    
    example1_basic();
    example2_passing();
    example3_overloading();
    example4_defaults();
    example5_recursion();
    example6_function_pointers();
    example7_lambdas();
    example8_return_reference();
    example9_function_scope();
    example10_memoization();
    
    cout << "\n=======================================" << endl;
    cout << "       All Examples Completed!" << endl;
    cout << "=======================================" << endl;
    
    return 0;
}
