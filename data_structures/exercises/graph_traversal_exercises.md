# Exercises: Graph Traversal (BFS/DFS)

## Easy

1. BFS traversal from source.
2. DFS traversal recursive.
3. DFS traversal iterative.
4. Check path exists between two nodes.
5. Count reachable nodes from source.

## Medium

1. Shortest path in unweighted graph.
2. Bipartite check using BFS.
3. Cycle detection in undirected graph.
4. Topological sort using DFS.
5. Detect cycle in directed graph.

## Hard

1. Strongly connected components.
2. Articulation points and bridges.
3. Euler path/circuit existence.
4. Multi-source BFS problems.
5. Grid shortest path with obstacles.

## Challenge

Implement graph explorer CLI that runs BFS/DFS and reports traversal tree.

## C++11 Exercise Example: DFS

```cpp
#include <vector>
using namespace std;

void dfs(int u, const vector<vector<int> >& g, vector<int>& vis) {
    vis[u] = 1;
    for (int v : g[u]) if (!vis[v]) dfs(v, g, vis);
}
```
