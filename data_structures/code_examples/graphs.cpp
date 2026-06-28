// graphs.cpp
// Adjacency-list graph supporting breadth-first (BFS) and depth-first (DFS) traversal.
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Graph: stores vertices 0..n-1 as an adjacency list (vector of neighbor lists).
// Space O(n + edges). BFS and DFS each visit every vertex/edge once: O(n + edges).
class Graph {
private:
    int n;
    vector<vector<int>> adj;  // adj[u] = list of vertices adjacent to u

    // Recursive DFS helper: mark u, print it, then recurse into unvisited neighbors.
    void dfsUtil(int u, vector<bool>& visited) const {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) dfsUtil(v, visited);  // descend only into unseen vertices
        }
    }

public:
    // Build an empty graph with the given vertex count. O(n).
    Graph(int vertices) : n(vertices), adj(vertices) {}

    // Add edge u->v; for undirected graphs also add the reverse edge v->u. O(1).
    void addEdge(int u, int v, bool undirected = true) {
        adj[u].push_back(v);
        if (undirected) adj[v].push_back(u);
    }

    // Breadth-first traversal from start, printing vertices in level order. O(n + edges).
    void bfs(int start) const {
        vector<bool> visited(n, false);
        queue<int> q;
        visited[start] = true;  // mark on enqueue to avoid re-adding the same vertex
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();  // dequeue next frontier vertex
            cout << u << " ";
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;   // mark before enqueue (prevents duplicate enqueues)
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

    // Depth-first traversal from start via the recursive helper. O(n + edges).
    void dfs(int start) const {
        vector<bool> visited(n, false);
        dfsUtil(start, visited);
        cout << endl;
    }
};

// Driver: builds a small graph and prints its BFS and DFS orders from vertex 0.
int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "BFS from 0: ";
    g.bfs(0);

    cout << "DFS from 0: ";
    g.dfs(0);

    return 0;
}
