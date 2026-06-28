# Bubble Sort

## Concept

Bubble Sort repeatedly steps through the array, comparing each adjacent pair and swapping them if they are out of order. Each full pass "bubbles" the largest remaining element up to its correct position at the end, so after pass *k* the last *k* elements are final and sorted. The invariant is that the unsorted region shrinks by one each pass while the sorted tail grows. With an early-exit flag, a pass that makes no swaps means the array is already sorted, giving an O(n) best case. It is simple and stable but quadratic, so it is used mainly for teaching or tiny / nearly-sorted inputs.

## Mermaid

```mermaid
flowchart TD
    A[Array of n elements] --> B[Pass over a 0..n-1-i]
    B --> C{a j > a j+1 ?}
    C -- Yes --> D[Swap adjacent pair]
    C -- No --> E[Leave as is]
    D --> F[Largest bubbles to the end]
    E --> F
    F --> G{Any swaps this pass?}
    G -- Yes --> B
    G -- No --> H[Array sorted]
```

## Complexity

- Time (Best): O(n) — already sorted, early exit on a swap-free pass
- Time (Average): O(n^2)
- Time (Worst): O(n^2) — reverse-sorted input
- Space: O(1) — in place
- Stable: Yes

## C++11 Code

```cpp
#include <vector>
#include <utility> // std::swap
using namespace std;

void bubbleSort(vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;            // detect an already-sorted array
        // After i passes the last i elements are in final position,
        // so we only compare up to n-1-i.
        for (int j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j + 1]) {       // adjacent pair out of order
                swap(a[j], a[j + 1]);    // bubble the larger one rightward
                swapped = true;
            }
        }
        if (!swapped) break;             // no swaps => fully sorted, stop
    }
}
```

## Mini Usage Example

```cpp
vector<int> a = {5, 1, 4, 2, 8};
bubbleSort(a);
// a is now {1, 2, 4, 5, 8}
```

## Code Snippet Flow

```mermaid
flowchart LR
    A[Start pass i] --> B[j from 0 to n-2-i]
    B --> C{a j > a j+1 ?}
    C -- Yes --> D[swap, set swapped]
    C -- No --> E[next j]
    D --> E
    E --> F{end of pass?}
    F -- No --> B
    F -- Yes --> G{swapped?}
    G -- Yes --> A
    G -- No --> H[done]
```
