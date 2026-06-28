# 03 — STL Tricks for Competitive Programming

The STL is your most powerful tool. Master these patterns to write correct solutions fast.

---

## 3.1 vector

```cpp
vector<int> v;
v.reserve(n);                    // preallocate — avoids reallocations
v.assign(n, 0);                  // fill n zeros
v.resize(n, -1);                 // resize, fill new slots with -1
v.insert(v.begin(), x);          // O(N) — insert at front (avoid in hot loops)
v.erase(v.begin() + i);          // O(N) — erase at index i

// Erase without preserving order — O(1)
v[i] = v.back(); v.pop_back();

// 2D vector initialisation
int R = 4, C = 5;
vector<vector<int>> grid(R, vector<int>(C, 0));

// Flatten 2D → 1D index
int idx = r * C + c;
int row = idx / C, col = idx % C;
```

---

## 3.2 Sorting

```cpp
sort(v.begin(), v.end());                         // ascending
sort(v.begin(), v.end(), greater<int>());          // descending

// Custom comparator — sort by second element of pair
sort(v.begin(), v.end(), [](const pii& a, const pii& b){
    return a.second < b.second;
});

// Stable sort — preserves original order of equal elements
stable_sort(v.begin(), v.end());

// Partial sort — only need top K elements
partial_sort(v.begin(), v.begin() + k, v.end());

// nth_element — O(N) — v[k] becomes what it would be if sorted
nth_element(v.begin(), v.begin() + k, v.end());
```

---

## 3.3 Binary Search

```cpp
// Requires sorted range
vector<int> v = {1, 3, 3, 5, 7};

// Does value exist?
bool found = binary_search(v.begin(), v.end(), 3);

// First position >= val
auto lb = lower_bound(v.begin(), v.end(), 3);
int  idx = lb - v.begin();   // 1 (index of first 3)

// First position > val
auto ub = upper_bound(v.begin(), v.end(), 3);
int  cnt = ub - lb;          // 2 (count of 3s)

// Count occurrences of val
int occurrences = upper_bound(v.begin(), v.end(), 3)
                - lower_bound(v.begin(), v.end(), 3);
```

---

## 3.4 pair and Sorting Tricks

```cpp
pair<int,int> p = make_pair(3, 7);
p.first;  p.second;

// Pairs sort lexicographically by default
vector<pair<int,int>> edges = {{3,1},{1,2},{2,4}};
sort(edges.begin(), edges.end());  // sorted by first, then second

// Trick: negate first element to sort by first descending,
// second ascending — no custom comparator needed
// Store {-weight, node} in priority queue for max→min ordering
```

---

## 3.5 set and multiset

```cpp
set<int> s;
s.insert(3);
s.erase(3);
s.count(3);          // 0 or 1
s.find(3);           // iterator (s.end() if not found)

// Lower/upper bound (same semantics as sorted vector)
auto it = s.lower_bound(5);  // first element >= 5
auto it = s.upper_bound(5);  // first element > 5

// Nearest smaller element:
auto it = s.lower_bound(x);
if (it != s.begin()) { --it; /* *it is largest value < x */ }

// multiset — allows duplicates
multiset<int> ms;
ms.insert(3); ms.insert(3);
ms.erase(ms.find(3));    // erase only ONE occurrence (not all!)
```

---

## 3.6 map and unordered_map

```cpp
map<string,int> freq;
freq["apple"]++;
freq.count("apple");      // 0 or 1
freq.find("apple");       // iterator

// Iterate in sorted key order
for (auto& kv : freq) cout << kv.first << " " << kv.second << "\n";

// unordered_map — O(1) average, no ordering
unordered_map<int,int> ump;
ump.reserve(n * 2);      // prevent rehashing — set larger than expected
ump.max_load_factor(0.25); // lower → fewer collisions, more memory

// Anti-hack for unordered_map (custom hash)
struct SafeHash {
    size_t operator()(int x) const {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        return (x >> 16) ^ x;
    }
};
unordered_map<int,int,SafeHash> safe_map;
```

---

## 3.7 priority_queue

```cpp
// Max-heap (default)
priority_queue<int> maxpq;
maxpq.push(3);
maxpq.top();     // largest
maxpq.pop();

// Min-heap
priority_queue<int, vector<int>, greater<int>> minpq;

// Min-heap of pairs — sorted by first element
priority_queue<pair<int,int>,
               vector<pair<int,int>>,
               greater<pair<int,int>>> dijkstra_pq;

// Push {distance, node}
dijkstra_pq.push({0, src});
auto [dist, u] = dijkstra_pq.top();   // C++17
// C++11: int dist = dijkstra_pq.top().first; int u = ...second;
```

---

## 3.8 deque and stack and queue

```cpp
deque<int> dq;
dq.push_front(1);  dq.push_back(2);
dq.pop_front();    dq.pop_back();
dq.front();        dq.back();

// Monotonic deque — sliding window maximum in O(N)
deque<int> mono;  // stores indices
for (int i = 0; i < n; ++i) {
    while (!mono.empty() && a[mono.back()] <= a[i]) mono.pop_back();
    mono.push_back(i);
    if (mono.front() <= i - k) mono.pop_front();   // out of window
    if (i >= k-1) cout << a[mono.front()] << " ";  // window max
}
```

---

## 3.9 Useful Algorithm Functions

```cpp
// Min/Max
int lo = *min_element(v.begin(), v.end());
int hi = *max_element(v.begin(), v.end());

// Sum of range
long long total = accumulate(v.begin(), v.end(), 0LL);

// Rotate
rotate(v.begin(), v.begin() + k, v.end());  // left-rotate by k

// Reverse
reverse(v.begin(), v.end());

// Remove duplicates (must sort first!)
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());

// Next/prev permutation
do {
    // process permutation
} while (next_permutation(v.begin(), v.end()));

// Fill range with iota (0, 1, 2, ...)
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(), [&](int a, int b){ return v[a] < v[b]; });
// idx is now a sorted index array
```

---

## 3.10 GCC Built-ins

```cpp
__builtin_popcount(x)    // count set bits in int
__builtin_popcountll(x)  // count set bits in long long
__builtin_clz(x)         // count leading zeros (undefined for x=0)
__builtin_ctz(x)         // count trailing zeros (undefined for x=0)
__builtin_parity(x)      // parity (0 if even # of set bits, 1 if odd)

// log2 via clz (fast):
int floor_log2 = 31 - __builtin_clz(x);  // x > 0

// Manual GCD (C++11 — no std::gcd until C++17)
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int lcm(int a, int b) { return a / gcd(a, b) * b; }
```

---

## 3.11 String Tips

```cpp
string s = "hello";
s.size();                    // length (avoid .length())
s.substr(pos, len);          // substring
s.find("ell");               // returns pos or string::npos
s += " world";               // append

// Number conversions
int    x = stoi("42");
ll     y = stoll("999999999999");
string t = to_string(42);

// Character operations
char c = 'A';
islower(c); isupper(c); isdigit(c); isalpha(c);
tolower(c); toupper(c);
int digit = c - '0';    // char '5' → int 5
char letter = 'a' + i;  // int 0-25 → 'a'-'z'
```

---

## Summary Table

| Task | Best Tool |
|------|-----------|
| Sorted unique elements | `set<T>` |
| Frequency map | `unordered_map<T,int>` (faster) or `map<T,int>` |
| Min/Max element quickly | `priority_queue` |
| Sorted order + nearest queries | `set` with `lower_bound`/`upper_bound` |
| Sliding window max/min | Monotonic deque |
| Coordinates compression | sort + `lower_bound` |

---

**Next**: [04 — Number Theory](04_number_theory.md)
