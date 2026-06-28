# 22 - Policy-Based Data Structures and Miscellaneous Tricks

Tricks used by top competitors that don't fit neatly elsewhere.
Covers __gnu_pbds (order-statistics tree), segment tree beats, persistent
segment tree, randomization tricks, and contest meta-skills.

---

## 22.1 Policy-Based Data Structures (PBDS)

GCC extension: `__gnu_pbds::tree` gives O(log N) order-statistics operations.
Available on Codeforces, AtCoder, and most online judges.

```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

// Order-statistics tree: like set but with O(log N) rank and kth-element
typedef tree<int, null_type, less<int>,
             rb_tree_tag, tree_order_statistics_node_update> ordered_set;

ordered_set os;
os.insert(5);
os.insert(3);
os.insert(7);
os.insert(1);

os.order_of_key(3);   // number of elements strictly less than 3 -> 1
*os.find_by_order(0); // element at 0-indexed position 0 -> 1 (smallest)
*os.find_by_order(2); // element at position 2 -> 5

// Handle duplicates: use pair<int,int> with unique second key
typedef tree<pair<int,int>, null_type, less<pair<int,int>>,
             rb_tree_tag, tree_order_statistics_node_update> ordered_multiset;
int uid = 0;
ordered_multiset oms;
oms.insert({5, uid++});
oms.insert({5, uid++});  // two 5s, different uid
// order_of_key({5, INT_MIN}) = count of elements < 5
// order_of_key({5, INT_MAX}) = count of elements <= 5

// PBDS hash map (faster than unordered_map in practice)
typedef gp_hash_table<int, int> PBDSHashMap;
PBDSHashMap hm;
hm[1] = 42;
```

**Classic use: count inversions online, kth-smallest queries**

```cpp
// Count inversions: for each element a[i], count how many previous elements > a[i]
ordered_set os2;
long long inv = 0;
for(int x : a) {
    inv += os2.size() - os2.order_of_key(x);  // elements >= x already inserted
    os2.insert(x);  // assumes all distinct; use pair trick for duplicates
}
```

---

## 22.2 Persistent Segment Tree

Supports historical queries: "what was the range sum/min at version t?"
Each update creates a new root, sharing unchanged nodes with previous version.

```cpp
struct PersistentSegTree {
    struct Node { int left, right; long long sum; };
    vector<Node> nodes;
    vector<int> roots;  // roots[i] = root of version i

    PersistentSegTree(int maxNodes = 20000005) { nodes.reserve(maxNodes); }

    int newNode() { nodes.push_back({0,0,0}); return nodes.size()-1; }

    int build(int l, int r) {
        int v = newNode();
        if(l == r) return v;
        int m = (l+r)/2;
        nodes[v].left  = build(l, m);
        nodes[v].right = build(m+1, r);
        return v;
    }

    int update(int prev, int l, int r, int i, long long val) {
        int v = newNode();
        nodes[v] = nodes[prev];
        if(l == r) { nodes[v].sum = val; return v; }
        int m = (l+r)/2;
        if(i <= m) nodes[v].left  = update(nodes[prev].left,  l, m, i, val);
        else       nodes[v].right = update(nodes[prev].right, m+1, r, i, val);
        nodes[v].sum = nodes[nodes[v].left].sum + nodes[nodes[v].right].sum;
        return v;
    }

    long long query(int v, int l, int r, int ql, int qr) {
        if(qr < l || r < ql) return 0;
        if(ql <= l && r <= qr) return nodes[v].sum;
        int m = (l+r)/2;
        return query(nodes[v].left, l, m, ql, qr) +
               query(nodes[v].right, m+1, r, ql, qr);
    }
};

// Usage: k-th smallest in range [l, r]
// Build persistent seg tree on sorted values
// At each index i, insert a[i] into the tree (new version)
// kth(roots[r], roots[l-1], 1, n, k): find k-th position where right_cnt - left_cnt >= k
```

**K-th smallest in range [l, r] using persistent segment tree:**

```cpp
// Build: insert elements left to right, each creating a new version
// Query: kth(roots[r], roots[l-1], lo, hi, k) traverses both trees simultaneously
int kthRange(int vr, int vl, int l, int r, int k) {
    if(l == r) return l;  // coordinate-compressed value
    int m = (l+r)/2;
    int leftCnt = nodes[nodes[vr].left].sum - nodes[nodes[vl].left].sum;
    if(leftCnt >= k) return kthRange(nodes[vr].left, nodes[vl].left, l, m, k);
    else             return kthRange(nodes[vr].right, nodes[vl].right, m+1, r, k - leftCnt);
}
```

---

## 22.3 Segment Tree Beats (Ji Driver Segmentation)

Supports "range chmin/chmax update" with O(N log^2 N) amortized.
Useful for: range set min, range set max, range sum queries.

```cpp
// Maintains for each node:
//   max1: first maximum, max2: second maximum, cntMax: count of max
//   min1: first minimum, min2: second minimum, cntMin: count of min
//   sum: range sum
struct SegBeats {
    struct Node {
        long long sum;
        int max1, max2, cntMax;
        int min1, min2, cntMin;
        int lazy;  // pending add
    };
    int n; vector<Node> t;
    SegBeats(int n, vector<int> &a): n(n), t(4*n) { build(a, 1, 1, n); }

    void push_up(int v) {
        auto &L = t[2*v], &R = t[2*v+1], &V = t[v];
        V.sum = L.sum + R.sum;
        // max
        if(L.max1 == R.max1) { V.max1 = L.max1; V.cntMax = L.cntMax + R.cntMax; V.max2 = max(L.max2, R.max2); }
        else if(L.max1 > R.max1) { V.max1 = L.max1; V.cntMax = L.cntMax; V.max2 = max(L.max2, R.max1); }
        else { V.max1 = R.max1; V.cntMax = R.cntMax; V.max2 = max(L.max1, R.max2); }
        // min (symmetric)
        if(L.min1 == R.min1) { V.min1 = L.min1; V.cntMin = L.cntMin + R.cntMin; V.min2 = min(L.min2, R.min2); }
        else if(L.min1 < R.min1) { V.min1 = L.min1; V.cntMin = L.cntMin; V.min2 = min(L.min2, R.min1); }
        else { V.min1 = R.min1; V.cntMin = R.cntMin; V.min2 = min(L.min1, R.min2); }
    }
    // ... (full implementation is ~150 lines; use trusted library for contest)
    // Key insight: break condition when max1 <= new_cap (chmin) -> only update max values, not recurse
};
```

---

## 22.4 Randomization Tricks

### Randomized quicksort to avoid worst case

```cpp
// Always shuffle before sorting to prevent adversarial O(N^2) inputs
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
shuffle(a.begin(), a.end(), rng);
sort(a.begin(), a.end());
```

### Random hashing for equality checking

```cpp
// Assign each element a random 64-bit value.
// Set of elements S has hash = XOR of hash[x] for x in S.
// Two sets are equal iff their XOR hashes match (with high probability).
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());
map<int, unsigned long long> elemHash;
auto getHash = [&](int x) -> unsigned long long {
    if(!elemHash.count(x)) elemHash[x] = rng64();
    return elemHash[x];
};
// Hash of set {a,b,c} = getHash(a) ^ getHash(b) ^ getHash(c)
// Very useful for: "do two multisets become equal after operations?"
```

### Freivalds' algorithm (matrix multiplication verification)

```cpp
// Verify A*B == C in O(N^2) instead of O(N^3)
// Random vector r; check A*(B*r) == C*r
bool verify(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int n) {
    vector<int> r(n);
    for(int &x : r) x = rng() & 1;
    // Compute Br
    vector<long long> Br(n, 0);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            Br[i] += B[i][j] * r[j];
    // Compute A(Br)
    vector<long long> ABr(n, 0);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            ABr[i] += A[i][j] * Br[j];
    // Compute Cr
    vector<long long> Cr(n, 0);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            Cr[i] += C[i][j] * r[j];
    return ABr == Cr;
}
```

---

## 22.5 __int128 Printing and Arithmetic

```cpp
// __int128 cannot be printed with cout directly
typedef __int128 lll;

void print128(lll x) {
    if(x < 0) { putchar('-'); x = -x; }
    if(x > 9) print128(x / 10);
    putchar('0' + x % 10);
}

// Read __int128
lll read128() {
    lll x = 0; int sign = 1; char c = getchar();
    while(c == ' ' || c == '\n') c = getchar();
    if(c == '-') { sign = -1; c = getchar(); }
    while(c >= '0' && c <= '9') { x = x*10 + c-'0'; c = getchar(); }
    return x * sign;
}

// Safe multiplication without overflow (use __int128 intermediary)
long long mulmod(long long a, long long b, long long m) {
    return (__int128)a * b % m;
}
```

---

## 22.6 Rope / Implicit Treap (for sequence operations)

Supports O(log N) split, merge, reverse, rotate on sequences.
Alternative to `rope` (SGI extension).

```cpp
// Implicit treap: array-like structure with O(log N) split/merge
// Node index = implicit key (position in sequence)
mt19937 treap_rng(42);

struct TreapNode {
    int val, priority, size;
    bool rev;  // lazy reverse flag
    int left, right;
} T[500005];
int treap_cnt = 0;

int newNode(int val) {
    T[++treap_cnt] = {val, (int)treap_rng(), 1, false, 0, 0};
    return treap_cnt;
}

void push(int v) {
    if(!v || !T[v].rev) return;
    swap(T[v].left, T[v].right);
    if(T[v].left)  T[T[v].left].rev  ^= 1;
    if(T[v].right) T[T[v].right].rev ^= 1;
    T[v].rev = false;
}

int sz(int v) { return v ? T[v].size : 0; }
void upd(int v) { if(v) T[v].size = 1 + sz(T[v].left) + sz(T[v].right); }

// Split into [0, pos-1] and [pos, n-1]
pair<int,int> split(int v, int pos) {
    if(!v) return {0, 0};
    push(v);
    int leftSz = sz(T[v].left);
    if(leftSz >= pos) {
        auto [l, r] = split(T[v].left, pos);
        T[v].left = r; upd(v);
        return {l, v};
    } else {
        auto [l, r] = split(T[v].right, pos - leftSz - 1);
        T[v].right = l; upd(v);
        return {v, r};
    }
}

int merge_treap(int l, int r) {
    if(!l || !r) return l | r;
    push(l); push(r);
    if(T[l].priority > T[r].priority) { T[l].right = merge_treap(T[l].right, r); upd(l); return l; }
    else { T[r].left = merge_treap(l, T[r].left); upd(r); return r; }
}

// Reverse segment [l, r] (0-indexed)
void reverse_seg(int &root, int l, int r) {
    auto [left, mid_right] = split(root, l);
    auto [mid, right] = split(mid_right, r - l + 1);
    T[mid].rev ^= 1;
    root = merge_treap(merge_treap(left, mid), right);
}
```

---

## 22.7 Tricks from Top Competitors

### Tourist's tricks
```cpp
// Always use "int" for n (vector sizes, loop limits), cast to ll for products
// Keep code short: define lambdas for recurring local logic
// Use `auto` for iterator types, structured bindings where available (C++17)
// Global arrays are zero-initialized; prefer them over vectors for large allocations
```

### Errichto's tricks
```cpp
// Stress testing: generate random tests, compare brute force vs fast solution
// Time limit: if N<=2000 and TL=2s, O(N^2) is fine; N<=300 for O(N^3)
// "Think about the answer, not the algorithm": what does the optimal solution look like?
// Use long long from the start if any multiplication is involved
```

### neal's tricks (Codeforces)
```cpp
// Use __builtin_ctz, __builtin_clz for fast bit operations
// For segment trees: use iterative (bottom-up) instead of recursive for ~2x speedup
// Compress coordinates before anything involving a range
// If stuck: check if problem is a known reduction (max flow, matching, SCC)
```

### Um_nik / jiangly tricks
```cpp
// For hashing: always use two independent hashes to reduce collision probability
// polynomial hash with mod1 and mod2 simultaneously
const long long MOD1 = 1e9+7, MOD2 = 1e9+9;
const long long BASE1 = 131, BASE2 = 137;
// Hash = (h1, h2): collision probability ~ 1/(MOD1 * MOD2) ~ 10^{-18}

// For graph problems: always draw small examples and find the pattern
// Suspected greedy: prove by exchange argument or find a counterexample
```

---

## 22.8 Iterative Segment Tree (Bottom-Up, 2x Faster)

```cpp
// Iterative segment tree: faster constant, simpler code
// 1-indexed: leaves at [n, 2n-1]
struct SegTreeFast {
    int n; vector<long long> t;
    SegTreeFast(int n): n(n), t(2*n, 0) {}
    void update(int i, long long val) {  // 0-indexed
        for(t[i+=n]=val, i>>=1; i>=1; i>>=1) t[i]=t[2*i]+t[2*i+1];
    }
    long long query(int l, int r) {  // 0-indexed, [l,r]
        long long res=0;
        for(l+=n,r+=n+1; l<r; l>>=1,r>>=1) {
            if(l&1) res+=t[l++];
            if(r&1) res+=t[--r];
        }
        return res;
    }
};
```

---

## 22.9 Sqrt Trick: Divide Array into Blocks

**Block decomposition as an alternative when specialized DS is hard to implement:**

```cpp
// Range sum with point updates in O(sqrt N)
int B = sqrt(n) + 1;  // block size
vector<long long> block(n/B + 1, 0);
vector<int> a(n);

auto update = [&](int i, int val) {
    block[i/B] += val - a[i];
    a[i] = val;
};
auto query = [&](int l, int r) -> long long {
    long long res = 0;
    int bl = l/B, br = r/B;
    if(bl == br) { for(int i=l;i<=r;i++) res+=a[i]; return res; }
    for(int i=l; i<(bl+1)*B; i++) res+=a[i];
    for(int b=bl+1; b<br; b++) res+=block[b];
    for(int i=br*B; i<=r; i++) res+=a[i];
    return res;
};
```

---

## 22.10 Helpful GCC-Specific Built-ins

```cpp
__builtin_popcount(x)    // count set bits in int
__builtin_popcountll(x)  // count set bits in long long
__builtin_ctz(x)         // count trailing zeros (undefined for x=0)
__builtin_clz(x)         // count leading zeros (undefined for x=0)
__builtin_parity(x)      // parity of set bits (XOR of all bits)
__lg(x)                  // floor(log2(x)) for x > 0

// Fast lowest set bit
int lsb = x & (-x);      // or x & -x

// Clear lowest set bit
x = x & (x-1);           // iterate over all subsets: while(x) { use x; x&=x-1; }

// Iterate over all subsets of mask (excluding empty):
for(int sub = mask; sub > 0; sub = (sub-1) & mask) { /* use sub */ }
```

---

## 22.11 Summary: When to Use

| Tool | Use Case |
|------|----------|
| `ordered_set` (PBDS) | Online kth-smallest, inversion count, rank queries |
| Persistent seg tree | Historical queries, offline kth-smallest in range |
| Segment tree beats | Range chmin/chmax with range sum queries |
| Random XOR hash | Fast set equality / multiset comparison |
| Freivalds | Verify matrix product in O(N^2) |
| Implicit treap | Sequence with split/merge/reverse in O(log N) |
| Iterative seg tree | Drop-in replacement for recursive, ~2x faster |
| Sqrt decomp | When specialized DS is too hard to code under time pressure |
| PBDS hash map | Faster than `unordered_map` with same API |
