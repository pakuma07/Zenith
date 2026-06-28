# Chapter 4: Pointers & References - Practice Exercises

## 1. Pointer Basics

### Exercise 1.1: Pointer Declaration & Usage
Create a program that:
- Declares int variable x = 42
- Creates pointer ptr pointing to x
- Print value of x, address of x, value via pointer
- Change x through pointer and verify

### Exercise 1.2: Pointer Arithmetic
Write code demonstrating:
- Increment/decrement pointers
- Add/subtract offsets to pointers
- Pointer difference calculation
- Array element access via pointer arithmetic

### Exercise 1.3: Null and Void Pointers
Create a program showing:
- Null pointer initialization
- Checking for null before dereference
- Void pointer (generic pointer)
- Type casting void pointers

## 2. Pointers to Arrays

### Exercise 2.1: Array as Pointer
Given array int arr[] = {10, 20, 30, 40, 50}:
- Access elements using pointer notation
- Print all elements using pointer and loop
- Modify elements via pointer
- Calculate array size using pointers

### Exercise 2.2: Pointer to Array
Write code with:
- Declare pointer to entire array
- Access elements through pointer
- Modify elements
- Compare with pointer to first element

### Exercise 2.3: Dynamic Arrays
Create program that:
- Allocates dynamic array of size n
- Takes input and fills array
- Prints array
- Deallocates properly

## 3. Pointer to Pointers

### Exercise 3.1: Double Pointer
Create program demonstrating:
- Pointer to pointer declaration
- Access original value through double pointer
- Modify values
- Print addresses at each level

### Exercise 3.2: Memory Levels
Show memory hierarchy:
- Variable x
- Pointer to x (ptr1)
- Pointer to ptr1 (ptr2)
- Print addresses and values at each level

### Exercise 3.3: Function with Pointer to Pointer
Write function `swap(int** a, int** b)`:
- Swaps pointers themselves (not values)
- Show difference from regular swap
- Demonstrate use case

## 4. References

### Exercise 4.1: Reference Basics
Create program that:
- Declare int x = 10
- Create reference ref = x
- Modify via reference
- Show that ref always refers to x
- Cannot create null reference

### Exercise 4.2: References vs Pointers
Compare both:
- Declare pointer and reference to same variable
- Modify through both
- Show differences in syntax
- Explain when to use each

### Exercise 4.3: Const References
Write code using:
- `const int& ref = x` - cannot modify original
- Efficient parameter passing for large objects
- Use in function parameters

## 5. Function Parameters

### Exercise 5.1: Pass by Value
Create function `modify(int x)`:
- Takes value, modifies local copy
- Original unchanged
- Show in main(): original not modified

### Exercise 5.2: Pass by Reference
Create two functions:
- `void swap(int& a, int& b)` - actually swaps
- `void modify(int& x)` - modifies original
- Show difference from pass by value

### Exercise 5.3: Pass by Const Reference
Write functions with:
- `void print(const string& str)` - efficient, safe
- Avoid unnecessary copying
- Apply to vector, map parameters

## 6. Function Pointers

### Exercise 6.1: Function Pointer Declaration
Create functions:
- `int add(int a, int b)`
- `int subtract(int a, int b)`
- Store in function pointer
- Call through pointer

### Exercise 6.2: Function Pointer Array
Create array of function pointers:
- `int (*ops[])(int, int) = {add, subtract, multiply, divide}`
- Call different operations via array
- Implement menu system

### Exercise 6.3: Function Pointer as Parameter
Write function:
- `int applyOperation(int a, int b, int (*func)(int, int))`
- Pass different function pointers
- Create callback system

## 7. Dynamic Memory Management

### Exercise 7.1: New and Delete
Create program:
- Allocate int with `new`
- Allocate array with `new[]`
- Deallocate with `delete` and `delete[]`
- Proper cleanup

### Exercise 7.2: Memory Leaks
Write deliberately leaky code:
- Allocate but don't delete
- Show detection strategies
- Rewrite with proper cleanup
- Use valgrind or similar tools

### Exercise 7.3: Dynamic Data Structures
Create:
- Dynamically allocated vector of pointers
- Dynamically allocated structures
- Proper initialization and cleanup
- Check for null pointers

## 8. Pointers and Structures

### Exercise 8.1: Structure Pointer
Create structure Person:
- Allocate dynamically
- Access members via pointer: `ptr->member`
- Access via dereference: `(*ptr).member`
- Show both syntaxes

### Exercise 8.2: Array of Structures
Create:
- Array of Person structures
- Access each person and their members
- Modify through pointers
- Print all records

### Exercise 8.3: Structure with Pointers
Create LinkedListNode:
- Contains pointer to next node
- Create chain of nodes
- Traverse chain
- Deallocate properly

## 9. Strings and Character Pointers

### Exercise 9.1: Char Pointer vs String
Compare:
- `char* str = "Hello"` - pointer to string literal
- `string s = "Hello"` - string object
- Different operations on each
- Safety considerations

### Exercise 9.2: Character Array
Create:
- `char str[50]` - character array
- `char* ptr = str` - pointer to array
- Access characters via pointer
- Modify array

### Exercise 9.3: String Operations with Pointers
Write functions:
- `int strlen(char* str)` - length
- `void strcpy(char* dest, char* src)` - copy
- `int strcmp(char* s1, char* s2)` - compare
- Implement without using library functions

## 10. Smart Pointers (C++11)

### Exercise 10.1: Unique Pointer
Create program using:
- `unique_ptr<int> ptr(new int(42))`
- Transfer ownership with `move()`
- Automatic cleanup
- No manual `delete`

### Exercise 10.2: Shared Pointer
Create program with:
- `shared_ptr<string> ptr1(new string("Hello"))`
- Multiple owners pointing to same data
- Check reference count with `use_count()`
- Automatic cleanup when count reaches 0

### Exercise 10.3: Smart Pointer with Structures
Create:
- `shared_ptr<Person> p1(new Person("Alice", 25))`
- Array of smart pointers
- Automatic memory management
- No memory leaks

## 11. Pointer Pitfalls & Best Practices

### Exercise 11.1: Dangling Pointers
Demonstrate the problem:
- Pointer after deletion
- Prevent with nullptr assignment
- Rewrite using smart pointers

### Exercise 11.2: Memory Corruption
Show risks of:
- Accessing uninitialized pointers
- Buffer overflow
- Out-of-bounds access
- Fixes and best practices

### Exercise 11.3: Resource Management
Create class with:
- Raw pointers (show problems)
- Smart pointers (show benefits)
- Rule of five implementation
- RAII principle

## Challenge Problems

### Challenge 12.1: Circular Linked List
Create:
- Node structure with pointer to next
- Last node points back to first
- Insert operation
- Traverse and print
- Deallocate

### Challenge 12.2: Pointer-Based Sorting
Implement:
- Sort array of pointers to integers
- Without moving actual integers
- Compare what pointers point to
- Demonstrate use case

### Challenge 12.3: Binary Search Tree
Create node structure:
- Pointer to left child
- Pointer to right child
- Insert values
- In-order traversal
- Search operation
- Deallocate tree

---

## Tips for Solving

1. **Visualize memory**: Draw pointers and addresses
2. **Always initialize**: ptr = nullptr or pt = &var
3. **Check for null**: Before dereferencing
4. **Paired allocation**: new/delete, new[]/delete[]
5. **Use smart pointers**: Prefer over raw pointers
6. **Test carefully**: Pointer bugs are tricky
7. **Use tools**: Valgrind, addresssanitizer

## Difficulty Levels
- **Easy**: Exercises 1.1, 2.1, 4.1, 4.2, 5.1, 6.1, 7.1, 9.1
- **Medium**: Exercises 1.2-1.3, 2.2, 3.1, 4.3, 5.2-5.3, 6.2-6.3, 7.2-7.3, 8.1, 9.2-9.3, 10.1-10.2, 11.1-11.2
- **Hard**: Exercises 2.3, 3.2-3.3, 8.2-8.3, 10.3, 11.3, Challenge 12.1-12.3

---

## Common Mistakes to Avoid

1. **Dereferencing null pointers** - Always check first
2. **Forgetting to delete** - Memory leak
3. **Deleting already-deleted memory** - Crash
4. **Dangling pointers** - Use after delete
5. **Mixing new/delete with delete[]** - Undefined behavior
6. **Uninitialized pointers** - Random address
7. **Pointer arithmetic errors** - Wrong memory access

---

## Helpful Debugging Checklist

- [ ] Is pointer initialized?
- [ ] Is pointer null?
- [ ] Is memory allocated for the pointer?
- [ ] Are new/delete paired correctly?
- [ ] Is delete called for each new?
- [ ] Any segmentation faults?
- [ ] Check with Valgrind?
- [ ] Use smart pointers instead?

## C++11 Exercise Example: Function Overloading

```cpp
int add(int a, int b) { return a + b; }
double add(double a, double b) { return a + b; }
```