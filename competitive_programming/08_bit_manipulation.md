# 08 — Bit Manipulation

Bit tricks give O(1) solutions to many problems and speed up DP with bitmask states.

---

## 8.1 Fundamentals

```cpp
// Bit indexing: bit 0 is the least significant (rightmost)
int x = 0b1010;   // binary literal (C++14); use 10 in C++11

// The six bitwise operators
x & y    // AND  — 1 only where both bits are 1
x | y    // OR   — 1 where at least one bit is 1
x ^ y    // XOR  — 1 where bits differ
~x       // NOT  — flip all bits
x << k   // left shift  — multiply by 2^k
x >> k   // right shift — divide by 2^k (arithmetic for signed)
```

---

## 8.2 Single-Bit Operations

```cpp
int x = 42;
int k = 3;    // target bit position

// Check bit k
bool isSet = (x >> k) & 1;

// Set bit k
x |= (1 << k);

// Clear bit k
x &= ~(1 << k);

// Toggle bit k
x ^= (1 << k);

// Get lowest set bit (LSB)
int lsb = x & (-x);          // isolates the lowest 1

// Clear lowest set bit
x &= x - 1;

// Count set bits
int cnt = __builtin_popcount(x);      // int
int cnt = __builtin_popcountll(x);    // long long

// Check power of 2
bool isPow2 = (x > 0) && (x & (x - 1)) == 0;

// Next power of 2 >= x
unsigned int np2(unsigned int x) {
    if (x == 0) return 1;
    x--;
    x |= x >> 1; x |= x >> 2; x |= x >> 4;
    x |= x >> 8; x |= x >> 16;
    return x + 1;
}
```

---

## 8.3 Arithmetic Tricks

```cpp
x * 2   → x << 1
x / 2   → x >> 1
x % 2   → x & 1
x * 4   → x << 2
x / 8   → x >> 3

// Absolute value without branching
int mask = x >> 31;          // all 0s if positive, all 1s if negative
int abs_x = (x + mask) ^ mask;

// Min and max without branching
int mn = y ^ ((x ^ y) & -(x < y));
int mx = x ^ ((x ^ y) & -(x < y));

// Swap without temp
a ^= b; b ^= a; a ^= b;    // only works for distinct memory locations

// Average without overflow
int avg = (a & b) + ((a ^ b) >> 1);
```

---

## 8.4 Subset Enumeration

```cpp
int mask = 7;   // e.g. bitmask of N=3 elements

// Enumerate ALL subsets of a given mask (including empty subset)
for (int sub = mask; ; sub = (sub - 1) & mask) {
    // process sub
    if (sub == 0) break;
}

// Enumerate ALL masks with exactly k bits set (out of N bits)
// — iterate all 2^N masks and check popcount
for (int m = 0; m < (1 << N); ++m)
    if (__builtin_popcount(m) == k) { /* process m */ }

// Sum over subsets DP (SOS DP) — O(N × 2^N)
// dp[mask] = sum of a[sub] for all sub ⊆ mask
vector<long long> dp(1 << N);
for (int i = 0; i < N; ++i)
    for (int mask = 0; mask < (1 << N); ++mask)
        if (mask & (1 << i)) dp[mask] += dp[mask ^ (1 << i)];
```

---

## 8.5 XOR Properties

```cpp
// Key XOR identities:
x ^ x == 0
x ^ 0 == x
x ^ y == y ^ x     // commutative
(x^y)^z == x^(y^z) // associative

// Find the single non-duplicate in array where all others appear twice
int single = 0;
for (int x : arr) single ^= x;
// Result is the unique element

// Find two non-duplicates (a and b) when all others appear twice
int xorAll = 0;
for (int x : arr) xorAll ^= x;  // = a ^ b
int bit = xorAll & (-xorAll);   // any differing bit
int a = 0, b = 0;
for (int x : arr) {
    if (x & bit) a ^= x;
    else          b ^= x;
}
// a and b are the two unique elements

// Maximum XOR of any two elements — use binary trie
// Insert all numbers, then for each x query the trie greedily
```

---

## 8.6 Bitset

```cpp
#include <bitset>

bitset<100> bs;          // 100 bits, all 0
bs.set(5);               // set bit 5
bs.reset(5);             // clear bit 5
bs.flip(5);              // toggle bit 5
bs.test(5);              // check bit 5
bs.count();              // number of set bits
bs.none();               // true if all 0
bs.any();                // true if any 1
bs.all();                // true if all 1

// Bitwise ops on whole bitset — each op is O(N/64)
bitset<100> a, b;
a & b;  a | b;  a ^ b;  ~a;
a <<= 3;  a >>= 2;

// String conversion
bitset<8> bs2("10110011");
string s = bs2.to_string();
unsigned long ul = bs2.to_ulong();

// Bitset DP — knapsack in O(N × W/64) using bitset shift
bitset<MAXW> dp;
dp[0] = 1;
for (int w : weights) dp |= (dp << w);
// dp[W] is 1 if weight W is achievable
```

---

## 8.7 Bitmask DP Patterns

```cpp
// State: visited set (bitmask) + current position
// dp[mask][v] = min cost to visit all in mask, ending at v

// Check if bit i is set in mask
(mask >> i) & 1

// Add vertex i to visited set
mask | (1 << i)

// Remove vertex i from visited set
mask & ~(1 << i)

// Full mask (all N vertices visited)
(1 << N) - 1

// Complement of mask
((1 << N) - 1) ^ mask

// Common: iterate over all subsets of complement
int comp = ((1 << N) - 1) ^ mask;
for (int sub = comp; sub > 0; sub = (sub - 1) & comp) { /* ... */ }
```

---

## 8.8 Tricks Summary Table

| Task | Expression |
|------|-----------|
| Check bit k | `(x >> k) & 1` |
| Set bit k | `x \| (1 << k)` |
| Clear bit k | `x & ~(1 << k)` |
| Toggle bit k | `x ^ (1 << k)` |
| Lowest set bit | `x & (-x)` |
| Clear lowest set bit | `x & (x - 1)` |
| Count set bits | `__builtin_popcount(x)` |
| Is power of 2? | `x > 0 && (x & (x-1)) == 0` |
| All subsets of mask | `for (sub=mask; sub; sub=(sub-1)&mask)` |
| XOR all → find single | Fold XOR over array |
| Bitset knapsack | `dp \|= (dp << w)` |

---

**Next**: [09 — Geometry](09_geometry.md)
