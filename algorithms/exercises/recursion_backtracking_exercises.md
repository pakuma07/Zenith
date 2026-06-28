# Exercises: Recursion and Backtracking

## Easy

1. Factorial recursively.
2. Power function recursively.
3. Generate all subsets.
4. Generate all binary strings of length n.
5. Print all root-to-leaf paths in a tree.

## Medium

1. Generate permutations.
2. Combination sum.
3. Phone keypad combinations.
4. Word search in a grid.
5. Palindrome partitioning.

## Hard

1. N-Queens.
2. Sudoku solver.
3. Rat in a maze with path listing.
4. Expression add operators.
5. Partition into k equal sum subsets.

## Challenge

Implement a generic backtracking template with choose / explore / undo stages and apply it to 3 problems.

---

## Next Steps

- Read the matching theory: [../04_recursion_backtracking/recursion_backtracking.md](../04_recursion_backtracking/recursion_backtracking.md)
- Previous: [Pattern-Based Exercises](pattern_based_exercises.md)
- Next: [Divide and Conquer Exercises](divide_and_conquer_exercises.md)

## C++11 Exercise Example: Generate Permutations

```cpp
#include <vector>
using namespace std;

void permute(int i, vector<int>& a, vector<vector<int> >& out) {
    if (i == (int)a.size()) { out.push_back(a); return; }
    for (int j = i; j < (int)a.size(); ++j) {
        swap(a[i], a[j]);
        permute(i + 1, a, out);
        swap(a[i], a[j]);
    }
}
```