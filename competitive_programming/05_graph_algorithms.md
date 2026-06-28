# 05 — Graph Algorithms

Graph problems are the most common category in competitive programming. Know every template cold.

---

## 5.1 Graph Representation

```cpp
int N, M;   // N nodes (0-indexed or 1-indexed), M edges

// Adjacency list — standard for sparse graphs
vector<vector<int>> adj(N + 1);
adj[u].push_back(v);
adj[v].push_back(u);   // undirected

// Weighted adjacency list
vector<vector<pair<int,int>>> wadj(N + 1);  // {neighbor, weight}
wadj[u].push_back({v, w});

// Adjacency matrix — for dense graphs or Floyd-Warshall
int dist[305][305];
fill(&dist[0][0], &dist[0][0] + 305*305, 1e9);
for (int i = 0; i <= N; ++i) dist[i][i] = 0;

// Edge list — for Kruskal's
struct Edge { int u, v, w; };
vector<Edge> edges;
```

---

## 5.2 BFS — O(V + E)

Use for: **shortest path in unweighted graph**, level-order traversal, flood fill.

```cpp
vector<int> bfs(int src, const vector<vector<int>>& adj, int N) {
    vector<int> dist(N + 1, -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;   // dist[i] = shortest hops from src, -1 if unreachable
}
```

---

## 5.3 DFS — O(V + E)

Use for: **cycle detection**, **connected components**, **topological sort**, **tree problems**.

```cpp
vector<bool> visited;

void dfs(int u, const vector<vector<int>>& adj) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfs(v, adj);
    }
}

// Count connected components
int components = 0;
visited.assign(N + 1, false);
for (int i = 1; i <= N; ++i) {
    if (!visited[i]) { dfs(i, adj); ++components; }
}
```

---

## 5.4 Dijkstra — O((V + E) log V)

Use for: **shortest path in weighted graph with non-negative weights**.

```cpp
typedef pair<long long,int> pli;

vector<long long> dijkstra(int src, const vector<vector<pair<int,int>>>& adj, int N) {
    vector<long long> dist(N + 1, LLONG_MAX);
    priority_queue<pli, vector<pli>, greater<pli>> pq;  // min-heap
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        long long d = top.first; int u = top.second;
        if (d > dist[u]) continue;    // stale entry — skip
        for (auto& e : adj[u]) {
            int v = e.first; long long w = e.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

---

## 5.5 Bellman-Ford — O(V × E)

Use for: **negative weight edges**, **detecting negative cycles**.

```cpp
struct Edge { int u, v, w; };

vector<long long> bellmanFord(int src, vector<Edge>& edges, int N) {
    vector<long long> dist(N + 1, LLONG_MAX);
    dist[src] = 0;
    for (int iter = 0; iter < N - 1; ++iter) {
        for (auto& e : edges) {
            if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v])
                dist[e.v] = dist[e.u] + e.w;
        }
    }
    // N-th relaxation detects negative cycle
    for (auto& e : edges) {
        if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v])
            cout << "Negative cycle detected\n";
    }
    return dist;
}
```

---

## 5.6 Floyd-Warshall — O(V³)

Use for: **all-pairs shortest paths**, small N (≤ 400).

```cpp
void floydWarshall(vector<vector<long long>>& d, int N) {
    for (int k = 1; k <= N; ++k)
        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= N; ++j)
                if (d[i][k] != LLONG_MAX && d[k][j] != LLONG_MAX)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    // Negative cycle: d[i][i] < 0 for some i
}
```

---

## 5.7 Topological Sort

**Kahn's Algorithm (BFS-based)** — detects cycles too:

```cpp
vector<int> topoSort(const vector<vector<int>>& adj, int N) {
    vector<int> indegree(N + 1, 0);
    for (int u = 1; u <= N; ++u)
        for (int v : adj[u]) indegree[v]++;

    queue<int> q;
    for (int i = 1; i <= N; ++i) if (indegree[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indegree[v] == 0) q.push(v);
    }
    // If order.size() != N, graph has a cycle
    return order;
}
```

---

## 5.8 Disjoint Set Union (DSU / Union-Find)

```cpp
struct DSU {
    vector<int> parent, rank_;
    DSU(int n) : parent(n+1), rank_(n+1, 0) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);  // path compression
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;   // already connected
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
        return true;
    }
    bool connected(int a, int b) { return find(a) == find(b); }
};
```

---

## 5.9 Minimum Spanning Tree

### Kruskal's — O(E log E)

```cpp
long long kruskal(vector<Edge>& edges, int N) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b){ return a.w < b.w; });
    DSU dsu(N);
    long long cost = 0; int cnt = 0;
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            cost += e.w;
            if (++cnt == N - 1) break;
        }
    }
    return cost;  // cost of MST; cnt < N-1 means graph is disconnected
}
```

### Prim's — O(E log V) with priority queue

```cpp
long long prim(const vector<vector<pair<int,int>>>& adj, int N) {
    vector<long long> key(N + 1, LLONG_MAX);
    vector<bool> inMST(N + 1, false);
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    key[1] = 0; pq.push({0, 1});
    long long total = 0;
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        long long w = top.first; int u = top.second;
        if (inMST[u]) continue;
        inMST[u] = true; total += w;
        for (auto& e : adj[u]) {
            int v = e.first; long long ew = e.second;
            if (!inMST[v] && ew < key[v]) { key[v] = ew; pq.push({key[v], v}); }
        }
    }
    return total;
}
```

---

## 5.10 Bridges and Articulation Points — O(V + E)

```cpp
int disc[MAXN], low[MAXN], timer_val = 0;
bool isArticulation[MAXN];
vector<pair<int,int>> bridges;

void dfs_bridge(int u, int parent, const vector<vector<int>>& adj) {
    disc[u] = low[u] = ++timer_val;
    int children = 0;
    for (int v : adj[u]) {
        if (!disc[v]) {
            children++;
            dfs_bridge(v, u, adj);
            low[u] = min(low[u], low[v]);
            if (parent == -1 && children > 1)       isArticulation[u] = true;
            if (parent != -1 && low[v] >= disc[u])   isArticulation[u] = true;
            if (low[v] > disc[u]) bridges.push_back({u, v});
        } else if (v != parent) {
            low[u] = min(low[u], disc[v]);
        }
    }
}
```

---

## 5.11 Strongly Connected Components — Kosaraju's O(V + E)

```cpp
vector<int> order;
vector<bool> visited;

void dfs1(int u, const vector<vector<int>>& adj) {
    visited[u] = true;
    for (int v : adj[u]) if (!visited[v]) dfs1(v, adj);
    order.push_back(u);
}

void dfs2(int u, int comp, vector<int>& component, const vector<vector<int>>& radj) {
    component[u] = comp;
    for (int v : radj[u]) if (!component[v]) dfs2(v, comp, component, radj);
}

int kosaraju(int N, const vector<vector<int>>& adj, const vector<vector<int>>& radj) {
    visited.assign(N + 1, false);
    for (int i = 1; i <= N; ++i) if (!visited[i]) dfs1(i, adj);
    vector<int> component(N + 1, 0);
    int comp = 0;
    for (int i = (int)order.size() - 1; i >= 0; --i) {
        int u = order[i];
        if (!component[u]) dfs2(u, ++comp, component, radj);
    }
    return comp;  // number of SCCs
}
```

---

## 5.12 Bipartite Check

```cpp
bool isBipartite(const vector<vector<int>>& adj, int N) {
    vector<int> color(N + 1, -1);
    for (int s = 1; s <= N; ++s) {
        if (color[s] != -1) continue;
        queue<int> q; q.push(s); color[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) { color[v] = 1 - color[u]; q.push(v); }
                else if (color[v] == color[u]) return false;
            }
        }
    }
    return true;
}
```

---

## 5.13 LCA — Lowest Common Ancestor (Binary Lifting) — O(N log N) / O(log N)

```cpp
const int LOG = 17;
int up[100005][LOG], dep[100005];
vector<int> ch[100005];

void dfs_lca(int u, int p, int d) {
    up[u][0] = p; dep[u] = d;
    for (int j = 1; j < LOG; ++j) up[u][j] = up[up[u][j-1]][j-1];
    for (int v : ch[u]) if (v != p) dfs_lca(v, u, d+1);
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    int diff = dep[u] - dep[v];
    for (int j = 0; j < LOG; ++j) if ((diff>>j)&1) u = up[u][j];
    if (u == v) return u;
    for (int j = LOG-1; j >= 0; --j) if (up[u][j] != up[v][j]) { u=up[u][j]; v=up[v][j]; }
    return up[u][0];
}
// Tree distance: dep[u] + dep[v] - 2*dep[lca(u,v)]
// Call: dfs_lca(root, root, 0);
```

---

## 5.14 0-1 BFS — O(V + E)

**Use for**: graphs where edge weights are only 0 or 1. Replace priority_queue with a deque.

```cpp
vector<int> bfs01(int src, const vector<vector<pair<int,int>>>& adj, int N) {
    vector<int> dist(N+1, INT_MAX);
    deque<int> dq;
    dist[src] = 0; dq.push_back(src);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);   // 0-weight: push front
                else        dq.push_back(v);     // 1-weight: push back
            }
        }
    }
    return dist;
}
```

---

## 5.15 Graph Algorithm Summary

| Problem | Algorithm | Complexity |
|---------|-----------|------------|
| Shortest path, unweighted | BFS | O(V + E) |
| Shortest path, 0/1 weights | 0-1 BFS | O(V + E) |
| Shortest path, non-negative weights | Dijkstra | O((V+E) log V) |
| Shortest path, negative weights | Bellman-Ford | O(VE) |
| All-pairs shortest path | Floyd-Warshall | O(V³) |
| Topological order | Kahn's / DFS | O(V + E) |
| Connected components | DFS / DSU | O(V + E) |
| Minimum spanning tree | Kruskal / Prim | O(E log E) |
| Bridges | Tarjan DFS | O(V + E) |
| SCCs | Kosaraju / Tarjan | O(V + E) |
| Bipartite check | BFS colouring | O(V + E) |
| LCA | Binary lifting | O(N log N) build, O(log N) query |
| Path queries on tree | HLD (see ch13) | O(log² N) |

---

**Next**: [06 — Dynamic Programming](06_dynamic_programming.md)
