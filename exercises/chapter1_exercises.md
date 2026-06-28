# Chapter 1: Basics - Practice Exercises

## 1. Data Types & Variables

### Exercise 1.1: Calculate Circle Area
Write a program that:
- Declares a variable `radius` of type `double` and initialize it with 5.0
- Declares a constant `PI` with value 3.14159
- Calculate and display the area of a circle: `area = PI * radius * radius`

**Expected Output:** Area of circle with radius 5.0 is approximately 78.54

### Exercise 1.2: Type Sizes
Write a program that displays the size (in bytes) of:
- char, short, int, long, long long
- float, double, long double
- bool, int*, double*

Use the `sizeof` operator and create a nicely formatted table.

### Exercise 1.3: Temperature Conversion
Write a program that:
- Reads a temperature in Celsius as input
- Converts it to Fahrenheit using formula: `F = C * 9/5 + 32`
- Displays both values with 2 decimal places

### Exercise 1.4: Simple Banking
Create variables for:
- Account holder name (string)
- Account number (integer)
- Balance (double)
- Account type (char: 'S' for Savings, 'C' for Checking)

Display all information in a formatted way.

## 2. Constants & Literals

### Exercise 2.1: Octal, Hex, Binary
Write a program that:
- Defines a number in octal (e.g., 052)
- Defines the same number in hexadecimal (e.g., 0x2A)
- Defines the same number in binary (C++14, e.g., 0b101010)
- Displays all three and verify they're equal

### Exercise 2.2: String Literals
Create a program with different types of string literals:
- Regular C-string: "Hello"
- C++ std::string: std::string("Hello")
- Character literal: 'H'
- Escape sequences: "Line1\nLine2", "Tab\tSeparated"

Display each type.

## 3. Type Conversion

### Exercise 3.1: Implicit vs Explicit Casting
Write a program that:
- Declares `int x = 10`
- Implicitly converts to `double` and displays
- Declares `double y = 3.99`
- Explicitly casts to `int` using static_cast and displays (should be 3)
- Show the data loss with comments

### Exercise 3.2: String to Number Conversion
Write a program that:
- Reads a string from input: "42"
- Converts it to integer using `stoi()`
- Converts "3.14" to double using `stod()`
- Converts "123" to long using `stol()`
- Display all values with their types

### Exercise 3.3: Number to String
Write a program that:
- Declares `int x = 42`, `double d = 3.14`
- Converts both to strings using `to_string()`
- Concatenates strings and displays result

## 4. Scope & Initialization

### Exercise 4.1: Global vs Local Variables
Create a program with:
- A global variable `globalX = 100`
- A function that declares `localX = 50`
- Display both before and after function call
- Show that global remains unchanged

### Exercise 4.2: Variable Shadowing
Write a program that:
- Declares `int x = 10` in outer scope
- Declares `int x = 20` in inner scope (block)
- Display both to show shadowing effect
- Show address of both using `&x`

### Exercise 4.3: C++11 Uniform Initialization
Write a program that:
- Uses uniform initialization with braces: `int x{10}`
- Try to initialize `int y{3.14}` (should compile error - narrowing)
- Initialize an array: `int arr[] = {1, 2, 3}`
- Comment out the failing line

## 5. Input/Output Formatting

### Exercise 5.1: Formatted Table Output
Create a formatted table displaying:
- Product | Price | Quantity | Total
- ------- | ----- | -------- | -----
- Item1 | $19.99 | 5 | $99.95
- Item2 | $9.99 | 3 | $29.97

Use `setw()`, `setprecision()`, `left`/`right` for alignment.

### Exercise 5.2: Number Base Display
Write a program that reads an integer and displays it in:
- Decimal (base 10)
- Hexadecimal (base 16)
- Octal (base 8)

Example: 255 → 255 (decimal), ff (hex), 377 (octal)

### Exercise 5.3: Histogram
Create a program that:
- Reads 5 numbers from user
- Displays a simple histogram using asterisks
- Example: 3 → ***, 7 → *******

## 6. Arithmetic Operations

### Exercise 6.1: BMI Calculator
Calculate Body Mass Index (BMI):
- Read weight (kg) and height (m)
- Formula: BMI = weight / (height * height)
- Display BMI with 1 decimal place
- Display category: Underweight (<18.5), Normal (18.5-24.9), etc.

### Exercise 6.2: Modulo Operations
Write a program that:
- Tests if a number is even or odd using modulo
- Extracts last digit of a number using modulo
- Tests if a number is divisible by 3 and 5
- Displays results for numbers 1-20

### Exercise 6.3: Power Calculation
Write a program that:
- Calculates x^y manually without using pow()
- Example: 2^5 = 2 * 2 * 2 * 2 * 2 = 32
- Try with multiple bases and exponents

## 7. Challenge Problems

### Challenge 7.1: Currency Converter
Create a program that:
- Defines exchange rates (USD, EUR, GBP, JPY) as constants
- Reads amount in one currency and target currency
- Converts and displays result
- Validates input

### Challenge 7.2: Simple Vending Machine
Simulate a simple calculator:
- Declare total price = 5.50
- User enters payment amount
- Calculate and display change with proper precision
- Validate: payment >= total price

### Challenge 7.3: Age Calculator
Write a program that:
- Reads birth year as input
- Calculates current age (assume current year is 2026)
- Handles edge cases (future birth year, etc.)
- Displays age category: Child, Teen, Adult, Senior

---

## Solutions Tips

For each exercise, think about:
- What data types do I need?
- What values need to be constants?
- How should I format the output?
- What validation do I need?

Use `#include <iostream>`, `#include <string>`, `#include <iomanip>` as needed.

---

## Difficulty Levels
- **Easy**: Exercises 1.1-1.4, 2.1, 3.1
- **Medium**: Exercises 3.2-3.3, 4.1-4.3, 5.1-5.3, 6.1-6.2
- **Hard**: Exercises 6.3, Challenge 7.1-7.3

## C++11 Exercise Example: Circle Area

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    const double PI = 3.14159;
    double radius = 5.0;
    cout << fixed << setprecision(2) << PI * radius * radius << "\n";
    return 0;
}
```