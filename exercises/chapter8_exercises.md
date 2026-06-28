# Chapter 8: Operator Overloading - Exercises

## Section 1: Arithmetic Operators 🟢

1. Create a `Complex` class and overload `+` and `-` operators

2. Implement `Vector` class with overloaded `*` for scalar multiplication

3. Design a `Money` class with overloaded arithmetic operators

## Section 2: Comparison Operators 🟡

4. Create `Person` class with overloaded `==` and `<` operators

5. Implement `Date` class with all comparison operators

6. Design a sorting system using overloaded comparison operators

## Section 3: Stream Operators 🟡

7. Overload `<<` operator for custom class display

8. Overload `>>` operator for custom class input parsing

9. Create a logging system using stream operators

## Section 4: Subscript Operator 🟡

10. Implement `Array` class with `[]` operator for bounds checking

11. Create `Matrix` class with `[]` operator for 2D access

12. Design `Dictionary` class with `[]` for key-value access

## Section 5: Increment/Decrement 🟡

13. Overload prefix `++` and postfix `++` for a counter class

14. Implement both `++` and `--` operators

15. Show performance difference: prefix vs postfix

## Section 6: Assignment Operator 🟡

16. Implement deep copy assignment for class with dynamic memory

17. Create copy assignment and move assignment operators

18. Demonstrate resource transfer with move assignment

## Section 7: Function Call Operator 🔴

19. Create functor class overloading `()` for callable objects

20. Implement comparator functor for STL algorithms

21. Design mathematical function objects

## Section 8: Type Conversion 🔴

22. Overload conversion operator for implicit type casting

23. Create explicit conversion operators with `explicit` keyword

24. Demonstrate conversion chain and ambiguities

## Section 9: Logical Operators 🟡

25. Overload `&&` and `||` for custom classes (use carefully!)

26. Implement boolean conversion operator

27. Design decision-making classes with logical operators

## Section 10: Member Access Operators 🔴

28. Overload `->` operator for proxy classes 🏆

29. Implement `*` for iterator-like access

30. Create smart pointer-like class with custom operators

---

## Tips for Success

- **Binary vs Unary**: Remember operator arity
- **Member vs Non-member**: Most can be both
- **Return types**: Return reference for chaining
- **Prefix vs Postfix**: Prefix is more efficient
- **Stream operators**: Often friend functions
- **Avoid abuse**: Only overload when intuitive
- **const correctness**: Mark appropriate operators const
- **Move semantics**: Use with assignment operators
- **Conversion operators**: Be explicit when possible

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Basic arithmetic, simple operators
- **Medium (🟡)**: 18 exercises - Comparisons, streams, increment/decrement
- **Hard (🔴)**: 9 exercises - Functors, conversions, member access

## Challenge Problems 🏆

- **Challenge 1**: Custom member access with operator->
- **Challenge 2**: Type conversion operator chains
- **Challenge 3**: Functor optimization for algorithms

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Not returning reference for `=` operator
- Forgetting `const` on comparison operators
- Postfix `++` creating temporary (inefficient)
- Ambiguous conversion operator chains
- Changing operator precedence/associativity
- Overloading `&&`/`||` (short-circuit semantics lost)

## Learning Outcomes

After completing these exercises, you will:
✓ Understand when and how to overload operators
✓ Implement all major operator categories
✓ Use friend functions correctly
✓ Handle const correctness in operators
✓ Design type conversions safely
✓ Create functors for STL algorithms
✓ Implement move semantics with operators
✓ Avoid operator overloading pitfalls

## C++11 Exercise Example: Polymorphism

```cpp
#include <string>
using namespace std;

struct Shape { virtual ~Shape() {} virtual string draw() const = 0; };
struct Circle : Shape { string draw() const override { return "circle"; } };
```