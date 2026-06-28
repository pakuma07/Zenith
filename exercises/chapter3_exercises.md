# Chapter 3: Functions - Practice Exercises

## 1. Basic Functions

### Exercise 1.1: Calculator Functions
Create functions for basic operations:
- `int add(int a, int b)` - returns sum
- `int subtract(int a, int b)` - returns difference
- `int multiply(int a, int b)` - returns product
- `double divide(double a, double b)` - returns quotient (handle division by zero)

Test each function with sample values from main().

### Exercise 1.2: String Operations
Create functions:
- `int stringLength(string s)` - return string length
- `string reverseString(string s)` - return reversed string
- `bool isPalindrome(string s)` - check if palindrome (ignore case)
- `string toUpperCase(string s)` - convert to uppercase

### Exercise 1.3: Geometric Functions
Create functions for geometry:
- `double circleArea(double radius)`
- `double rectangleArea(double length, double width)`
- `double triangleArea(double base, double height)`
- `double sphereVolume(double radius)`

## 2. Parameters & Arguments

### Exercise 2.1: Pass by Value vs Reference
Write functions that:
- `void incrementByValue(int x)` - increments local copy
- `void incrementByReference(int& x)` - increments original

In main(), call both and show the difference.

### Exercise 2.2: Swap Values
Create two versions:
- `void swapByValue(int a, int b)` - won't work!
- `void swapByReference(int& a, int& b)` - properly swaps

Show why the first doesn't work and the second does.

### Exercise 2.3: Const References
Create a function:
- `void printVector(const vector<int>& v)` - cannot modify but efficient
- Modify vector and display it from main

## 3. Function Overloading

### Exercise 3.1: Overloaded Add
Create overloaded `add()` functions for:
- Two integers
- Two doubles
- Three integers
- String concatenation (add two strings)

### Exercise 3.2: Overloaded Print
Create overloaded `print()` functions for:
- `print(int x)`
- `print(double x)`
- `print(string s)`
- `print(vector<int> v)` - print all elements

### Exercise 3.3: Overloaded Area
Create overloaded `area()` functions:
- `area(double side)` - square area
- `area(double length, double width)` - rectangle
- `area(double radius, bool circle)` - circle (use bool to distinguish)

## 4. Default Arguments

### Exercise 4.1: Flexible Greeting
Create function: `void greet(string name = "Guest", string greeting = "Hello")`

Call it with:
- No arguments
- Just name
- Both name and greeting

### Exercise 4.2: Range Printer
Create: `void printRange(int start = 1, int end = 10, int step = 1)`

Display ranges with different parameters:
- Default (1-10)
- Custom start and end
- Custom step

### Exercise 4.3: Customizable Output
Create: `void printArray(vector<int> arr, string prefix = "Element", string separator = " ")`

Print arrays with different prefixes and separators.

## 5. Recursion

### Exercise 5.1: Mathematical Functions
Implement recursively:
- `int factorial(int n)` - n!
- `int fibonacci(int n)` - nth Fibonacci number
- `int power(int base, int exp)` - base^exp
- `int gcd(int a, int b)` - greatest common divisor

### Exercise 5.2: Array Processing
Recursive functions on arrays:
- `int sumArray(int arr[], int size)` - sum all elements
- `int maxArray(int arr[], int size)` - find maximum
- `void reverseArray(int arr[], int start, int end)` - reverse in-place
- `void printArray(int arr[], int size)` - print all

### Exercise 5.3: String Recursion
Create recursive functions:
- `bool isPalindrome(string s, int start, int end)` - check palindrome
- `int countOccurrences(string s, char c, int index)` - count character occurrences
- `string removeVowels(string s, int index)` - remove all vowels

### Exercise 5.4: Recursion with Backtracking
Implement permutations:
- `void permute(string s, int l, int r)` - print all permutations of string

## 6. Function Pointers

### Exercise 6.1: Function Pointer Operations
Create:
- `int operation(int a, int b, int (*func)(int, int))`
- Use with different functions (add, multiply, subtract)

### Exercise 6.2: Array of Function Pointers
Create array of functions:
- Define `add()`, `subtract()`, `multiply()`, `divide()`
- Store in array: `int (*ops[])(int, int)`
- Call different operations via array

### Exercise 6.3: Sort with Comparator
Implement sorting with function pointer:
- `void sortArray(int arr[], int size, bool (*compare)(int, int))`
- Use with ascending and descending comparators

## 7. Lambdas (C++11)

### Exercise 7.1: Basic Lambdas
Create lambdas for:
- `auto square = [](int x) { return x * x; };`
- `auto isEven = [](int x) { return x % 2 == 0; };`
- `auto greet = [](string name) { return "Hello, " + name; };`

### Exercise 7.2: Lambda with Capture
Create lambdas that capture:
- Multiply by factor: `auto multiply = [factor](int x) { return x * factor; };`
- Count occurrences by reference: `auto counter = [&count](){ count++; };`

### Exercise 7.3: STL with Lambdas
Use lambdas with algorithms:
- `find_if()` to find first even number in vector
- `count_if()` to count numbers > 10
- `transform()` to square all elements
- `sort()` with custom comparator

## 8. Advanced Function Concepts

### Exercise 8.1: Recursive Lambda
Create a recursive lambda using `function<>`:
```cpp
function<int(int)> factorial = [&factorial](int n) {
    return n <= 1 ? 1 : n * factorial(n-1);
};
```

### Exercise 8.2: Higher-Order Functions
Create a function that:
- Takes a function pointer as parameter
- Returns a new function that modifies behavior
- Example: compose functions, create wrapper

### Exercise 8.3: Memoization
Implement memoized Fibonacci:
- Use `map<int, int>` to cache results
- Compare performance vs regular recursion

## 9. Return Values

### Exercise 9.1: Multiple Return Values
Create functions that return:
- By value (simple case)
- By reference (careful!)
- By const reference (safer)
- Tuple/struct (multiple values)

### Exercise 9.2: Error Handling
Functions that return status:
- `bool divide(double a, double b, double& result)`
- `int findElement(vector<int> v, int target)`
- Return -1 for not found, 0+ for index

### Exercise 9.3: Return Objects
Functions that return:
- Simple objects: `Person createPerson(string name, int age)`
- Vector/array: `vector<int> generateFibonacci(int n)`
- Pointer: `int* allocateArray(int size)`

## Challenge Problems

### Challenge 10.1: Calculator with Functions
Create a simple calculator that:
- Uses function pointers for operations
- Takes operator (+, -, *, /) and two numbers
- Displays result

### Challenge 10.2: Recursive Directory Size
Simulate recursive directory traversal:
- `int dirSize(string path, int depth)`
- Display tree structure with sizes

### Challenge 10.3: Function Composition
Create functions that:
- Compose multiple operations
- Example: `int result = apply_many(5, {square, double, add5});`

---

## Tips for Solving

1. **Start simple**: Write basic function first, then enhance
2. **Test thoroughly**: Call with various inputs including edge cases
3. **Use clear names**: Function names should describe what they do
4. **Document parameters**: Add comments explaining parameter requirements
5. **Handle errors**: Check for invalid inputs (division by zero, null pointers, etc.)

## Difficulty Levels
- **Easy**: Exercises 1.1-1.3, 2.1, 3.1-3.2, 4.1-4.2
- **Medium**: Exercises 2.2-2.3, 3.3, 4.3, 5.1-5.2, 6.1-6.2, 7.1-7.2
- **Hard**: Exercises 5.3-5.4, 6.3, 7.3, 8.1-8.3, Challenge 10.1-10.3
