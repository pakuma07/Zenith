// Chapter 11: STL Algorithms - Runnable Examples
// Compile with: g++ -std=c++11 chapter11_stl_algorithms.cpp -o chapter11_stl_algorithms

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// ============================================================
// EXAMPLE 1: Find Algorithms
// ============================================================
void example1_find() {
    cout << "\n=== EXAMPLE 1: Find Algorithms ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // find
    auto it = find(v.begin(), v.end(), 5);
    if (it != v.end()) cout << "Found 5 at position " << distance(v.begin(), it) << endl;
    
    // find_if
    auto it2 = find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    cout << "First element > 5: " << *it2 << endl;
    
    // count
    int count = ::count(v.begin(), v.end(), 3);
    cout << "Count of 3: " << count << endl;
    
    // count_if
    int count_even = count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    cout << "Count of even numbers: " << count_even << endl;
}

// ============================================================
// EXAMPLE 2: Sort and Binary Search
// ============================================================
void example2_sort_search() {
    cout << "\n=== EXAMPLE 2: Sort and Binary Search ===" << endl;
    
    vector<int> v = {64, 34, 25, 12, 22, 11, 90};
    
    cout << "Original: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // sort ascending
    sort(v.begin(), v.end());
    cout << "Sorted ascending: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // sort descending
    sort(v.begin(), v.end(), greater<int>());
    cout << "Sorted descending: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // binary search (requires sorted)
    sort(v.begin(), v.end());
    bool found = binary_search(v.begin(), v.end(), 25);
    cout << "Binary search for 25: " << (found ? "Found" : "Not found") << endl;
}

// ============================================================
// EXAMPLE 3: Transform Algorithm
// ============================================================
void example3_transform() {
    cout << "\n=== EXAMPLE 3: Transform Algorithm ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    cout << "Original: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Transform in place
    transform(v.begin(), v.end(), v.begin(), [](int x) { return x * x; });
    cout << "After square: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Transform to new container
    vector<int> v2;
    transform(v.begin(), v.end(), back_inserter(v2), [](int x) { return x / 2; });
    cout << "Halved in new vector: ";
    for (int x : v2) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 4: Copy Algorithms
// ============================================================
void example4_copy() {
    cout << "\n=== EXAMPLE 4: Copy Algorithms ===" << endl;
    
    vector<int> src = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> dst;
    
    // copy
    copy(src.begin(), src.end(), back_inserter(dst));
    cout << "Copy all: ";
    for (int x : dst) cout << x << " ";
    cout << endl;
    
    // copy_if
    vector<int> even;
    copy_if(src.begin(), src.end(), back_inserter(even), [](int x) { return x % 2 == 0; });
    cout << "Copy even numbers: ";
    for (int x : even) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 5: For_each Algorithm
// ============================================================
void example5_for_each() {
    cout << "\n=== EXAMPLE 5: For_each Algorithm ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    cout << "Using for_each to print: ";
    for_each(v.begin(), v.end(), [](int x) { cout << x << " "; });
    cout << endl;
    
    cout << "Using for_each to sum: ";
    int sum = 0;
    for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
    cout << "Sum = " << sum << endl;
}

// ============================================================
// EXAMPLE 6: Accumulate (Numeric)
// ============================================================
void example6_accumulate() {
    cout << "\n=== EXAMPLE 6: Accumulate (Numeric) ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    // Sum
    int sum = accumulate(v.begin(), v.end(), 0);
    cout << "Sum: " << sum << endl;
    
    // Product
    int product = accumulate(v.begin(), v.end(), 1, [](int a, int b) { return a * b; });
    cout << "Product: " << product << endl;
}

// ============================================================
// EXAMPLE 7: Fill and Replace
// ============================================================
void example7_fill_replace() {
    cout << "\n=== EXAMPLE 7: Fill and Replace ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    
    cout << "Original: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // fill
    vector<int> v1 = v;
    fill(v1.begin(), v1.begin() + 3, 99);
    cout << "After fill first 3 with 99: ";
    for (int x : v1) cout << x << " ";
    cout << endl;
    
    // replace
    vector<int> v2 = v;
    replace(v2.begin(), v2.end(), 3, 999);
    cout << "After replace 3 with 999: ";
    for (int x : v2) cout << x << " ";
    cout << endl;
    
    // replace_if
    vector<int> v3 = v;
    replace_if(v3.begin(), v3.end(), [](int x) { return x % 2 == 0; }, 0);
    cout << "After replace even with 0: ";
    for (int x : v3) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 8: Reverse and Rotate
// ============================================================
void example8_reverse_rotate() {
    cout << "\n=== EXAMPLE 8: Reverse and Rotate ===" << endl;
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    cout << "Original: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // reverse
    reverse(v.begin(), v.end());
    cout << "Reversed: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // rotate
    rotate(v.begin(), v.begin() + 2, v.end());
    cout << "After rotate by 2: ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 9: Unique and Remove
// ============================================================
void example9_unique_remove() {
    cout << "\n=== EXAMPLE 9: Unique and Remove ===" << endl;
    
    // unique
    vector<int> v1 = {1, 1, 2, 2, 2, 3, 3, 4};
    auto new_end = unique(v1.begin(), v1.end());
    v1.erase(new_end, v1.end());
    cout << "After unique: ";
    for (int x : v1) cout << x << " ";
    cout << endl;
    
    // remove and erase
    vector<int> v2 = {1, 2, 3, 4, 5, 3, 6, 3, 7};
    auto removed = remove(v2.begin(), v2.end(), 3);
    v2.erase(removed, v2.end());
    cout << "After remove 3: ";
    for (int x : v2) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 10: Min/Max Elements
// ============================================================
void example10_min_max() {
    cout << "\n=== EXAMPLE 10: Min/Max Elements ===" << endl;
    
    vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    auto min_it = min_element(v.begin(), v.end());
    auto max_it = max_element(v.begin(), v.end());
    
    cout << "Min element: " << *min_it << endl;
    cout << "Max element: " << *max_it << endl;
    
    auto result = minmax_element(v.begin(), v.end());
    cout << "Min and max: " << *result.first << " and " << *result.second << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 11: STL ALGORITHMS (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_find();
    example2_sort_search();
    example3_transform();
    example4_copy();
    example5_for_each();
    example6_accumulate();
    example7_fill_replace();
    example8_reverse_rotate();
    example9_unique_remove();
    example10_min_max();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Find Algorithms**: Searching containers
   - find: locate specific value
   - find_if: locate with predicate
   - count: count occurrences

2. **Sorting**: Organize data
   - sort: ascending or custom order
   - binary_search: fast lookup in sorted

3. **Transform**: Apply function to elements
   - In-place or to new container
   - Common with lambdas

4. **Copy Algorithms**: Duplicate data
   - copy: all elements
   - copy_if: conditional copy
   - back_inserter for growing

5. **For_each**: Process each element
   - Good for side effects
   - Lambda captures for state

6. **Accumulate**: Reduce to single value
   - Sum, product, or custom
   - Initial value important

7. **Fill/Replace**: Modify values
   - fill: set range to value
   - replace: change specific value
   - replace_if: conditional

8. **Reverse/Rotate**: Reorder
   - reverse: flip order
   - rotate: circular shift

9. **Unique/Remove**: Eliminate duplicates
   - unique: adjacent duplicates
   - remove: logical removal
   - erase for actual removal

10. **Min/Max**: Find extremes
    - Single or pair lookup
    - Custom comparators
*/
