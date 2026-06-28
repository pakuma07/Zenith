# Chapter 7: Inheritance & Polymorphism - Exercises

## Section 1: Basic Inheritance 🟢

1. Create a `Vehicle` base class with:
   - Member variables: `make`, `model`, `year`
   - Methods: `displayInfo()`, `accelerate()`
   - Create derived class `Car` with additional member `numDoors`

2. Implement a `Shape` base class and derive `Circle` and `Rectangle`. Each should have:
   - Virtual method `area()`
   - Virtual method `perimeter()`
   - Non-virtual method `display()`

3. Create a `Person` class and derive `Student` class:
   - `Person` has: name, age
   - `Student` has: student_id, gpa
   - Implement display methods

## Section 2: Virtual Functions & Polymorphism 🟡

4. Create a polymorphic container that stores different shape pointers and calculates total area

5. Write a function that accepts a base class pointer and calls virtual methods polymorphically

6. Implement a `Bank Account` hierarchy:
   - Base: `Account`
   - Derived: `SavingsAccount`, `CheckingAccount`
   - Each with virtual `calculateInterest()` method
   - Demonstrate polymorphic behavior

## Section 3: Abstract Classes 🟡

7. Create an abstract `Employee` class with pure virtual methods:
   - `calculateSalary()`
   - `displayDetails()`
   - Create concrete `Manager` and `Developer` subclasses

8. Design an `Animal` interface with pure virtual `makeSound()` and derive concrete animals

9. Create abstract `DataWriter` class with pure virtual `write()` method:
   - Implement `FileWriter` and `ConsoleWriter`

## Section 4: Multiple Inheritance 🟡

10. Create two base classes `Reader` and `Writer`:
    - Derive a `TextProcessor` that inherits from both
    - Implement all virtual methods

11. Design a `Drawable` interface and `Movable` interface:
    - Create a `Sprite` class inheriting from both
    - Demonstrate multiple inheritance benefits

## Section 5: Constructor & Destructor 🔴

12. Create a hierarchy where constructors/destructors print messages to show call order

13. Implement a base class with parameterized constructor and verify derived class initialization

14. Create a scenario where destructor order matters for resource cleanup

15. Implement virtual destructors and show why they're important 🏆

## Section 6: Protected Members 🟡

16. Create a base class with protected members and show derived class access

17. Design a class hierarchy using protected inheritance

18. Create a scenario showing access levels: public, protected, private

## Section 7: Upcasting & Downcasting 🟡

19. Demonstrate upcasting (safe, implicit):
    - Derive pointer → Base pointer

20. Implement downcasting with `dynamic_cast`:
    - Base pointer → Derive pointer
    - Handle failed cast

21. Compare `dynamic_cast` vs. C-style casts for type safety

## Section 8: Polymorphic Containers 🟡

22. Create a `vector` of base class pointers storing various derived objects

23. Implement a collection that uses polymorphism to call appropriate methods

24. Design a game system with polymorphic `Character` container

## Section 9: Override Keyword (C++11) 🟡

25. Use `override` keyword to ensure methods match base class signatures

26. Create compilation errors without `override` and fix them

27. Demonstrate `final` keyword to prevent further overriding

## Section 10: Virtual Function Pitfalls 🔴

28. Show problematic access to virtual functions from constructors 🏆

29. Demonstrate slicing problem when passing derived objects by value

30. Create example of virtual function call inefficiencies

---

## Tips for Success

- **Virtual functions**: Use for behavior that varies by type
- **Abstract classes**: Define interfaces without implementation
- **RAII in derived classes**: Properly manage resources
- **override**: Always use with C++11 to catch errors
- **Smart pointers**: Use with polymorphic types for safety
- **Multiple inheritance**: Use carefully, prefer composition
- **Dynamic cast**: Needed when you need type information
- **Destructor order**: Base destructor called last

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Basic inheritance, simple polymorphism
- **Medium (🟡)**: 20 exercises - Virtual functions, multiple inheritance, containers
- **Hard (🔴)**: 7 exercises - Advanced patterns, destructor management, pitfalls

## Challenge Problems 🏆

- **Challenge 1**: Virtual destructors and resource cleanup
- **Challenge 2**: Virtual function calls in constructors
- **Challenge 3**: Diamond problem in multiple inheritance

## Expected Time Commitment

- Easy: 5-10 minutes per exercise
- Medium: 15-30 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Learning Outcomes

After completing these exercises, you will:
✓ Understand inheritance hierarchies and design
✓ Master virtual functions and polymorphism
✓ Use abstract classes effectively
✓ Handle multiple inheritance correctly
✓ Manage object lifetime in polymorphic contexts
✓ Use modern C++11 override/final keywords
✓ Avoid common pitfalls with inheritance

## C++11 Exercise Example: Class Basics

```cpp
#include <string>
using namespace std;

class Student {
public:
    string name;
    int age;
    Student(string n, int a) : name(move(n)), age(a) {}
};
```