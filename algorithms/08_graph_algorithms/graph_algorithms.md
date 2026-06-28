# Graph Algorithms

This chapter has been split into micro-files.

## Structure Files

1. [Graph Traversals](structures/traversals.md)
2. [Dijkstra](structures/dijkstra.md)
3. [Bellman-Ford](structures/bellman_ford.md)
4. [Floyd-Warshall](structures/floyd_warshall.md)
5. [Topological Sort](structures/topological_sort.md)
6. [Minimum Spanning Tree](structures/minimum_spanning_tree.md)

## Practice

- ../exercises/graph_algorithms_exercises.md

## Related Data Structures

- ../../data_structures/04_graphs/graphs.md

---

## Next Steps

- Review the cheat sheet: [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md)
- Previous: [Chapter 7: Dynamic Programming](../07_dynamic_programming/dynamic_programming.md)
- Next: [Chapter 9: String Algorithms](../09_string_algorithms/string_algorithms.md)


## C++11 Example: BFS Traversal

```cpp
#include <vector>
#include <queue>
using namespace std;

vector<int> bfs(int src, const vector<vector<int> >& g) {
    vector<int> vis(g.size(), 0), order;
    queue<int> q;
    vis[src] = 1;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) if (!vis[v]) { vis[v] = 1; q.push(v); }
    }
    return order;
}
```