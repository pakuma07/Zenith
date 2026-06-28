// pattern_based.cpp
// Classic array patterns: two-pointer search and fixed-size sliding window.
#include <iostream>
#include <vector>
using namespace std;

// Two-pointer pair sum: does any pair in a SORTED array add up to target?
// Technique: converge two ends toward the middle. Time O(n), Space O(1).
bool hasPairSum(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        int s = a[l] + a[r];
        if (s == target) return true;
        if (s < target) l++; // need a larger sum -> advance left up
        else r--;            // need a smaller sum -> pull right down
    }
    return false;
}

// Maximum sum of any contiguous window of size k.
// Technique: fixed-size sliding window. Time O(n), Space O(1).
int maxSumK(const vector<int>& a, int k) {
    if ((int)a.size() < k) return 0;
    int sum = 0;
    for (int i = 0; i < k; ++i) sum += a[i]; // sum of the first window
    int best = sum;
    for (int i = k; i < (int)a.size(); ++i) {
        sum += a[i] - a[i - k]; // slide: add entering, drop leaving element
        best = max(best, sum);
    }
    return best;
}

int main() {
    vector<int> sorted = {1, 2, 3, 4, 7, 10};
    cout << "pair sum 11: " << hasPairSum(sorted, 11) << endl;

    vector<int> a = {2, 1, 5, 1, 3, 2};
    cout << "max sum window size 3: " << maxSumK(a, 3) << endl;
    return 0;
}
