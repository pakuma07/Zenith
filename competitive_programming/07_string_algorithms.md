# 07 — String Algorithms

---

## 7.1 KMP (Knuth-Morris-Pratt) — O(N + M)

**Use for**: pattern matching, find all occurrences of pattern in text.

```cpp
// Build failure function (prefix function)
vector<int> buildKMP(const string& pat) {
    int m = pat.size();
    vector<int> fail(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = fail[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = fail[j - 1];
        if (pat[i] == pat[j]) j++;
        fail[i] = j;
    }
    return fail;
}

// Search: returns all starting positions (0-indexed)
vector<int> kmpSearch(const string& text, const string& pat) {
    vector<int> fail = buildKMP(pat);
    vector<int> matches;
    int j = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j]) j = fail[j - 1];
        if (text[i] == pat[j]) j++;
        if (j == (int)pat.size()) {
            matches.push_back(i - j + 1);
            j = fail[j - 1];
        }
    }
    return matches;
}

// Useful property: fail[m-1] = length of longest proper prefix = suffix
// Period of string s: m - fail[m-1]  (if m % period == 0, s is periodic)
```

---

## 7.2 Z-Function — O(N)

**Use for**: pattern matching (same power as KMP, often simpler), count occurrences, string period.

`z[i]` = length of the longest string starting at `s[i]` that is also a prefix of `s`.

```cpp
vector<int> zFunction(const string& s) {
    int n = s.size();
    vector<int> z(n, 0);
    z[0] = n;
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}

// Pattern matching: search for pat in text
// Build s = pat + "#" + text, compute z-function
// z[i] == pat.size() means pattern found at text[i - pat.size() - 1]
vector<int> zSearch(const string& text, const string& pat) {
    string s = pat + "#" + text;
    vector<int> z = zFunction(s);
    int m = pat.size();
    vector<int> matches;
    for (int i = m + 1; i < (int)s.size(); ++i)
        if (z[i] == m) matches.push_back(i - m - 1);
    return matches;
}
```

---

## 7.3 Hashing — O(N) build, O(1) query

**Use for**: fast string comparison, finding duplicate substrings, rolling hash.

```cpp
const long long BASE = 131, MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;

struct DoubleHash {
    int n;
    vector<long long> h1, h2, pw1, pw2;

    DoubleHash(const string& s) : n(s.size()),
        h1(n+1,0), h2(n+1,0), pw1(n+1,1), pw2(n+1,1) {
        for (int i = 0; i < n; ++i) {
            h1[i+1] = (h1[i] * BASE + s[i]) % MOD1;
            h2[i+1] = (h2[i] * BASE + s[i]) % MOD2;
            pw1[i+1] = pw1[i] * BASE % MOD1;
            pw2[i+1] = pw2[i] * BASE % MOD2;
        }
    }

    // Hash of s[l..r] (0-indexed, inclusive)
    pair<long long,long long> get(int l, int r) {
        long long v1 = (h1[r+1] - h1[l] * pw1[r-l+1] % MOD1 + MOD1 * 2) % MOD1;
        long long v2 = (h2[r+1] - h2[l] * pw2[r-l+1] % MOD2 + MOD2 * 2) % MOD2;
        return {v1, v2};
    }

    bool equal(int l1, int r1, int l2, int r2) { return get(l1,r1) == get(l2,r2); }
};
```

---

## 7.4 Trie (Prefix Tree)

**Use for**: autocomplete, prefix queries, XOR maximisation.

```cpp
struct TrieNode {
    int children[26];
    bool isEnd;
    int count;   // number of strings passing through
    TrieNode() : isEnd(false), count(0) { fill(children, children+26, -1); }
};

struct Trie {
    vector<TrieNode> nodes;
    Trie() { nodes.push_back(TrieNode()); }

    void insert(const string& s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) {
                nodes[cur].children[idx] = nodes.size();
                nodes.push_back(TrieNode());
            }
            cur = nodes[cur].children[idx];
            nodes[cur].count++;
        }
        nodes[cur].isEnd = true;
    }

    bool search(const string& s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) return false;
            cur = nodes[cur].children[idx];
        }
        return nodes[cur].isEnd;
    }

    int countPrefix(const string& prefix) {
        int cur = 0;
        for (char c : prefix) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) return 0;
            cur = nodes[cur].children[idx];
        }
        return nodes[cur].count;
    }
};

// Binary Trie for XOR maximum — store numbers bit by bit (high to low)
// Same structure but children[0] and children[1] for bits
```

---

## 7.5 Manacher's Algorithm — O(N)

**Use for**: longest palindromic substring.

```cpp
// Returns p[i] = radius of palindrome centred at i in transformed string
string manacher(const string& s) {
    // Transform: abc → #a#b#c#  (handles even/odd uniformly)
    string t = "#";
    for (char c : s) { t += c; t += '#'; }
    int n = t.size();
    vector<int> p(n, 0);
    int c = 0, r = 0;
    for (int i = 0; i < n; ++i) {
        if (i < r) p[i] = min(r - i, p[2*c - i]);
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < n && t[i-p[i]-1] == t[i+p[i]+1])
            p[i]++;
        if (i + p[i] > r) { c = i; r = i + p[i]; }
    }
    // Longest palindrome length = max(p) — odd length
    // Corresponding position in s: (i - p[i]) / 2 .. (i + p[i]) / 2 - 1
    int maxLen = *max_element(p.begin(), p.end());
    int centre  = max_element(p.begin(), p.end()) - p.begin();
    int start   = (centre - maxLen) / 2;
    return s.substr(start, maxLen);
}
```

---

## 7.6 Suffix Array — O(N log N)

**Use for**: number of distinct substrings, longest common substring, string comparisons.

```cpp
// SA-IS is complex; here's the O(N log²N) version suitable for CP
vector<int> buildSuffixArray(const string& s) {
    int n = s.size();
    vector<int> sa(n), rank_(n), tmp(n);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) rank_[i] = s[i];

    for (int gap = 1; gap < n; gap <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            int ra = a+gap < n ? rank_[a+gap] : -1;
            int rb = b+gap < n ? rank_[b+gap] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
        rank_ = tmp;
    }
    return sa;
}

// Number of distinct substrings = N*(N+1)/2 - sum(lcp)
// Build LCP array with Kasai's algorithm after suffix array
```

---

## 7.7 String Algorithm Summary

| Algorithm | Use Case | Complexity |
|-----------|----------|------------|
| KMP | All occurrences of pattern in text | O(N + M) |
| Z-function | Same as KMP, also string period | O(N) |
| Rabin-Karp hashing | Duplicate substring, multi-pattern | O(N) avg |
| Double hashing | Substring equality in O(1) | O(N) build |
| Trie | Prefix queries, XOR max | O(NL) |
| Manacher | Longest palindromic substring | O(N) |
| Suffix array | All suffix-based queries | O(N log²N) |

---

**Next**: [08 — Bit Manipulation](08_bit_manipulation.md)
