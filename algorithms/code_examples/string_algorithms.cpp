// string_algorithms.cpp
// String matching prerequisite: the KMP longest-prefix-suffix (failure) table.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Builds the KMP LPS table: lps[i] = length of the longest proper prefix of
// p[0..i] that is also a suffix of it. Time O(m), space O(m).
vector<int> buildLPS(const string& p) {
    vector<int> lps(p.size(), 0);
    // i = index being filled; len = length of current matching prefix.
    for (int i = 1, len = 0; i < (int)p.size();) {
        if (p[i] == p[len]) lps[i++] = ++len;   // extend match: prefix grows by 1
        else if (len) len = lps[len - 1];        // mismatch: fall back to shorter border
        else lps[i++] = 0;                       // no border possible at this position
    }
    return lps;
}

int main() {
    string p = "ababcabab";
    vector<int> lps = buildLPS(p);
    for (int x : lps) cout << x << " ";
    cout << endl;
    return 0;
}
