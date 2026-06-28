# 20 - Sweep Line and Offline Algorithms

Essential for geometry problems, interval problems, and offline query processing.
Heavily used in Codeforces Div 1 C/D and ICPC problems.

---

## 20.1 Sweep Line Fundamentals

**Core idea**: Sort events by one dimension (usually x or time), process left to right
while maintaining a data structure for the other dimension.

**Event types:**
- **Point event**: process at a single coordinate
- **Interval event**: open at left endpoint, close at right endpoint

```cpp
// Generic sweep line template
struct Event {
    int x;    // x-coordinate
    int type; // -1 = close, 0 = point, 1 = open (process closes before opens)
    int id;   // which interval/query
};
vector<Event> events;
sort(events.begin(), events.end(), [](const Event &a, const Event &b){
    return a.x < b.x || (a.x == b.x && a.type < b.type);
});
```

---

## 20.2 Count Rectangles Containing a Point

Given N rectangles and Q query points, count how many rectangles contain each point.

```cpp
// Each rectangle [x1,x2] x [y1,y2], query point (px, py)
// Sweep x: at x1 add y-interval, at x2+1 remove y-interval
// At query x, count active y-intervals covering py using BIT
struct Event { int x, type, y1, y2, idx; };
vector<Event> events;
for(int i = 0; i < N; i++) {
    events.push_back({x1[i], 1, y1[i], y2[i], i});   // open
    events.push_back({x2[i]+1, -1, y1[i], y2[i], i}); // close
}
for(int i = 0; i < Q; i++) {
    events.push_back({px[i], 0, py[i], py[i], i});    // query
}
sort(events.begin(), events.end(), [](auto &a, auto &b){
    return a.x < b.x || (a.x == b.x && a.type < b.type);
});

BIT bit(MAXY);
vector<int> ans(Q, 0);
for(auto &e : events) {
    if(e.type == 1)  { bit.upd(e.y1, 1); bit.upd(e.y2+1, -1); }  // add
    else if(e.type == -1){ bit.upd(e.y1,-1); bit.upd(e.y2+1, 1); } // remove
    else             ans[e.idx] = bit.qry(e.y1);                    // query prefix sum
}
```

---

## 20.3 Area of Union of Rectangles

Sweep x, maintain active y-intervals in a segment tree with lazy counting.

```cpp
// Segment tree that counts covered length of y-axis
struct SegCover {
    int n;
    vector<int> cnt;   // how many times this node's range is fully covered
    vector<int> len;   // covered length
    SegCover(int n): n(n), cnt(2*n+4, 0), len(2*n+4, 0) {}
    void upd(int l, int r, int v, int node, int lo, int hi) {
        if(r < lo || hi < l) return;
        if(l <= lo && hi <= r) { cnt[node] += v; }
        else {
            int mid = (lo + hi) / 2;
            upd(l,r,v,2*node,lo,mid);
            upd(l,r,v,2*node+1,mid+1,hi);
        }
        if(cnt[node]) len[node] = hi - lo + 1;
        else if(lo == hi) len[node] = 0;
        else len[node] = len[2*node] + len[2*node+1];
    }
    void update(int l, int r, int v) { upd(l,r,v,1,0,n-1); }
    int query() { return len[1]; }
};

// Main: coordinate compress y, sweep x
// At left edge of rectangle: update(y1, y2, +1)
// At right edge:             update(y1, y2, -1)
// Between events: area += covered_length * (next_x - cur_x)
```

---

## 20.4 Closest Pair of Points (Sweep Line)

O(N log N) algorithm using sweep line + multiset sorted by y-coordinate.

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<long long,long long> pll;

long long dist2(pll a, pll b) {
    return (a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second);
}

double closestPair(vector<pll> pts) {
    int n = pts.size();
    sort(pts.begin(), pts.end());
    double d = 1e18;  // current min distance
    // Strip contains points within x-distance d
    set<pair<long long,int>> strip;  // {y, index}
    int left = 0;
    for(int i = 0; i < n; i++) {
        long long D = (long long)ceil(d);
        // Remove points too far left
        while(left < i && pts[i].first - pts[left].first > D) {
            strip.erase({pts[left].second, left}); left++;
        }
        // Check candidates within y-strip of size 2d
        auto it = strip.lower_bound({pts[i].second - D, INT_MIN});
        while(it != strip.end() && it->first <= pts[i].second + D) {
            d = min(d, sqrt((double)dist2(pts[i], pts[it->second])));
            ++it;
        }
        strip.insert({pts[i].second, i});
    }
    return d;
}
```

---

## 20.5 Merge Sort Inversion Count

Count pairs (i, j) where i < j but a[i] > a[j]. Classic sweep / CDQ technique.

```cpp
long long mergeCount(vector<int> &a, int l, int r) {
    if(r - l <= 1) return 0;
    int mid = (l + r) / 2;
    long long cnt = mergeCount(a, l, mid) + mergeCount(a, mid, r);
    vector<int> tmp;
    int i = l, j = mid;
    while(i < mid && j < r) {
        if(a[i] <= a[j]) tmp.push_back(a[i++]);
        else { cnt += mid - i; tmp.push_back(a[j++]); }
    }
    while(i < mid) tmp.push_back(a[i++]);
    while(j < r)   tmp.push_back(a[j++]);
    for(int k = l; k < r; k++) a[k] = tmp[k-l];
    return cnt;
}
// Call: long long inv = mergeCount(a, 0, n);
```

---

## 20.6 CDQ (Offline Divide and Conquer)

**Used by top CF competitors instead of segment trees with persistent structures.**
CDQ handles problems where updates and queries are offline and can be split.

**Classic problem**: Count points (x,y) in rectangle (0,0) to (qx,qy) after k insertions.
Offline: sort by x, split by time/index, count with BIT.

```cpp
// 3D dominance: count pairs (i,j) with i<j, a[j]<a[i], b[j]<b[i], c[j]<c[i]
// (or: each query asks "how many insertions before this query have a<=qA, b<=qB")
struct Point { int a, b, c, type; int idx; };  // type: 0=update, 1=query

void cdq(vector<Point> &pts, int l, int r, BIT &bit, vector<int> &ans) {
    if(r - l <= 1) return;
    int mid = (l + r) / 2;
    cdq(pts, l, mid, bit, ans);
    cdq(pts, mid, r, bit, ans);
    // Merge: left side updates, right side queries, sorted by b
    vector<Point> left_upd, right_qry;
    for(int i = l; i < mid; i++) if(pts[i].type == 0) left_upd.push_back(pts[i]);
    for(int i = mid; i < r; i++) if(pts[i].type == 1) right_qry.push_back(pts[i]);
    // Sort both by b
    sort(left_upd.begin(), left_upd.end(), [](auto&a,auto&b){return a.b<b.b;});
    sort(right_qry.begin(), right_qry.end(), [](auto&a,auto&b){return a.b<b.b;});
    int j = 0;
    for(auto &q : right_qry) {
        while(j < (int)left_upd.size() && left_upd[j].b <= q.b) {
            bit.upd(left_upd[j].c, 1); j++;
        }
        ans[q.idx] += bit.qry(q.c);
    }
    // Clean up BIT
    for(int k = 0; k < j; k++) bit.upd(left_upd[k].c, -1);
    // Merge the two halves by a (already sorted by a due to recursion)
    inplace_merge(pts.begin()+l, pts.begin()+mid, pts.begin()+r,
        [](auto&a,auto&b){return a.a<b.a;});
}
```

**When to use CDQ:**
- Multi-dimensional dominance counting
- Point updates + range queries in offline setting
- Avoids 2D segment tree or persistent structures

---

## 20.7 Offline BFS / Kruskal Order Queries

**Pattern**: process edges sorted by weight; DSU connects components; answer "min edge on path u-v" queries as u and v merge.

```cpp
// For each query (u, v): find minimum weight edge on path u-v in MST
// Offline: sort edges by weight, sort queries, use Kruskal + small-to-large / LCT
// Simple version: binary search on sorted edges + DSU
struct Query { int u, v, idx; };
// Sort edges by weight ascending
// Binary search: for each query, find minimum weight w such that u,v are connected
// Use parallel binary search or offline sort

// Offline Kruskal approach:
// Build MST. For each query, LCA on Kruskal tree gives min edge on path.
// Kruskal tree: when merging components A and B with edge w,
//               create new node with value w, children = roots of A and B.
```

---

## 20.8 Offline Queries with Mo's Algorithm (covered in 13, extended here)

**Mo's on trees**: answer path queries offline.
Flatten tree with Euler tour, use parity trick for path queries.

```cpp
// Mo's on tree for path queries [u, v]:
// Use in/out times. If euler[u] > euler[v], swap.
// If lca(u,v) == u: query segment [in[u], in[v]]
// Else: query segment [out[u], in[v]], add lca separately

// Block size for Mo's: sqrt(n)
// Sort queries: primary = block of left endpoint, secondary = right endpoint
//   (alternate secondary direction for odd/even blocks for better constant)
auto cmp = [&](Query &a, Query &b) {
    int ba = a.l / BLOCK, bb = b.l / BLOCK;
    if(ba != bb) return ba < bb;
    return (ba & 1) ? a.r > b.r : a.r < b.r;  // Hilbert curve order
};
```

---

## 20.9 Sweep Line for Interval Problems

**Maximum number of overlapping intervals at any point:**

```cpp
vector<pii> intervals;  // {start, end}
vector<pii> events;
for(auto [l,r] : intervals) {
    events.push_back({l, 1});   // start
    events.push_back({r+1, -1}); // end (exclusive)
}
sort(events.begin(), events.end());
int cur = 0, maxOverlap = 0;
for(auto [x, d] : events) { cur += d; maxOverlap = max(maxOverlap, cur); }
```

**Minimum number of intervals to remove so no two overlap:**
= total intervals - maximum non-overlapping intervals (activity selection).

---

## 20.10 Coordinate Compression for Sweep Line

```cpp
// Compress y-coordinates before building segment tree for sweep
vector<int> ys;
for(auto [x1,y1,x2,y2] : rects) { ys.push_back(y1); ys.push_back(y2+1); }
sort(ys.begin(), ys.end());
ys.erase(unique(ys.begin(), ys.end()), ys.end());
int M = ys.size();
auto getIdx = [&](int y) {
    return (int)(lower_bound(ys.begin(), ys.end(), y) - ys.begin());
};
// Now segment tree has size M, each unit represents compressed y-coordinate
```

---

## 20.11 Summary

| Problem | Technique | Complexity |
|---------|-----------|-----------|
| Count rectangles containing point | Sweep x + BIT on y | O((N+Q) log N) |
| Area of union of rectangles | Sweep x + segment tree (count coverage) | O(N log N) |
| Closest pair | Sweep x + sorted strip (multiset) | O(N log N) |
| Inversion count | Merge sort / BIT | O(N log N) |
| 3D dominance counting | CDQ D&C + BIT | O(N log^2 N) |
| Point updates + range queries offline | CDQ / offline BIT | O(N log N) |
| Max overlap of intervals | Sweep with events | O(N log N) |
| Path queries on trees offline | Mo's on trees | O((N+Q) sqrt N) |
| Min edge on path queries | Kruskal tree + LCA | O((N+Q) log N) |
