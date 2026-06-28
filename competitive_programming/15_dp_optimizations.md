# 15 — DP Optimizations

These techniques reduce O(N²) or O(N³) DP to O(N log N) or O(N). They appear in IOI and Codeforces Div 1 E.

---

## 15.1 Convex Hull Trick (CHT) — O(N log N) or O(N)

**When to use**: DP of the form  
`dp[i] = min over j<i of { dp[j] + b[j]*x[i] + a[i] }`  
where the lines have the form `y = b[j]*x + dp[j]`.

### Li Chao Tree (handles arbitrary x query order) — O(N log RANGE)

```cpp
struct Line { long long m, b; long long eval(long long x) const { return m*x + b; } };

struct LiChaoTree {
    static const long long NEG_INF = -4e18;
    static const int LO = -1e9, HI = 1e9;
    struct Node { Line line; int l, r; };
    vector<Node> nodes;

    LiChaoTree() { nodes.push_back({{0, 4e18}, -1, -1}); }  // init with "infinity" line

    void addLine(int node, int lo, int hi, Line line) {
        int mid = lo + (hi - lo) / 2;
        bool leftBetter  = line.eval(lo) < nodes[node].line.eval(lo);
        bool midBetter   = line.eval(mid) < nodes[node].line.eval(mid);
        if (midBetter) swap(nodes[node].line, line);
        if (lo == hi) return;
        if (leftBetter != midBetter) {
            if (nodes[node].l == -1) { nodes[node].l = nodes.size(); nodes.push_back({{0,4e18},-1,-1}); }
            addLine(nodes[node].l, lo, mid, line);
        } else {
            if (nodes[node].r == -1) { nodes[node].r = nodes.size(); nodes.push_back({{0,4e18},-1,-1}); }
            addLine(nodes[node].r, mid+1, hi, line);
        }
    }

    long long query(int node, int lo, int hi, long long x) {
        long long res = nodes[node].line.eval(x);
        if (lo == hi) return res;
        int mid = lo + (hi - lo) / 2;
        if (x <= mid && nodes[node].l != -1) return min(res, query(nodes[node].l, lo, mid, x));
        if (x > mid && nodes[node].r != -1) return min(res, query(nodes[node].r, mid+1, hi, x));
        return res;
    }

    void add(long long m, long long b)    { addLine(0, LO, HI, {m, b}); }
    long long query(long long x)          { return query(0, LO, HI, x); }
};
```

### Monotone CHT (when queries and slopes are both sorted) — O(N)

```cpp
struct MonotoneStack {
    struct Line { long long m, b; };
    deque<Line> hull;

    bool bad(Line l1, Line l2, Line l3) {
        // l2 is useless if intersection(l1,l3) is below l2 at that point
        return (__int128)(l3.b-l1.b)*(l1.m-l2.m) <= (__int128)(l2.b-l1.b)*(l1.m-l3.m);
    }

    void addLine(long long m, long long b) {  // slopes added in DECREASING order
        Line l = {m, b};
        while (hull.size() >= 2 && bad(hull[hull.size()-2], hull[hull.size()-1], l))
            hull.pop_back();
        hull.push_back(l);
    }

    long long query(long long x) {  // x queries in INCREASING order
        while (hull.size() >= 2 && hull[0].m*x+hull[0].b >= hull[1].m*x+hull[1].b)
            hull.pop_front();
        return hull[0].m*x + hull[0].b;
    }
};
```

---

## 15.2 Divide and Conquer DP Optimization — O(N log N)

**When to use**: `dp[i][j] = min over k<j { dp[i-1][k] + C(k,j) }` where the optimal `k` is monotone: `opt(j) <= opt(j+1)`.

```cpp
// Compute dp[i][lo..hi] using opt in [optLo..optHi]
void solve(int i, int lo, int hi, int optLo, int optHi,
           vector<vector<long long>>& dp, auto cost) {
    if (lo > hi) return;
    int mid = (lo + hi) / 2;
    int bestK = optLo;
    long long bestVal = LLONG_MAX;
    for (int k = optLo; k <= min(mid-1, optHi); ++k) {
        long long val = dp[i-1][k] + cost(k, mid);
        if (val < bestVal) { bestVal = val; bestK = k; }
    }
    dp[i][mid] = bestVal;
    solve(i, lo, mid-1, optLo, bestK, dp, cost);
    solve(i, mid+1, hi, bestK, optHi, dp, cost);
}
```

---

## 15.3 Knuth's Optimization — O(N²) → O(N²) with smaller constant → sometimes O(N²) DP becomes O(N²)

**When to use**: interval DP `dp[i][j] = min over i<=k<j { dp[i][k] + dp[k+1][j] + C(i,j) }` where C satisfies the **quadrangle inequality**: `C(a,c) + C(b,d) <= C(a,d) + C(b,c)` for a<=b<=c<=d.

```cpp
// opt[i][j] = optimal split point for interval [i,j]
// Constraint: opt[i][j-1] <= opt[i][j] <= opt[i+1][j]

vector<vector<long long>> dp(n+1, vector<long long>(n+1, LLONG_MAX/2));
vector<vector<int>> opt(n+1, vector<int>(n+1, 0));

for (int i = 1; i <= n; ++i) { dp[i][i] = 0; opt[i][i] = i; }

for (int len = 2; len <= n; ++len) {
    for (int i = 1; i + len - 1 <= n; ++i) {
        int j = i + len - 1;
        dp[i][j] = LLONG_MAX/2;
        for (int k = opt[i][j-1]; k <= min(j-1, opt[i+1][j]); ++k) {
            long long val = dp[i][k] + dp[k+1][j] + cost(i, j);
            if (val < dp[i][j]) { dp[i][j] = val; opt[i][j] = k; }
        }
    }
}
```

---

## 15.4 Aliens Trick (WQS Binary Search / Lambda Optimization)

**When to use**: "find optimal value using exactly K items" where the cost-as-a-function-of-K is convex.

```cpp
// Instead of dp[i][k] (too many states), solve dp_lambda[i] where
// each item costs an extra penalty lambda. Binary search on lambda
// to find the solution with exactly K items.

long long solve(double lambda, int n, /* problem data */ int& usedCount) {
    // Standard DP but add lambda to cost of each item used
    // Track count of items used in optimal solution
    usedCount = 0;
    // return dp[n];
    return 0;
}

long long aliensDP(int K, int n) {
    double lo = -1e9, hi = 1e9;
    for (int iter = 0; iter < 200; ++iter) {  // binary search on lambda
        double mid = (lo + hi) / 2;
        int cnt; solve(mid, n, cnt);
        if (cnt >= K) lo = mid;
        else          hi = mid;
    }
    int cnt;
    long long ans = solve((lo+hi)/2, n, cnt);
    return ans - (long long)((lo+hi)/2) * K;  // adjust for the lambda penalty
}
```

---

## 15.5 Sum over Subsets (SOS) DP — O(N × 2^N)

```cpp
// dp[mask] = sum of a[sub] for all sub that are subsets of mask
// Build:
for (int i = 0; i < N; ++i)
    for (int mask = 0; mask < (1 << N); ++mask)
        if (mask & (1 << i))
            dp[mask] += dp[mask ^ (1 << i)];

// Sum over supersets (complement direction):
for (int i = 0; i < N; ++i)
    for (int mask = (1<<N)-1; mask >= 0; --mask)
        if (!(mask & (1 << i)))
            dp[mask] += dp[mask | (1 << i)];
```

---

## 15.6 Profile DP (Broken Profile DP)

**Use for**: tiling problems, counting perfect matchings, grid DP with a bitmask of one row as the state.

```cpp
// State: dp[mask] = ways to tile first (col) columns with mask describing
// which cells of column col+1 are already filled by pieces from col.
// Transition: try all ways to fill column col starting from mask state.
// O(N × 2^M) where M = number of rows
```

---

## 15.7 DP Optimization Selection Guide

```
Is the recurrence dp[i] = min(dp[j] + b[j]*x[i])?
  Slopes are monotone AND queries are monotone  → Monotone CHT, O(N)
  Otherwise                                     → Li Chao Tree, O(N log N)

Is it interval DP with monotone opt?
  Prove quadrangle inequality holds             → Knuth's O(N²)
  Prove opt[i][j-1] <= opt[i+1][j]             → D&C optimization O(N log N)

Is it "minimize cost using exactly K items"?
  Cost function is convex in K                  → Aliens trick O(N log RANGE)

Is it "sum over all subsets"?
  Bitmask of ≤ 20 elements                      → SOS DP O(N × 2^N)

Is the DP state an entire row/column of a grid?
  Tiling, matching on grid                      → Profile DP
```

---

## 15.8 Verifying CHT Applicability

```
dp[i] = min over j<i of { f(j) + g(i, j) }

Rewrite g(i,j) = slope(j) * query(i) + intercept(j)

If slope(j) is monotone              → Monotone CHT
If query(i) is also monotone         → Pure deque, O(N)
If only slope monotone, mixed query  → Sort queries + monotone deque
If neither monotone                  → Li Chao tree, O(N log N)
```

---

**Next**: [16 — Advanced Strings](16_advanced_strings.md)
