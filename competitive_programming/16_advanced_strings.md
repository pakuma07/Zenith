# 16 — Advanced String Algorithms

These algorithms handle multi-pattern matching, all substring queries, and palindrome structures efficiently.

---

## 16.1 Aho-Corasick Automaton — O(total pattern length + text length)

**Use for**: find all occurrences of multiple patterns in a text simultaneously.

```cpp
struct AhoCorasick {
    static const int ALPHA = 26;
    struct Node {
        int children[ALPHA], fail, output;
        Node() : fail(0), output(0) { fill(children, children+ALPHA, -1); }
    };
    vector<Node> nodes;

    AhoCorasick() { nodes.push_back(Node()); }

    void insert(const string& s, int patternId) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) {
                nodes[cur].children[idx] = nodes.size();
                nodes.push_back(Node());
            }
            cur = nodes[cur].children[idx];
        }
        nodes[cur].output |= (1 << patternId);  // or store a list
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < ALPHA; ++c) {
            if (nodes[0].children[c] == -1) nodes[0].children[c] = 0;
            else { nodes[nodes[0].children[c]].fail = 0; q.push(nodes[0].children[c]); }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            nodes[u].output |= nodes[nodes[u].fail].output;  // inherit from fail
            for (int c = 0; c < ALPHA; ++c) {
                if (nodes[u].children[c] == -1)
                    nodes[u].children[c] = nodes[nodes[u].fail].children[c];
                else {
                    nodes[nodes[u].children[c]].fail = nodes[nodes[u].fail].children[c];
                    q.push(nodes[u].children[c]);
                }
            }
        }
    }

    // Search: returns count of pattern occurrences (or use output bitmask)
    vector<int> search(const string& text, int numPatterns) {
        vector<int> cnt(numPatterns, 0);
        int cur = 0;
        for (char c : text) {
            cur = nodes[cur].children[c - 'a'];
            int tmp = cur;
            while (tmp) {
                if (nodes[tmp].output)
                    for (int i = 0; i < numPatterns; ++i)
                        if (nodes[tmp].output & (1<<i)) cnt[i]++;
                tmp = nodes[tmp].fail;
            }
        }
        return cnt;
    }
};
```

---

## 16.2 Suffix Array with LCP — O(N log N) build + O(N) Kasai LCP

**Use for**: all substring queries, longest common substring of two strings, number of distinct substrings.

```cpp
// O(N log N) Suffix Array (DC3 / prefix doubling)
vector<int> buildSA(const string& s) {
    int n = s.size();
    vector<int> sa(n), rank_(n), tmp(n), cnt(max(256, n), 0);
    for (int i = 0; i < n; ++i) cnt[(unsigned char)s[i]]++;
    for (int i = 1; i < 256; ++i) cnt[i] += cnt[i-1];
    for (int i = n-1; i >= 0; --i) sa[--cnt[(unsigned char)s[i]]] = i;
    rank_[sa[0]] = 0;
    for (int i = 1; i < n; ++i)
        rank_[sa[i]] = rank_[sa[i-1]] + (s[sa[i]] != s[sa[i-1]]);

    for (int gap = 1; rank_[sa[n-1]] < n-1; gap <<= 1) {
        // Sort by second key (suffix starting gap positions later)
        vector<int> sa2(n);
        int idx = 0;
        for (int i = n-gap; i < n; ++i) sa2[idx++] = i;
        for (int i : sa) if (i >= gap) sa2[idx++] = i - gap;
        // Sort by first key (stable)
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; ++i) cnt[rank_[sa2[i]]]++;
        for (int i = 1; i < n; ++i) cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; --i) sa[--cnt[rank_[sa2[i]]]] = sa2[i];
        // Update ranks
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            tmp[sa[i]] = tmp[sa[i-1]];
            int a = sa[i], b = sa[i-1];
            if (rank_[a] != rank_[b] || (a+gap<n?rank_[a+gap]:-1) != (b+gap<n?rank_[b+gap]:-1))
                tmp[sa[i]]++;
        }
        rank_ = tmp;
    }
    return sa;
}

// Kasai's algorithm — O(N) LCP array
vector<int> buildLCP(const string& s, const vector<int>& sa) {
    int n = s.size();
    vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; ++i) rank_[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank_[i] > 0) {
            int j = sa[rank_[i] - 1];
            while (i+h < n && j+h < n && s[i+h] == s[j+h]) h++;
            lcp[rank_[i]] = h;
            if (h > 0) h--;
        }
    }
    return lcp;
}

// Number of distinct substrings = N*(N+1)/2 - sum(lcp)
long long distinctSubstrings(const string& s) {
    vector<int> sa = buildSA(s + "$");  // sentinel must be lexicographically smallest
    vector<int> lcp = buildLCP(s + "$", sa);
    int n = s.size();
    long long total = (long long)n*(n+1)/2;
    for (int x : lcp) total -= x;
    return total;
}
```

---

## 16.3 Suffix Automaton (SAM) — O(N) build

**Use for**: all distinct substrings, longest common substring of multiple strings, occurrences of every substring.

```cpp
struct SuffixAutomaton {
    struct State {
        int len, link;
        map<char,int> next;
        long long cnt;  // number of times this state's substrings appear
        State() : len(0), link(-1), cnt(0) {}
    };
    vector<State> st;
    int last;

    SuffixAutomaton() {
        st.push_back(State());
        last = 0;
    }

    void extend(char c) {
        int cur = st.size(); st.push_back(State());
        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;
        int p = last;
        while (p != -1 && !st[p].next.count(c)) { st[p].next[c] = cur; p = st[p].link; }
        if (p == -1) { st[cur].link = 0; }
        else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) { st[cur].link = q; }
            else {
                int clone = st.size(); st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].cnt = 0;
                while (p != -1 && st[p].next[c] == q) { st[p].next[c] = clone; p = st[p].link; }
                st[q].link = clone; st[cur].link = clone;
            }
        }
        last = cur;
    }

    // After building, propagate counts through suffix links (topological order by len)
    void computeCounts() {
        int n = st.size();
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b){ return st[a].len > st[b].len; });
        for (int v : order) if (st[v].link != -1) st[st[v].link].cnt += st[v].cnt;
    }

    // Number of distinct substrings
    long long distinctSubstrings() {
        long long ans = 0;
        for (int i = 1; i < (int)st.size(); ++i)
            ans += st[i].len - st[st[i].link].len;
        return ans;
    }
};
```

---

## 16.4 Palindromic Tree (Eertree) — O(N)

**Use for**: all distinct palindromic substrings, number of palindrome occurrences, palindrome factorizations.

```cpp
struct PalindromicTree {
    struct Node {
        map<char,int> children;
        int link, len, cnt;
        Node(int len, int link) : link(link), len(len), cnt(0) {}
    };
    vector<Node> nodes;
    string s;
    int last;

    PalindromicTree() {
        nodes.push_back(Node(-1, 0));  // root for odd-length palindromes (len=-1)
        nodes.push_back(Node(0,  0));  // root for even-length palindromes (len=0)
        last = 1;
    }

    int getSuffLink(int v, int i) {
        while (i - nodes[v].len - 1 < 0 || s[i - nodes[v].len - 1] != s[i]) v = nodes[v].link;
        return v;
    }

    void addChar(char c) {
        s += c;
        int i = s.size() - 1;
        int cur = getSuffLink(last, i);
        char ch = c;
        if (!nodes[cur].children.count(ch)) {
            int newLen = nodes[cur].len + 2;
            int newLink;
            if (newLen == 1) newLink = 1;  // single char: link to even root
            else { int tmp = getSuffLink(nodes[cur].link, i); newLink = nodes[tmp].children[ch]; }
            nodes.push_back(Node(newLen, newLink));
            nodes[cur].children[ch] = nodes.size() - 1;
        }
        last = nodes[cur].children[ch];
        nodes[last].cnt++;
    }

    // After adding all chars, propagate counts
    void computeCounts() {
        for (int i = nodes.size()-1; i >= 2; --i)
            nodes[nodes[i].link].cnt += nodes[i].cnt;
    }

    int numDistinctPalindromes() { return (int)nodes.size() - 2; }
};
```

---

## 16.5 Hashing: Anti-Hack Techniques

On Codeforces, adversarial tests can break `unordered_map` and polynomial hashing with fixed bases.

```cpp
// Custom hash for unordered_map (splitmix64)
struct SafeHash {
    size_t operator()(unsigned long long x) const {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
};
unordered_map<long long, int, SafeHash> safe_map;

// Random base hashing (to avoid anti-hash tests)
const long long MOD1 = (1LL<<61) - 1;  // Mersenne prime
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());
const long long BASE = uniform_int_distribution<long long>(1e9, 2e9)(rng64);

// Merging two (hash, len) pairs: useful for hashing concatenation
pair<long long,int> combine(pair<long long,int> a, pair<long long,int> b) {
    return {((__int128)a.first * /* base^b.second */ 1 + b.first) % MOD1, a.second + b.second};
}
```

---

## 16.6 String Algorithm Selection

```
Find all occurrences of one pattern?             → KMP or Z-function
Find all occurrences of MULTIPLE patterns?       → Aho-Corasick
Substring equality queries O(1)?                 → Double polynomial hashing
All distinct substrings, count occurrences?      → Suffix Automaton (SAM)
Longest common substring of two strings?         → SA + LCP or SAM of concatenation
Longest palindromic substring?                   → Manacher
ALL distinct palindromic substrings?             → Palindromic Tree (Eertree)
Sorted suffix order / lexicographic comparisons? → Suffix Array + LCP
```

---

## 16.7 Summary Table

| Algorithm | Build | Query | Main Use |
|-----------|-------|-------|----------|
| KMP | O(M) | O(N) | Single pattern search |
| Z-function | O(N) | — | Prefix matching, period |
| Aho-Corasick | O(ΣM) | O(N) | Multi-pattern search |
| Suffix Array + LCP | O(N log N) | O(log N) | Lexicographic suffix queries |
| Suffix Automaton | O(N) | O(N) | All substrings, occurrences |
| Palindromic Tree | O(N) | O(N) | All distinct palindromes |
| Manacher | O(N) | O(1) | Longest palindromic substring |
| Hashing (double) | O(N) | O(1) | Substring equality |

---

**Next**: [17 — Advanced Number Theory](17_advanced_number_theory.md)
