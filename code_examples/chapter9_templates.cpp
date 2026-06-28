// Chapter 9: Templates - Runnable Examples
// Compile with: g++ -std=c++11 chapter9_templates.cpp -o chapter9_templates

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// ============================================================
// EXAMPLE 1: Function Templates
// ============================================================
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

void example1_function_templates() {
    cout << "\n=== EXAMPLE 1: Function Templates ===" << endl;
    
    cout << "Maximum of 5 and 3: " << maximum(5, 3) << endl;
    cout << "Maximum of 3.5 and 2.1: " << maximum(3.5, 2.1) << endl;
    cout << "Maximum of \"apple\" and \"banana\": " << maximum("apple", "banana") << endl;
    
    int x = 10, y = 20;
    cout << "Before swap: x=" << x << ", y=" << y << endl;
    swap(x, y);
    cout << "After swap: x=" << x << ", y=" << y << endl;
}

// ============================================================
// EXAMPLE 2: Class Templates
// ============================================================
template <typename T>
class Stack {
private:
    vector<T> data;
public:
    void push(const T& value) {
        data.push_back(value);
    }
    
    T pop() {
        T val = data.back();
        data.pop_back();
        return val;
    }
    
    bool empty() const { return data.empty(); }
    int size() const { return data.size(); }
};

void example2_class_templates() {
    cout << "\n=== EXAMPLE 2: Class Templates ===" << endl;
    
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    
    cout << "Integer stack size: " << intStack.size() << endl;
    cout << "Pop: " << intStack.pop() << endl;
    cout << "Pop: " << intStack.pop() << endl;
    
    Stack<string> stringStack;
    stringStack.push("Hello");
    stringStack.push("World");
    
    cout << "String stack size: " << stringStack.size() << endl;
    cout << "Pop: " << stringStack.pop() << endl;
}

// ============================================================
// EXAMPLE 3: Template Specialization
// ============================================================
template <typename T>
void display(T value) {
    cout << "Generic: " << value << endl;
}

template <>
void display(bool value) {
    cout << "Specialized for bool: " << (value ? "true" : "false") << endl;
}

template <>
void display(const char* value) {
    cout << "Specialized for string: \"" << value << "\"" << endl;
}

void example3_template_specialization() {
    cout << "\n=== EXAMPLE 3: Template Specialization ===" << endl;
    
    display(42);
    display(3.14);
    display(true);
    display("Hello");
}

// ============================================================
// EXAMPLE 4: Multiple Template Parameters
// ============================================================
template <typename K, typename V>
class Pair {
private:
    K key;
    V value;
public:
    Pair(const K& k, const V& v) : key(k), value(v) {}
    
    K getKey() const { return key; }
    V getValue() const { return value; }
    
    void display() const {
        cout << "Key: " << key << ", Value: " << value << endl;
    }
};

void example4_multiple_parameters() {
    cout << "\n=== EXAMPLE 4: Multiple Template Parameters ===" << endl;
    
    Pair<string, int> p1("age", 25);
    Pair<string, double> p2("height", 5.9);
    Pair<int, string> p3(1, "one");
    
    p1.display();
    p2.display();
    p3.display();
}

// ============================================================
// EXAMPLE 5: Template with Default Parameters
// ============================================================
template <typename T = int, int SIZE = 10>
class Array {
private:
    T data[SIZE];
    int count;
public:
    Array() : count(0) {}
    
    void add(const T& value) {
        if (count < SIZE) data[count++] = value;
    }
    
    void display() {
        cout << "Array (" << count << " elements): ";
        for (int i = 0; i < count; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

void example5_default_parameters() {
    cout << "\n=== EXAMPLE 5: Template with Default Parameters ===" << endl;
    
    Array<> a1;      // Uses default int and size 10
    a1.add(1);
    a1.add(2);
    a1.add(3);
    a1.display();
    
    Array<double, 5> a2;  // Custom size
    a2.add(1.1);
    a2.add(2.2);
    a2.display();
}

// ============================================================
// EXAMPLE 6: Variadic Templates (C++11)
// ============================================================
void printAll() {
    cout << endl;
}

template <typename T, typename... Args>
void printAll(T first, Args... rest) {
    cout << first << " ";
    printAll(rest...);
}

void example6_variadic_templates() {
    cout << "\n=== EXAMPLE 6: Variadic Templates (C++11) ===" << endl;
    
    cout << "Printing multiple values: ";
    printAll(1, 2.5, "hello", true, 42);
}

// ============================================================
// EXAMPLE 7: Template with Constraints
// ============================================================
template <typename T>
typename enable_if<is_integral<T>::value, void>::type
processInteger(T value) {
    cout << "Integer: " << value << endl;
}

template <typename T>
typename enable_if<is_floating_point<T>::value, void>::type
processNumber(T value) {
    cout << "Floating point: " << value << endl;
}

void example7_template_constraints() {
    cout << "\n=== EXAMPLE 7: Template with Constraints ===" << endl;
    
    processInteger(42);
    processNumber(3.14);
}

// ============================================================
// EXAMPLE 8: Partial Template Specialization
// ============================================================
template <typename T>
void process(T value) {
    cout << "Single template parameter" << endl;
}

template <typename T>
void process(T* pointer) {
    cout << "Pointer template" << endl;
}

template <typename T>
void process(vector<T>& vec) {
    cout << "Vector template" << endl;
}

void example8_partial_specialization() {
    cout << "\n=== EXAMPLE 8: Partial Template Specialization ===" << endl;
    
    int x = 5;
    int* ptr = &x;
    vector<int> v = {1, 2, 3};
    
    process(x);
    process(ptr);
    process(v);
}

// ============================================================
// EXAMPLE 9: Template Functions with Templates
// ============================================================
template <typename T>
class Container {
private:
    vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    
    template <typename Predicate>
    int count_if(Predicate pred) {
        int cnt = 0;
        for (const T& item : items) {
            if (pred(item)) cnt++;
        }
        return cnt;
    }
};

void example9_template_with_templates() {
    cout << "\n=== EXAMPLE 9: Template Functions with Templates ===" << endl;
    
    Container<int> c;
    c.add(1);
    c.add(2);
    c.add(3);
    c.add(4);
    c.add(5);
    
    int even_count = c.count_if([](int x) { return x % 2 == 0; });
    cout << "Even numbers: " << even_count << endl;
}

// ============================================================
// EXAMPLE 10: SFINAE (Substitution Failure Is Not An Error)
// ============================================================
template <typename T>
typename enable_if<sizeof(T) <= 4, string>::type
getSize(T) {
    return "Small";
}

template <typename T>
typename enable_if<(sizeof(T) > 4 && sizeof(T) <= 8), string>::type
getSize(T) {
    return "Medium";
}

template <typename T>
typename enable_if<sizeof(T) > 8, string>::type
getSize(T) {
    return "Large";
}

void example10_sfinae() {
    cout << "\n=== EXAMPLE 10: SFINAE (Substitution Failure) ===" << endl;
    
    char c;
    int i;
    long long ll;
    double d;
    
    cout << "sizeof(char) = " << sizeof(c) << " - " << getSize(c) << endl;
    cout << "sizeof(int) = " << sizeof(i) << " - " << getSize(i) << endl;
    cout << "sizeof(long long) = " << sizeof(ll) << " - " << getSize(ll) << endl;
    cout << "sizeof(double) = " << sizeof(d) << " - " << getSize(d) << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 9: TEMPLATES (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_function_templates();
    example2_class_templates();
    example3_template_specialization();
    example4_multiple_parameters();
    example5_default_parameters();
    example6_variadic_templates();
    example7_template_constraints();
    example8_partial_specialization();
    example9_template_with_templates();
    example10_sfinae();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Function Templates**: Generic functions
   - Template parameter: typename T
   - Compiler instantiates for each type

2. **Class Templates**: Generic classes
   - Each specialization is separate class
   - Can have multiple template parameters

3. **Template Specialization**: Override for specific types
   - Full specialization for exact types
   - Partial specialization for patterns

4. **Multiple Parameters**: More flexibility
   - Different types for different purposes
   - Named independently

5. **Default Parameters**: Sensible defaults
   - Reduces boilerplate
   - Can override when needed

6. **Variadic Templates**: Variable arguments
   - Recursive instantiation
   - Modern C++11 feature

7. **Template Constraints**: SFINAE/enable_if
   - Restrict instantiation
   - Type traits for conditions

8. **Partial Specialization**: Pattern matching
   - Match pointer types
   - Match container types

9. **Nested Templates**: Templates in templates
   - Template member functions
   - Powerful for generic algorithms

10. **SFINAE**: Elegant error handling
    - Substitution failure = no error
    - Multiple overloads tried
    - Type-safe dispatch
*/
