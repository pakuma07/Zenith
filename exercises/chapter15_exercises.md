# Chapter 15: Advanced C++11 Features - Exercises

## Section 1: Lambda Functions - Basics 🟢

1. Create simple lambda with no parameters

2. Lambda with parameters and return value

3. Use lambda as function pointer

## Section 2: Lambda Captures 🟡

4. Capture by value [=]

5. Capture by reference [&]

6. Mixed captures [=, &x]

## Section 3: Auto Type Deduction 🟡

7. Use auto with variables (int, double, string)

8. Auto with containers (vector, map, set)

9. Auto in range-based for loops

## Section 4: Range-Based For Loop 🟡

10. Iterate vector by value

11. Iterate by reference and modify

12. Iterate with const reference

## Section 5: Nullptr 🟡

13. Use nullptr vs NULL vs 0

14. Type safety with nullptr

15. Function overloading with nullptr

## Section 6: Type Traits 🟡

16. Check is_integral for types

17. Use is_floating_point for dispatch

18. Check is_pointer and is_const

## Section 7: Constexpr 🟡

19. Compute factorial at compile time

20. Implement constexpr Fibonacci

21. Use constexpr in template arguments

## Section 8: Rvalue References & Move 🔴

22. Implement move constructor

23. Create move assignment operator

24. Demonstrate move efficiency vs copy

## Section 9: Delegating Constructors 🟡

25. Constructor delegating to another

26. Use delegating constructors to reduce duplication

27. Design multi-level delegation

## Section 10: Modern C++11 Practices 🔴

28. Combine multiple C++11 features in project

29. Design using auto, lambdas, range-for, smart pointers 🏆

30. Create comprehensive C++11 compatible system

---

## Tips for Success

- **Lambda syntax**: `[capture](params) { body }`
- **Auto**: Cleaner code, still type-safe
- **Range-for**: Works with containers and arrays
- **Nullptr**: Always prefer over NULL or 0
- **Type traits**: Foundation for meta-programming
- **Constexpr**: Evaluated at compile-time
- **Move semantics**: Transfer vs copy for efficiency
- **Delegation**: Constructor calling another constructor

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Lambda basics, auto introduction
- **Medium (🟡)**: 18 exercises - Captures, type deduction, range-for
- **Hard (🔴)**: 9 exercises - Move semantics, advanced patterns

## Challenge Problems 🏆

- **Challenge 1**: Complex modern C++11 system design
- **Challenge 2**: Move semantics optimization
- **Challenge 3**: Advanced lambda usage with capture

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Lambda capture dangling references [&] with outlived scope
- Confusing auto with type inference limits
- Not understanding rvalue references (&&)
- Range-for with temporary containers
- Overusing type_traits (keep it simple)
- Constexpr limitations (no I/O, limited operations)
- Move constructor not noexcept
- Delegating constructor cycles

## Learning Outcomes

After completing these exercises, you will:
✓ Write clean lambda functions with proper captures
✓ Use auto for type safety and readability
✓ Implement range-based for loops effectively
✓ Prefer nullptr over old-style null pointers
✓ Apply type traits for generic programming
✓ Use constexpr for compile-time computation
✓ Implement efficient move semantics
✓ Use delegating constructors effectively
✓ Write modern, idiomatic C++11 code
✓ Combine multiple features naturally

## Modern C++11 Feature Checklist

After all exercises, verify you can:

**Basics:**
- [ ] Use auto for type deduction
- [ ] Write lambda functions with captures
- [ ] Use range-based for loops
- [ ] Prefer nullptr over NULL/0

**Memory & Resources:**
- [ ] Use smart pointers (unique_ptr, shared_ptr, weak_ptr)
- [ ] Implement move constructors
- [ ] Apply RAII principles
- [ ] Write exception-safe code

**Containers & Algorithms:**
- [ ] Choose appropriate containers
- [ ] Use STL algorithms with lambdas
- [ ] Work with iterators correctly
- [ ] Combine containers and algorithms

**Advanced Techniques:**
- [ ] Write function and class templates
- [ ] Use partial template specialization
- [ ] Apply SFINAE and enable_if
- [ ] Understand type traits

**Design Patterns:**
- [ ] Implement virtual inheritance
- [ ] Use polymorphic containers
- [ ] Override with keyword
- [ ] Overload operators correctly

**Quality:**
- [ ] Write exception-safe code
- [ ] Use const correctness
- [ ] Apply RAII for resource management
- [ ] Follow C++11 best practices

## Integration Projects

After mastering individual chapters:

1. **Small Project (10 hours)**
   - Student management system
   - File I/O, containers, algorithms
   - Exception handling, basic OOP

2. **Medium Project (30 hours)**
   - Banking system with polymorphism
   - Advanced OOP, templates, smart pointers
   - Comprehensive exception handling

3. **Large Project (60+ hours)**
   - Game engine or framework
   - All C++11 features combined
   - Professional-quality architecture

## C++11 Exercise Example: Strategy Pattern

```cpp
#include <memory>
using namespace std;

struct Op { virtual ~Op() {} virtual int run(int a, int b) const = 0; };
struct Add : Op { int run(int a, int b) const override { return a + b; } };
```