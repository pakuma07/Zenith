# Greedy Algorithms

This chapter has been split into micro-files.

## Structure Files

1. [Greedy-Choice Property](structures/greedy_choice_property.md)
2. [Activity Selection](structures/activity_selection.md)
3. [Greedy Patterns](structures/greedy_patterns.md)

## Practice

- ../exercises/greedy_exercises.md

---

## Next Steps

- Review the cheat sheet: [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md)
- Previous: [Chapter 5: Divide and Conquer](../05_divide_and_conquer/divide_and_conquer.md)
- Next: [Chapter 7: Dynamic Programming](../07_dynamic_programming/dynamic_programming.md)

## C++11 Example: Activity Selection

```cpp
#include <vector>
#include <algorithm>
using namespace std;

int maxActivities(vector<pair<int,int> > a) {
    sort(a.begin(), a.end(), [](const pair<int,int>& x, const pair<int,int>& y) { return x.second < y.second; });
    int ans = 0, endT = -1;
    for (const auto& it : a) if (it.first >= endT) { ++ans; endT = it.second; }
    return ans;
}
```