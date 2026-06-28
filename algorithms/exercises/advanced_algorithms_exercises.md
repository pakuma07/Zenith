# Exercises: Advanced Algorithms

## Easy

1. Floyd-Warshall on a 4-node graph.
2. Detect negative cycle via diagonal after Floyd-Warshall.
3. Residual graph update concept check.
4. Binary lifting ancestor table setup.
5. Convex hull orientation test.

## Medium

1. Bellman-Ford versus Dijkstra comparison case.
2. Implement Edmonds-Karp BFS augmenting path step.
3. SCC condensation graph construction.
4. Range ancestor queries with binary lifting.
5. Monotonic hull construction basics.

## Hard

1. Full Edmonds-Karp implementation.
2. Tarjan SCC implementation.
3. Lowest common ancestor with binary lifting.
4. Bitmask DP for TSP.
5. Convex hull trick overview problem.

## Challenge

Pick one advanced algorithm and write a note explaining when it becomes necessary over simpler alternatives.

---

## Next Steps

- Read the matching theory: [../11_advanced_algorithms/advanced_algorithms.md](../11_advanced_algorithms/advanced_algorithms.md)
- Previous: [Number Theory and Bit Exercises](number_theory_bit_exercises.md)
- Return to [Algorithms Exercises Overview](README.md)

## C++11 Exercise Example: Topological Sort

```cpp
#include <vector>
#include <queue>
using namespace std;

vector<int> topoSort(int n, const vector<vector<int> >& g) {
    vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; ++u) for (int v : g[u]) ++indeg[v];
    queue<int> q;
    for (int i = 0; i < n; ++i) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order;
}
```