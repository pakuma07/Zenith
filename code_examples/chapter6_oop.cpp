// Chapter 6: Object-Oriented Programming - Code Examples
// Compile: g++ -std=c++11 chapter6_oop.cpp -o chapter6_oop
// Run: ./chapter6_oop

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// EXAMPLE 1: Basic Class Definition
// ============================================================
class Person {
private:
    string name;
    int age;

public:
    // Constructor
    Person(string n, int a) : name(n), age(a) {
        cout << "Person created: " << name << endl;
    }
    
    // Destructor
    ~Person() {
        cout << "Person destroyed: " << name << endl;
    }
    
    // Getter
    string getName() const { return name; }
    int getAge() const { return age; }
    
    // Setter
    void setAge(int a) { 
        if (a > 0) age = a;
    }
    
    // Member function
    void display() const {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};

void example1_basic_class() {
    cout << "\n=== EXAMPLE 1: Basic Class ===" << endl;
    
    Person p1("Alice", 25);
    p1.display();
    
    Person p2("Bob", 30);
    p2.display();
    
    cout << "Bob's age: " << p2.getAge() << endl;
    p2.setAge(31);
    cout << "After birthday: " << p2.getAge() << endl;
}

// ============================================================
// EXAMPLE 2: Access Modifiers
// ============================================================
class Rectangle {
private:
    double width;
    double height;
    
    bool isValid() const {
        return width > 0 && height > 0;
    }

protected:
    void setDimensions(double w, double h) {
        if (w > 0 && h > 0) {
            width = w;
            height = h;
        }
    }

public:
    Rectangle(double w, double h) : width(w), height(h) { }
    
    double getArea() const {
        return isValid() ? width * height : 0;
    }
    
    double getPerimeter() const {
        return isValid() ? 2 * (width + height) : 0;
    }
};

void example2_access() {
    cout << "\n=== EXAMPLE 2: Access Modifiers ===" << endl;
    
    Rectangle r(5, 10);
    cout << "Area: " << r.getArea() << endl;
    cout << "Perimeter: " << r.getPerimeter() << endl;
    
    // r.width = -5;  // ERROR: private
}

// ============================================================
// EXAMPLE 3: Static Members
// ============================================================
class Counter {
private:
    static int count;
    int id;

public:
    Counter() : id(++count) {
        cout << "Counter " << id << " created" << endl;
    }
    
    int getId() const { return id; }
    
    static int getCount() { return count; }
};

int Counter::count = 0;  // Initialize static member

void example3_static() {
    cout << "\n=== EXAMPLE 3: Static Members ===" << endl;
    
    Counter c1;
    Counter c2;
    Counter c3;
    
    cout << "Total counters: " << Counter::getCount() << endl;
    cout << "c1 ID: " << c1.getId() << endl;
    cout << "c3 ID: " << c3.getId() << endl;
}

// ============================================================
// EXAMPLE 4: Const Member Functions
// ============================================================
class Temperature {
private:
    double celsius;

public:
    Temperature(double c) : celsius(c) { }
    
    // Const member function (cannot modify members)
    double getCelsius() const { return celsius; }
    double getFahrenheit() const {
        return celsius * 9.0 / 5.0 + 32.0;
    }
    
    // Non-const function (can modify)
    void setCelsius(double c) { celsius = c; }
};

void example4_const() {
    cout << "\n=== EXAMPLE 4: Const Member Functions ===" << endl;
    
    Temperature t(25);
    cout << "Celsius: " << t.getCelsius() << "°C" << endl;
    cout << "Fahrenheit: " << t.getFahrenheit() << "°F" << endl;
    
    t.setCelsius(30);
    cout << "New Fahrenheit: " << t.getFahrenheit() << "°F" << endl;
}

// ============================================================
// EXAMPLE 5: This Pointer
// ============================================================
class Number {
private:
    int value;

public:
    Number(int v) : value(v) { }
    
    Number add(const Number& other) {
        this->value += other.value;
        return *this;
    }
    
    Number& operator=(const Number& other) {
        if (this != &other) {
            this->value = other.value;
        }
        return *this;
    }
    
    int getValue() const { return value; }
};

void example5_this() {
    cout << "\n=== EXAMPLE 5: This Pointer ===" << endl;
    
    Number n1(10);
    Number n2(5);
    
    n1.add(n2).add(n2);  // Chain calls
    cout << "Result: " << n1.getValue() << endl;
}

// ============================================================
// EXAMPLE 6: Encapsulation
// ============================================================
class BankAccount {
private:
    string accountNumber;
    double balance;
    
    bool isValidAmount(double amount) {
        return amount > 0 && amount <= 1e9;
    }

public:
    BankAccount(string num, double initial) 
        : accountNumber(num), balance(initial) { }
    
    double getBalance() const { return balance; }
    
    bool deposit(double amount) {
        if (isValidAmount(amount)) {
            balance += amount;
            return true;
        }
        return false;
    }
    
    bool withdraw(double amount) {
        if (isValidAmount(amount) && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

void example6_encapsulation() {
    cout << "\n=== EXAMPLE 6: Encapsulation ===" << endl;
    
    BankAccount acc("ACC001", 1000);
    cout << "Initial balance: $" << acc.getBalance() << endl;
    
    acc.deposit(500);
    cout << "After deposit: $" << acc.getBalance() << endl;
    
    acc.withdraw(200);
    cout << "After withdrawal: $" << acc.getBalance() << endl;
    
    bool result = acc.withdraw(2000);
    cout << "Withdraw $2000: " << (result ? "Success" : "Failed") << endl;
}

// ============================================================
// EXAMPLE 7: Constructor Types
// ============================================================
class String {
private:
    char* data;
    int length;

public:
    // Default constructor
    String() : data(nullptr), length(0) { }
    
    // Parameterized constructor
    String(const char* str) {
        if (str) {
            length = strlen(str);
            data = new char[length + 1];
            strcpy(data, str);
        }
    }
    
    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }
    
    // Destructor
    ~String() {
        delete[] data;
    }
    
    void display() const {
        if (data) cout << data << endl;
    }
};

void example7_constructors() {
    cout << "\n=== EXAMPLE 7: Constructor Types ===" << endl;
    
    String s1;                    // Default
    String s2("Hello");           // Parameterized
    String s3 = s2;               // Copy
    
    s1.display();                 // (empty)
    s2.display();                 // Hello
    s3.display();                 // Hello
}

// ============================================================
// EXAMPLE 8: Friend Functions
// ============================================================
class Box {
private:
    int width;
    int height;

public:
    Box(int w, int h) : width(w), height(h) { }
    
    friend void printBox(const Box& b);
    friend bool compareBoxes(const Box& b1, const Box& b2);
};

void printBox(const Box& b) {
    cout << "Box: " << b.width << "x" << b.height << endl;
}

bool compareBoxes(const Box& b1, const Box& b2) {
    return (b1.width * b1.height) == (b2.width * b2.height);
}

void example8_friend() {
    cout << "\n=== EXAMPLE 8: Friend Functions ===" << endl;
    
    Box b1(5, 10);
    Box b2(10, 5);
    Box b3(2, 25);
    
    printBox(b1);
    printBox(b2);
    
    cout << "b1 and b2 same area: " << compareBoxes(b1, b2) << endl;
    cout << "b1 and b3 same area: " << compareBoxes(b1, b3) << endl;
}

// ============================================================
// EXAMPLE 9: Object Lifecycle
// ============================================================
class Logger {
private:
    string name;

public:
    Logger(string n) : name(n) {
        cout << "[Created] " << name << endl;
    }
    
    ~Logger() {
        cout << "[Destroyed] " << name << endl;
    }
    
    void log(string msg) {
        cout << "[" << name << "] " << msg << endl;
    }
};

void example9_lifecycle() {
    cout << "\n=== EXAMPLE 9: Object Lifecycle ===" << endl;
    
    Logger l1("Main");
    l1.log("Program started");
    
    {
        Logger l2("Block");
        l2.log("In block");
    }  // l2 destroyed here
    
    l1.log("After block");
}  // l1 destroyed here

// ============================================================
// EXAMPLE 10: Array of Objects
// ============================================================
void example10_arrays() {
    cout << "\n=== EXAMPLE 10: Array of Objects ===" << endl;
    
    vector<Person> people;
    people.push_back(Person("Alice", 25));
    people.push_back(Person("Bob", 30));
    people.push_back(Person("Charlie", 35));
    
    cout << "\nPeople in array:" << endl;
    for (const auto& person : people) {
        person.display();
    }
}

// ============================================================
// Main function
// ============================================================
int main() {
    cout << "=======================================" << endl;
    cout << "    Chapter 6: Object-Oriented Programming" << endl;
    cout << "=======================================" << endl;
    
    example1_basic_class();
    example2_access();
    example3_static();
    example4_const();
    example5_this();
    example6_encapsulation();
    example7_constructors();
    example8_friend();
    example9_lifecycle();
    example10_arrays();
    
    cout << "\n=======================================" << endl;
    cout << "       All Examples Completed!" << endl;
    cout << "=======================================" << endl;
    
    return 0;
}
