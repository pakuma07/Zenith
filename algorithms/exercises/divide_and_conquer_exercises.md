# Exercises: Divide and Conquer

## Easy

1. Binary search recursively.
2. Merge two sorted halves.
3. Find max/min recursively.
4. Implement merge sort.
5. Implement quick sort partition step.

## Medium

1. Maximum subarray via divide and conquer.
2. Count inversions.
3. Search in rotated sorted array.
4. Closest element to target in sorted array.
5. Majority element using divide and conquer.

## Hard

1. Closest pair of points overview implementation.
2. Skyline problem.
3. Fast polynomial multiplication overview.
4. Merge k sorted arrays with divide and conquer.
5. Count smaller numbers after self via merge idea.

## Challenge

Compare one problem solved by divide and conquer versus a DP or greedy alternative and explain tradeoffs.

---

## Next Steps

- Read the matching theory: [../05_divide_and_conquer/divide_and_conquer.md](../05_divide_and_conquer/divide_and_conquer.md)
- Previous: [Recursion and Backtracking Exercises](recursion_backtracking_exercises.md)
- Next: [Greedy Exercises](greedy_exercises.md)

## C++11 Exercise Example: Merge Two Sorted Arrays

```cpp
#include <vector>
using namespace std;

vector<int> mergeTwo(const vector<int>& a, const vector<int>& b) {
    vector<int> out;
    int i = 0, j = 0;
    while (i < (int)a.size() && j < (int)b.size()) out.push_back(a[i] <= b[j] ? a[i++] : b[j++]);
    while (i < (int)a.size()) out.push_back(a[i++]);
    while (j < (int)b.size()) out.push_back(b[j++]);
    return out;
}
```