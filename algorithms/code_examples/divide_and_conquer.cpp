// divide_and_conquer.cpp
// Maximum-subarray sum solved by divide and conquer.
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Best subarray sum that MUST straddle the midpoint m (uses both halves).
// Technique: grow a run leftward from m and rightward from m+1. Time O(r-l), Space O(1).
int maxCrossing(const vector<int>& a, int l, int m, int r) {
    int leftBest = INT_MIN, sum = 0;
    for (int i = m; i >= l; --i) {     // extend left from the center
        sum += a[i];
        leftBest = max(leftBest, sum);
    }
    int rightBest = INT_MIN;
    sum = 0;
    for (int i = m + 1; i <= r; ++i) { // extend right from the center
        sum += a[i];
        rightBest = max(rightBest, sum);
    }
    return leftBest + rightBest;       // both halves are non-empty by construction
}

// Maximum subarray sum: answer is the best of left half, right half, or crossing.
// Technique: divide and conquer. Time O(n log n), Space O(log n) recursion depth.
int maxSubarrayDC(const vector<int>& a, int l, int r) {
    if (l == r) return a[l];           // base case: single element
    int m = l + (r - l) / 2;
    return max(max(maxSubarrayDC(a, l, m), maxSubarrayDC(a, m + 1, r)),
               maxCrossing(a, l, m, r));
}

int main() {
    vector<int> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << "max subarray = " << maxSubarrayDC(a, 0, (int)a.size() - 1) << endl;
    return 0;
}
