// Chapter 7: Inheritance & Polymorphism - Runnable Examples
// Compile with: g++ -std=c++11 chapter7_inheritance_polymorphism.cpp -o chapter7_inheritance_polymorphism

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// EXAMPLE 1: Basic Inheritance
// ============================================================
class Animal {
protected:
    string name;
public:
    Animal(string n) : name(n) {}
    void sleep() { cout << name << " is sleeping" << endl; }
    virtual void speak() { cout << name << " makes a sound" << endl; }
    virtual ~Animal() { cout << name << " destroyed" << endl; }
};

class Dog : public Animal {
public:
    Dog(string n) : Animal(n) {}
    void speak() override { cout << name << " barks: Woof!" << endl; }
    void fetch() { cout << name << " fetches the ball" << endl; }
};

class Cat : public Animal {
public:
    Cat(string n) : Animal(n) {}
    void speak() override { cout << name << " meows: Meow!" << endl; }
};

void example1_basic_inheritance() {
    cout << "\n=== EXAMPLE 1: Basic Inheritance ===" << endl;
    
    Dog dog("Rex");
    Cat cat("Whiskers");
    
    dog.speak();
    dog.sleep();
    dog.fetch();
    
    cat.speak();
    cat.sleep();
}

// ============================================================
// EXAMPLE 2: Virtual Functions & Polymorphism
// ============================================================
void example2_virtual_functions() {
    cout << "\n=== EXAMPLE 2: Virtual Functions & Polymorphism ===" << endl;
    
    Animal* animals[] = {
        new Dog("Buddy"),
        new Cat("Mittens"),
        new Dog("Max")
    };
    
    cout << "Calling speak() through Animal pointers:" << endl;
    for (int i = 0; i < 3; i++) {
        animals[i]->speak();  // Calls correct derived class method
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        delete animals[i];  // Virtual destructor called
    }
}

// ============================================================
// EXAMPLE 3: Abstract Base Classes
// ============================================================
class Shape {
public:
    virtual double area() const = 0;        // Pure virtual
    virtual double perimeter() const = 0;   // Pure virtual
    virtual void display() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
    double perimeter() const override { return 2 * 3.14159 * radius; }
    void display() const override {
        cout << "Circle: radius=" << radius << ", area=" << area() << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    void display() const override {
        cout << "Rectangle: " << width << "x" << height << ", area=" << area() << endl;
    }
};

void example3_abstract_classes() {
    cout << "\n=== EXAMPLE 3: Abstract Base Classes ===" << endl;
    
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5));
    shapes.push_back(make_unique<Rectangle>(3, 4));
    shapes.push_back(make_unique<Circle>(2));
    
    for (auto& shape : shapes) {
        shape->display();
    }
}

// ============================================================
// EXAMPLE 4: Multiple Inheritance
// ============================================================
class Swimmer {
public:
    virtual void swim() { cout << "Swimming" << endl; }
};

class Flyer {
public:
    virtual void fly() { cout << "Flying" << endl; }
};

class Duck : public Swimmer, public Flyer {
public:
    void swim() override { cout << "Duck swimming" << endl; }
    void fly() override { cout << "Duck flying" << endl; }
    void quack() { cout << "Duck quacking: Quack!" << endl; }
};

void example4_multiple_inheritance() {
    cout << "\n=== EXAMPLE 4: Multiple Inheritance ===" << endl;
    
    Duck duck;
    duck.swim();
    duck.fly();
    duck.quack();
    
    Swimmer* s = &duck;
    Flyer* f = &duck;
    s->swim();
    f->fly();
}

// ============================================================
// EXAMPLE 5: Constructor & Destructor in Inheritance
// ============================================================
class Vehicle {
protected:
    string type;
public:
    Vehicle(string t) : type(t) { cout << "Vehicle constructor: " << t << endl; }
    virtual ~Vehicle() { cout << "Vehicle destructor" << endl; }
};

class Car : public Vehicle {
private:
    int doors;
public:
    Car(string t, int d) : Vehicle(t), doors(d) { cout << "Car constructor: " << doors << " doors" << endl; }
    ~Car() { cout << "Car destructor" << endl; }
};

void example5_constructors_destructors() {
    cout << "\n=== EXAMPLE 5: Constructor & Destructor in Inheritance ===" << endl;
    
    {
        Car myCar("Sedan", 4);
        cout << "Car created" << endl;
    }  // Destructors called in reverse order
}

// ============================================================
// EXAMPLE 6: Protected Members
// ============================================================
class Account {
protected:
    double balance;
public:
    Account(double b) : balance(b) {}
    virtual void display() const { cout << "Balance: $" << balance << endl; }
};

class SavingsAccount : public Account {
private:
    double interest_rate;
public:
    SavingsAccount(double b, double r) : Account(b), interest_rate(r) {}
    void addInterest() {
        balance += balance * interest_rate;  // Access protected member
        cout << "Interest added. New balance: $" << balance << endl;
    }
    void display() const override {
        cout << "Savings Account - Balance: $" << balance << ", Rate: " << interest_rate * 100 << "%" << endl;
    }
};

void example6_protected_members() {
    cout << "\n=== EXAMPLE 6: Protected Members ===" << endl;
    
    SavingsAccount account(1000, 0.05);
    account.display();
    account.addInterest();
    account.display();
}

// ============================================================
// EXAMPLE 7: Virtual Destructors (IMPORTANT!)
// ============================================================
class Base {
public:
    Base() { cout << "Base constructor" << endl; }
    virtual ~Base() { cout << "Base destructor" << endl; }
};

class Derived : public Base {
private:
    int* data;
public:
    Derived() : data(new int[10]) { cout << "Derived constructor" << endl; }
    ~Derived() { cout << "Derived destructor"; delete[] data; cout << " - cleaned up" << endl; }
};

void example7_virtual_destructors() {
    cout << "\n=== EXAMPLE 7: Virtual Destructors ===" << endl;
    
    cout << "Without virtual destructor (simulated - would leak memory):" << endl;
    {
        Base* ptr = new Derived();
        delete ptr;  // Derived destructor is called because Base::~Base() is virtual
    }
}

// ============================================================
// EXAMPLE 8: Upcasting & Downcasting
// ============================================================
void example8_casting() {
    cout << "\n=== EXAMPLE 8: Upcasting & Downcasting ===" << endl;
    
    // Upcasting (always safe)
    Dog dog("Spot");
    Animal* animal = &dog;  // Upcasting
    animal->speak();
    
    // Downcasting (requires care)
    Dog* dog_ptr = dynamic_cast<Dog*>(animal);
    if (dog_ptr) {
        cout << "Downcasting succeeded, calling fetch()" << endl;
        dog_ptr->fetch();
    }
    
    // Failed downcast
    Cat cat("Felix");
    Animal* cat_animal = &cat;
    Dog* failed_cast = dynamic_cast<Dog*>(cat_animal);
    if (failed_cast == nullptr) {
        cout << "Downcasting failed (as expected)" << endl;
    }
}

// ============================================================
// EXAMPLE 9: Polymorphic Container
// ============================================================
void example9_polymorphic_container() {
    cout << "\n=== EXAMPLE 9: Polymorphic Container ===" << endl;
    
    vector<unique_ptr<Animal>> animals;
    animals.push_back(make_unique<Dog>("Fido"));
    animals.push_back(make_unique<Cat>("Tom"));
    animals.push_back(make_unique<Dog>("Pax"));
    animals.push_back(make_unique<Cat>("Sylvester"));
    
    cout << "All animals speak:" << endl;
    for (auto& animal : animals) {
        animal->speak();
    }
    
    cout << "\nAll animals sleep:" << endl;
    for (auto& animal : animals) {
        animal->sleep();
    }
}

// ============================================================
// EXAMPLE 10: Override Keyword (C++11)
// ============================================================
class Base10 {
public:
    virtual void method() { cout << "Base method" << endl; }
    virtual ~Base10() {}
};

class Derived10 : public Base10 {
public:
    void method() override { cout << "Derived method (override keyword ensures correct)"; endl; }
};

void example10_override_keyword() {
    cout << "\n=== EXAMPLE 10: Override Keyword (C++11) ===" << endl;
    
    Base10* ptr = new Derived10();
    ptr->method();  // Calls Derived10::method()
    delete ptr;
    
    cout << "override keyword prevents accidental errors" << endl;
    cout << "If method signature didn't match, compiler would error" << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 7: INHERITANCE & POLYMORPHISM (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_basic_inheritance();
    example2_virtual_functions();
    example3_abstract_classes();
    example4_multiple_inheritance();
    example5_constructors_destructors();
    example6_protected_members();
    example7_virtual_destructors();
    example8_casting();
    example9_polymorphic_container();
    example10_override_keyword();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Basic Inheritance**: Derive classes from base
   - Reuse code through inheritance
   - Override methods in derived classes

2. **Virtual Functions**: Enable polymorphism
   - Base pointer/reference calls derived implementation
   - Dynamic dispatch at runtime

3. **Abstract Base Classes**: Define interfaces
   - Pure virtual functions (= 0)
   - Cannot instantiate abstract classes
   - Define contract for derived classes

4. **Multiple Inheritance**: Inherit from multiple bases
   - Powerful but complex
   - Watch for diamond problem

5. **Constructors/Destructors**: Initialization order
   - Base constructor called first
   - Derived constructor called after
   - Destructors called in reverse

6. **Protected Members**: Inheritance-specific access
   - Inaccessible outside class hierarchy
   - Accessible to derived classes
   - Weaker than private

7. **Virtual Destructors**: Always virtual in base
   - Ensures proper cleanup
   - Derived destructor always called through pointer

8. **Upcasting/Downcasting**: Type conversions
   - Upcasting always safe (implicit)
   - Downcasting needs dynamic_cast for safety
   - Check for null after dynamic_cast

9. **Polymorphic Containers**: Store different types
   - Use base class pointers/smart_ptr
   - Enables generic algorithms
   - Powerful for extensibility

10. **Override Keyword**: C++11 safety
    - Compiler checks correct signature
    - Prevents accidental new virtual method
    - Makes intent explicit
*/
