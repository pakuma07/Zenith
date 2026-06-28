# String Algorithm Overview

## Concept

String algorithms answer questions about sequences of characters: where a pattern occurs, what structure repeats, and how strings relate. The chapter centers on **exact pattern matching** and the trade-offs between approaches. Naive matching needs no preprocessing but is O(n*m) in the worst case; KMP and the Z algorithm preprocess the pattern (or a combined string) to guarantee linear O(n+m) time; Rabin-Karp uses a rolling hash for O(n+m) average time and shines when searching for many patterns at once. Choosing among them depends on guarantees you need (worst-case vs average), whether you search repeatedly, and whether hashing collisions are acceptable. The table below summarizes when to reach for each.

## Mermaid

```mermaid
flowchart TD
    A["Need to find a pattern in text"] --> B{"Preprocessing allowed ?"}
    B -- No, one-off small input --> C["Naive O of n times m"]
    B -- Yes --> D{"Need worst-case guarantee ?"}
    D -- Yes --> E["KMP or Z algorithm O of n plus m"]
    D -- No, average is fine --> F["Rabin-Karp rolling hash"]
    F --> G{"Searching many patterns ?"}
    G -- Yes --> H["Hash all pattern lengths once"]
    G -- No --> I["Single rolling scan"]
```

## Complexity

| Algorithm            | Best use case                                         | Preprocessing | Search time      | Space   |
|----------------------|-------------------------------------------------------|---------------|------------------|---------|
| Naive matching       | Tiny inputs, one-off search, baseline                 | none          | O(n*m) worst     | O(1)    |
| KMP                  | Guaranteed linear single-pattern search               | O(m) LPS      | O(n+m)           | O(m)    |
| Z algorithm          | Linear matching, prefix-structure queries             | O(n+m) combined | O(n+m)         | O(n+m)  |
| Rabin-Karp (hash)    | Multi-pattern / many queries, average-case speed      | O(m)          | O(n+m) avg, O(n*m) worst | O(1) |

Here `n` is the text length and `m` is the pattern length.

## C++11 Code

```cpp
#include <string>
#include <vector>

// A convenience baseline used throughout the chapter: naive search that
// returns every start index where pat occurs in text (O(n*m) worst case).
// Smarter algorithms (KMP, Z, Rabin-Karp) optimize this with preprocessing.
std::vector<int> findMatches(const std::string& text, const std::string& pat) {
    std::vector<int> pos;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pat.size());
    if (m == 0 || m > n) return pos;
    for (int i = 0; i + m <= n; ++i) {       // each candidate alignment
        int j = 0;
        while (j < m && text[i + j] == pat[j]) // compare the window
            ++j;
        if (j == m) pos.push_back(i);          // full match
    }
    return pos;
}
```

## Mini Usage Example

```cpp
#include <iostream>

int main() {
    std::vector<int> hits = findMatches("aaaaa", "aa");
    for (int p : hits) std::cout << p << " ";  // 0 1 2 3
    std::cout << "\n";
    return 0;
}
```

## Code Snippet Flow

```mermaid
flowchart LR
    A["Pick algorithm by need"] --> B["Naive: no preprocessing"]
    A --> C["KMP: build LPS then scan"]
    A --> D["Z: build Z array on pat+sep+text"]
    A --> E["Rabin-Karp: roll hash, verify hits"]
    B --> F["Return match positions"]
    C --> F
    D --> F
    E --> F
```
