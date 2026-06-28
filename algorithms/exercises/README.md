# Algorithms Exercises

Chapter-style exercise sets for the algorithms module.

## How to Use

1. Read the theory chapter first.
2. Solve Easy -> Medium -> Hard in order.
3. Implement from scratch before using STL shortcuts.
4. Revisit failed problems after a gap.

## Exercise Chapters

1. basics_exercises.md
2. searching_exercises.md
3. sorting_exercises.md
4. pattern_based_exercises.md
5. recursion_backtracking_exercises.md
6. divide_and_conquer_exercises.md
7. greedy_exercises.md
8. dynamic_programming_exercises.md
9. graph_algorithms_exercises.md
10. string_algorithms_exercises.md
11. number_theory_bit_exercises.md
12. advanced_algorithms_exercises.md

## Suggested Pace

- 1 chapter per day for interview prep
- 2-3 problems per category for revision cycles
- Maintain notes for invariants, transitions, and edge cases

---

## Navigation

- Start at [Basics Exercises](basics_exercises.md)
- Pair practice with [../README.md](../README.md)
- Keep [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md) open while solving

## C++11 Exercises Driver Template

```cpp
#include <bits/stdc++.h>
using namespace std;

void solveOneCase() {
    // Pick one exercise from this chapter and implement here.
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    while (t--) solveOneCase();
    return 0;
}
```