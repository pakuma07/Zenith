# Chapter 10: STL Containers - Exercises

## Section 1: Vector Operations 🟢

1. Create a vector of integers and implement: push_back, pop_back, size, empty

2. Write a function to find max element in vector

3. Implement vector element modification and iteration

## Section 2: Deque Operations 🟡

4. Use deque for efficient front/back operations

5. Implement queue-like behavior with deque

6. Compare deque vs vector performance for different operations

## Section 3: List Operations 🟡

7. Create linked list and implement insertion in middle

8. Write function to reverse a list

9. Compare list performance vs vector for middle operations

## Section 4: Set Operations 🟡

10. Create set to store unique integers and demonstrate auto-sorting

11. Implement set intersection, union operations

12. Use set for duplicate removal

## Section 5: Map Operations 🟡

13. Create map with string keys and integer values

14. Implement word frequency counter using map

15. Design phone book system with map

## Section 6: Unordered Containers 🟡

16. Compare performance: `set` vs `unordered_set`

17. Create hash table simulation with unordered_map

18. Implement caching system with unordered_map

## Section 7: Container Adapters 🟡

19. Implement stack-based expression evaluation

20. Create queue simulation for task scheduling

21. Design priority queue for event management

## Section 8: Iterator Usage 🟡

22. Iterate containers using iterators

23. Implement bidirectional iteration with list

24. Use iterator validity after modifications

## Section 9: Container Selection 🔴

25. Choose appropriate container for each scenario 🏆

26. Implement custom comparator for container sorting

27. Design system demonstrating all container types

## Section 10: Container Algorithms Integration 🔴

28. Combine containers with STL algorithms

29. Implement transform operation on container

30. Create filtering system using multiple containers

---

## Tips for Success

- **Vector**: General-purpose, random access needed
- **Deque**: Front/back operations needed
- **List**: Frequent middle insertions needed
- **Set**: Unique sorted elements needed
- **Map**: Key-value pairs, sorting by key
- **Unordered_set/map**: Hash-based for speed
- **Container traits**: Memory, access patterns, operations
- **Iterator categories**: Understand limitations

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Basic vector operations
- **Medium (🟡)**: 18 exercises - All container types, adapters, iterators
- **Hard (🔴)**: 9 exercises - Container selection, algorithm integration

## Challenge Problems 🏆

- **Challenge 1**: Choose optimal containers for complex system
- **Challenge 2**: Custom comparators for sorting
- **Challenge 3**: Efficient container usage with algorithms

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Invalidated iterators after modifications
- Wrong container for operation pattern
- Performance issues from poor container choice
- Confusing find() return value (end() if not found)
- Exception safety with iterators
- Memory complexity vs time complexity trade-offs

## Learning Outcomes

After completing these exercises, you will:
✓ Master all standard containers
✓ Understand when to use each container
✓ Work with container adapters
✓ Use iterators effectively
✓ Optimize container selection for performance
✓ Combine containers with algorithms
✓ Handle iterator validity
✓ Design efficient data structures

## C++11 Exercise Example: Vector + Map

```cpp
#include <vector>
#include <map>
using namespace std;

map<int,int> freq(const vector<int>& a) {
    map<int,int> m;
    for (int x : a) ++m[x];
    return m;
}
```