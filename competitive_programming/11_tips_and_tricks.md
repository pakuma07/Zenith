# 11 — Tips, Tricks, and Contest Strategy

The difference between a good coder and a good competitive programmer is **contest discipline**.

---

## 11.1 Reading the Problem (Do This Every Time)

```
✓ Read the problem TWICE before writing any code
✓ Identify: input format, output format, constraints (N, M, T, value ranges)
✓ Check: is output a number? a string? a sequence? yes/no?
✓ Note: how many test cases? (T ≤ ?, per input or single?)
✓ Check time limit and memory limit
✓ Re-read the examples — understand them fully
✓ Look at the limits: what complexity is allowed?
✓ Note any special conditions ("distinct", "connected", "non-negative")
```

---

## 11.2 Contest Strategy

### Time Management

```
First pass (10 min): Read ALL problems, sort by perceived difficulty
Code easiest first — early AC builds momentum
Allocate time budgets per problem (e.g., 30/45/60 min)
If stuck for 20+ min → move to next problem, come back later
Never leave a solved problem un-submitted — partial credit counts
Last 10 min: Check if any penalty-free attempts remain
```

### Attempt Order

```
1. Problems you can solve immediately (A, B in Codeforces Div 2)
2. Problems you see a clear approach but need implementation
3. Problems you need to think about
4. Problems you have no idea about yet
```

---

## 11.3 Before You Submit

```
✓ Sample test cases all pass
✓ Long long used where values can exceed 2 × 10^9
✓ Array/vector sizes are N+5 or larger (off-by-one safety)
✓ Modular arithmetic applied everywhere needed
✓ No endl in loops (causes TLE on slow output)
✓ All debug output removed
✓ Return 0; in main
✓ Edge cases checked (see 11.4)
```

---

## 11.4 Universal Edge Cases Checklist

```
N = 0, N = 1, N = 2 (smallest valid inputs)
N = MAX_N (largest input — check for TLE/MLE)
All elements equal
All elements 0 or negative
Sorted input (ascending and descending)
Answer is 0 or negative
Empty string, single character
Self-loops and multiple edges in graphs
Disconnected graph when problem implies connectivity
Graph with a single node
Overflow: multiply two values near INT_MAX
```

---

## 11.5 Debugging Tips

### Binary Search Your Bug

```
Work backward from wrong output to wrong state
Insert assert() calls at intermediate steps:
  assert(dist[src] == 0);
  assert(answer >= 0 && answer <= N);
  assert(dp[i] >= dp[i-1]);   // if DP should be monotone
```

### Stress Testing

```cpp
// Run your slow brute force vs fast solution on random inputs
#include <cstdlib>   // rand(), srand()
#include <ctime>

int brute(int n) { /* O(N^2) correct solution */ return 0; }
int fast(int n)  { /* O(N log N) solution being tested */ return 0; }

int main() {
    srand(42);
    for (int iter = 0; iter < 10000; ++iter) {
        int n = rand() % 20 + 1;
        // generate random input...
        if (brute(n) != fast(n)) {
            cout << "MISMATCH at n=" << n << "\n";
            return 1;
        }
    }
    cout << "All tests passed\n";
    return 0;
}
```

---

## 11.6 Common Mistakes

### Off-By-One Errors

```cpp
// Wrong: loop one too many or too few
for (int i = 1; i < n; ++i)   // misses i = n
for (int i = 0; i <= n; ++i)  // array out of bounds if size n

// Fix: always double-check loop bounds against array size
```

### Integer Overflow

```cpp
// Wrong
int a = 100000, b = 100000;
int c = a * b;           // 10^10 overflows!

// Right
long long c = (long long)a * b;
```

### Uninitialized dp/dist Arrays

```cpp
// Wrong: dp[0] might be garbage
int dp[1005];

// Right: always initialise
int dp[1005];
fill(dp, dp + 1005, 0);
// or: memset(dp, 0, sizeof(dp));
// or: memset(dp, 0x3f, sizeof(dp));  → fills with ~10^9 (use as INF)
```

### Wrong Modular Subtraction

```cpp
// Wrong: can go negative
long long ans = (a - b) % MOD;

// Right: add MOD before taking %
long long ans = (a - b % MOD + MOD) % MOD;
```

### Using `endl` Instead of `"\n"`

```cpp
// endl flushes the buffer — 100x slower than "\n"
for (int i = 0; i < n; ++i) cout << i << endl;  // TLE risk

// Fix:
for (int i = 0; i < n; ++i) cout << i << "\n";
```

---

## 11.7 Algorithm Selection Guide

```
Sum/Product queries on static array?  → Prefix sums / sparse table
Range update + range query?           → Segment tree with lazy propagation
Connectivity of dynamic edge set?     → DSU
Shortest path, positive weights?      → Dijkstra
Shortest path, negative weights?      → Bellman-Ford
All pairs?                            → Floyd-Warshall (small N)
Frequencies/existence queries?        → Hash map or counting sort
Pattern in string?                    → KMP or Z-function
Binary search on answer?              → When "is X achievable?" is easy to check
Greedy exchange argument?             → Sort + scan
DP state explosion?                   → Try bitmask, reduce dimension
```

---

## 11.8 Binary Search on the Answer

When: the answer is monotonic — if X is achievable, X-1 is too (or vice versa).

```cpp
// Is it possible to achieve score >= mid?
bool check(int mid) { /* problem-specific */ return true; }

int lo = 0, hi = 1e9;
while (lo < hi) {
    int mid = lo + (hi - lo + 1) / 2;  // upper mid — avoids infinite loop
    if (check(mid)) lo = mid;
    else             hi = mid - 1;
}
cout << lo << "\n";   // largest value where check is true

// For "smallest X where check is true":
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;   // lower mid
    if (check(mid)) hi = mid;
    else             lo = mid + 1;
}
cout << lo << "\n";
```

---

## 11.9 Coordinate Compression

When values are large but count is small (N ≤ 10^5, values ≤ 10^9):

```cpp
vector<int> vals = { /* raw values */ };
sort(vals.begin(), vals.end());
vals.erase(unique(vals.begin(), vals.end()), vals.end());

// Compress: map value to index 0..K-1
auto compress = [&](int v) {
    return (int)(lower_bound(vals.begin(), vals.end(), v) - vals.begin());
};
// Now use compressed indices in segment tree / BIT
```

---

## 11.10 Two Pointers and Sliding Window

```cpp
// Two pointers — sorted array, find pair summing to target
int l = 0, r = n - 1;
while (l < r) {
    int s = a[l] + a[r];
    if      (s == target) { /* found */ l++; r--; }
    else if (s <  target) l++;
    else                  r--;
}

// Sliding window — longest subarray with sum ≤ K
int lo = 0, curSum = 0, maxLen = 0;
for (int hi = 0; hi < n; ++hi) {
    curSum += a[hi];
    while (curSum > K) curSum -= a[lo++];
    maxLen = max(maxLen, hi - lo + 1);
}
```

---

## 11.11 Greedy Tips

```
Exchange argument: assume optimal differs from greedy, show swapping two adjacent elements
doesn't improve → greedy is optimal.

Sort by what you're optimising: deadlines, ratios, differences.

Common greedy patterns:
  - Interval scheduling: sort by end time, take earliest-ending non-overlapping
  - Fractional knapsack: sort by value/weight ratio
  - Huffman coding: always merge two smallest
  - Activity selection: sort by start or end time
```

---

## 11.12 Mental Model for Counting Problems

```
Permutations of n distinct: n!
Choose k from n (order doesn't matter): C(n,k) = n! / (k!(n-k)!)
Arrange n with repetitions (e.g. a of A, b of B): (a+b)! / (a! × b!)
Stars and bars (k non-negative integers summing to n): C(n+k-1, k-1)
Inclusion-exclusion: |A ∪ B| = |A| + |B| - |A ∩ B|
Derangements (no fixed point): D(n) = (n-1)(D(n-1) + D(n-2))
```

---

## Summary: The CP Mindset

```
1. Identify the problem type immediately
2. Verify complexity BEFORE coding
3. Write readable code — bugs cost more time than clean code saves
4. Test on samples, then on edge cases
5. If WA: think, don't randomly change code
6. If TLE: first verify algorithm is correct, then optimise
7. If MLE: check array sizes, use rolling arrays
8. Never give up on a problem you have partial insight into
```

---

**Next**: [12 — CP Template](12_cp_template.md)
