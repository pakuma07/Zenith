# Disjoint Set Union (Union-Find)

This chapter was split for consistency.

Use the detailed version here:

- [structures/disjoint_set_union.md](structures/disjoint_set_union.md)

Practice problems:

- ../exercises/dsu_exercises.md

## C++11 Example: DSU

```cpp
#include <vector>
using namespace std;

struct DSU {
    vector<int> p, r;
    explicit DSU(int n) : p(n), r(n, 0) { for (int i = 0; i < n; ++i) p[i] = i; }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
        return true;
    }
};
```