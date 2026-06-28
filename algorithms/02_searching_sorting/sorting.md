# Sorting Algorithms

This chapter has been split into micro-files.

## Structure Files

1. [Bubble Sort](structures/bubble_sort.md)
2. [Selection Sort](structures/selection_sort.md)
3. [Insertion Sort](structures/insertion_sort.md)
4. [Merge Sort](structures/merge_sort.md)
5. [Quick Sort](structures/quick_sort.md)
6. [Heap Sort](structures/heap_sort.md)
7. [Sorting Complexity Table](structures/sorting_complexity_table.md)

## Practice

- ../exercises/sorting_exercises.md

## Related Chapter

- searching.md

---

## Next Steps

- Review the cheat sheet: [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md)
- Previous in this chapter: [Searching Algorithms](searching.md)
- Next chapter: [Chapter 3: Pattern-Based Algorithms](../03_pattern_based/two_pointers_sliding_window_prefix.md)

## C++11 Example: Merge Sort

```cpp
#include <vector>
using namespace std;

void mergeRange(vector<int>& a, int l, int m, int r) {
    vector<int> L(a.begin() + l, a.begin() + m + 1);
    vector<int> R(a.begin() + m + 1, a.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)L.size() && j < (int)R.size()) a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < (int)L.size()) a[k++] = L[i++];
    while (j < (int)R.size()) a[k++] = R[j++];
}
```