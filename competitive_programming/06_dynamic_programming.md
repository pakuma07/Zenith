# 06 — Dynamic Programming

DP = breaking a problem into overlapping subproblems and storing results to avoid recomputation.

---

## 6.1 DP Framework

```
1. Define the state: dp[i], dp[i][j], dp[mask][i], ...
2. Write the recurrence (transition)
3. Identify base cases
4. Choose direction: top-down (memoisation) or bottom-up (tabulation)
5. Identify the answer
```

---

## 6.2 Classic DPs

### Longest Common Subsequence (LCS) — O(NM)

```cpp
int lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = (a[i-1] == b[j-1]) ? dp[i-1][j-1] + 1
                                            : max(dp[i-1][j], dp[i][j-1]);
    return dp[n][m];
}
```

### Longest Increasing Subsequence (LIS) — O(N log N)

```cpp
int lis(const vector<int>& a) {
    vector<int> tails;  // tails[i] = smallest tail of IS of length i+1
    for (int x : a) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return (int)tails.size();
}
// For non-strict (non-decreasing): use upper_bound instead of lower_bound
```

### 0/1 Knapsack — O(N × W)

```cpp
int knapsack(int W, const vector<int>& weight, const vector<int>& val) {
    int n = weight.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; ++i)
        for (int w = W; w >= weight[i]; --w)  // reverse to avoid using item twice
            dp[w] = max(dp[w], dp[w - weight[i]] + val[i]);
    return dp[W];
}
```

### Unbounded Knapsack (each item unlimited times)

```cpp
int unboundedKnapsack(int W, const vector<int>& weight, const vector<int>& val) {
    int n = weight.size();
    vector<int> dp(W + 1, 0);
    for (int w = 1; w <= W; ++w)
        for (int i = 0; i < n; ++i)
            if (w >= weight[i])
                dp[w] = max(dp[w], dp[w - weight[i]] + val[i]);
    return dp[W];
}
```

### Coin Change — O(N × amount)

```cpp
int coinChange(const vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i)
        for (int c : coins)
            if (c <= i && dp[i - c] != INT_MAX)
                dp[i] = min(dp[i], dp[i - c] + 1);
    return dp[amount] == INT_MAX ? -1 : dp[amount];
}
```

---

## 6.3 Interval DP

Use for: merging intervals (matrix chain multiplication, balloon burst, stone merging).

```cpp
// Template: dp[l][r] = optimal cost for range [l, r]
int intervalDP(const vector<int>& a) {
    int n = a.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    // len = length of interval
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            dp[l][r] = INT_MAX;
            for (int k = l; k < r; ++k)
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k+1][r] + /*merge cost*/0);
        }
    }
    return dp[0][n-1];
}
```

---

## 6.4 Bitmask DP

Use for: small N (≤ 20), "visit all subsets", assignment problems, TSP.

```cpp
// Traveling Salesman Problem (TSP) — O(2^N × N²)
int tsp(const vector<vector<int>>& dist, int N) {
    vector<vector<int>> dp(1 << N, vector<int>(N, INT_MAX));
    dp[1][0] = 0;   // start at node 0, visited = {0}

    for (int mask = 1; mask < (1 << N); ++mask) {
        for (int u = 0; u < N; ++u) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == INT_MAX) continue;
            for (int v = 0; v < N; ++v) {
                if (mask & (1 << v)) continue;
                int newMask = mask | (1 << v);
                dp[newMask][v] = min(dp[newMask][v],
                                     dp[mask][u] + dist[u][v]);
            }
        }
    }
    int ans = INT_MAX;
    int fullMask = (1 << N) - 1;
    for (int u = 1; u < N; ++u)
        if (dp[fullMask][u] != INT_MAX)
            ans = min(ans, dp[fullMask][u] + dist[u][0]);
    return ans;
}

// Enumerate all subsets of a mask
for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
    // process sub
}

// Enumerate all masks of size N with exactly k bits set
// Use next_permutation on bit string, or iterate all masks and popcount
```

---

## 6.5 Tree DP

```cpp
// dp[u][0] = best if u not selected
// dp[u][1] = best if u selected
// Example: Maximum Independent Set on tree
vector<int> dp0, dp1;
void treeDp(int u, int parent, const vector<vector<int>>& adj, const vector<int>& val) {
    dp0[u] = 0;
    dp1[u] = val[u];
    for (int v : adj[u]) {
        if (v == parent) continue;
        treeDp(v, u, adj, val);
        dp0[u] += max(dp0[v], dp1[v]);  // u not selected: child can be either
        dp1[u] += dp0[v];               // u selected: child must not be selected
    }
}
int maxIndependentSet(int root) { return max(dp0[root], dp1[root]); }
```

---

## 6.6 Digit DP

Use for: count numbers in [L, R] satisfying some digit-based property.

```cpp
string num;
int memo[20][2][2];   // dp[pos][tight][started]

int digitDP(int pos, bool tight, bool started) {
    if (pos == (int)num.size()) return started ? 1 : 0;
    if (memo[pos][tight][started] != -1) return memo[pos][tight][started];
    int limit = tight ? (num[pos] - '0') : 9;
    int result = 0;
    for (int d = 0; d <= limit; ++d) {
        bool newTight   = tight && (d == limit);
        bool newStarted = started || (d > 0);
        // Add your constraint check here:
        // e.g., if (!started && d == 0) skip some conditions
        result += digitDP(pos + 1, newTight, newStarted);
    }
    return memo[pos][tight][started] = result;
}

// Count numbers in [1, N] satisfying condition:
// memset(memo, -1, sizeof(memo));
// num = to_string(N);
// int ans = digitDP(0, true, false);
```

---

## 6.7 DP Optimisations

### Prefix Sum Optimisation

```cpp
// If dp[i] = sum of dp[j] for all j < i satisfying some condition
// Maintain a running prefix sum instead of a nested loop
// Reduces O(N²) → O(N)
vector<long long> prefix(n + 1, 0);
for (int i = 1; i <= n; ++i) {
    dp[i] = prefix[i - 1];    // sum of all valid previous dp values
    prefix[i] = prefix[i - 1] + dp[i];
}
```

### Sliding Window Optimisation (Deque)

```cpp
// dp[i] = min(dp[j] + cost) for j in [i-k, i-1]
// Use monotonic deque to get min in O(1)
deque<int> dq;
for (int i = 1; i <= n; ++i) {
    while (!dq.empty() && dq.front() < i - k) dq.pop_front();
    dp[i] = dp[dq.front()] + cost[i];
    while (!dq.empty() && dp[dq.back()] >= dp[i]) dq.pop_back();
    dq.push_back(i);
}
```

### Space Optimisation (Rolling Array)

```cpp
// If dp[i] only depends on dp[i-1]:
// Replace dp[n+1] with dp[2] and use i % 2
int dp[2][MAXM];
for (int i = 1; i <= n; ++i) {
    int cur = i % 2, prev = 1 - cur;
    for (int j = 0; j <= m; ++j)
        dp[cur][j] = max(dp[prev][j], ...);
}
```

---

## 6.8 Common DP Patterns

| Problem Type | State | Transition |
|-------------|-------|------------|
| Subsequence | dp[i] = best ending at i | dp[i] = f(dp[j]) for j < i |
| Substring | dp[i][j] = answer for [i,j] | dp[i][j] = f(dp[i+1][j], dp[i][j-1]) |
| Knapsack | dp[w] = best with weight ≤ w | dp[w] = max(dp[w], dp[w-wi]+vi) |
| Counting paths | dp[i][j] = # ways to reach (i,j) | dp[i][j] = dp[i-1][j] + dp[i][j-1] |
| Partition | dp[i] = best split for prefix i | dp[i] = min(dp[j] + cost(j,i)) |
| Bitmask | dp[mask][i] = cost visiting mask ending at i | TSP-style |

---

**Next**: [07 — String Algorithms](07_string_algorithms.md)
