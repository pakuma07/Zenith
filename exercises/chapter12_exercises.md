# Chapter 12: Memory Management - Exercises

## Section 1: Raw Pointers 🟢

1. Allocate and deallocate memory with new/delete

2. Create array with new[] and clean up with delete[]

3. Demonstrate pointer arithmetic

## Section 2: Unique Pointer 🟡

4. Create unique_ptr and transfer ownership with move

5. Use unique_ptr in container

6. Access through unique_ptr (operator* and ->)

## Section 3: Shared Pointer 🟡

7. Create shared_ptr and share ownership

8. Monitor reference count with use_count()

9. Use shared_ptr in multiple containers

## Section 4: Make Functions 🟡

10. Use `make_unique` instead of `new`

11. Use `make_shared` for efficiency

12. Compare make_shared vs shared_ptr constructor

## Section 5: Weak Pointer 🟡

13. Create weak_ptr to break circular references

14. Demonstrate circular reference problem and solution

15. Convert weak_ptr to shared_ptr with lock()

## Section 6: RAII Pattern 🟡

16. Implement custom resource class with RAII

17. Create file handle with automatic cleanup

18. Design resource-managing class

## Section 7: Move Semantics 🟡

19. Implement move constructor for resource class

20. Implement move assignment operator

21. Demonstrate efficient resource transfer

## Section 8: Smart Pointers with Polymorphism 🔴

22. Use unique_ptr with polymorphic types

23. Create container of shared_ptr to base class

24. Implement virtual destructor with smart pointers

## Section 9: Custom Deleters 🔴

25. Create unique_ptr with custom deleter 🏆

26. Implement shared_ptr with custom cleanup

27. Design deleter for array vs single object

## Section 10: Memory Safety Practices 🔴

28. Identify and fix memory leaks

29. Demonstrate exception safety with smart pointers

30. Create leak-proof resource management system

---

## Tips for Success

- **Unique_ptr**: Single ownership, no overhead
- **Shared_ptr**: Multiple owners, reference counted
- **Weak_ptr**: Break circular references
- **Make functions**: Preferred for creation
- **Move semantics**: Efficient transfer with `move()`
- **RAII principle**: Resources = initialization
- **Never raw new/delete**: Use smart pointers
- **Virtual destructors**: Essential with polymorphism

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Raw pointer basics
- **Medium (🟡)**: 18 exercises - All smart pointer types, RAII, move
- **Hard (🔴)**: 9 exercises - Polymorphism, custom deleters, safety

## Challenge Problems 🏆

- **Challenge 1**: Custom deleters for complex resources
- **Challenge 2**: Circular reference resolution
- **Challenge 3**: Exception-safe resource management

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Circular references with shared_ptr
- Forgetting virtual destructors
- Mixing raw and smart pointers
- Dangling weak_ptr references
- Custom deleter syntax complexity
- Exception safety issues
- Resource leaks from exceptions

## Learning Outcomes

After completing these exercises, you will:
✓ Eliminate memory leaks with smart pointers
✓ Understand ownership models
✓ Implement RAII correctly
✓ Use move semantics effectively
✓ Handle circular references with weak_ptr
✓ Design resource-managing classes
✓ Work with polymorphism safely
✓ Implement custom deleters
✓ Write exception-safe code

## C++11 Exercise Example: unique_ptr Ownership

```cpp
#include <memory>
using namespace std;

struct Node { int v; explicit Node(int x) : v(x) {} };

unique_ptr<Node> makeNode(int x) {
    return unique_ptr<Node>(new Node(x));
}
```