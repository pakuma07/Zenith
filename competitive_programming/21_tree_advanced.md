# 21 - Advanced Tree Techniques

Tree problems appear in nearly every Codeforces and AtCoder round.
This file covers techniques that top competitors use beyond basic DFS/LCA.

---

## 21.1 Euler Tour (DFS Order / Linearization)

**Flatten a tree into an array so subtree queries become range queries.**
Every subtree of node u corresponds to the range [in[u], out[u]] in the DFS order.

```cpp
const int MAXN = 200005;
int in_time[MAXN], out_time[MAXN], order[MAXN];
int timer_val = 0;

void dfs_euler(int u, int p, const vector<vector<int>> &adj) {
    in_time[u] = timer_val;
    order[timer_val++] = u;
    for(int v : adj[u]) {
        if(v != p) dfs_euler(v, u, adj);
    }
    out_time[u] = timer_val - 1;
}
// After DFS:
// - Subtree of u = array indices [in_time[u], out_time[u]]
// - Subtree sum = segTree.query(in_time[u], out_time[u])
// - Update subtree values = segTree.update(in_time[u], out_time[u], val)
```

**Heavy path linearization for HLD (see 13_advanced_data_structures.md)**:
Same Euler tour but visiting heavy child first.

**Euler tour for LCA (alternative to binary lifting):**
The LCA of u and v is the node with minimum depth in the Euler tour between
the first occurrence of u and the first occurrence of v.
Build sparse table on depths in Euler tour -> O(1) LCA queries.

```cpp
// Euler tour LCA: visits 2n-1 nodes (back to parent after each child)
vector<int> euler_lca;  // 2n-1 entries
vector<int> depth_lca, first_occ(MAXN);

void dfs_lca_euler(int u, int p, int d, const vector<vector<int>> &adj) {
    first_occ[u] = euler_lca.size();
    euler_lca.push_back(u);
    depth_lca.push_back(d);
    for(int v : adj[u]) {
        if(v != p) {
            dfs_lca_euler(v, u, d+1, adj);
            euler_lca.push_back(u);
            depth_lca.push_back(d);
        }
    }
}
// Build sparse table on depth_lca
// lca(u, v) = euler_lca[argmin_{depth_lca} in [first_occ[u], first_occ[v]]]
```

---

## 21.2 Rerooting DP (Tree DP with Rerooting)

**Problem**: Compute some value f(root=u) for every node u as root.
**Technique**: Two-pass DFS.
1. Root at node 1, compute f() for all subtrees downward.
2. Reroot: pass parent's contribution down, recompute f() for each node as root.

**Classic example: sum of distances from each node to all other nodes**

```cpp
// dist_sum[u] = sum of distances from u to all other nodes
// Step 1: count subtree sizes and downward dist sums
vector<int> sz(n+1, 1);
vector<long long> down(n+1, 0);  // sum of distances to nodes in subtree

function<void(int,int)> dfs1 = [&](int u, int p) {
    for(int v : adj[u]) if(v != p) {
        dfs1(v, u);
        sz[u] += sz[v];
        down[u] += down[v] + sz[v];  // each node in subtree[v] is one step further
    }
};
dfs1(1, 0);

// Step 2: reroot - propagate answer from parent to child
vector<long long> ans(n+1, 0);
ans[1] = down[1];

function<void(int,int)> dfs2 = [&](int u, int p) {
    for(int v : adj[u]) if(v != p) {
        // When we reroot to v:
        // - nodes in subtree[v]: distance decreases by 1 each -> -sz[v]
        // - nodes outside subtree[v]: distance increases by 1 each -> +(n-sz[v])
        ans[v] = ans[u] - sz[v] + (n - sz[v]);
        dfs2(v, u);
    }
};
dfs2(1, 0);
// ans[u] = sum of distances from u to all other nodes
```

**General rerooting template:**
```cpp
// f(u, parent) = some function of subtree
// When moving root from u to child v:
//   remove v's contribution from f(u), add u's contribution to f(v)
// The key: find inverse operation of "adding a child"
```

---

## 21.3 Functional Graphs

A graph where each node has **exactly one outgoing edge**. Consists of several
"rho" structures: a cycle with trees hanging off it.

**Applications**: permutation cycles, "follow the pointer" problems.

```cpp
// Find cycle length and distance to cycle for each node
int n;
vector<int> nxt(n+1);  // nxt[u] = single outgoing edge from u
vector<int> visited(n+1, 0), onCycle(n+1, 0);
vector<int> cycleLen(n+1, 0), distToCycle(n+1, -1);

// Floyd's detection or coloring
vector<int> color(n+1, 0), parent_fg(n+1, -1);
for(int start = 1; start <= n; start++) {
    if(color[start]) continue;
    // Trace the path from start
    vector<int> path;
    int cur = start;
    while(!color[cur]) {
        color[cur] = start;  // mark with current component id
        path.push_back(cur);
        cur = nxt[cur];
    }
    if(color[cur] == start) {
        // Found a new cycle: cur is on the cycle
        int len = 0;
        int tmp = cur;
        do { onCycle[tmp] = 1; tmp = nxt[tmp]; len++; } while(tmp != cur);
        for(int node : path) {
            if(onCycle[node]) { cycleLen[node] = len; distToCycle[node] = 0; }
        }
    }
    // Compute distToCycle for tree nodes
    for(int i = path.size()-1; i >= 0; i--) {
        int u = path[i];
        if(distToCycle[u] == -1)
            distToCycle[u] = distToCycle[nxt[u]] + 1;
    }
}
```

**K-th successor in functional graph** (jumping): use binary lifting.

```cpp
// k-th step from u in functional graph
// Binary lifting: up[u][j] = 2^j-th successor of u
int up[MAXN][LOG];
for(int i = 1; i <= n; i++) up[i][0] = nxt[i];
for(int j = 1; j < LOG; j++)
    for(int i = 1; i <= n; i++)
        up[i][j] = up[up[i][j-1]][j-1];

int kthSuccessor(int u, long long k) {
    for(int j = 0; j < LOG; j++)
        if((k >> j) & 1) u = up[u][j];
    return u;
}
```

---

## 21.4 Virtual Tree (Auxiliary Tree)

Given a tree and a set of K key nodes, build a virtual tree containing only
the key nodes and their LCAs. Size O(K).

**Use case**: Problems where queries involve only a small subset K of nodes.
Reduces O(N) tree DP to O(K log N).

```cpp
// Steps:
// 1. Sort key nodes by DFS in-time
// 2. Add consecutive LCAs to the key set
// 3. Build virtual tree using a stack

// Requires: precomputed LCA (binary lifting) and in_time[]
vector<int> buildVirtualTree(vector<int> keys) {
    sort(keys.begin(), keys.end(), [](int a, int b){ return in_time[a] < in_time[b]; });
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    int sz = keys.size();
    // Add pairwise LCAs of adjacent keys in sorted order
    vector<int> nodes = keys;
    for(int i = 0; i + 1 < sz; i++) nodes.push_back(lca(keys[i], keys[i+1]));
    // Add root if not already there
    nodes.push_back(lca(keys[0], keys[sz-1]));
    // Sort by in_time, deduplicate
    sort(nodes.begin(), nodes.end(), [](int a, int b){ return in_time[a] < in_time[b]; });
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    // Build adjacency using stack
    vector<vector<int>> vAdj(nodes.size());
    // ... (use stack of ancestors to connect edges)
    return nodes;  // virtual tree nodes
}
```

---

## 21.5 Small-to-Large Merging (DSU on Tree / Heavy-Light Merging)

When merging two sets at a tree node, always iterate over the smaller set.
Each element is moved O(log N) times total -> O(N log N) overall.

```cpp
// Merge sets at tree nodes, maintaining some property
// Each node has a set; merge children's sets into parent's set
vector<set<int>> nodeSet(n+1);
// Initialize: nodeSet[leaf] = {value[leaf]}

function<void(int,int)> dfs = [&](int u, int p) {
    for(int v : adj[u]) if(v != p) {
        dfs(v, u);
        // Merge smaller into larger
        if(nodeSet[u].size() < nodeSet[v].size())
            swap(nodeSet[u], nodeSet[v]);
        for(int x : nodeSet[v]) nodeSet[u].insert(x);
    }
    // Now nodeSet[u] contains all values in subtree of u
    // Answer queries for u here
};
```

---

## 21.6 Centroid Decomposition (covered in 13, extended pattern)

**Pattern: solve path queries by decomposing at centroid**

```cpp
// centDist[u][v] = sum of edge weights on path u -> centroid -> v
// For all paths through centroid c:
//   answer(u, v) = f(u, c) combined with f(v, c)
// Store f(u, c) in a hash map / sorted array at centroid c
// After processing, "delete" centroid and recurse on subtrees

// Template:
bool removed[MAXN] = {};
int sz_cent[MAXN];

void getSize(int u, int p) {
    sz_cent[u] = 1;
    for(int v : adj[u]) if(v != p && !removed[v]) {
        getSize(v, u); sz_cent[u] += sz_cent[v];
    }
}
int getCentroid(int u, int p, int treeSize) {
    for(int v : adj[u]) if(v != p && !removed[v])
        if(sz_cent[v] > treeSize / 2) return getCentroid(v, u, treeSize);
    return u;
}
void decompose(int u) {
    getSize(u, -1);
    int c = getCentroid(u, -1, sz_cent[u]);
    removed[c] = true;
    // Process all paths through c
    // ...
    for(int v : adj[c]) if(!removed[v]) decompose(v);
}
```

---

## 21.7 Tree Isomorphism

Check if two trees are identical (up to relabeling).
AHU (Aho-Hopcroft-Ullman) algorithm: O(N log N).

```cpp
// Canonical hash for rooted tree
// Hash of leaf = 0 (base case)
// Hash of node = hash of sorted multiset of children hashes

map<vector<int>, int> hashTable;
int hashCounter = 0;

function<int(int,int)> treeHash = [&](int u, int p) -> int {
    vector<int> childHashes;
    for(int v : adj[u]) if(v != p)
        childHashes.push_back(treeHash(v, u));
    sort(childHashes.begin(), childHashes.end());
    if(!hashTable.count(childHashes))
        hashTable[childHashes] = hashCounter++;
    return hashTable[childHashes];
};

// Two rooted trees T1, T2 (at roots r1, r2) are isomorphic iff
//   treeHash(r1, -1) == treeHash(r2, -1)

// For unrooted trees: try all possible roots (centroid(s)) as root
```

---

## 21.8 Link-Cut Tree (Dynamic Trees)

Maintains a forest under link/cut operations, supporting path queries in O(log N).
Complex to implement; used in advanced CF problems.

```cpp
// Simplified interface (use a trusted implementation):
struct LCT {
    // path-parent pointer tree using splay trees
    // Operations: link(u,v), cut(u,v), findRoot(u), pathSum(u,v), pathMin(u,v)
    // All O(log N) amortized
};

// When to use Link-Cut Tree:
// - Dynamic tree with edge insertions/deletions AND path queries
// - Online minimum spanning forest with deletions
// - Network flow related dynamic problems

// If edges are only added (no deletions), use offline DSU instead.
// If queries are offline, use centroid decomp or virtual tree.
```

---

## 21.9 Tree DP Patterns

### Pattern 1: DP on subtrees with counting

```cpp
// dp[u][j] = number of ways to choose j nodes from subtree of u
// Merge: dp[u] = convolution of dp[u] with dp[v] for each child v
// Naive: O(N^2) but OK for most problems with N<=1000
// With knapsack trick: O(N^2) total (each pair considered once)
vector<long long> dp(n+1, 0);
dp[u] = 1;  // choose u itself
for(int v : adj[u]) if(v != parent) {
    // dp[u] = dp[u] * (1 + dp[v]) or more complex merge
    vector<long long> newDp(dp[u].size() + dp[v].size(), 0);
    for(int j = 0; j < dp[u].size(); j++)
        for(int k = 0; k < dp[v].size(); k++)
            newDp[j+k] += dp[u][j] * dp[v][k];
    dp[u] = newDp;
}
```

### Pattern 2: Max independent set on tree

```cpp
// include[u] = max independent set in subtree of u where u IS included
// exclude[u] = max independent set in subtree of u where u is NOT included
long long inc[MAXN], exc[MAXN];
function<void(int,int)> dfs = [&](int u, int p) {
    inc[u] = val[u];
    exc[u] = 0;
    for(int v : adj[u]) if(v != p) {
        dfs(v, u);
        inc[u] += exc[v];          // if u included, children must be excluded
        exc[u] += max(inc[v], exc[v]);  // if u excluded, children can be either
    }
};
// Answer: max(inc[root], exc[root])
```

### Pattern 3: Longest path in tree (diameter)

```cpp
long long diameter = 0;
long long dfs_diameter(int u, int p) {
    long long best1 = 0, best2 = 0;  // two longest downward paths
    for(int v : adj[u]) if(v != p) {
        long long d = dfs_diameter(v, u) + 1;
        if(d >= best1) { best2 = best1; best1 = d; }
        else if(d > best2) best2 = d;
    }
    diameter = max(diameter, best1 + best2);
    return best1;
}
// Alternatively: BFS from any node, then BFS from farthest -> diameter
```

---

## 21.10 Summary

| Technique | Use Case | Complexity |
|-----------|----------|-----------|
| Euler Tour | Subtree queries with segment tree | O(N log N) per query |
| Rerooting DP | Compute f(root=u) for all u | O(N) |
| Functional Graph | Permutation cycles, k-th successor | O(N log N) build |
| Virtual Tree | Path queries on K key nodes | O(K log N) per query set |
| Small-to-Large | Merge sets at tree nodes | O(N log N) total |
| Centroid Decomp | Path queries, offline or online | O(N log^2 N) |
| Tree Isomorphism | Check structural equality | O(N log N) |
| Link-Cut Tree | Dynamic tree with path queries | O(log N) per op |
| Rerooting + DP | Sum of distances, answer for all roots | O(N) |
| Tree diameter | Longest path | O(N) BFS or O(N) DFS |
