# Disjoint Set Union

## Concept

A disjoint set union (DSU), also called union-find, maintains a collection of non-overlapping sets and answers two questions efficiently: which set does an element belong to (`find`), and merge the sets of two elements (`union`). Each set is a tree whose root acts as the set's representative; two elements are in the same set exactly when they share a root. Two optimizations make it almost constant time: union by rank attaches the shorter tree under the taller one to keep trees flat, and path compression points every node visited during `find` directly at the root. It is the standard tool for connectivity queries, Kruskal's MST, and grouping equivalences.

## Mermaid

```mermaid
graph TD
    R((root 1)) --> A((0))
    R --> B((4))
    B --> C((2))
    S((root 3)) --> D((5))
```

Two sets: {0,1,2,4} rooted at 1, and {3,5} rooted at 3.

## Complexity

- find / union: O(alpha(n)) amortized, where alpha is the inverse Ackermann function (effectively constant, < 5 for any practical n)
- Without optimizations: O(n) worst case per operation
- Space: O(n) for the parent and rank arrays

## C++11 Code

```cpp
#include <vector>
#include <numeric>
using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> rank_;   // upper bound on tree height

    explicit DSU(int n) : parent(n), rank_(n, 0) {
        iota(parent.begin(), parent.end(), 0); // each element its own root
    }

    // Find with path compression: re-point nodes straight to the root.
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    // Union by rank: hang the shorter tree under the taller one.
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;        // already in the same set
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
        return true;
    }

    bool connected(int a, int b) { return find(a) == find(b); }
};
```

## Mini Usage Example

```cpp
DSU dsu(6);
dsu.unite(0, 1);
dsu.unite(2, 4);
dsu.unite(0, 4);

dsu.connected(1, 2);  // true  (1-0-4-2 all merged)
dsu.connected(1, 3);  // false (3 is still alone)
```

## Code Snippet Flow

```mermaid
flowchart TD
    A[unite a,b] --> B[ra = find a, rb = find b]
    B --> C{ra == rb?}
    C -- yes --> D[Already same set, do nothing]
    C -- no --> E{rank ra < rank rb?}
    E -- yes --> F[swap so ra is taller]
    E -- no --> G[keep ra as root]
    F --> H[parent rb = ra]
    G --> H
    H --> I{equal ranks?}
    I -- yes --> J[increment rank ra]
```
