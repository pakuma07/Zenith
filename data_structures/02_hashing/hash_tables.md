# Hash Tables

This chapter has been split into micro-files.

## Structure Files

1. [Hash Table: Separate Chaining](structures/hash_table_chaining.md)
2. [Hash Table: Open Addressing](structures/hash_table_open_addressing.md)
3. [unordered_map and unordered_set](structures/unordered_map_unordered_set.md)
4. [Hashing Complexity Table](structures/hashing_complexity_table.md)

## Practice

- ../exercises/hash_tables_exercises.md

## C++11 Example: Frequency Map

```cpp
#include <unordered_map>
#include <vector>
using namespace std;

unordered_map<int,int> freqCount(const vector<int>& a) {
    unordered_map<int,int> freq;
    for (int x : a) ++freq[x];
    return freq;
}
```