# KMP

## Concept

The Knuth-Morris-Pratt algorithm finds all occurrences of a pattern `P` (length `m`) in a text `T` (length `n`) in linear time by never re-examining text characters it has already matched. It precomputes a **longest-prefix-suffix (LPS)** array, where `lps[i]` is the length of the longest proper prefix of `P[0..i]` that is also a suffix of it. During the scan, when a mismatch occurs after matching `j` characters, instead of shifting by one, KMP falls back to `lps[j-1]`, reusing the known matched prefix and avoiding redundant comparisons. Because the text pointer only ever moves forward, the search is O(n + m). It is the standard choice when you need guaranteed linear matching without hashing.

## Mermaid

```mermaid
flowchart TD
    A["Build LPS array from pattern"] --> B["i = 0 over text, j = 0 over pattern"]
    B --> C{"T at i == P at j ?"}
    C -- Yes --> D["i++, j++"]
    D --> E{"j == m ?"}
    E -- Yes --> F["record match at i - m; j = lps at j-1"]
    E -- No --> C
    C -- No --> G{"j > 0 ?"}
    G -- Yes --> H["j = lps at j-1"]
    G -- No --> I["i++"]
    H --> C
    I --> C
```

## Complexity

- Time: O(n + m) -- O(m) to build the LPS array, O(n) for the search.
- Space: O(m) for the LPS array.

## C++11 Code

```cpp
#include <string>
#include <vector>

// Build the longest-proper-prefix-which-is-also-suffix array for pat.
std::vector<int> buildLPS(const std::string& pat) {
    int m = static_cast<int>(pat.size());
    std::vector<int> lps(m, 0);
    int len = 0;                 // length of current matching prefix
    for (int i = 1; i < m; ) {
        if (pat[i] == pat[len]) {
            lps[i++] = ++len;    // extend the matched prefix
        } else if (len > 0) {
            len = lps[len - 1];  // fall back without advancing i
        } else {
            lps[i++] = 0;        // no prefix matches here
        }
    }
    return lps;
}

// Return every start index where pat occurs in text.
std::vector<int> kmpSearch(const std::string& text, const std::string& pat) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pat.size());
    if (m == 0 || m > n) return matches;

    std::vector<int> lps = buildLPS(pat);
    int i = 0;                   // index into text
    int j = 0;                   // index into pattern
    while (i < n) {
        if (text[i] == pat[j]) {
            ++i; ++j;
            if (j == m) {                 // full pattern matched
                matches.push_back(i - m);
                j = lps[j - 1];           // continue searching
            }
        } else if (j > 0) {
            j = lps[j - 1];               // reuse matched prefix
        } else {
            ++i;                          // nothing matched, advance text
        }
    }
    return matches;
}
```

## Mini Usage Example

```cpp
#include <iostream>

int main() {
    std::vector<int> hits = kmpSearch("ababcabcabababd", "ababd");
    for (int p : hits) std::cout << p << " ";  // 10
    std::cout << "\n";
    return 0;
}
```

## Code Snippet Flow

```mermaid
flowchart LR
    A["buildLPS pattern"] --> B["i = 0, j = 0"]
    B --> C{"text i == pat j ?"}
    C -- Yes --> D["i++, j++"]
    D --> E{"j == m ?"}
    E -- Yes --> F["push i-m; j = lps j-1"]
    E -- No --> C
    C -- No --> G{"j > 0 ?"}
    G -- Yes --> H["j = lps j-1"]
    G -- No --> I["i++"]
    F --> C
    H --> C
    I --> C
```
