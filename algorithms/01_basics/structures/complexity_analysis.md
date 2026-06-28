# Complexity Analysis

## Concept

Complexity analysis describes how an algorithm's running time and memory grow as the input size `n` grows, ignoring constant factors and lower-order terms. Big-O gives an upper bound (worst case), Big-Omega a lower bound, and Big-Theta a tight bound. We count the dominant operations: a single loop over `n` items is `O(n)`, two nested loops are `O(n^2)`, repeatedly halving the input is `O(log n)`, and a sort is typically `O(n log n)`. The point of the analysis is to predict scalability and pick an approach that fits the constraints before writing code. Amortized analysis (e.g. a dynamic array's push_back averaging `O(1)`) and space complexity (extra memory beyond the input) are part of the same toolkit.

## Mermaid

```mermaid
flowchart TD
    A[Count dominant operations as f(n)] --> B[Drop constants & lower-order terms]
    B --> C{Growth shape?}
    C -->|constant| D[O(1)]
    C -->|halving| E[O(log n)]
    C -->|single pass| F[O(n)]
    C -->|sort / n passes of log| G[O(n log n)]
    C -->|nested loops| H[O(n^2)]
    C -->|branch per element| I[O(2^n)]
```

## Complexity

Common growth classes from fastest to slowest, with a representative example:

| Big-O | Name | Example |
|-------|------|---------|
| O(1) | constant | array index, push_back (amortized) |
| O(log n) | logarithmic | binary search |
| O(n) | linear | linear search, single scan |
| O(n log n) | linearithmic | mergesort, sort + sweep |
| O(n^2) | quadratic | naive all-pairs, bubble sort |
| O(2^n) | exponential | subset enumeration |
| O(n!) | factorial | brute-force permutations |

## C++11 Code

```cpp
#include <vector>
using namespace std;

// Two implementations of "does any value repeat?" to contrast complexity.

// O(n^2) time, O(1) extra space: compare every pair.
bool hasDuplicateQuadratic(const vector<int>& a) {
    for (size_t i = 0; i < a.size(); ++i)
        for (size_t j = i + 1; j < a.size(); ++j)
            if (a[i] == a[j]) return true;
    return false;
}

// O(n log n) time (sort dominates), O(n) extra space for the copy.
#include <algorithm>
bool hasDuplicateSorted(vector<int> a) {       // by value: we sort a copy
    sort(a.begin(), a.end());                  // O(n log n)
    for (size_t i = 1; i < a.size(); ++i)      // O(n) single pass
        if (a[i] == a[i - 1]) return true;     // duplicates are now adjacent
    return false;
}
```

## Mini Usage Example

```cpp
vector<int> data = {4, 2, 8, 2};
bool dupA = hasDuplicateQuadratic(data); // true, O(n^2)
bool dupB = hasDuplicateSorted(data);    // true, O(n log n)
(void)dupA; (void)dupB;
```

## Code Snippet Flow

```mermaid
flowchart LR
    A[Same problem: detect a duplicate] --> B[Quadratic: compare all pairs O(n^2)]
    A --> C[Sort then scan O(n log n)]
    B --> D[Trade time for low space]
    C --> E[Trade space for faster time]
```
