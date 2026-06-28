// searching_sorting.cpp
// Core searching (linear, binary) and sorting (merge sort) algorithms.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Linear search: scan every element until the target is found.
// Technique: brute-force sequential scan. Time O(n), Space O(1).
int linearSearch(const vector<int>& a, int target) {
    for (int i = 0; i < (int)a.size(); ++i) if (a[i] == target) return i;
    return -1; // not present
}

// Binary search: locate target in a SORTED array by halving the search range.
// Technique: divide and conquer on indices. Time O(log n), Space O(1).
int binarySearch(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2; // avoids (l+r) integer overflow
        if (a[mid] == target) return mid;
        if (a[mid] < target) l = mid + 1; // target is in the right half
        else r = mid - 1;                 // target is in the left half
    }
    return -1;
}

// Merge step: combine two adjacent sorted runs a[l..m] and a[m+1..r] in place.
// Technique: two-pointer stable merge. Time O(r-l), Space O(r-l).
void mergeVec(vector<int>& a, int l, int m, int r) {
    vector<int> left(a.begin() + l, a.begin() + m + 1);     // copy left run
    vector<int> right(a.begin() + m + 1, a.begin() + r + 1); // copy right run
    int i = 0, j = 0, k = l;
    // Pick the smaller front element each step; <= keeps the sort stable.
    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i] <= right[j]) a[k++] = left[i++];
        else a[k++] = right[j++];
    }
    // Drain whichever run still has leftovers.
    while (i < (int)left.size()) a[k++] = left[i++];
    while (j < (int)right.size()) a[k++] = right[j++];
}

// Merge sort: recursively sort each half, then merge.
// Technique: divide and conquer. Time O(n log n), Space O(n).
void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;        // base case: 0 or 1 element is already sorted
    int m = l + (r - l) / 2;   // split point
    mergeSort(a, l, m);        // sort left half
    mergeSort(a, m + 1, r);    // sort right half
    mergeVec(a, l, m, r);      // merge the two sorted halves
}

int main() {
    vector<int> a = {7, 2, 9, 4, 1};
    cout << "linearSearch(9): " << linearSearch(a, 9) << endl;
    mergeSort(a, 0, (int)a.size() - 1);
    cout << "sorted: ";
    for (int x : a) cout << x << " ";
    cout << endl;
    cout << "binarySearch(4): " << binarySearch(a, 4) << endl;
    return 0;
}
