# Range Query Structures

This chapter was split for consistency.

## Structure Files

1. [Fenwick Tree](structures/fenwick_tree.md)
2. [Segment Tree](structures/segment_tree.md)
3. [Advanced Complexity Table](structures/advanced_complexity_table.md)

## Practice

- ../exercises/fenwick_tree_exercises.md
- ../exercises/segment_tree_exercises.md

## C++11 Example: Segment Tree (Range Sum)

```cpp
#include <vector>
using namespace std;

struct SegTree {
    int n;
    vector<long long> t;
    explicit SegTree(int n) : n(n), t(4 * n, 0) {}
    void update(int v, int tl, int tr, int pos, int val) {
        if (tl == tr) { t[v] = val; return; }
        int tm = (tl + tr) / 2;
        if (pos <= tm) update(v * 2, tl, tm, pos, val);
        else update(v * 2 + 1, tm + 1, tr, pos, val);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }
};
```