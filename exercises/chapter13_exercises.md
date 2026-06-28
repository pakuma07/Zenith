# Chapter 13: Exception Handling - Exercises

## Section 1: Try-Catch Basics 🟢

1. Write try-catch for division by zero

2. Create try-catch for array out of bounds

3. Handle multiple exception types in catch block

## Section 2: Exception Hierarchy 🟡

4. Catch specific vs generic exceptions

5. Use standard exception hierarchy (logic_error, runtime_error)

6. Design exception class hierarchy for application

## Section 3: Custom Exceptions 🟡

7. Create custom exception inheriting from std::exception

8. Implement what() method for error messages

9. Create domain-specific exception hierarchy

## Section 4: Multiple Catch Blocks 🟡

10. Catch different exception types in sequence

11. Ensure specific catches before generic catch

12. Handle partial error scenarios

## Section 5: Re-throwing Exceptions 🟡

13. Catch and re-throw to propagate exception

14. Add context information while re-throwing

15. Implement exception chaining

## Section 6: RAII with Exceptions 🟡

16. Ensure cleanup happens with exceptions

17. Use destructors for resource release

18. Design transaction-like behavior with RAII

## Section 7: Noexcept Functions 🟡

19. Mark functions with noexcept guarantee

20. Check noexcept status at compile time

21. Design noexcept-compatible classes

## Section 8: Constructor Exception Safety 🔴

22. Handle exceptions in constructor initialization

23. Create exception-safe constructor

24. Demonstrate strong exception guarantee

## Section 9: Exception in Destructors 🔴

25. Mark destructors noexcept (they should not throw) 🏆

26. Handle exceptions safely in cleanup code

27. Design exception-safe resource management

## Section 10: Advanced Exception Handling 🔴

28. Design exception handling strategy for system

29. Create exception handling wrapper for functions 🏆

30. Implement logging with exception context

---

## Tips for Success

- **Specific before generic**: Order matters in catch blocks
- **what() method**: Provide meaningful error messages
- **Noexcept**: Mark functions that don't throw
- **Destructors never throw**: Use noexcept
- **RAII**: Resources cleaned even with exceptions
- **Re-throw**: Use bare throw without arguments
- **Exception guarantee**: Strong (rollback), basic, or nothrow
- **Standard exceptions**: Reuse std library types

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Basic try-catch, handling
- **Medium (🟡)**: 18 exercises - Custom exceptions, propagation, RAII
- **Hard (🔴)**: 9 exercises - Safety guarantees, advanced patterns

## Challenge Problems 🏆

- **Challenge 1**: Safe destructors with cleanup
- **Challenge 2**: Exception-safe resource management
- **Challenge 3**: Complex exception handling strategy

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Throwing in destructors
- Generic catch without specific handlers
- Forgetting to re-throw
- Resource leaks in exception paths
- Exception slicing in polymorphic catches
- Incorrect noexcept specifications
- Catching by value instead of reference

## Learning Outcomes

After completing these exercises, you will:
✓ Design effective exception hierarchies
✓ Create custom exceptions
✓ Combine RAII with exceptions
✓ Implement exception-safe code
✓ Use noexcept effectively
✓ Handle destructors safely
✓ Provide strong exception guarantees
✓ Log and propagate exceptions correctly
✓ Build robust error handling systems

## C++11 Exercise Example: Lambda Comparator

```cpp
#include <vector>
#include <algorithm>
using namespace std;

void sortDesc(vector<int>& a) {
    sort(a.begin(), a.end(), [](int x, int y) { return x > y; });
}
```