# Traversals

## Concept

Graph traversal systematically visits every reachable vertex from a source, using a "visited" set to avoid revisiting nodes (which also prevents infinite loops on cycles). Breadth-first search (BFS) uses a FIFO queue, exploring vertices in increasing order of edge-distance from the source, so it naturally yields shortest paths in unweighted graphs. Depth-first search (DFS) follows one branch as far as possible before backtracking, typically via recursion (an implicit stack) or an explicit stack. Both run in O(V+E) on an adjacency list. Use BFS for shortest-hop or level-order problems and DFS for cycle detection, topological ordering, and connectivity analysis.

## Mermaid

```mermaid
flowchart TD
    A[Input Graph] --> B{Traversal Type}
    B -- BFS --> C[Level-order exploration]
    B -- DFS --> D[Depth-first exploration]
    C --> E[Process vertices by distance]
    D --> F[Process vertices by depth]
    E --> G[BFS Order]
    F --> H[DFS Order]
```

## Complexity

- Time: O(V+E)
- Space: O(V)

## C++11 Code

```cpp
#include <vector>
#include <queue>
#include <stack>
using namespace std;

vector<int> bfsTraversal(int src, const vector<vector<int> >& g) {
    vector<int> vis(g.size(), 0), order;
    queue<int> q;
    vis[src] = 1;
    q.push(src);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (!vis[v]) {
                vis[v] = 1;
                q.push(v);
            }
        }
    }
    return order;
}

void dfsHelper(int u, const vector<vector<int> >& g, vector<int>& vis, vector<int>& order) {
    vis[u] = 1;
    order.push_back(u);
    for (int v : g[u]) {
        if (!vis[v]) {
            dfsHelper(v, g, vis, order);
        }
    }
}

vector<int> dfsTraversal(int src, const vector<vector<int> >& g) {
    vector<int> vis(g.size(), 0), order;
    dfsHelper(src, g, vis, order);
    return order;
}
```

## Mini Usage Example

```cpp
vector<vector<int> > g = {{1, 2}, {0, 3}, {0, 3}, {1, 2}};
vector<int> bfsOrder = bfsTraversal(0, g);
vector<int> dfsOrder = dfsTraversal(0, g);
```

## Code Snippet Flow

```mermaid
flowchart LR
    A[Start Traversal] --> B{Choose Method}
    B -- BFS --> C[Initialize queue with source]
    B -- DFS --> D[Initialize recursion stack]
    C --> E[While queue not empty]
    D --> F{More vertices to visit?}
    E --> G[Dequeue and mark visited]
    G --> H[Enqueue all unvisited neighbors]
    H --> I[Add to BFS order]
    I --> J{Queue empty?}
    F -- Yes --> K[Visit recursively]
    K --> L[Mark as visited]
    L --> M[Recurse on neighbors]
    M --> N[Add to DFS order]
    N --> O{More neighbors?}
    J -- Yes --> E
    J -- No --> P[Return order]
    O -- Yes --> M
    O -- No --> P
```