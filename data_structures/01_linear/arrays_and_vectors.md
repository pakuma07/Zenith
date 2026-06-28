# Arrays and Vectors

This chapter has been split into micro-files.

## Structure Files

1. [Array](structures/array.md)
2. [Vector](structures/vector.md)
3. [Linear Complexity Table](structures/linear_complexity_table.md)

## Practice

- ../exercises/arrays_vectors_exercises.md

## C++11 Example: Dynamic Array Usage

```cpp
#include <vector>
using namespace std;

vector<int> buildArray(int n) {
    vector<int> a;
    for (int i = 0; i < n; ++i) a.push_back(i * i);
    return a;
}
```