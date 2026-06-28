# Code Examples Directory

This directory contains runnable C++11 code examples for each chapter of the C++ learning resource.

## Getting Started

Each file demonstrates key concepts with 10 complete, independent examples. All code is compiled with the C++11 standard.

### Compilation

To compile any example file, use:

```bash
g++ -std=c++11 chapter<N>_<topic>.cpp -o chapter<N>_<topic>
./chapter<N>_<topic>
```

### Example File Structure

Each example file includes:
- 10 independent, runnable examples
- Clear section headers and comments
- Educational notes explaining concepts
- Output demonstrations
- Best practices

## Available Examples

### Chapter 1: Basics
**File**: `chapter1_basics.cpp`
- Variable declaration and initialization
- Data types (int, double, char, bool)
- Uniform initialization (C++11)
- Auto type deduction (C++11)
- Constants and literals
- Type casting
- I/O formatting
- Scope and visibility
- Compound types
- Sizeof operator

### Chapter 2: Control Flow & Loops
**File**: `chapter2_control_flow.cpp`
- If-else statements and nesting
- Switch statements with break
- While loops with conditions
- Do-while loops (menu systems)
- For loops (counting patterns)
- Range-based for loops (C++11)
- Break statements (early exit)
- Continue statements (skip iteration)
- Nested loops (patterns)
- Complex control flow combinations

### Chapter 3: Functions
**File**: `chapter3_functions.cpp`
- Basic function definition
- Parameters and arguments
- Pass by value vs reference
- Function overloading
- Default arguments
- Recursion (factorial, fibonacci)
- Function pointers
- Lambda functions (C++11)
- Return by reference (careful!)
- Function composition

### Chapter 4: Pointers & References
**File**: `chapter4_pointers_references.cpp`
- Pointer declaration and usage
- Pointer arithmetic on arrays
- Pointer to pointers (double indirection)
- Reference basics and rebinding
- References vs pointers comparison
- Pass by reference (swapping values)
- Function pointers and callbacks
- Dynamic memory (new/delete)
- Smart pointers (C++11: unique_ptr, shared_ptr)
- Const references (safe, efficient)

### Chapter 5: Arrays & Strings
**File**: `chapter5_arrays_strings.cpp`
- Array declaration and access
- Multi-dimensional arrays (2D matrices)
- Array searching and sorting
- Vector container (dynamic arrays)
- String basics and concatenation
- String operations (substr, find, replace)
- Case conversion (upper/lower)
- String comparison and search
- Vector vs array comparison
- Complex text processing

### Chapter 6: OOP Basics
**File**: `chapter6_oop.cpp`
- Class definition and access modifiers
- Constructors and destructors
- Static members and methods
- Const member functions
- The this pointer
- Encapsulation and data hiding
- Friend functions
- Object creation (stack/heap)
- Object arrays
- Initialization order

## Running the Examples

### Single Example
```bash
g++ -std=c++11 chapter1_basics.cpp -o ch1
./ch1
```

### Compile All
```bash
for file in chapter*.cpp; do
    g++ -std=c++11 "$file" -o "${file%.cpp}"
done
```

### Run All
```bash
for file in chapter*; do
    if [ -x "$file" ]; then
        echo "=== Running $file ===" 
        ./"$file"
    fi
done
```

## Learning Tips

1. **Read the code**: Understand what each example does
2. **Run the program**: See the output
3. **Modify examples**: Change values and parameters
4. **Combine concepts**: Merge multiple examples
5. **Compare variations**: See different approaches
6. **Read the notes**: Learn why code works that way

## Compilation Flags

For better error detection:
```bash
# With warnings
g++ -std=c++11 -Wall -Wextra chapter1_basics.cpp -o ch1

# With debugging symbols
g++ -std=c++11 -g chapter1_basics.cpp -o ch1

# With optimization
g++ -std=c++11 -O2 chapter1_basics.cpp -o ch1
```

## Next Steps

1. **Run each example**: Understand the output
2. **Study the code**: Read comments and learn concepts
3. **Modify examples**: Change parameters and see results
4. **Create variations**: Write your own similar examples
5. **Use in projects**: Apply these patterns in real code

## Related Resources

- **Chapter READMEs**: [../01_basics/README.md](../01_basics/README.md), etc.
- **Exercises**: [../exercises/](../exercises/) - Practice problems
- **Quick Reference**: [../resources/QUICK_REFERENCE.md](../resources/QUICK_REFERENCE.md)
- **OOP Cheatsheet**: [../resources/OOP_CHEATSHEET.md](../resources/OOP_CHEATSHEET.md)
- **STL Cheatsheet**: [../resources/STL_CHEATSHEET.md](../resources/STL_CHEATSHEET.md)

## Progress Tracker

- [x] Chapter 1 - Basics (10 examples)
- [x] Chapter 2 - Control Flow (10 examples)
- [x] Chapter 3 - Functions (10 examples)
- [x] Chapter 4 - Pointers & References (10 examples)
- [x] Chapter 5 - Arrays & Strings (10 examples)
- [x] Chapter 6 - OOP Basics (10 examples)
- [x] Chapter 7 - Inheritance & Polymorphism (10 examples)
- [x] Chapter 8 - Operator Overloading (10 examples)
- [x] Chapter 9 - Templates (10 examples)
- [x] Chapter 10 - STL Containers (10 examples)
- [x] Chapter 11 - STL Algorithms (10 examples)
- [x] Chapter 12 - Memory Management (10 examples)
- [x] Chapter 13 - Exception Handling (10 examples)
- [x] Chapter 14 - File I/O (10 examples)
- [x] Chapter 15 - Advanced Features (10 examples)
- [x] Chapter 16 - Concurrency (10 examples)
- [x] Chapter 17 - Template Metaprogramming (7 examples): `chapter17_tmp.cpp`
- [x] Chapter 18 - Memory Model (8 examples): `chapter18_memory_model.cpp`

**STATUS: 18/18 CHAPTERS COMPLETE ✅ (100% CODE EXAMPLES)**

## Questions & Debugging

If you encounter issues:

1. **Compilation errors**: Check syntax in code
2. **Runtime errors**: Check input/output operations
3. **Logic errors**: Trace through the code step by step
4. **Crashes**: Check pointer operations and bounds

## Standards

All examples follow these conventions:
- **Standard**: C++11 (specified with `-std=c++11`)
- **Style**: Clear, readable code with comments
- **Output**: Demonstrates what the code does
- **Learning**: Educational value over optimization

## C++11 Code-Examples Entry Snippet

```cpp
#include <iostream>
using namespace std;

void runChapterExample(const string& chapter) {
    cout << "Run chapter sample: " << chapter << "\n";
    // Each real sample is in this folder as a standalone .cpp file.
}

int main() {
    runChapterExample("stl_algorithms");
    return 0;
}
```