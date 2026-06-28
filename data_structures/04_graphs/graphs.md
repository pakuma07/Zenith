# Graphs

A graph is a collection of vertices and edges.

This chapter has been split into one file per representation and traversal.

## Graph Structure Chapters

1. [Adjacency Matrix](structures/adjacency_matrix.md)
2. [Adjacency List](structures/adjacency_list.md)
3. [BFS](structures/bfs.md)
4. [DFS](structures/dfs.md)

## Complexity

| Representation | Space | Edge Check |
|----------------|-------|------------|
| Adjacency Matrix | O(V^2) | O(1) |
| Adjacency List | O(V + E) | O(deg(u)) |

## Practice

Use chapter-style exercises from:
- ../exercises/graph_representation_exercises.md
- ../exercises/graph_traversal_exercises.md

## C++11 Example: Adjacency List Build

```cpp
#include <vector>
using namespace std;

vector<vector<int> > buildGraph(int n, const vector<pair<int,int> >& edges) {
    vector<vector<int> > g(n);
    for (const auto& e : edges) {
        g[e.first].push_back(e.second);
        g[e.second].push_back(e.first);
    }
    return g;
}
```