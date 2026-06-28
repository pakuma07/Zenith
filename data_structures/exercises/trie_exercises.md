# Exercises: Trie

## Easy

1. Insert and search words.
2. Prefix check (startsWith).
3. Count words with given prefix.
4. Delete a word from trie.
5. Print all words stored in lexicographic order.

## Medium

1. Longest common prefix of words.
2. Replace words in sentence by root dictionary.
3. Word dictionary with wildcard dot character.
4. Count distinct substrings using trie.
5. Auto-complete top suggestions.

## Hard

1. Implement compressed trie (radix tree) overview.
2. Maximum xor pair using bitwise trie.
3. Streaming dictionary matcher.
4. Word break with trie + DP.
5. Multi-pattern search (Aho-Corasick intro).

## Challenge

Design contact search engine with ranked prefix suggestions.

## C++11 Exercise Example: Trie Insert/Search

```cpp
#include <array>
#include <string>
using namespace std;

struct Trie {
    struct Node { array<int, 26> nxt; bool end; Node() : end(false) { nxt.fill(-1); } };
    vector<Node> t = {Node()};

    void insert(const string& s) {
        int v = 0;
        for (char c : s) {
            int x = c - 'a';
            if (t[v].nxt[x] == -1) { t[v].nxt[x] = (int)t.size(); t.push_back(Node()); }
            v = t[v].nxt[x];
        }
        t[v].end = true;
    }
};
```
