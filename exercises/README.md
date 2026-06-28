# Exercises Directory

This directory contains practice problems for each chapter of the C++ learning resource. The exercises are designed to reinforce understanding and build practical skills.

## Organization

Each exercise file is named `chapter<N>_exercises.md` and contains multiple exercise sections organized by difficulty level.

## Difficulty Levels

Exercises are marked by difficulty:

- 🟢 **Easy**: Fundamental concepts, straightforward implementation
- 🟡 **Medium**: Combine multiple concepts, require some problem-solving
- 🔴 **Hard**: Complex challenges, require deep understanding and creativity

## How to Use Exercises

### 1. Read the Exercise Description
Understand what you need to implement:
```
Exercise 3.1: Calculator Functions
Create functions for basic operations:
- int add(int a, int b) - returns sum
- int subtract(int a, int b) - returns difference
...
```

### 2. Implement the Solution
Write C++ code to solve the problem:
```cpp
int add(int a, int b) {
    return a + b;
}
```

### 3. Test Your Code
Compile and run with different inputs:
```bash
g++ -std=c++11 solution.cpp -o solution
./solution
```

### 4. Compare with Examples
Check if your approach matches the code examples in [../code_examples/](../code_examples/)

### 5. Review Tips
Check the tips provided in each exercise file

## Available Exercise Sets

### Chapter 1: Basics (25+ exercises)
**File**: `chapter1_exercises.md`

**Topics**:
- Data type operations
- Constants and literals
- Type conversions
- Variable scope
- I/O formatting
- Arithmetic operations
- Challenge problems

**Difficulty Distribution**:
- Easy: 4 exercises
- Medium: 6 exercises
- Hard: 3 exercises

### Chapter 2: Control Flow (40+ exercises)
**File**: `chapter2_exercises.md`

**Topics**:
- If-else statements
- Switch statements
- While loops
- Do-while loops
- For loops
- Range-based for (C++11)
- Break and continue
- Nested loops
- Infinite loops
- Complex control flow

**Difficulty Distribution**:
- Easy: 6 exercises
- Medium: 8 exercises
- Hard: 4 exercises

### Chapter 3: Functions (40+ exercises)
**File**: `chapter3_exercises.md`

**Topics**:
- Basic function definition
- Parameters and pass-by-value
- Pass-by-reference
- Overloading
- Default arguments
- Recursion
- Function pointers
- Lambda functions (C++11)
- Return values
- Challenge problems

**Difficulty Distribution**:
- Easy: 6 exercises
- Medium: 8 exercises
- Hard: 5 exercises

### Chapter 4: Pointers & References (30+ exercises)
**File**: `chapter4_exercises.md`

**Topics**:
- Pointer declaration and dereferencing
- Pointer arithmetic
- Pointers to pointers
- References
- Function parameters (pass by ref)
- Function pointers
- Dynamic memory
- Smart pointers (C++11)
- Structures with pointers
- Circular linked lists, BST

**Difficulty Distribution**:
- Easy: 8 exercises
- Medium: 10 exercises
- Hard: 5 exercises

### Chapter 5: Arrays & Strings (35+ exercises)
**File**: `chapter5_exercises.md`

**Topics**:
- Array basics and indexing
- Multi-dimensional arrays
- Copying and comparing arrays
- Vector container
- String operations
- Case conversion
- String parsing
- Array vs vector
- Sorting and searching
- Complex data structures

**Difficulty Distribution**:
- Easy: 8 exercises
- Medium: 10 exercises
- Hard: 5 exercises

### Chapter 16: Concurrency (30+ exercises)
**File**: `chapter16_concurrency_exercises.md`

**Topics**:
- Thread creation and joining
- Mutex, lock_guard, and unique_lock
- Condition variables
- Atomic counters and flags
- async, future, promise, packaged_task
- Deadlock avoidance and lifecycle management

**Difficulty Distribution**:
- Easy: 3 exercises
- Medium: 12 exercises
- Hard: 12 exercises

### Chapter 17: Template Metaprogramming (32 exercises)
**File**: `chapter17_tmp_exercises.md`

**Topics**:
- Struct TMP compile-time computation
- Type traits from scratch (is_same, is_pointer, remove_const, conditional)
- Tag dispatch and policy-based design
- Expression templates and lazy evaluation
- Advanced constexpr (single-return C++11 style)
- CRTP — static polymorphism and mixin patterns
- Template template parameters

**Difficulty Distribution**:
- Easy: 5 exercises
- Medium: 11 exercises
- Hard: 11 exercises
- Challenges: 3 problems

### Chapter 18: Memory Model (30 exercises)
**File**: `chapter18_memory_model_exercises.md`

**Topics**:
- Data races and atomic<T> basics
- memory_order values (relaxed, acquire, release, acq_rel, seq_cst)
- Acquire/release message passing and happens-before
- Relaxed ordering for counters
- atomic_thread_fence for grouped publishing
- Compare-and-swap (CAS) loops
- Lock-free data structures (stack, queue, ring buffer)
- thread_local storage and call_once

**Difficulty Distribution**:
- Easy: 7 exercises
- Medium: 12 exercises
- Hard: 8 exercises
- Challenges: 3 problems

## Exercise Statistics

| Chapter | Easy | Medium | Hard | Total |
|---------|------|--------|------|-------|
| 1       | 4    | 6      | 3    | 13+   |
| 2       | 6    | 8      | 4    | 18+   |
| 3       | 6    | 8      | 5    | 19+   |
| 4       | 8    | 10     | 5    | 23+   |
| 5       | 8    | 10     | 5    | 23+   |
| 16      | 3    | 12     | 12   | 30+   |
| 6-15    | See individual files | See individual files | See individual files | Available |

**Total Completed**: 188+ exercises

## Recommended Learning Path

### Day 1-2: Fundamentals
1. Start with **Chapter 1** - Basics
2. Do 2-3 easy exercises per session
3. Understand data types and I/O

### Day 3-4: Control Flow
1. Practice **Chapter 2** - Control Flow
2. Focus on loop patterns
3. Build intuition for conditionals

### Day 5-6: Functions
1. Work through **Chapter 3** - Functions
2. Implement various function types
3. Practice recursion

### Day 7-8: Pointers
1. Study **Chapter 4** - Pointers & References
2. Start with simple pointer exercises
3. Progress to smart pointers

### Day 9-10: Data Structures
1. Practice **Chapter 5** - Arrays & Strings
2. Implement searching and sorting
3. Use vectors and strings

### Day 11-12: Concurrency
1. Study **Chapter 16** - Concurrency
2. Practice thread lifecycle and mutex safety
3. Build producer/consumer and future-based solutions

## Tips for Success

### Before Coding
- [ ] Read the exercise description carefully
- [ ] Understand what input/output is expected
- [ ] Sketch algorithm on paper if complex
- [ ] Think about edge cases

### While Coding
- [ ] Write clean, readable code
- [ ] Use meaningful variable names
- [ ] Add comments for complex logic
- [ ] Test with multiple inputs

### After Coding
- [ ] Verify output is correct
- [ ] Check edge cases (empty, single, large inputs)
- [ ] Compare with example code
- [ ] Refactor for clarity

### Common Mistakes to Avoid
- Off-by-one errors in loops
- Uninitialized variables
- Not handling special cases
- Missing function parameter checks
- Forgetting to deallocate memory

## Solution Structure

A typical solution looks like:

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Your function implementations
int myFunction(int x) {
    // Implementation
}

int main() {
    // Test your functions
    cout << myFunction(5) << endl;
    
    return 0;
}
```

## Testing Your Solutions

### Compile
```bash
g++ -std=c++11 -Wall solution.cpp -o solution
```

### Run
```bash
./solution
```

### Test with Input
```bash
echo "5" | ./solution
```

### Debug with GDB
```bash
g++ -std=c++11 -g solution.cpp -o solution
gdb ./solution
```

## When You're Stuck

1. **Review the code example**: Check [../code_examples/](../code_examples/)
2. **Read the chapter**: See [../01_basics/README.md](../01_basics/README.md), etc.
3. **Check the tips section**: Each exercise file has helpful tips
4. **Try a simpler version**: Start with an easier variant
5. **Ask questions**: Refer to quick reference sheets

## Quick Reference Links

- **C++ Syntax**: [../resources/QUICK_REFERENCE.md](../resources/QUICK_REFERENCE.md)
- **OOP Guide**: [../resources/OOP_CHEATSHEET.md](../resources/OOP_CHEATSHEET.md)
- **STL Guide**: [../resources/STL_CHEATSHEET.md](../resources/STL_CHEATSHEET.md)
- **Chapter Topics**: [../README.md](../README.md)

## Progress Tracker

Track your completion:

- [x] Chapter 1: Basics (13+ exercises) ✅ Complete
- [x] Chapter 2: Control Flow (18+ exercises) ✅ Complete
- [x] Chapter 3: Functions (19+ exercises) ✅ Complete
- [x] Chapter 4: Pointers (23+ exercises) ✅ Complete
- [x] Chapter 5: Arrays/Strings (23+ exercises) ✅ Complete
- [x] Chapter 6: OOP Basics (20+ exercises) ✅ Complete
- [x] Chapter 7: Inheritance (30+ exercises) ✅ Complete
- [x] Chapter 8: Operators (30+ exercises) ✅ Complete
- [x] Chapter 9: Templates (30+ exercises) ✅ Complete
- [x] Chapter 10: STL Containers (30+ exercises) ✅ Complete
- [x] Chapter 11: Algorithms (30+ exercises) ✅ Complete
- [x] Chapter 12: Memory (30+ exercises) ✅ Complete
- [x] Chapter 13: Exceptions (30+ exercises) ✅ Complete
- [x] Chapter 14: File I/O (30+ exercises) ✅ Complete
- [x] Chapter 15: Advanced (30+ exercises) ✅ Complete

**STATUS: 18/18 CHAPTERS COMPLETE ✅ (100% EXERCISES)**

## Challenge Problems

Each chapter ends with challenge problems marked with 🏆:

- **Challenge 10.x**: Comprehensive problem combining multiple concepts
- Recommended after completing all basic and medium exercises
- Great for portfolio and interview preparation

## Expected Time Commitment

- **Easy exercises**: 10-20 minutes each
- **Medium exercises**: 20-40 minutes each
- **Hard exercises**: 40-90+ minutes each
- **Challenge problems**: 60-120+ minutes each

Plan 1-2 hours per chapter for thorough practice.

## Feedback & Improvement

As you work through exercises:
- Note which concepts need more practice
- Revisit easier exercises if later topics confuse you
- Create your own variations to solidify understanding
- Teach someone else to deepen your knowledge

---

**Remember**: The goal is learning, not just solving. Take time to understand the "why" behind each solution.
