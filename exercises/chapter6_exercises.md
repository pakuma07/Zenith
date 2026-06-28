# Chapter 6: OOP Basics - Practice Exercises

## 1. Class Declaration & Objects

### Exercise 1.1: Simple Class
Create a `Rectangle` class:
- Private members: `width`, `height`
- Public methods: `getWidth()`, `getHeight()`, `setWidth()`, `setHeight()`
- Constructor: `Rectangle(int w, int h)`
- Test with multiple objects

### Exercise 1.2: Class for Point
Create a `Point` class:
- Private: `x`, `y` coordinates
- Public: getters, setters
- Method: `distance()` - calculate distance from origin
- Method: `distanceTo(Point p)` - distance to another point

### Exercise 1.3: Circle Class
Create a `Circle` class:
- Private: `radius`, `centerX`, `centerY`
- Public: `getArea()`, `getCircumference()`, `getRadius()`
- Constructor with parameters
- Const methods for read-only access

## 2. Constructors & Destructors

### Exercise 2.1: Multiple Constructors
Create `Book` class:
- Default constructor (no parameters)
- Parameterized constructor (title, author, pages)
- Constructor with default values
- Test all constructors

### Exercise 2.2: Constructor Initialization List
Create `Person` class:
- Members: `name`, `age`, `email`
- Use initialization list in constructor
- Show order of initialization
- Compare with assignment in body

### Exercise 2.3: Destructor Implementation
Create `FileHandle` class:
- Constructor: allocates resource (simulate with cout)
- Destructor: releases resource
- Show constructor/destructor calls
- Create array of objects to test

## 3. Access Modifiers

### Exercise 3.1: Public, Private, Protected
Create base class and derived class:
- Public members: accessible everywhere
- Private members: only in class
- Protected members: in class and derived
- Test access from different contexts

### Exercise 3.2: Encapsulation
Create `BankAccount` class:
- Private: `balance`, `accountNumber`
- Public: `deposit()`, `withdraw()`, `getBalance()`
- Prevent direct balance manipulation
- Validate operations in public methods

### Exercise 3.3: Data Hiding
Create `Employee` class:
- Hide salary with private member
- Public getter returns base info only
- Public setter validates input
- Show benefits of encapsulation

## 4. Static Members

### Exercise 4.1: Static Variables
Create `Counter` class:
- Static member: `count`
- Constructor increments count
- Static method: `getCount()`
- Create multiple objects and check count

### Exercise 4.2: Static Method
Create `Math` utility class:
- Static methods: `add()`, `subtract()`, `multiply()`, `divide()`
- No object creation needed
- Call via `Math::add()`

### Exercise 4.3: Class Constants
Create `Constants` class:
- Static const members: `PI`, `E`, `MAX_SIZE`
- Use in calculations
- Compare with #define

## 5. Const Member Functions

### Exercise 5.1: Const Methods
Create `Temperature` class:
- Method `getCelsius()` const - read-only
- Method `setFahrenheit(double)` - modifying
- Call both types from const and non-const objects

### Exercise 5.2: Const Objects
Create program with:
- `const Rectangle rect(10, 20)`
- Can only call const methods
- Try to call non-const (should error)
- Show const correctness

### Exercise 5.3: Const References
Pass objects efficiently:
- `void display(const Shape& shape)`
- Cannot modify through const reference
- More efficient than copying
- Use with large objects

## 6. The This Pointer

### Exercise 6.1: This Usage
Create `LinkedListNode` class:
- Data and pointer to next
- Method `getNext()` returns `this->next`
- Method `printNode()` uses `this`
- Show explicit vs implicit this

### Exercise 6.2: This for Chaining
Create `StringBuilder` class:
- Method `append(string)` returns `*this`
- Enable method chaining:
  ```cpp
  builder.append("Hello").append(" ").append("World");
  ```

### Exercise 6.3: Self-Reference Check
Create `Node` class:
- Assignment operator checks `if (this != &other)`
- Prevents self-assignment problems
- Copy data only if not same object

## 7. Object Composition

### Exercise 7.1: Object Membership
Create:
- `Address` class (street, city, zip)
- `Person` class (name, `Address address`)
- Access nested members: `person.address.city`

### Exercise 7.2: Aggregate Objects
Create:
- `Engine` class
- `Car` class contains `Engine`
- `ParkingLot` contains multiple `Car` objects
- Show composition hierarchy

### Exercise 7.3: Delegation
Create:
- `Logger` utility class
- `Application` class uses `Logger`
- Application delegates logging to Logger
- Show "has-a" relationship

## 8. Friend Functions

### Exercise 8.1: Friend Functions
Create `SecretClass` with private members:
- Friend function: `void displaySecret(SecretClass s)`
- Can access private members
- Show access rights

### Exercise 8.2: Friend Classes
Create two classes:
- `Engine` with private data
- `Car` is friend of `Engine`
- Car can access Engine's private members
- Another class cannot

### Exercise 8.3: Operator Friends
Create `Vector` class:
- Overload operator<< as friend
- Access private members
- Print formatted output

## 9. Initialization Order

### Exercise 9.1: Member Initialization Order
Create class with multiple members:
- Initialize in different orders in constructor
- Show that order follows declaration order
- Not initialization list order
- Demonstrate with output

### Exercise 9.2: Array Initialization
Create class containing:
- Array of objects
- Initialize each element in loop
- Show constructor calls for each

### Exercise 9.3: Default Values
Create class with:
- Members with default values
- Constructor overloads
- Show default value usage
- Compare with explicit initialization

## 10. Complete OOP Example

### Exercise 10.1: Student Management
Create `Student` class:
- Members: ID, name, GPA, courses (vector)
- Constructor, destructor
- Methods: addCourse, removeCourse, getGPA
- Grade calculation

### Exercise 10.2: Library System
Create `Book` and `Library` classes:
- Book: title, author, ISBN, available
- Library: collection of Books
- Methods: addBook, removeBook, borrowBook, returnBook
- Show composition and encapsulation

### Exercise 10.3: Game Characters
Create:
- `Character` base concept (health, name)
- `Warrior` and `Mage` variations
- Equipment system (separate class)
- Interaction methods

## Challenge Problems

### Challenge 11.1: Banking System
Create complete bank system:
- `BankAccount` class (account number, balance)
- `Customer` class (personal info, accounts)
- `Bank` class (manages customers and accounts)
- Transactions, validations, reporting

### Challenge 11.2: Graphics System
Create shape hierarchy:
- `Shape` base class (abstract-like)
- `Circle`, `Rectangle`, `Triangle` derived
- Virtual methods: `area()`, `perimeter()`
- Collection of shapes with calculations

### Challenge 11.3: Company Payroll
Create:
- `Employee` base class
- `Manager` and `Developer` variations
- `Department` managing employees
- Salary calculations, overtime handling
- Reporting and statistics

---

## Tips for Solving

1. **Design first**: Sketch class structure on paper
2. **Start simple**: Basic class, then add features
3. **Test methods**: Verify each function works
4. **Use const**: Mark read-only methods
5. **Initialize properly**: Use initialization lists
6. **Encapsulate**: Hide internal details
7. **Compose wisely**: Use objects within objects
8. **Document**: Comment on public interface

## Difficulty Levels
- **Easy**: Exercises 1.1-1.3, 2.1, 3.1-3.2, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1
- **Medium**: Exercises 2.2-2.3, 3.3, 4.2-4.3, 5.2-5.3, 6.2-6.3, 7.2-7.3, 8.2-8.3, 9.2-9.3, 10.1
- **Hard**: Exercises 10.2-10.3, Challenge 11.1-11.3

---

## Object-Oriented Principles

As you solve these exercises, practice:

1. **Encapsulation**: Hide implementation, expose interface
2. **Abstraction**: Focus on what, not how
3. **Inheritance**: Share common functionality (later)
4. **Polymorphism**: Different behavior, same interface (later)

These are foundations for larger OOP projects.

---

## Compilation Reminder

All solutions should compile with:
```bash
g++ -std=c++11 -Wall solution.cpp -o solution
```

---

## Common OOP Mistakes

1. **Direct member access**: Use methods, not public members
2. **Uninitialized members**: Always initialize in constructor
3. **Memory in objects**: Use smart pointers
4. **Mutable static**: Avoid, hard to debug
5. **Circular dependencies**: Design carefully
6. **Too many friends**: Encapsulation defeats purpose
7. **Heavy inheritance**: Prefer composition
