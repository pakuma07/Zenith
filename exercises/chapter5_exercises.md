# Chapter 5: Arrays & Strings - Practice Exercises

## 1. Basic Arrays

### Exercise 1.1: Array Declaration & Access
Create program that:
- Declares int arr[5]
- Initializes with values
- Prints all elements using loop
- Access specific elements by index
- Modify elements

### Exercise 1.2: Array Input & Output
Write program to:
- Read n integers from user
- Store in array
- Calculate and print: sum, average, max, min
- Count positive and negative numbers

### Exercise 1.3: Array Size & Bounds
Create program with:
- Calculate array size using sizeof
- Test boundary conditions
- Demonstrate out-of-bounds access (undefined behavior)
- Discuss proper bounds checking

## 2. Multi-dimensional Arrays

### Exercise 2.1: 2D Array Basics
Given 3×3 matrix:
- Initialize with values
- Print all elements
- Access diagonal elements
- Calculate sum of row, column, main diagonal

### Exercise 2.2: Matrix Operations
Create two 2×2 matrices:
- Add matrices (A + B)
- Multiply matrices (A × B)
- Transpose matrix
- Display results

### Exercise 2.3: 3D Array
Create program with:
- 2×3×4 3D array
- Initialize with values
- Access specific elements
- Calculate total sum
- Find maximum value

## 3. Copying & Comparing Arrays

### Exercise 3.1: Array Copy
Write functions:
- `void copy(int src[], int dst[], int size)` - copy elements
- Verify copy independence (changing one doesn't affect other)
- Use both loop and library functions

### Exercise 3.2: Array Comparison
Create functions:
- `bool areEqual(int arr1[], int arr2[], int size)`
- `int findDifference(int arr1[], int arr2[], int size)` - first index where different
- Test with various arrays

### Exercise 3.3: Array Search
Implement:
- `int linearSearch(int arr[], int size, int target)` - returns index
- `int binarySearch(int arr[], int size, int target)` - requires sorted
- Compare performance with timing

## 4. Vector Container (C++11)

### Exercise 4.1: Vector Basics
Create program using:
- `vector<int> v = {1, 2, 3}`
- Add elements: `push_back()`
- Access elements: `at()`, `[]`, `.front()`, `.back()`
- Iterate: range-based for loop
- Check size and capacity

### Exercise 4.2: Vector Operations
Write code demonstrating:
- Insert at position
- Erase element
- Clear all elements
- Resize vector
- Reserve space

### Exercise 4.3: Vector of Structures
Create:
- `struct Student { string name; int score; }`
- `vector<Student> students`
- Add, remove, search students
- Sort by score
- Print all records

## 5. String Operations

### Exercise 5.1: String Basics
Create program with:
- `string s = "Hello"`
- Access characters: `s[0]`, `s.at(0)`
- Get length: `s.length()`, `s.size()`
- Concatenate: `+` operator
- Modify: `s[0] = 'J'`

### Exercise 5.2: String Functions
Implement or use:
- `substr(pos, len)` - extract substring
- `find(substr)` - locate substring
- `replace(pos, len, str)` - replace portion
- `insert(pos, str)` - insert substring
- `erase(pos, len)` - remove portion

### Exercise 5.3: String Case & Comparison
Write functions:
- `string toUpperCase(string s)` - convert to uppercase
- `string toLowerCase(string s)` - convert to lowercase
- `bool isPalindrome(string s)` - check palindrome
- `bool isAnagram(string s1, string s2)` - check anagram

## 6. String Searching & Manipulation

### Exercise 6.1: Pattern Matching
Create functions:
- Count occurrences of substring
- Find all positions of character
- Check if string contains another
- Replace all occurrences

### Exercise 6.2: Word Operations
Write program that:
- Reads a sentence
- Count words (split by space)
- Find longest word
- Reverse word order
- Print each word on new line

### Exercise 6.3: String Parsing
Parse CSV string "Alice,25,Engineer":
- Split by delimiter
- Extract fields
- Store in structure
- Reconstruct CSV

## 7. Character Array vs String Object

### Exercise 7.1: C-style Strings
Using char arrays:
- `char str[50] = "Hello"`
- Access characters
- Manual null terminator handling
- Compare safety with std::string

### Exercise 7.2: String Object Benefits
Using std::string:
- Automatic length handling
- Built-in operations
- Safer boundary checking
- Cleaner syntax

### Exercise 7.3: Conversion
Create functions:
- Convert C-string to std::string
- Convert std::string to C-string using `.c_str()`
- Handle legacy code requirements

## 8. Vector vs Array

### Exercise 8.1: Vector Advantages
Demonstrate:
- Dynamic sizing
- No need to know size beforehand
- Built-in functions
- Safety of `.at()` with bounds checking

### Exercise 8.2: Performance Comparison
Compare vector and array:
- Access speed (same - both O(1))
- Insertion (array O(n), vector O(n))
- Dynamic growth efficiency
- Memory usage

### Exercise 8.3: When to Use Each
Create scenarios:
- Fixed size at compile time → array
- Dynamic size → vector
- Performance critical → array
- Convenience important → vector

## 9. Sorting & Searching

### Exercise 9.1: Bubble Sort
Implement bubble sort:
- Sort array of integers
- Count comparisons
- Optimize by stopping early
- Time complexity: O(n²)

### Exercise 9.2: Binary Search
Implement binary search:
- Requires sorted array
- Divide and conquer approach
- Time complexity: O(log n)
- Return index or -1 if not found

### Exercise 9.3: Using STL Algorithms
Use STL without implementing:
- `sort()` - automatic sorting
- `binary_search()` - boolean result
- `lower_bound()`, `upper_bound()` - positioning
- Compare with manual implementations

## 10. Sorting Structures

### Exercise 10.1: Sort by Different Criteria
Create Person structure:
- `struct Person { string name; int age; }`
- Sort by name (alphabetically)
- Sort by age (numerically)
- Use custom comparator

### Exercise 10.2: Multi-key Sorting
Sort array of students by:
- Primary: grade (descending)
- Secondary: name (ascending)
- Implement custom comparator
- Use in sort function

### Exercise 10.3: Sorting Pointers
Create:
- Array of pointers to integers
- Sort pointers (not values)
- Access through pointer to verify sorting
- Demonstrate use case

## 11. Special Containers

### Exercise 11.1: Deque Usage
Use `deque<int>`:
- Add to front and back efficiently
- Remove from front and back
- Compare with vector
- Use for queue-like structures

### Exercise 11.2: Set for Unique Values
Using `set<int>`:
- Automatically sorted, unique
- Insert and access
- Find operations
- Difference from vector

### Exercise 11.3: Map for Key-Value
Using `map<string, int>`:
- Store name → score mapping
- Access by key
- Iterate through pairs
- Find and erase

## Challenge Problems

### Challenge 12.1: Sudoku Validator
Create program that:
- Reads 9×9 grid
- Validates all rows contain 1-9
- Validates all columns contain 1-9
- Validates all 3×3 boxes contain 1-9
- Multi-dimensional array practice

### Challenge 12.2: String Compression
Implement:
- Input: "aaabbbc"
- Output: "a3b3c1"
- Also reverse: decompress
- Handle empty strings and special cases

### Challenge 12.3: Anagram Grouper
Given list of words:
- Group anagrams together
- Use vector and map
- Sort within groups
- Display results

---

## Tips for Solving

1. **Arrays**: Remember index starts at 0
2. **Bounds**: Always check valid indices
3. **Strings**: Don't forget null terminator with C-strings
4. **Vectors**: More flexible than arrays
5. **Algorithms**: Use STL instead of manual code
6. **Performance**: Profile before optimizing
7. **Sorting**: Understand time complexity

## Difficulty Levels
- **Easy**: Exercises 1.1-1.2, 2.1, 4.1, 5.1-5.2, 7.1, 8.1
- **Medium**: Exercises 1.3, 2.2, 3.1-3.2, 4.2-4.3, 5.3, 6.1, 7.2-7.3, 8.2, 9.1-9.2, 10.1, 11.1-11.2
- **Hard**: Exercises 2.3, 3.3, 6.2-6.3, 8.3, 9.3, 10.2-10.3, 11.3, Challenge 12.1-12.3

---

## Common Array Mistakes

1. **Out of bounds access** - Undefined behavior
2. **Off-by-one errors** - Accessing index n when array size is n
3. **Not initializing** - Garbage values
4. **Fixed size arrays** - Cannot resize
5. **Array name decay** - Understand pointer conversion
6. **Wrong copy semantics** - Shallow vs deep copy

---

## String Safety Tips

1. **Use std::string** when possible over char arrays
2. **Check bounds** with `.at()` instead of `[]`
3. **Use `.find()` safely** - returns string::npos if not found
4. **Null termination** - Important for C-strings
5. **Substr bounds** - Check length before substring

---

## Reference: STL Containers Summary

| Container | Best For | Access | Insert |
|-----------|----------|--------|--------|
| array | Fixed size | O(1) | - |
| vector | General purpose | O(1) | O(n) |
| deque | Front/back ops | O(1) | O(n) avg |
| list | Middle insert/delete | O(n) | O(1) |
| set | Unique, sorted | O(log n) | O(log n) |
| map | Key-value pairs | O(log n) | O(log n) |

## C++11 Exercise Example: Reverse String

```cpp
#include <string>
#include <algorithm>
using namespace std;

string reversed(string s) {
    reverse(s.begin(), s.end());
    return s;
}
```