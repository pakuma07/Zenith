# 18 - Greedy and Constructive Algorithms

Used constantly by top Codeforces/AtCoder competitors. Greedy solves the
majority of Div 2 C/D and many Div 1 B/C problems. Constructive appears in
almost every round.

---

## 18.1 The Exchange Argument (Greedy Proof Technique)

**How to prove a greedy ordering is optimal:**

Assume an optimal solution differs from the greedy solution in some adjacent
pair (A, B). Show that swapping them to match greedy order does not worsen the
answer. Therefore greedy is at least as good as optimal.

**Classic example: Job Scheduling (minimize weighted completion time)**

Jobs have processing time `p[i]` and weight `w[i]`. Optimal order:
sort by `p[i] / w[i]` ascending (equivalently `p[i] * w[j] < p[j] * w[i]`).

```cpp
// Sort jobs to minimize sum of (completion_time * weight)
sort(jobs.begin(), jobs.end(), [](pii a, pii b){
    // a.first=p, a.second=w
    return (long long)a.first * b.second < (long long)b.first * a.second;
});
```

**Exchange argument checklist:**
1. Define your ordering comparator (say `A before B` is better)
2. Show transitivity: `A<B` and `B<C` implies `A<C`
3. Show that any out-of-order adjacent swap worsens the answer
4. Conclude: any permutation can be sorted to greedy order without loss

---

## 18.2 Greedy with Priority Queues

**Pattern: always process the locally best option available.**

```cpp
// Classic: Task scheduler - assign tasks to machines, minimize makespan
// Sort tasks descending, assign to least loaded machine
int n, m;  // n tasks, m machines
vector<int> tasks = {5, 3, 8, 2, 7};
sort(tasks.rbegin(), tasks.rend());
priority_queue<int,vector<int>,greater<int>> pq;  // min-heap of machine loads
for(int i = 0; i < m; i++) pq.push(0);
for(int t : tasks) {
    int load = pq.top(); pq.pop();
    pq.push(load + t);
}
int makespan = 0;
while(!pq.empty()) { makespan = max(makespan, pq.top()); pq.pop(); }
```

**Pattern: Scheduling with deadlines (maximize jobs completed)**

```cpp
// Each job has (deadline, profit). Maximize total profit.
// Greedy: sort by profit desc, use DSU to find latest free slot <= deadline
sort(jobs.begin(), jobs.end(), [](auto&a,auto&b){return a.second>b.second;});
vector<int> par(maxD+2);
iota(par.begin(), par.end(), 0);
function<int(int)> find = [&](int x) -> int {
    return par[x]==x ? x : par[x]=find(par[x]);
};
int profit = 0;
for(auto [d,p] : jobs) {
    int slot = find(d);
    if(slot > 0) { profit += p; par[slot] = slot - 1; }
}
```

---

## 18.3 Interval Greedy Patterns

### Activity Selection (maximize non-overlapping intervals)
Sort by **right endpoint** ascending. Greedily pick intervals that don't overlap.

```cpp
// intervals = {left, right}
sort(intervals.begin(), intervals.end(), [](auto&a,auto&b){ return a.second<b.second; });
int count = 0, last = INT_MIN;
for(auto [l,r] : intervals) {
    if(l >= last) { count++; last = r; }
}
```

### Interval Cover (minimum intervals to cover [0, L])
Sort by left endpoint. Among all intervals starting <= current reach, pick the one extending furthest right.

```cpp
sort(intervals.begin(), intervals.end());
int count = 0, reach = 0, best = 0, i = 0, n = intervals.size();
while(reach < L) {
    while(i < n && intervals[i].first <= reach)
        best = max(best, intervals[i++].second);
    if(best == reach) { /* impossible */ break; }
    reach = best; count++;
}
```

### Merge Overlapping Intervals
Sort by left. Merge if current.left <= prev.right.

```cpp
sort(intervals.begin(), intervals.end());
vector<pii> merged;
for(auto [l,r] : intervals) {
    if(!merged.empty() && l <= merged.back().second)
        merged.back().second = max(merged.back().second, r);
    else
        merged.push_back({l,r});
}
```

---

## 18.4 Median Minimization

**Claim**: To minimize the sum of |x - a[i]|, set x = median(a[]).
**Claim**: To minimize the maximum |x - a[i]|, set x = (min + max) / 2.

```cpp
// Minimize sum of |x - a[i]| where x is integer
sort(a.begin(), a.end());
int x = a[n/2];  // median

// For two arrays: merge, take median
// For weighted: weighted median (point where cumulative weight >= total/2)
```

---

## 18.5 Greedy: Always Take Positive / Avoid Negative

**Subarray sum problems:**
- Max subarray sum: Kadane's algorithm (reset to 0 when prefix goes negative)

```cpp
ll maxSub = 0, cur = 0;
for(int x : a) { cur = max((ll)x, cur+x); maxSub = max(maxSub, cur); }
```

**Maximize sum with at most k sign flips:**
Sort absolute values descending. Flip k smallest negative (or if more flips needed
and all positive, flip smallest positive repeatedly).

```cpp
// Maximize sum of array by flipping exactly k elements' signs
sort(a.begin(), a.end());
for(int i = 0; i < n && k > 0 && a[i] < 0; i++, k--) a[i] = -a[i];
if(k % 2 == 1) {  // odd flips remaining: flip smallest absolute value
    sort(a.begin(), a.end(), [](int x,int y){ return abs(x)<abs(y); });
    a[0] = -a[0];
}
```

---

## 18.6 Constructive: Common Patterns

### Pattern 1: Split into two groups with some property
Think: color with 2 colors, assign 0/1, place on left/right.

```cpp
// Make array non-decreasing by assigning each element to either A or B
// If a[i] >= last_A -> put in A, else if a[i] >= last_B -> put in B, else impossible
int lastA = INT_MIN, lastB = INT_MIN;
bool ok = true;
for(int x : a) {
    if(x >= lastA) lastA = x;
    else if(x >= lastB) lastB = x;
    else { ok = false; break; }
}
```

### Pattern 2: Build from both ends (greedy two-pointer)
Place the largest unused element at left or right to construct a permutation.

```cpp
// Construct permutation 1..n such that |p[i]-p[i+1]| alternates large/small
// Tourist's trick: place n, 1, n-1, 2, n-2, 3 ...
int lo = 1, hi = n;
vector<int> res;
bool take_hi = true;
for(int i = 0; i < n; i++) {
    if(take_hi) res.push_back(hi--);
    else         res.push_back(lo++);
    take_hi = !take_hi;
}
```

### Pattern 3: Greedy construction with invariant
Maintain an invariant as you build the answer. Undo a decision if it breaks the invariant.

```cpp
// Build string with no two identical adjacent characters using given counts
// Greedy: always place the most frequent remaining character (not same as last)
string ans;
priority_queue<pii> pq;  // {count, char}
for(auto [c,cnt] : charCount) pq.push({cnt, c});
while(!pq.empty()) {
    auto [cnt1, c1] = pq.top(); pq.pop();
    if(!ans.empty() && ans.back() == c1) {
        if(pq.empty()) { ans = ""; break; }  // impossible
        auto [cnt2, c2] = pq.top(); pq.pop();
        ans += c2;
        if(cnt2 - 1 > 0) pq.push({cnt2-1, c2});
        pq.push({cnt1, c1});
    } else {
        ans += c1;
        if(cnt1 - 1 > 0) pq.push({cnt1-1, c1});
    }
}
```

### Pattern 4: Work backwards
If the forward greedy is hard, reverse the operations and apply greedy in reverse.

```cpp
// Example: repeatedly remove "ab" from string until none remain.
// How many operations? Work backwards: insert "ab"s.
// Or just simulate with a stack:
string s = "aababb";
string stk;
int count = 0;
for(char c : s) {
    if(c == 'b' && !stk.empty() && stk.back() == 'a') {
        stk.pop_back(); count++;
    } else stk += c;
}
```

---

## 18.7 Ternary Search (Unimodal Functions)

Find minimum/maximum of a **unimodal** function on [lo, hi].
f(x) is unimodal if it first decreases then increases (or vice versa).

```cpp
// Integer ternary search: minimize f(x) on [lo, hi]
// Converges in O(log_{1.5}(hi-lo)) ~ 100 iterations for 10^9 range
auto ternarySearch = [&](long long lo, long long hi) -> long long {
    while(hi - lo > 2) {
        long long m1 = lo + (hi - lo) / 3;
        long long m2 = hi - (hi - lo) / 3;
        if(f(m1) < f(m2)) hi = m2;  // change to > for maximum
        else               lo = m1;
    }
    long long best = lo;
    for(long long x = lo+1; x <= hi; x++)
        if(f(x) < f(best)) best = x;  // change to > for maximum
    return best;
};

// Floating point ternary search (100 iterations is enough)
auto ternaryF = [&](double lo, double hi) -> double {
    for(int iter = 0; iter < 200; iter++) {
        double m1 = lo + (hi - lo) / 3;
        double m2 = hi - (hi - lo) / 3;
        if(f(m1) < f(m2)) hi = m2;
        else               lo = m1;
    }
    return (lo + hi) / 2;
};
```

**Ternary search on 2D**: apply nested ternary searches when f(x,y) is unimodal in each coordinate for fixed other.

---

## 18.8 Parallel Binary Search

Run B binary searches simultaneously, reducing total DFS/query cost.

**Use case**: Q offline queries "what is the answer for query i?" where the
answer is monotone in some parameter and checking requires a data structure update.

```cpp
// Parallel binary search template
// For each query i, answer is in [lo[i], hi[i]]
// check(mid, i) = does query i have answer <= mid?
int Q = queries.size();
vector<int> lo(Q, 0), hi(Q, MAXVAL), ans(Q);
for(int iter = 0; iter < 30; iter++) {
    vector<vector<int>> buckets(MAXVAL + 1);
    for(int i = 0; i < Q; i++) {
        if(lo[i] <= hi[i]) {
            int mid = (lo[i] + hi[i]) / 2;
            buckets[mid].push_back(i);
        }
    }
    // Process values 0..MAXVAL, accumulating state
    // state.reset();
    for(int v = 0; v <= MAXVAL; v++) {
        // state.add(v);  // e.g., insert edge v, process event v
        for(int i : buckets[v]) {
            if(/*state satisfies query i*/ true) hi[i] = v - 1;
            else                               lo[i] = v + 1;
        }
    }
}
for(int i = 0; i < Q; i++) ans[i] = lo[i];
```

**Complexity**: O((N + Q) log N × cost_per_check)
**Classic use**: "minimum edge weight on path between u and v" for Q queries — binary search on sorted edges + DSU.

---

## 18.9 Fractional Programming (Binary Search on Ratio)

Maximize f(S) = sum(a[i] for i in S) / sum(b[i] for i in S).

**Key insight**: f(S) >= lambda iff sum(a[i] - lambda*b[i]) >= 0 for best S.

```cpp
// Maximize average: choose k elements to maximize sum(a) / sum(b)
// Binary search on answer lambda in [0, max_a/min_b]
// check(lambda): can we achieve ratio >= lambda?
//   greedily pick k elements with largest (a[i] - lambda * b[i])
auto check = [&](double lam) -> bool {
    vector<double> diff(n);
    for(int i = 0; i < n; i++) diff[i] = a[i] - lam * b[i];
    sort(diff.rbegin(), diff.rend());
    double s = 0;
    for(int i = 0; i < k; i++) s += diff[i];
    return s >= 0;
};
double lo = 0, hi = 1e9;
for(int iter = 0; iter < 100; iter++) {
    double mid = (lo + hi) / 2;
    if(check(mid)) lo = mid;
    else           hi = mid;
}
```

---

## 18.10 Common Greedy Decision Checklist

```
1. Sort by some key and process in order
   -> What key? Try: value, -value, ratio, deadline, size, finish time
2. Always pick the locally best option (priority queue)
   -> Prove it with exchange argument
3. Reduce to known greedy: interval scheduling, coin change, Huffman
4. If greedy fails, try DP
5. Constructive: think about what the answer looks like, then build it
6. Invariant: what property does your partial solution maintain?
7. Work backwards if forward is hard
8. Check: does it matter which tied elements you pick first?
```

---

## 18.11 Summary Table

| Problem Type | Greedy Strategy | Proof |
|-------------|----------------|-------|
| Maximize intervals selected | Sort by right endpoint | Exchange argument |
| Minimize intervals to cover | Sort by left, extend reach | Exchange argument |
| Job scheduling (minimize lateness) | Sort by deadline | Exchange argument |
| Huffman coding | Priority queue, merge two smallest | Optimal substructure |
| Maximize sum with k flips | Flip k most negative | Local optimality |
| Fractional knapsack | Sort by value/weight ratio | Exchange argument |
| Find minimum spanning tree | Kruskal / Prim | Cut property |
| Maximize average of k elements | Binary search + sort | Parametric search |
| Unimodal function min/max | Ternary search | Unimodality |
| Q binary searches with shared state | Parallel binary search | Amortization |
