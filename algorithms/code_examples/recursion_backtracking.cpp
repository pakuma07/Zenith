// recursion_backtracking.cpp
// Recursion (factorial) and backtracking (enumerating all subsets).
#include <iostream>
#include <vector>
using namespace std;

// Factorial via recursion: n! = n * (n-1)!.
// Technique: linear recursion. Time O(n), Space O(n) call stack.
long long fact(int n) {
    if (n <= 1) return 1;            // base case 0! = 1! = 1
    return 1LL * n * fact(n - 1);    // 1LL forces 64-bit multiply
}

// Enumerate all subsets of a via DFS: at each index, either skip or include it.
// Technique: backtracking over a binary choice tree. Time O(n * 2^n), Space O(n) depth.
void dfsSubsets(int idx, const vector<int>& a, vector<int>& cur, vector<vector<int>>& out) {
    if (idx == (int)a.size()) {      // reached a leaf: one complete subset
        out.push_back(cur);
        return;
    }
    dfsSubsets(idx + 1, a, cur, out); // choice 1: exclude a[idx]
    cur.push_back(a[idx]);            // choice 2: include a[idx]
    dfsSubsets(idx + 1, a, cur, out);
    cur.pop_back();                   // backtrack: undo the include
}

int main() {
    cout << "fact(5) = " << fact(5) << endl;
    vector<int> a = {1, 2, 3};
    vector<int> cur;
    vector<vector<int>> subsets;
    dfsSubsets(0, a, cur, subsets);
    cout << "subsets count = " << subsets.size() << endl;
    return 0;
}
