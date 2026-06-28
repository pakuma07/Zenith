# Chapter 11: STL Algorithms - Exercises

## Section 1: Find Algorithms 🟢

1. Use `find()` to locate first occurrence of value

2. Implement `find_if()` with custom predicate

3. Use `count()` and `count_if()` for counting

## Section 2: Sort Algorithms 🟡

4. Sort vector ascending and descending

5. Create custom comparator for sorting objects

6. Implement partial sort for top N elements

## Section 3: Transform Algorithms 🟡

7. Use `transform()` to apply function to all elements

8. Square all elements in vector

9. Convert string vector to uppercase with transform

## Section 4: Copy Algorithms 🟡

10. Copy container elements to another

11. Implement `copy_if()` for conditional copy

12. Use `remove_copy()` to filter elements

## Section 5: For_each Algorithm 🟡

13. Use `for_each()` to apply function to elements

14. Process container with lambda in for_each

15. Accumulate result using for_each with capture

## Section 6: Numeric Algorithms 🟡

16. Calculate sum using `accumulate()`

17. Implement product calculation with accumulate

18. Create custom binary operation for accumulate

## Section 7: Fill and Replace 🟡

19. Fill container with value

20. Replace specific values in container

21. Implement `replace_if()` with predicate

## Section 8: Reverse and Rotate 🟡

22. Reverse container elements

23. Implement circular rotation

24. Compare reverse before and after

## Section 9: Unique and Remove 🔴

25. Remove adjacent duplicates

26. Implement erase-remove idiom for filtering 🏆

27. Remove conditionally with remove_if

## Section 10: Algorithm Combinations 🔴

28. Chain multiple algorithms for complex operations

29. Create pipeline: filter -> transform -> aggregate 🏆

30. Design sorting and searching system combining algorithms

---

## Tips for Success

- **Range requirements**: Know what each algorithm expects
- **Iterator categories**: Forward, bidirectional, random access
- **Erase-remove**: Two-step process for actual removal
- **In-place vs copy**: Choose variant wisely
- **Lambda predicates**: Powerful with modern C++11
- **Performance**: Understand time complexity
- **Side effects**: Use for_each carefully
- **Numeric algorithms**: Include `<numeric>`

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Find and basic operations
- **Medium (🟡)**: 18 exercises - All algorithm categories
- **Hard (🔴)**: 9 exercises - Complex combinations, optimization

## Challenge Problems 🏆

- **Challenge 1**: Erase-remove idiom mastery
- **Challenge 2**: Complex algorithm pipelines
- **Challenge 3**: Optimized algorithm selection

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Forgetting erase() after remove()
- Wrong iterator type for algorithm
- Using wrong variant (in-place vs copy)
- Predicate side effects
- Iterator invalidation
- Not updating iterator after operations
- Performance issues from algorithm choice

## Learning Outcomes

After completing these exercises, you will:
✓ Master all major STL algorithms
✓ Combine algorithms effectively
✓ Write efficient predicates and functions
✓ Understand algorithm complexity
✓ Use erase-remove idiom correctly
✓ Chain algorithms for complex operations
✓ Optimize algorithm selection
✓ Work seamlessly with modern C++11 lambdas

## C++11 Exercise Example: STL sort + lower_bound

```cpp
#include <vector>
#include <algorithm>
using namespace std;

int firstAtLeast(vector<int> a, int x) {
    sort(a.begin(), a.end());
    auto it = lower_bound(a.begin(), a.end(), x);
    return it == a.end() ? -1 : (int)(it - a.begin());
}
```