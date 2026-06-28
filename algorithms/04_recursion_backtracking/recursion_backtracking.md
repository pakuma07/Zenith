# Recursion and Backtracking

This chapter has been split into micro-files.

## Structure Files

1. [Recursion Basics](structures/recursion_basics.md)
2. [Backtracking](structures/backtracking.md)

## Practice

- ../exercises/recursion_backtracking_exercises.md

---

## Next Steps

- Review the cheat sheet: [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md)
- Previous: [Chapter 3: Pattern-Based Algorithms](../03_pattern_based/two_pointers_sliding_window_prefix.md)
- Next: [Chapter 5: Divide and Conquer](../05_divide_and_conquer/divide_and_conquer.md)

## C++11 Example: Backtracking Skeleton

```cpp
#include <vector>
using namespace std;

void buildSubsets(int idx, const vector<int>& a, vector<int>& cur, vector<vector<int> >& out) {
    if (idx == (int)a.size()) { out.push_back(cur); return; }
    buildSubsets(idx + 1, a, cur, out);
    cur.push_back(a[idx]);
    buildSubsets(idx + 1, a, cur, out);
    cur.pop_back();
}
```