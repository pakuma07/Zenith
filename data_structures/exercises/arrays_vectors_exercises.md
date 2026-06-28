# Exercises: Arrays and Vectors

## Easy

1. Find maximum and minimum element in an array.
2. Reverse an array in-place.
3. Rotate array right by k positions.
4. Remove duplicates from a sorted vector.
5. Merge two sorted arrays.

## Medium

1. Find second largest element in one pass.
2. Implement dynamic array resize logic manually.
3. Compute prefix sums and answer range sum queries.
4. Rearrange array by sign (positive/negative alternating).
5. Find majority element (Boyer-Moore).

## Hard

1. Maximum subarray sum (Kadane + index reconstruction).
2. Count inversions in O(n log n).
3. Trapping rain water.
4. Product of array except self without division.
5. Longest consecutive sequence.

## Challenge

Implement a mini-vector class with:
- push_back
- pop_back
- reserve
- operator[]
- size/capacity

## C++11 Exercise Example: Prefix Sum Queries

```cpp
#include <vector>
using namespace std;

vector<long long> buildPref(const vector<int>& a) {
    vector<long long> pref(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); ++i) pref[i + 1] = pref[i] + a[i];
    return pref;
}
```
