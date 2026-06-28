# 13 — Advanced Data Structures

These are the data structures that separate Div 1 D/E solvers from the rest.

---

## 13.1 Sparse Table — O(N log N) build, O(1) RMQ

**Use for**: static range minimum/maximum queries (no updates). The gold standard for RMQ.

```cpp
struct SparseTable {
    int n, LOG;
    vector<vector<int>> table;
    vector<int> log2_;

    void build(const vector<int>& a) {
        n = a.size(); LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        table.assign(LOG, vector<int>(n));
        log2_.resize(n + 1);
        log2_[1] = 0;
        for (int i = 2; i <= n; ++i) log2_[i] = log2_[i/2] + 1;

        table[0] = a;
        for (int j = 1; j < LOG; ++j)
            for (int i = 0; i + (1<<j) <= n; ++i)
                table[j][i] = min(table[j-1][i], table[j-1][i + (1<<(j-1))]);
    }

    // Range minimum on [l, r] (0-indexed), O(1)
    int query(int l, int r) {
        int k = log2_[r - l + 1];
        return min(table[k][l], table[k][r - (1<<k) + 1]);
    }
};

// For RMQ returning the INDEX (needed for LCA):
// Store pairs {value, index} and take min by value
```

---

## 13.2 Segment Tree with Lazy Propagation

**Use for**: range update + range query (sum, min, max).

```cpp
struct LazySegTree {
    int n;
    vector<long long> tree, lazy;

    LazySegTree(int n) : n(n), tree(4*n, 0), lazy(4*n, 0) {}

    void push_down(int node) {
        if (lazy[node]) {
            for (int child : {2*node, 2*node+1}) {
                tree[child] += lazy[node];   // adapt for your operation
                lazy[child] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update(int node, int l, int r, int ql, int qr, long long val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            tree[node] += val * (r - l + 1);  // range sum: add val to all
            lazy[node] += val;
            return;
        }
        push_down(node);
        int mid = (l + r) / 2;
        update(2*node, l, mid, ql, qr, val);
        update(2*node+1, mid+1, r, ql, qr, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    long long query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node];
        push_down(node);
        int mid = (l + r) / 2;
        return query(2*node, l, mid, ql, qr) + query(2*node+1, mid+1, r, ql, qr);
    }

    void update(int l, int r, long long val) { update(1, 1, n, l, r, val); }
    long long query(int l, int r)            { return query(1, 1, n, l, r); }
};

// For range assign (set all in [l,r] to val):
// lazy means "assigned value", -1 means "no pending assign"
// push_down: if lazy[node] != -1, set children's tree and lazy
```

---

## 13.3 LCA — Lowest Common Ancestor (Binary Lifting)

**Use for**: LCA queries, path queries on trees, distance between nodes.  
Build: O(N log N). Query: O(log N).

```cpp
const int MAXN = 100005, LOG = 17;
int parent[MAXN][LOG], depth[MAXN];
vector<int> tree[MAXN];

void dfs(int u, int p, int d) {
    parent[u][0] = p; depth[u] = d;
    for (int v : tree[u]) if (v != p) dfs(v, u, d+1);
}

void buildLCA(int root, int n) {
    dfs(root, root, 0);
    for (int j = 1; j < LOG; ++j)
        for (int i = 1; i <= n; ++i)
            parent[i][j] = parent[parent[i][j-1]][j-1];
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int j = 0; j < LOG; ++j) if ((diff >> j) & 1) u = parent[u][j];
    if (u == v) return u;
    for (int j = LOG-1; j >= 0; --j)
        if (parent[u][j] != parent[v][j]) { u = parent[u][j]; v = parent[v][j]; }
    return parent[u][0];
}

int dist(int u, int v) { return depth[u] + depth[v] - 2*depth[lca(u,v)]; }
```

---

## 13.4 Heavy-Light Decomposition (HLD)

**Use for**: path queries/updates on trees — decompose any root-to-node path into O(log N) chains, then use a segment tree for each chain.

```cpp
const int MAXN = 100005;
int par[MAXN], dep[MAXN], sz[MAXN], heavy[MAXN];
int head[MAXN], pos[MAXN], cur_pos;
vector<int> adj[MAXN];

int dfs_sz(int u, int p, int d) {
    par[u] = p; dep[u] = d; sz[u] = 1; heavy[u] = -1;
    int maxSz = 0;
    for (int v : adj[u]) if (v != p) {
        sz[u] += dfs_sz(v, u, d+1);
        if (sz[v] > maxSz) { maxSz = sz[v]; heavy[u] = v; }
    }
    return sz[u];
}

void dfs_hld(int u, int h) {
    head[u] = h; pos[u] = cur_pos++;
    if (heavy[u] != -1) dfs_hld(heavy[u], h);
    for (int v : adj[u]) if (v != par[u] && v != heavy[u]) dfs_hld(v, v);
}

void buildHLD(int root) {
    cur_pos = 0;
    dfs_sz(root, root, 0);
    dfs_hld(root, root);
}

// Query path [u, v] — O(log^2 N) with segment tree
long long queryPath(int u, int v, SegTree& seg) {
    long long res = 0;
    while (head[u] != head[v]) {
        if (dep[head[u]] < dep[head[v]]) swap(u, v);
        res += seg.query(pos[head[u]], pos[u]);
        u = par[head[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    res += seg.query(pos[u], pos[v]);
    return res;
}
```

---

## 13.5 Centroid Decomposition

**Use for**: problems involving paths of a certain length, distance queries on trees.

```cpp
int sz2[MAXN], centroid_par[MAXN];
bool removed[MAXN];

int getSize(int u, int p) {
    sz2[u] = 1;
    for (int v : adj[u]) if (v != p && !removed[v]) sz2[u] += getSize(v, u);
    return sz2[u];
}

int getCentroid(int u, int p, int treeSize) {
    for (int v : adj[u]) if (v != p && !removed[v])
        if (sz2[v] > treeSize / 2) return getCentroid(v, u, treeSize);
    return u;
}

void decompose(int u, int p) {
    int sz = getSize(u, -1);
    int c = getCentroid(u, -1, sz);
    centroid_par[c] = p;
    removed[c] = true;
    // Process: for every node in current component, dist to centroid is computable
    for (int v : adj[c]) if (!removed[v]) decompose(v, c);
}
```

---

## 13.6 Mo's Algorithm — O((N + Q) √N)

**Use for**: offline range queries where adding/removing elements is O(1) or O(log N).

```cpp
struct Query { int l, r, idx; };

int BLOCK;
bool cmp(const Query& a, const Query& b) {
    int ba = a.l / BLOCK, bb = b.l / BLOCK;
    if (ba != bb) return ba < bb;
    return (ba & 1) ? (a.r > b.r) : (a.r < b.r);  // Hilbert curve order
}

// Current window state
int cnt[MAXN]; int distinct = 0;
void add(int x) { if (++cnt[x] == 1) ++distinct; }
void rem(int x) { if (--cnt[x] == 0) --distinct; }

vector<int> mo(vector<Query>& queries, const vector<int>& a) {
    int n = a.size(), q = queries.size();
    BLOCK = max(1, (int)sqrt(n));
    sort(queries.begin(), queries.end(), cmp);

    vector<int> ans(q);
    int curL = 0, curR = -1;
    for (auto& query : queries) {
        while (curR < query.r) add(a[++curR]);
        while (curL > query.l) add(a[--curL]);
        while (curR > query.r) rem(a[curR--]);
        while (curL < query.l) rem(a[curL++]);
        ans[query.idx] = distinct;  // or whatever aggregate you need
    }
    return ans;
}
```

---

## 13.7 Treap (Implicit Key Treap)

**Use for**: sequences with insert/delete/split/merge in O(log N) — more flexible than segment tree.

```cpp
mt19937 rng(42);

struct Node {
    int val, pri, sz;
    Node *l, *r;
    Node(int v) : val(v), pri(rng()), sz(1), l(nullptr), r(nullptr) {}
};

int sz(Node* t) { return t ? t->sz : 0; }
void upd(Node* t) { if (t) t->sz = 1 + sz(t->l) + sz(t->r); }

// Split into left (size k) and right
pair<Node*,Node*> split(Node* t, int k) {
    if (!t) return {nullptr, nullptr};
    if (sz(t->l) >= k) {
        auto [l, r] = split(t->l, k);
        t->l = r; upd(t);
        return {l, t};
    } else {
        auto [l, r] = split(t->r, k - sz(t->l) - 1);
        t->r = l; upd(t);
        return {t, r};
    }
}

Node* merge(Node* l, Node* r) {
    if (!l) return r; if (!r) return l;
    if (l->pri > r->pri) { l->r = merge(l->r, r); upd(l); return l; }
    else                  { r->l = merge(l, r->l); upd(r); return r; }
}

// Insert at position k (0-indexed)
Node* insert(Node* t, int k, int val) {
    auto [l, r] = split(t, k);
    return merge(merge(l, new Node(val)), r);
}

// Erase position k
Node* erase(Node* t, int k) {
    auto [l, mr] = split(t, k);
    auto [m, r] = split(mr, 1);
    delete m;
    return merge(l, r);
}
```

---

## 13.8 DSU on Tree (Small-to-Large Merging)

**Use for**: subtree queries — count distinct values, most frequent element, etc.

```cpp
// Each node has a set of values. Merge children into parent.
// Total complexity: O(N log N) due to small-to-large invariant.

set<int>* dsu_solve(int u, int p, const vector<int>& val, const vector<vector<int>>& adj) {
    set<int>* cur = new set<int>();
    cur->insert(val[u]);
    for (int v : adj[u]) {
        if (v == p) continue;
        set<int>* child = dsu_solve(v, u, val, adj);
        if (child->size() > cur->size()) swap(cur, child);
        for (int x : *child) cur->insert(x);
        delete child;
    }
    // ans[u] = cur->size() (distinct values in subtree of u)
    return cur;
}
```

---

## 13.9 Sqrt Decomposition — O(√N) per query/update

```cpp
// Block decomposition for range sum
struct SqrtDecomp {
    int n, B;
    vector<long long> a, block;

    SqrtDecomp(const vector<int>& arr) : n(arr.size()), B((int)sqrt(arr.size()) + 1),
        a(arr.begin(), arr.end()), block((arr.size()/((int)sqrt(arr.size())+1)) + 1, 0) {
        for (int i = 0; i < n; ++i) block[i/B] += a[i];
    }

    void update(int i, long long val) {
        block[i/B] += val - a[i];
        a[i] = val;
    }

    long long query(int l, int r) {
        long long res = 0;
        int bl = l/B, br = r/B;
        if (bl == br) { for (int i = l; i <= r; ++i) res += a[i]; return res; }
        for (int i = l; i < (bl+1)*B; ++i) res += a[i];
        for (int b = bl+1; b < br; ++b) res += block[b];
        for (int i = br*B; i <= r; ++i) res += a[i];
        return res;
    }
};
```

---

## 13.10 Summary

| Structure | Build | Query | Update | Best Use Case |
|-----------|-------|-------|--------|--------------|
| Sparse Table | O(N log N) | O(1) | — | Static RMQ |
| Segment Tree | O(N) | O(log N) | O(log N) | Dynamic range queries |
| Lazy Seg Tree | O(N) | O(log N) | O(log N) | Range update + range query |
| Fenwick Tree | O(N) | O(log N) | O(log N) | Prefix sums |
| LCA (binary lifting) | O(N log N) | O(log N) | — | Tree path queries |
| HLD | O(N log N) | O(log² N) | O(log² N) | Path queries on trees |
| Centroid Decomp | O(N log N) | O(log N) | — | Distance-based tree problems |
| Mo's Algorithm | O(N√N) | O(√N) amort. | — | Offline range queries |
| Treap | O(log N) | O(log N) | O(log N) | Flexible sequence ops |
| Sqrt Decomp | O(N) | O(√N) | O(1) | When log is too slow to code |

---

**Next**: [14 — Flows and Matching](14_flows_and_matching.md)
