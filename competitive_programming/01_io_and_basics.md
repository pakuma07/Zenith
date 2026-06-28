# 01 — I/O and CP Basics

Fast I/O can be the difference between AC and TLE. Set it up **first**, before writing any logic.

---

## 1.1 The Essential Fast I/O Line

```cpp
ios_base::sync_with_stdio(false);
cin.tie(NULL);
```

- `sync_with_stdio(false)` — decouples C++ streams from C stdio (~3–5× faster `cin`)
- `cin.tie(NULL)` — stops `cout` from flushing before every `cin`

**Rule**: Use this in every contest. Never mix `printf`/`scanf` with `cin`/`cout` after this line — it causes undefined output ordering.

---

## 1.2 Full I/O Setup Template

```cpp
#include <bits/stdc++.h>   // GCC only — includes everything
// OR include individually (portable):
// #include <iostream>
// #include <vector>
// #include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    // ...
    return 0;
}
```

### When to use `scanf`/`printf` instead

- When you **cannot** use `bits/stdc++.h` (MSVC, some judges)
- When mixing with legacy C code
- For floating-point with fixed precision: `printf("%.6f\n", ans);`

---

## 1.3 Reading Until EOF

```cpp
// Pattern 1: read integers until EOF
int x;
while (cin >> x) {
    // process x
}

// Pattern 2: read lines until EOF
string line;
while (getline(cin, line)) {
    // process line
}

// Pattern 3: T test cases
int T;
cin >> T;
while (T--) {
    // solve one test case
}
```

---

## 1.4 Reading a Whole Line After a Number

```cpp
int n;
cin >> n;
cin.ignore();          // consume the leftover '\n'
string line;
getline(cin, line);    // now reads correctly
```

---

## 1.5 Output Tips

```cpp
// Faster than endl — does NOT flush the buffer
cout << "\n";

// Use endl only when you MUST flush (interactive problems)
cout << ans << endl;   // flushes — slow in loops

// Fixed floating point
cout << fixed;
cout.precision(9);
cout << 3.14159265358979 << "\n";  // prints 9 decimal places

// printf equivalent for doubles
printf("%.9f\n", 3.14159265358979);
```

---

## 1.6 Common Macros and Typedefs

```cpp
// --- Type aliases ---
typedef long long           ll;
typedef unsigned long long  ull;
typedef pair<int,int>       pii;
typedef pair<ll,ll>         pll;
typedef vector<int>         vi;
typedef vector<ll>          vl;
typedef vector<pii>         vpii;

// --- Short names for common values ---
const int  INF  = 1e9;          // infinity for int problems
const ll   LINF = 4e18;         // infinity for long long
const int  MOD  = 1e9 + 7;     // standard modulo (prime)
const int  MOD2 = 998244353;    // alternative prime modulo

// --- Loops ---
#define FOR(i, a, b)  for (int i = (a); i < (b); ++i)
#define REP(i, n)     for (int i = 0; i < (n); ++i)
#define RFOR(i, b, a) for (int i = (b)-1; i >= (a); --i)

// --- Container shortcuts ---
#define all(v)   (v).begin(), (v).end()
#define sz(v)    (int)(v).size()
#define pb       push_back
#define mp       make_pair
#define fi       first
#define se       second
```

---

## 1.7 GCC-Specific Optimizations

```cpp
// At top of file — before any includes in some setups
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")   // SIMD — helps with bitset problems

// Use only on judges that allow it (Codeforces, AtCoder)
// May be disallowed in ICPC/IOI
```

---

## 1.8 Portable Headers (Non-GCC)

```cpp
// Instead of bits/stdc++.h, include what you need:
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <utility>     // pair
#include <cmath>
#include <climits>     // INT_MAX, LLONG_MAX
#include <cassert>
#include <numeric>     // iota, accumulate, gcd (C++17 only for gcd)
#include <functional>  // greater<>, less<>
```

---

## 1.9 Custom Fast Read (when even `scanf` is too slow)

```cpp
// Ultra-fast integer reader — for N > 10^6 inputs
inline int readInt() {
    int x = 0; char c = getchar_unlocked();
    while (c < '0' || c > '9') c = getchar_unlocked();
    while (c >= '0' && c <= '9') { x = x*10 + (c-'0'); c = getchar_unlocked(); }
    return x;
}
// On Windows: use getchar() instead of getchar_unlocked()
```

---

## 1.10 Submitting: Last-Minute Checklist

```
✓ Fast I/O added
✓ Return type of main is int, returns 0
✓ Array sizes are large enough (+1 for safety)
✓ long long used where values exceed 2×10^9
✓ Modular arithmetic applied where needed
✓ No endl in tight loops (use "\n")
✓ Removed all debug output
✓ Read the problem constraints one more time
```

---

## Summary

| Technique | When to use |
|-----------|-------------|
| `sync_with_stdio(false)` + `cin.tie(NULL)` | Always |
| `"\n"` instead of `endl` | Always in loops |
| `bits/stdc++.h` | GCC judges (Codeforces, AtCoder) |
| `scanf`/`printf` | Mixed C/C++ or when no `bits/stdc++.h` |
| Custom fast read | N > 10^6 integer inputs |
| `#pragma GCC optimize` | When TLE with correct algorithm |

---

**Next**: [02 — Time and Space Complexity](02_time_complexity.md)
