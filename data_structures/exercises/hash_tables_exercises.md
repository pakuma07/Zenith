# Exercises: Hash Tables

## Easy

1. Count frequency of elements.
2. Check if two arrays are equal as multisets.
3. Find first unique character in a string.
4. Group duplicate words with counts.
5. Check anagram using hashing.

## Medium

1. Two-sum and three-sum variations.
2. Longest substring without repeating characters.
3. Top-k frequent elements.
4. Subarray with sum k.
5. Longest consecutive sequence.

## Hard

1. Design hashmap from scratch (chaining).
2. Design hashmap from scratch (open addressing).
3. LFU cache.
4. Distinct count in every window of size k.
5. Find all pairs with given xor.

## Challenge

Benchmark custom hashmap vs unordered_map on large random input.

## C++11 Exercise Example: Two Sum

```cpp
#include <unordered_map>
#include <vector>
using namespace std;

pair<int,int> twoSum(const vector<int>& a, int target) {
    unordered_map<int,int> pos;
    for (int i = 0; i < (int)a.size(); ++i) {
        int need = target - a[i];
        if (pos.count(need)) return {pos[need], i};
        pos[a[i]] = i;
    }
    return {-1, -1};
}
```
