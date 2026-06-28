# 12 - CP Template Reference

The file `12_cp_template.cpp` is a world-class, ready-to-use contest template.
Copy it as your starting file for every problem.

---

## What Is Included

| Section | What it provides |
|---------|-----------------|
| Pragmas | `O2`, `unroll-loops`, `avx2,bmi,bmi2,popcnt` GCC speed flags |
| Includes | `bits/stdc++.h` (GCC) or individual headers (portable) |
| Types | `ll`, `ull`, `lll` (`__int128`), `pii`, `pll`, `vi`, `vl`, `vpii`, `vvi` |
| Constants | `INF=0x3f3f3f3f`, `LINF`, `MOD=1e9+7`, `MOD2=998244353`, `PI`, `EPS` |
| Macros | `FOR`, `REP`, `RFOR`, `all`, `rall`, `sz`, `pb`, `eb`, `mp`, `fi`, `se`, `YES`, `NO` |
| Random | `mt19937 rng` + `mt19937_64 rng64` (seeded with system clock) |
| Safe Hash | `SafeHash` (splitmix64) + `HashMap<K,V>` + `HashSet<K>` (anti-hack) |
| Math | `gcd`, `lcm`, `power`, `modInv`, `extgcd`, `C(n,k)` with precomputed factorials |
| DSU | Path compression + union by size, `comps` counter |
| Sparse Table | O(1) RMQ, 0-indexed, static array |
| Segment Tree | Point update, range sum, 1-indexed |
| Lazy Segment Tree | Range add update, range sum, lazy propagation, 1-indexed |
| Fenwick / BIT | Point update, prefix sum, range sum, 1-indexed |
| LCA | Binary lifting, O(N log N) build / O(log N) query, `treeDist()` |
| Dijkstra | Min-heap, 1-indexed, returns `dist[]` vector |
| BFS | Unweighted shortest path |
| 0-1 BFS | Deque-based BFS for 0/1 edge weights |
| Dinic Max Flow | Full struct: `addEdge`, `maxflow(s,t)` |
| NTT | Polynomial multiplication mod 998244353, `NTT::multiply(a,b)` |
| KMP | `kmpFail(pattern)` failure function |
| Z-function | `zFunc(s)` |
| Geometry | `struct P`, `cross(O,A,B)`, `convexHull(pts)` |
| Coordinate Compress | `compress(a)` - returns 0-indexed ranks |
| Miller-Rabin | `isPrime(n)` deterministic for n < 3.3e24 |
| Debug Macros | `dbg(x)`, `dbgv(v)`, `dbg2(x,y)` - active only with `-DLOCAL` |
| solve() | Your solution entry point |
| main() | Fast I/O + T test case loop template |

---

## How to Use

```bash
# Copy template to solution file
cp 12_cp_template.cpp solution.cpp

# Write your solution in solve()
# Remove unused data structures to keep the file short

# Compile for submission
g++ -std=c++11 -O2 -Wall solution.cpp -o solution

# Compile for local debugging
g++ -std=c++11 -O2 -DLOCAL solution.cpp -o solution && ./solution < input.txt
```

---

## Key Design Choices

### __int128 (lll)
Use for products that overflow `long long`. Cannot be printed with `cout` directly.
```cpp
lll x = (lll)a * b;   // safe product
// to print: cout << (ll)(x % MOD);
```

### Safe Hash (anti-hack)
Codeforces hacks can break `unordered_map` with crafted inputs. Use the provided
`HashMap` and `HashSet` which use a splitmix64-based custom hash.
```cpp
HashMap<int,int> freq;  // safe replacement for unordered_map<int,int>
HashSet<ll> seen;       // safe replacement for unordered_set<ll>
```

### MOD vs MOD2
- `MOD = 1e9+7` : classic prime, use for most modular arithmetic
- `MOD2 = 998244353` : NTT-friendly prime (primitive root = 3), use for polynomial multiplication

### Debug Macros
```cpp
// Compile with -DLOCAL to enable:
dbg(x);       // prints: x = <value>
dbgv(v);      // prints: v = v[0] v[1] ...
dbg2(a,b);    // prints: a=<val> b=<val>
// All are no-ops in submission builds
```

### Segment Tree vs Lazy Segment Tree
- `SegTree`: point update (set/add single index), range query
- `LazySegTree`: range update (add to interval), range query
Both are 1-indexed.

### LCA Usage
```cpp
vvi adj(N+1);
buildLCA(1, adj);            // call once on root
int anc  = lca(u, v);        // LCA of u and v
int dist = treeDist(u, v);   // edge distance
```

### Dinic Max Flow Usage
```cpp
Dinic G(N);                  // N nodes (0-indexed)
G.addEdge(u, v, capacity);   // directed edge; reverse auto-added with cap 0
ll flow = G.maxflow(source, sink);
```

### NTT Usage
```cpp
vl a = {1, 2, 3};
vl b = {4, 5, 6};
vl c = NTT::multiply(a, b);  // polynomial product mod 998244353
```

---

## Compile Commands

| Purpose | Command |
|---------|---------|
| Standard submission | `g++ -std=c++11 -O2 solution.cpp -o sol` |
| With debug output | `g++ -std=c++11 -O2 -DLOCAL solution.cpp -o sol` |
| Check for UB | `g++ -std=c++11 -fsanitize=address,undefined -g solution.cpp -o sol` |
| Extra speed | `g++ -std=c++11 -O2 -march=native solution.cpp -o sol` |

---

## Template vs File Index

The template is self-contained. The other files in this folder expand on each topic:

- Math details -> [04 - Number Theory](04_number_theory.md)
- Graph details -> [05 - Graph Algorithms](05_graph_algorithms.md)
- DP details -> [06 - Dynamic Programming](06_dynamic_programming.md)
- Advanced DS -> [13 - Advanced Data Structures](13_advanced_data_structures.md)
- Flow/Matching -> [14 - Flows and Matching](14_flows_and_matching.md)
- DP Optimizations -> [15 - DP Optimizations](15_dp_optimizations.md)
- Advanced Strings -> [16 - Advanced Strings](16_advanced_strings.md)
- Advanced NT -> [17 - Advanced Number Theory](17_advanced_number_theory.md)