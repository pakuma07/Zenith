# BFS

## Concept

Breadth-first search explores a graph level by level: it visits the source, then all vertices one edge away, then all two edges away, and so on. It uses a FIFO queue and a visited marker so each vertex is processed exactly once. Because it expands outward in rings, BFS finds the shortest path (fewest edges) from the source to every reachable vertex in an unweighted graph. Typical uses include shortest-path-by-hops, connectivity checks, and computing distances from a single source.

## Mermaid

```mermaid
graph TD
    0((0)) --> 1((1))
    0 --> 2((2))
    1 --> 3((3))
    2 --> 3
```

Visit order from source 0: `0, 1, 2, 3` (level 0, then level 1, then level 2).

## Complexity

- Time: O(V + E) — each vertex enqueued once, each edge examined once
- Space: O(V) for the visited array and the queue

## C++11 Code

```cpp
#include <vector>
#include <queue>
using namespace std;

// Returns vertices in the order BFS visits them, starting at src.
vector<int> bfsOrder(int src, const vector<vector<int>>& g) {
    vector<int> visited(g.size(), 0), order;
    queue<int> q;
    visited[src] = 1;      // mark before enqueue to avoid duplicates
    q.push(src);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);          // u is fully discovered
        for (int v : g[u]) {         // scan neighbors
            if (!visited[v]) {
                visited[v] = 1;
                q.push(v);
            }
        }
    }
    return order;
}
```

## Mini Usage Example

```cpp
// Adjacency list: 0->{1,2}, 1->{3}, 2->{3}, 3->{}
vector<vector<int>> g = {{1, 2}, {3}, {3}, {}};
vector<int> order = bfsOrder(0, g);
// order == {0, 1, 2, 3}
```

## Code Snippet Flow

```mermaid
flowchart TD
    A[Mark src visited, enqueue src] --> B{Queue empty?}
    B -- no --> C[Dequeue front u]
    C --> D[Append u to order]
    D --> E[For each neighbor v of u]
    E --> F{v visited?}
    F -- no --> G[Mark v, enqueue v]
    G --> E
    F -- yes --> E
    E --> B
    B -- yes --> H[Return order]
```
