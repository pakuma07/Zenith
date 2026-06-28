# 02 — Time and Space Complexity

Before writing a single line of code, verify your algorithm fits within the time and memory limits.

---

## 2.1 The Golden Rule

> **10^8 simple operations ≈ 1 second** on a typical online judge.

"Simple operation" = arithmetic, comparison, array access.  
Loops with heavy inner work (cache misses, function calls) may be 5–10× slower.

---

## 2.2 Constraint → Complexity Table

| N (constraint) | Max complexity | Notes |
|----------------|---------------|-------|
| N ≤ 10 | O(N! ) | Brute-force all permutations |
| N ≤ 15–20 | O(2^N · N) | Bitmask DP, subset enumeration |
| N ≤ 20 | O(2^N) | Meet-in-the-middle |
| N ≤ 100 | O(N³) | Floyd-Warshall, cubic DP |
| N ≤ 500 | O(N² log N) | — |
| N ≤ 1,000 | O(N²) | Bubble sort, quadratic DP |
| N ≤ 10,000 | O(N² / 64) | Bitset DP |
| N ≤ 100,000 | O(N √N) | Mo's algorithm, sqrt decomposition |
| N ≤ 200,000 | O(N log N) | Sort, Dijkstra, BIT, segment tree |
| N ≤ 1,000,000 | O(N) or O(N log N) | Sieve, two pointers, BFS/DFS |
| N ≤ 10^18 | O(log N) or O(√N) | Binary search, prime factorisation |

---

## 2.3 Big-O Cheat Sheet

```
O(1)       Constant   — array access, hash lookup (average)
O(log N)   Logarithmic— binary search, balanced BST operations
O(√N)      Square root— prime testing, sqrt decomposition
O(N)       Linear     — single pass, BFS/DFS
O(N log N) — sort, Dijkstra (with priority queue), segment tree build
O(N √N)    — Mo's algorithm
O(N²)      Quadratic  — naive O(N²) DP, bubble sort
O(N²logN)  — —
O(N³)      Cubic      — Floyd-Warshall, naive matrix multiply
O(2^N)     Exponential— bitmask over N elements
O(N!)      Factorial  — all permutations
```

---

## 2.4 Estimating Your Solution

```cpp
// Example: N = 200,000 elements, time limit = 2 seconds
// Your algorithm: O(N^2)
// Operations: 200000^2 = 4 × 10^10  →  ~400 seconds  ✗ TLE

// Alternative: O(N log N)
// Operations: 200000 × 17 ≈ 3.4 × 10^6  →  ~0.03 seconds  ✓ AC
```

**Mental math shortcut**:
```
log2(10^5)  ≈ 17
log2(10^6)  ≈ 20
log2(10^9)  ≈ 30
log2(10^18) ≈ 60
```

---

## 2.5 Memory Limits

Typical judge limit: **256 MB**

| Type | Size | Max count in 256 MB |
|------|------|---------------------|
| `int` (4 B) | 4 bytes | ~64 million |
| `long long` (8 B) | 8 bytes | ~32 million |
| `double` (8 B) | 8 bytes | ~32 million |
| `int[N][N]` | 4 × N² bytes | N ≤ ~8,000 |
| `long long[N][N]` | 8 × N² bytes | N ≤ ~5,600 |
| `bool` array | 1 byte each | ~256 million |
| `bitset<N>` | N/8 bytes | very efficient |

```cpp
// Danger: int dp[5000][5000]
// 5000 × 5000 × 4 bytes = 100 MB  — borderline

// Safe: int dp[1000][1000]
// 1000 × 1000 × 4 bytes = 4 MB  — fine
```

---

## 2.6 Overflow: Know Your Limits

```cpp
// int: up to ~2.1 × 10^9  (2^31 - 1 = 2,147,483,647)
// long long: up to ~9.2 × 10^18  (2^63 - 1)

// Common overflow situations:
int a = 100000, b = 100000;
int bad  = a * b;           // 10^10 overflows int!
ll  good = (ll)a * b;       // cast before multiply

// Always use long long when:
// - N * N > 2 × 10^9
// - Accumulating sums of large arrays
// - Path lengths or costs in graphs with large weights
// - Factorials, combinations without modular arithmetic

// __int128 for very large values (GCC only)
__int128 huge = (__int128)1e18 * 1e18;
```

---

## 2.7 Tight Time Limit Strategies

When your correct O(N log N) solution barely passes:

```cpp
// 1. Avoid endl — use "\n"
cout << ans << "\n";   // not endl

// 2. Reserve vector capacity upfront
vector<int> v;
v.reserve(n);

// 3. Use arrays instead of vectors for inner loops
int a[200005];   // faster cache access than vector

// 4. Avoid map — use unordered_map or sorted array + binary search
unordered_map<int,int> ump;   // O(1) avg vs O(log N)

// 5. Avoid recursion overhead — convert DFS to iterative
// 6. Use bitwise operations instead of division/modulo by 2
x >> 1;     // x / 2
x & 1;      // x % 2
x << 1;     // x * 2

// 7. GCC pragma (last resort)
#pragma GCC optimize("O2,unroll-loops")
```

---

## 2.8 Space vs Time Trade-offs

```
Memoisation (top-down DP)  — trades memory for time (avoid recomputation)
Prefix sums                — O(N) preprocess, O(1) range query
Hashing                    — O(N) build, O(1) lookup (vs O(log N) set)
Bitset                     — 64× compression of boolean arrays
```

---

## 2.9 Recurrence Relations — Complexity Analysis

```
T(N) = 2T(N/2) + O(N)    →  O(N log N)   [merge sort]
T(N) = T(N-1) + O(1)     →  O(N)         [linear recursion]
T(N) = 2T(N/2) + O(1)    →  O(N)         [binary search variant]
T(N) = T(N-1) + O(N)     →  O(N²)        [selection sort recursion]
T(N) = T(√N) + O(1)      →  O(log log N) [rare]
```

---

## Summary

```
Step 1: Read constraints → pick the allowed complexity
Step 2: Think of an algorithm with that complexity
Step 3: Estimate operations: algorithm_complexity(N) < 10^8 ?
Step 4: Check memory: total_memory < 256 MB ?
Step 5: Check overflow: any intermediate value > 2×10^9 → use long long
```

---

**Next**: [03 — STL Tricks](03_stl_tricks.md)
