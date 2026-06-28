# 14 — Flows, Matching, and 2-SAT

These topics appear in Codeforces Div 1 C/D and IOI. Master Dinic's and bipartite matching cold.

---

## 14.1 Maximum Flow — Dinic's Algorithm

**Complexity**: O(V² × E) general; O(E √V) for unit-capacity graphs (bipartite matching).

```cpp
struct Dinic {
    struct Edge { int to, rev; long long cap; };
    vector<vector<Edge>> graph;
    vector<int> level, iter;
    int n;

    Dinic(int n) : n(n), graph(n), level(n), iter(n) {}

    void addEdge(int from, int to, long long cap) {
        graph[from].push_back({to,   (int)graph[to].size(),   cap});
        graph[to].push_back  ({from, (int)graph[from].size()-1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto& e : graph[v])
                if (e.cap > 0 && level[e.to] < 0) { level[e.to] = level[v]+1; q.push(e.to); }
        }
        return level[t] >= 0;
    }

    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)graph[v].size(); ++i) {
            Edge& e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) { e.cap -= d; graph[e.to][e.rev].cap += d; return d; }
            }
        }
        return 0;
    }

    long long maxflow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            long long d;
            while ((d = dfs(s, t, LLONG_MAX)) > 0) flow += d;
        }
        return flow;
    }
};

// Usage:
// Dinic dinic(N + 2);   // N nodes + source 0 + sink N+1
// dinic.addEdge(s, v, cap);
// long long ans = dinic.maxflow(s, t);
```

---

## 14.2 Min Cut from Max Flow

By **Max-Flow Min-Cut Theorem**: max flow = min cut.

```cpp
// After maxflow, find the min cut:
// S-side: all nodes reachable from source in the residual graph
// T-side: everything else
// Cut edges: edges from S-side to T-side with 0 residual capacity

vector<bool> minCutSide(Dinic& dinic, int s) {
    int n = dinic.graph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    visited[s] = true; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (auto& e : dinic.graph[v])
            if (e.cap > 0 && !visited[e.to]) { visited[e.to] = true; q.push(e.to); }
    }
    return visited;  // true = S-side
}
```

---

## 14.3 Bipartite Matching — Hopcroft-Karp — O(E √V)

```cpp
struct HopcroftKarp {
    int n, m;                    // left: 0..n-1, right: 0..m-1
    vector<vector<int>> adj;
    vector<int> matchL, matchR, dist_;

    HopcroftKarp(int n, int m) : n(n), m(m), adj(n), matchL(n,-1), matchR(m,-1), dist_(n) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (matchL[u] == -1) { dist_[u] = 0; q.push(u); }
            else dist_[u] = INT_MAX;
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int w = matchR[v];
                if (w == -1) found = true;
                else if (dist_[w] == INT_MAX) { dist_[w] = dist_[u]+1; q.push(w); }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            int w = matchR[v];
            if (w == -1 || (dist_[w] == dist_[u]+1 && dfs(w))) {
                matchL[u] = v; matchR[v] = u; return true;
            }
        }
        dist_[u] = INT_MAX; return false;
    }

    int maxMatching() {
        int match = 0;
        while (bfs()) for (int u = 0; u < n; ++u) if (matchL[u]==-1 && dfs(u)) match++;
        return match;
    }
};
// Minimum vertex cover = max matching (König's theorem)
// Maximum independent set = n - max matching (in bipartite)
```

---

## 14.4 2-SAT (Two-Variable Boolean Satisfiability)

**Use for**: constraints of the form `(a OR b)` where a, b are boolean literals.  
Every 2-SAT problem is solvable in O(V + E) via SCC.

```cpp
struct TwoSAT {
    int n;
    vector<vector<int>> adj, radj;
    vector<int> order, comp;
    vector<bool> visited;

    TwoSAT(int n) : n(n), adj(2*n), radj(2*n), comp(2*n,-1), visited(2*n,false) {}

    // Variable i: true = node i, false = node i+n
    int pos(int i) { return i; }
    int neg(int i) { return i + n; }

    // Add clause: (a OR b) where a, b are literals (use pos/neg)
    void addClause(int u, int v) {
        adj[u^1].push_back(v);   // NOT u → v
        adj[v^1].push_back(u);   // NOT v → u
        radj[v].push_back(u^1);
        radj[u].push_back(v^1);
    }

    // More intuitive: x_i = val_i OR x_j = val_j
    // true literal for variable i: 2*i, false literal: 2*i+1
    void addClause2(int i, bool vi, int j, bool vj) {
        // (x_i == vi) OR (x_j == vj)
        // encoded: (2i + !vi) → (2j + vj)  and  (2j + !vj) → (2i + vi)
        adj[2*i + !vi].push_back(2*j + vj);
        adj[2*j + !vj].push_back(2*i + vi);
    }

    void dfs1(int v) { visited[v]=true; for(int u:adj[v]) if(!visited[u]) dfs1(u); order.push_back(v); }
    void dfs2(int v, int c) { comp[v]=c; for(int u:radj[v]) if(comp[u]==-1) dfs2(u,c); }

    // Returns true if satisfiable; values[i] = assignment for variable i
    bool solve(vector<bool>& values) {
        for (int i = 0; i < 2*n; ++i) if (!visited[i]) dfs1(i);
        int c = 0;
        for (int i = (int)order.size()-1; i >= 0; --i)
            if (comp[order[i]] == -1) dfs2(order[i], c++);
        values.resize(n);
        for (int i = 0; i < n; ++i) {
            if (comp[2*i] == comp[2*i+1]) return false;  // unsatisfiable
            values[i] = comp[2*i] > comp[2*i+1];
        }
        return true;
    }
};

// Example: "at most one of x_i, x_j is true" = NOT(x_i AND x_j) = (!x_i OR !x_j)
// sat.addClause2(i, false, j, false);
```

---

## 14.5 Minimum Cost Maximum Flow (MCMF)

**Use for**: flow with costs (assignment problems, transportation).

```cpp
struct MCMF {
    struct Edge { int to, cap, cost, flow; };
    int n;
    vector<Edge> edges;
    vector<vector<int>> g;
    vector<int> d, p, a;
    vector<bool> inq;

    MCMF(int n) : n(n), g(n), d(n), p(n), a(n), inq(n) {}

    void addEdge(int from, int to, int cap, int cost) {
        g[from].push_back(edges.size());
        edges.push_back({to, cap, cost, 0});
        g[to].push_back(edges.size());
        edges.push_back({from, 0, -cost, 0});
    }

    pair<int,int> minCostFlow(int s, int t, int maxFlow) {
        int flow = 0, cost = 0;
        while (flow < maxFlow) {
            fill(d.begin(), d.end(), INT_MAX);
            d[s] = 0; a[s] = maxFlow - flow;
            fill(inq.begin(), inq.end(), false);
            queue<int> q; q.push(s); inq[s] = true;
            while (!q.empty()) {        // SPFA (Bellman-Ford with queue)
                int u = q.front(); q.pop(); inq[u] = false;
                for (int id : g[u]) {
                    Edge& e = edges[id];
                    if (e.cap > e.flow && d[e.to] > d[u] + e.cost) {
                        d[e.to] = d[u] + e.cost;
                        p[e.to] = id;
                        a[e.to] = min(a[u], e.cap - e.flow);
                        if (!inq[e.to]) { inq[e.to]=true; q.push(e.to); }
                    }
                }
            }
            if (d[t] == INT_MAX) break;
            flow += a[t]; cost += a[t] * d[t];
            int u = t;
            while (u != s) { edges[p[u]].flow += a[t]; edges[p[u]^1].flow -= a[t]; u = edges[p[u]^1].to; }
        }
        return {flow, cost};
    }
};
```

---

## 14.6 Euler Path and Circuit — O(V + E)

**Euler path**: visits every edge exactly once.  
**Euler circuit**: same, but starts and ends at the same vertex.

```cpp
// Conditions (undirected):
// Circuit: all vertices have even degree (and graph is connected)
// Path: exactly 2 vertices have odd degree

// Hierholzer's algorithm
vector<int> eulerPath(int start, int N, vector<vector<pair<int,int>>>& adj) {
    // adj[u] = {v, edge_index}; mark used edges
    vector<bool> usedEdge;  // size = number of edges
    // ... (see below for complete iterative version)

    vector<int> path;
    stack<int> st;
    vector<int> ptr(N+1, 0);
    st.push(start);
    while (!st.empty()) {
        int u = st.top();
        if (ptr[u] == (int)adj[u].size()) { path.push_back(u); st.pop(); }
        else {
            auto [v, idx] = adj[u][ptr[u]++];
            if (!usedEdge[idx]) { usedEdge[idx] = true; st.push(v); }
        }
    }
    reverse(path.begin(), path.end());
    return path;  // size should be E+1 for Euler path/circuit
}
```

---

## 14.7 Flow/Matching Decision Guide

```
Maximum flow needed?                    → Dinic's
Bipartite matching?                     → Hopcroft-Karp (or Dinic's on bipartite)
Minimum vertex cover in bipartite?      → König: = max matching
Maximum independent set in bipartite?  → n - max matching
Assignment problem (min cost matching)? → MCMF or Hungarian O(N³)
Boolean satisfiability (2 literals)?    → 2-SAT
Euler circuit/path exists?              → Check degree conditions, Hierholzer
```

---

## 14.8 Key Theorems

```
Max-Flow Min-Cut   : max flow from s to t = min capacity of any s-t cut
König's Theorem    : in bipartite graph, max matching = min vertex cover
Hall's Theorem     : bipartite graph has perfect matching iff for every subset S
                     of left vertices, |N(S)| >= |S|
Dilworth's Theorem : in a DAG, min number of chains to cover = max antichain size
                     (solved with bipartite matching)
```

---

**Next**: [15 — DP Optimizations](15_dp_optimizations.md)
