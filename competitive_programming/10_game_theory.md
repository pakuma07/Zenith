# 10 — Game Theory (Sprague-Grundy)

Combinatorial game theory lets you determine the winner of impartial games without full search.

---

## 10.1 Nim

**Rules**: N piles, each with some stones. Players alternate taking any positive number from one pile. Player who cannot move **loses**.

**Winning condition**: XOR of all pile sizes ≠ 0.

```cpp
bool nimWinner(const vector<int>& piles) {
    int xorSum = 0;
    for (int p : piles) xorSum ^= p;
    return xorSum != 0;   // true = first player wins
}

// Winning move: find any pile i where piles[i] ^ xorSum < piles[i]
// Then reduce pile i to piles[i] ^ xorSum
void nimWinningMove(vector<int>& piles) {
    int xorSum = 0;
    for (int p : piles) xorSum ^= p;
    for (int i = 0; i < (int)piles.size(); ++i) {
        int target = piles[i] ^ xorSum;
        if (target < piles[i]) {
            cout << "Take " << (piles[i] - target)
                 << " from pile " << i << "\n";
            piles[i] = target;
            return;
        }
    }
}
```

---

## 10.2 Sprague-Grundy Theorem

**Every impartial game** (where both players have the same moves) can be assigned a **Grundy number (nimber)**:

```
G(position) = MEX({ G(reachable positions) })
```

Where **MEX** = Minimum Excludant = smallest non-negative integer NOT in the set.

```cpp
// Compute Grundy numbers for positions 0..MAXN
int grundy[MAXN];
bool computed[MAXN];

// Example: game where from position n you can move to n-1, n-2, or n-3
int computeGrundy(int n) {
    if (computed[n]) return grundy[n];
    computed[n] = true;
    set<int> reachable;
    if (n >= 1) reachable.insert(computeGrundy(n - 1));
    if (n >= 2) reachable.insert(computeGrundy(n - 2));
    if (n >= 3) reachable.insert(computeGrundy(n - 3));
    int mex = 0;
    while (reachable.count(mex)) mex++;
    return grundy[n] = mex;
}
```

---

## 10.3 MEX Function

```cpp
int mex(const vector<int>& values) {
    set<int> s(values.begin(), values.end());
    int m = 0;
    while (s.count(m)) m++;
    return m;
}
```

---

## 10.4 Combining Games (Sum of Games)

When a game splits into independent sub-games:
- **XOR** the Grundy numbers of each sub-game
- XOR ≠ 0 → first player wins
- XOR = 0 → second player wins

```cpp
bool compositeGameWinner(const vector<int>& subGameGrundys) {
    int xorAll = 0;
    for (int g : subGameGrundys) xorAll ^= g;
    return xorAll != 0;   // true = first player wins
}
```

---

## 10.5 Common Game Patterns

### Staircase Nim

Players can move stones from any step down to step 0. Only odd-indexed steps matter.

```cpp
bool staircaseNimWinner(const vector<int>& steps) {
    int xorOdd = 0;
    for (int i = 1; i < (int)steps.size(); i += 2)
        xorOdd ^= steps[i];
    return xorOdd != 0;
}
```

### Grundy Values for "take 1 to k" from a pile

```
G(n) = n % (k+1)
```
If `n % (k+1) == 0`, second player wins (Grundy = 0).

```cpp
// Take any amount from 1 to k
int grundyBounded(int n, int k) { return n % (k + 1); }
```

### Wythoff's Game

Two piles; can take any from one pile, or equal amounts from both.

```cpp
bool wythoffWinner(int a, int b) {
    if (a > b) swap(a, b);
    int k = b - a;
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    int ak = (int)(k * phi);
    return a != ak;   // true = first player wins
}
```

---

## 10.6 Green Hackenbush

Edges on a graph; player removes one edge, all disconnected parts removed.
- For trees: Grundy of a tree = XOR of edge counts on each path from root to leaf.
- Bamboo (single path of k edges): Grundy = k.

---

## 10.7 Template: Grundy Table

For games with small state spaces, precompute the full Grundy table:

```cpp
const int MAXN = 1005;
int G[MAXN];

void precomputeGrundy(int n) {
    G[0] = 0;  // base: losing position (no moves)
    for (int i = 1; i <= n; ++i) {
        set<int> reachable;
        // Replace with your game's valid moves from state i:
        for (int move : {1, 2, 3}) {
            if (i >= move) reachable.insert(G[i - move]);
        }
        int mex = 0;
        while (reachable.count(mex)) mex++;
        G[i] = mex;
    }
}

// Then for M piles: XOR all G[pile_i]
// XOR != 0 → first player wins
```

---

## 10.8 Decision Framework

```
Is it an impartial game?
  Yes → Compute Grundy numbers
    Single component → G(state) == 0 means second player wins
    Multiple independent components → XOR all Grundy numbers

Is it a partisan game (players have different moves)?
  → Use alpha-beta pruning or minimax (beyond Sprague-Grundy)

Is it a simple take-away game from one pile?
  → Look for period in Grundy sequence (often periodic quickly)

Nim variant?
  → Check: move options from pile of size n → MEX of G(n - moves)
```

---

## 10.9 Summary

| Concept | Rule |
|---------|------|
| Nim | XOR of pile sizes ≠ 0 → first player wins |
| Grundy number | MEX of Grundy numbers of reachable states |
| G = 0 | Losing position (second player wins) |
| G > 0 | Winning position (first player wins) |
| Sum of games | XOR the Grundy numbers |
| "Take 1 to k" | G(n) = n % (k+1) |
| Staircase Nim | XOR of odd-indexed piles |

---

**Next**: [11 — Tips and Tricks](11_tips_and_tricks.md)
