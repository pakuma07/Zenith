# Chapter 11: STL Algorithms

The STL algorithms in the `<algorithm>` and `<numeric>` headers are a collection of generic functions — searching, sorting, copying, transforming, counting, and more — that operate on ranges rather than on specific containers. They achieve this generality by working through *iterators*: a pair of iterators `[first, last)` describes a half-open range, and the same `sort` or `find` works on a `vector`, a `deque`, a raw array, or any other type that supplies the right kind of iterator. This separation of algorithms from containers, glued together by iterators, is the central design idea of the STL and lets a relatively small set of algorithms compose with every container.

Using these algorithms instead of hand-written loops makes code shorter, clearer, and less error-prone, and often faster since implementations are heavily optimized. Most algorithms accept a *predicate* or function object — frequently a lambda in C++11 — letting you customize behavior (e.g. `find_if`, `count_if`, custom sort comparators). This chapter walks through the major categories: iterators themselves, non-modifying and modifying algorithms, sorting, numeric operations, and the C++11 conveniences like range-based `for`, iterator adaptors, and the new `<numeric>` and `<algorithm>` additions. A key rule throughout: most algorithms work on iterator ranges and cannot change a container's *size*, so removal-style algorithms (like `unique` or `remove`) reorder elements and return a new logical end that you must pass to the container's `erase`.

## 11.1 Iterators

Iterators are objects that generalize pointers: they point at an element in a range and can be dereferenced with `*` and advanced with `++`, letting algorithms traverse any container uniformly. The STL defines a hierarchy of iterator *categories* with increasing capability — input/output, forward, bidirectional, and random-access — and each container supplies the strongest category it can support (a `vector` gives random-access iterators that support `+`, `-`, and `[]`, while a `list` gives only bidirectional ones). Beyond the ordinary `begin()`/`end()`, containers also offer `const_iterator` variants (`cbegin`/`cend`) for read-only access and `reverse_iterator` variants (`rbegin`/`rend`) that walk backward. The fundamental convention is the half-open range `[begin, end)`: `end()` points one past the last element and is never dereferenced — it serves as the "not found" sentinel returned by algorithms like `find`.

```cpp
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// Iterator types
vector<int>::iterator it = v.begin();           // Random access
vector<int>::const_iterator cit = v.cbegin();   // Const iterator
vector<int>::reverse_iterator rit = v.rbegin(); // Reverse iterator

// Dereference
cout << *it << endl;      // 1

// Increment
++it;
cout << *it << endl;      // 2

// Arithmetic
it += 2;
cout << *it << endl;      // 4

// Compare
if (it != v.end()) {
  cout << *it << endl;
}
```

---

## 11.2 Non-Modifying Algorithms

Non-modifying algorithms inspect a range without changing the elements within it — they search, count, compare, and test conditions but never reorder or alter values. Because they only read, they typically need just input or forward iterators and are safe to run on `const` ranges. Most of them are O(n) linear scans (`find`, `count`, `for_each`, the `*_of` family), with the notable exception of `binary_search` and friends, which run in O(log n) but *require the range to already be sorted*. Many of these algorithms come in a plain form and an `_if` form: the `_if` version takes a predicate (often a lambda) so you can match on a condition rather than an exact value.

### Search Algorithms

Search algorithms locate elements or count how many satisfy a condition. `find` returns an iterator to the first element equal to a value, while `find_if` returns the first element satisfying a predicate; both return `end()` if nothing matches, so always compare against `end()` before dereferencing. `count` and `count_if` tally matches in O(n). `binary_search` answers a yes/no membership question in O(log n) but only works correctly on a range that is already sorted by the same ordering — using it on unsorted data gives meaningless results. Use `distance(begin, it)` to convert a found iterator into a positional index.

```cpp
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 3, 2};

// find: first occurrence
auto it = find(v.begin(), v.end(), 3);
if (it != v.end()) {
  cout << "Found at position " << distance(v.begin(), it) << endl;
}

// find_if: first matching condition
auto it2 = find_if(v.begin(), v.end(), 
                   [](int x) { return x > 3; });

// count
int cnt = count(v.begin(), v.end(), 3);  // 2

// count_if
int cnt2 = count_if(v.begin(), v.end(), 
                    [](int x) { return x > 2; });

// binary_search (requires sorted container)
sort(v.begin(), v.end());
bool found = binary_search(v.begin(), v.end(), 4);
```

### Processing Algorithms

Processing algorithms apply a function to a range or test whether elements collectively meet a condition. `for_each` invokes a callable on every element in order — handy for printing or accumulating side effects without writing an explicit loop. The C++11 trio `all_of`, `any_of`, and `none_of` each take a predicate and return a `bool` answering, respectively, whether every element, at least one element, or no element satisfies it. They short-circuit (stopping as soon as the answer is determined) and, by convention, `all_of` returns `true` and `none_of` returns `true` for an empty range.

```cpp
// for_each
for_each(v.begin(), v.end(), 
         [](int x) { cout << x << " "; });

// all_of: all elements match condition
bool allPositive = all_of(v.begin(), v.end(),
                          [](int x) { return x > 0; });

// any_of: any element matches
bool hasNegative = any_of(v.begin(), v.end(),
                          [](int x) { return x < 0; });

// none_of: no elements match
bool noZeros = none_of(v.begin(), v.end(),
                       [](int x) { return x == 0; });
```

---

## 11.3 Modifying Algorithms

Modifying algorithms change the elements of a range — copying, transforming, filling, replacing, reversing, or removing duplicates. A critical rule is that algorithms operate on iterators and therefore *cannot resize a container*: "removing" algorithms like `unique` (and `remove`) work by shifting the kept elements toward the front and returning an iterator to the new logical end, leaving the tail untouched; you must then call the container's `erase(new_end, end())` to actually shrink it (the erase-remove idiom). When writing into a destination range you are responsible for ensuring it is large enough, or you use an inserter like `back_inserter` (see section 11.8) that grows the target as it goes. `transform` is especially common: it applies a function to each element and writes the results, optionally in place. Note `shuffle` is the C++11 replacement for the old `random_shuffle`.

```cpp
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// copy
vector<int> v2;
copy(v.begin(), v.end(), back_inserter(v2));

// copy_if
vector<int> evens;
copy_if(v.begin(), v.end(), back_inserter(evens),
        [](int x) { return x % 2 == 0; });

// transform: apply function
transform(v.begin(), v.end(), v.begin(),
          [](int x) { return x * 2; });

// fill: set all elements
fill(v.begin(), v.end(), 0);

// replace
replace(v.begin(), v.end(), 2, 20);

// reverse
reverse(v.begin(), v.end());

// rotate
rotate(v.begin(), v.begin() + 2, v.end());

// shuffle (C++11)
shuffle(v.begin(), v.end(), 
        default_random_engine(time(0)));

// unique (removes consecutive duplicates)
vector<int> dup = {1, 1, 2, 2, 3, 3};
auto new_end = unique(dup.begin(), dup.end());
dup.erase(new_end, dup.end());
```

---

## 11.4 Sorting Algorithms

Sorting algorithms arrange a range into order, defaulting to ascending via `operator<` but accepting any comparator — a function object like `greater<int>()` or a lambda — that defines a strict weak ordering. `sort` is the workhorse, running in O(n log n) and requiring random-access iterators (so it works on `vector`, `deque`, and arrays but not `list`, which has its own member `sort`). For partial needs there are cheaper specialized versions: `partial_sort` orders only the first *k* elements, and `nth_element` merely places the element that *would* sit at a given position there while partitioning the rest around it in average O(n) — ideal for finding medians or top-K without a full sort. `is_sorted` checks order in O(n). Note `sort` is not stable (equal elements may be reordered); use `stable_sort` when relative order of equal keys must be preserved.

```cpp
vector<int> v = {5, 2, 8, 1, 9};

// sort (ascending)
sort(v.begin(), v.end());

// sort (descending)
sort(v.begin(), v.end(), greater<int>());

// sort with custom comparator
sort(v.begin(), v.end(), 
     [](int a, int b) { return abs(a) < abs(b); });

// partial_sort (first n elements sorted)
partial_sort(v.begin(), v.begin() + 3, v.end());

// is_sorted
if (is_sorted(v.begin(), v.end())) {
  cout << "Vector is sorted" << endl;
}

// nth_element (partition around nth element)
nth_element(v.begin(), v.begin() + 2, v.end());
```

---

## 11.5 Numeric Algorithms

Numeric algorithms, found in the `<numeric>` header, perform running computations over a range rather than searching or reordering it. `accumulate` folds the range into a single value (summing by default, or applying any binary operation you supply) and is the standard way to total, multiply, or reduce a sequence. `partial_sum` produces a sequence of running totals (prefix sums), `adjacent_difference` writes the difference between each element and its predecessor, and `iota` fills a range with consecutive increasing values starting from a seed. These all take an output iterator for their results, so the destination must be large enough (or use an inserter). Watch the accumulator type: `accumulate(v.begin(), v.end(), 0)` accumulates in `int` even over a `double` range, so pass an initial value of the correct type (e.g. `0.0`) to avoid truncation.

```cpp
#include <numeric>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// accumulate (sum)
int sum = accumulate(v.begin(), v.end(), 0);

// accumulate with operation
int product = accumulate(v.begin(), v.end(), 1,
                         [](int a, int b) { return a * b; });

// adjacent_difference
vector<int> diff(v.size());
adjacent_difference(v.begin(), v.end(), diff.begin());
// diff = [1, 1, 1, 1, 1]

// partial_sum (cumulative sum)
vector<int> partial(v.size());
partial_sum(v.begin(), v.end(), partial.begin());
// partial = [1, 3, 6, 10, 15]

// iota (fill with incrementing values)
vector<int> seq(5);
iota(seq.begin(), seq.end(), 1);
// seq = [1, 2, 3, 4, 5]
```

---

## 11.6 Range-Based For (C++11)

The range-based `for` loop, new in C++11, iterates over every element of a container (or array, or anything with `begin()`/`end()`) without you managing iterators or indices manually, making loops shorter and harder to get wrong. The variable's declaration controls the access mode: `for (int x : v)` copies each element (read-only, with copy cost), `for (int& x : v)` binds a reference so you can modify elements in place, and `for (const auto& x : v)` reads each element by reference to avoid copies — the preferred form for expensive-to-copy types. It is purely syntactic sugar over the iterator pair, so the usual iterator-invalidation rules apply: do not insert into or erase from the container while ranging over it. Note that for a `map` each element is a `pair`, and in C++11 you access its members as `.first`/`.second` (the structured-binding `[key, value]` syntax is a later C++17 feature).

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// Read-only
for (int val : v) {
  cout << val << " ";
}

// Modify elements
for (int& val : v) {
  val *= 2;
}

// Use reference to avoid copy
for (const auto& val : v) {
  cout << val << " ";
}

// Works with maps
map<string, int> ages;
for (map<string, int>::const_iterator it = ages.begin(); it != ages.end(); ++it) {
  cout << it->first << ": " << it->second << endl;
}
```

---

## 11.7 Best Practices

This section collects practical guidance for writing idiomatic algorithm-based code. The overarching principle is to prefer a named standard algorithm over a hand-rolled loop: it states intent clearly, is less bug-prone, and is often better optimized — `count_if` reads more directly than a manual counter, and a lambda assigned to a well-named variable documents the condition it tests. When you need a multi-step transformation, compose several algorithms in sequence rather than fusing everything into one tangled loop. Finally, mind iterator invalidation when mutating: the canonical erase-while-iterating pattern reassigns the iterator from `erase()`'s return value (the next valid position) and only advances with `++it` when no erasure happened, since erasing invalidates the erased iterator.

```cpp
// ✅ Use appropriate algorithm
vector<int> v = {1, 2, 3};

// Instead of manual loop
// int count = 0;
// for (int x : v) {
//   if (x > 1) count++;
// }

// Use algorithm
int count = count_if(v.begin(), v.end(),
                     [](int x) { return x > 1; });

// ✅ Chain algorithms
vector<int> result;
v2 = v;
sort(v2.begin(), v2.end());
unique(v2.begin(), v2.end());
copy_if(v2.begin(), v2.end(), back_inserter(result),
        [](int x) { return x > 2; });

// ✅ Use lambda for clarity
auto isEven = [](int x) { return x % 2 == 0; };
auto evens = count_if(v.begin(), v.end(), isEven);

// ✅ Use iterators properly
// Standard pattern
for (auto it = v.begin(); it != v.end(); ++it) {
  if (condition(*it)) {
    it = v.erase(it);  // erase returns next iterator
  } else {
    ++it;
  }
}
```

---

## Summary

| Category | Algorithms |
|----------|-----------|
| **Search** | find, find_if, binary_search, count |
| **Processing** | for_each, all_of, any_of, none_of |
| **Modification** | copy, transform, fill, replace |
| **Sorting** | sort, partial_sort, nth_element |
| **Numeric** | accumulate, partial_sum, iota |
| **Utility** | reverse, rotate, shuffle, unique |

---

## 11.8 Iterator Adaptors

Iterator adaptors are special iterators that wrap containers or streams and adapt them for use with algorithms. They let you feed algorithm output directly into a container or stream without writing a loop.

```cpp
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <sstream>
using namespace std;

// --- back_insert_iterator / back_inserter ---
// Converts assign (*it = val) into push_back()
vector<int> src = {1, 2, 3, 4, 5};
vector<int> dst;
copy(src.begin(), src.end(), back_inserter(dst));
// dst = {1,2,3,4,5} — no pre-sizing needed

vector<int> evens;
copy_if(src.begin(), src.end(), back_inserter(evens),
        [](int x){ return x % 2 == 0; });
// evens = {2,4}

// --- front_insert_iterator / front_inserter ---
// Converts assign into push_front() — for list and deque
list<int> l;
copy(src.begin(), src.end(), front_inserter(l));
// l = {5,4,3,2,1} — reversed because push_front prepends

// --- insert_iterator / inserter ---
// Converts assign into insert() at a given position
set<int> s;
copy(src.begin(), src.end(), inserter(s, s.begin()));
// s = {1,2,3,4,5} — set auto-sorts and deduplicates

// --- ostream_iterator ---
// Converts assign into output stream write
ostream_iterator<int>    out_int(cout, " ");
ostream_iterator<string> out_str(cout, "\n");

copy(src.begin(), src.end(), out_int);   // prints: 1 2 3 4 5
cout << "\n";

vector<string> words = {"hello", "world"};
copy(words.begin(), words.end(), out_str); // prints each word on its own line

// Pipe transform straight to output — no intermediate container
transform(src.begin(), src.end(), out_int,
          [](int x){ return x * x; });   // prints: 1 4 9 16 25

// --- istream_iterator ---
// Reads from a stream, one whitespace-separated token at a time
istringstream iss("10 20 30 40 50");
istream_iterator<int> in_begin(iss), in_end;

vector<int> parsed(in_begin, in_end);   // fills vector from stream: {10,20,30,40,50}

// Sort input on-the-fly using istream + ostream iterators
istringstream iss2("5 3 1 4 2");
vector<int> nums(istream_iterator<int>(iss2), istream_iterator<int>());
sort(nums.begin(), nums.end());
copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); // 1 2 3 4 5
```

| Adaptor | Constructor | Effect |
|---------|-------------|--------|
| `back_inserter(c)` | container with `push_back` | Write calls `push_back` |
| `front_inserter(c)` | container with `push_front` | Write calls `push_front` |
| `inserter(c, pos)` | any container | Write calls `insert(pos, val)` |
| `ostream_iterator<T>(os, sep)` | any `ostream` | Write sends `os << val << sep` |
| `istream_iterator<T>(is)` | any `istream` | Read parses next `T` from stream |

**Why they matter:** They let you compose algorithms without allocating temporary containers and without writing explicit loops.

---

## Summary

| Category | Algorithms |
|----------|-----------|
| **Search** | find, find_if, binary_search, count |
| **Processing** | for_each, all_of, any_of, none_of |
| **Modification** | copy, transform, fill, replace |
| **Sorting** | sort, partial_sort, nth_element |
| **Numeric** | accumulate, partial_sum, iota |
| **Utility** | reverse, rotate, shuffle, unique |
| **Adaptors** | back_inserter, ostream_iterator, istream_iterator |

---

## 11.9 `<numeric>` Algorithms (C++11)

The `<numeric>` header provides algorithms for numerical operations on ranges.

```cpp
#include <numeric>
#include <vector>
#include <iostream>

std::vector<int> v = {1, 2, 3, 4, 5};

// iota: fill with sequentially increasing values
std::vector<int> idx(5);
std::iota(idx.begin(), idx.end(), 0);    // {0,1,2,3,4}
std::iota(idx.begin(), idx.end(), 10);   // {10,11,12,13,14}

// accumulate: fold left with an operation (default: addition)
int sum = std::accumulate(v.begin(), v.end(), 0);         // 15
int product = std::accumulate(v.begin(), v.end(), 1,
                              std::multiplies<int>());    // 120
// Custom accumulate: build a string
std::string s = std::accumulate(v.begin(), v.end(), std::string(),
    [](std::string acc, int x){ return acc + std::to_string(x) + " "; });
// s = "1 2 3 4 5 "

// inner_product: dot product of two ranges
std::vector<int> a = {1,2,3}, b = {4,5,6};
int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0); // 1*4+2*5+3*6=32

// Custom inner_product: count matching positions
int matches = std::inner_product(a.begin(), a.end(), b.begin(), 0,
    std::plus<int>(),
    [](int x, int y){ return x == y ? 1 : 0; }); // 0 matches here

// partial_sum: prefix sums (or prefix products, etc.)
std::vector<int> ps(5);
std::partial_sum(v.begin(), v.end(), ps.begin()); // {1,3,6,10,15}
std::partial_sum(v.begin(), v.end(), ps.begin(),
                 std::multiplies<int>());           // {1,2,6,24,120}

// adjacent_difference: differences between consecutive elements
std::vector<int> diff(5);
std::adjacent_difference(v.begin(), v.end(), diff.begin()); // {1,1,1,1,1}
// Custom: ratios
std::vector<double> vd = {1.0, 2.0, 4.0, 8.0};
std::vector<double> ratios(4);
std::adjacent_difference(vd.begin(), vd.end(), ratios.begin(),
    [](double cur, double prev){ return cur / prev; }); // {1,2,2,2}
```

**Summary table:**

| Algorithm | Description | Default op |
|-----------|-------------|------------|
| `iota(first, last, val)` | Fill with val, val+1, val+2... | N/A |
| `accumulate(first, last, init)` | Fold over range | `+` |
| `inner_product(f1,l1, f2, init)` | Sum of pairwise products | `+`, `*` |
| `partial_sum(first, last, out)` | Prefix sums | `+` |
| `adjacent_difference(first, last, out)` | Differences a[i]-a[i-1] | `-` |

---

## 11.10 C++11 Algorithm Additions

C++11 added several convenience algorithms and the free `begin`/`end` family that make generic code cleaner and enable element-by-element moving.

### `std::begin` / `std::end` and the `c`/`r` variants

The free functions `std::begin(c)` / `std::end(c)` work uniformly on **both** STL containers **and raw arrays**, so generic code no longer needs `c.begin()`.

```cpp
#include <iterator>   // std::begin, std::end
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int raw[] = {5, 2, 8, 1};
sort(begin(raw), end(raw));            // works on a raw array — no size needed!
for (int x : raw) cout << x << " ";    // 1 2 5 8

vector<int> v = {3, 1, 2};
sort(begin(v), end(v));                // also works on containers

// const and reverse variants (cbegin/cend added as free functions in C++14,
// but the member forms v.cbegin()/v.crbegin() are available in C++11):
for (auto it = v.cbegin(); it != v.cend(); ++it) cout << *it;   // read-only
for (auto it = v.rbegin(); it != v.rend(); ++it) cout << *it;   // reverse
```

### `std::move` algorithm — move a range instead of copying

Not to be confused with `std::move(x)` the cast. The **algorithm** `std::move(first, last, dest)` moves each element from a source range into a destination, leaving the sources in a valid-but-empty state.

```cpp
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

vector<string> src = {"alpha", "beta", "gamma"};
vector<string> dst(3);

move(src.begin(), src.end(), dst.begin());   // each string's buffer is moved
// dst == {"alpha","beta","gamma"}; src elements are now empty strings

// move_backward: same idea, copying from the end (for overlapping ranges)
move_backward(dst.begin(), dst.end(), dst.end());
```

### `min` / `max` / `minmax` with an `initializer_list`

C++11 lets `min`/`max` take a braced list of more than two values, and adds `minmax` which returns both at once as a `pair`.

```cpp
#include <algorithm>
#include <utility>
using namespace std;

int lo = min({4, 2, 9, 1, 7});        // 1  — any number of args
int hi = max({4, 2, 9, 1, 7});        // 9

pair<int,int> mm = minmax({4, 2, 9, 1, 7});   // {1, 9}
cout << mm.first << " " << mm.second; // 1 9

// minmax_element: get iterators to the smallest AND largest in one pass
vector<int> v = {4, 2, 9, 1, 7};
auto pr = minmax_element(v.begin(), v.end());
cout << *pr.first << " " << *pr.second;   // 1 9

// is_partitioned / partition_point and is_sorted_until are also new in C++11
```

| Addition | Purpose |
|---|---|
| `begin(c)` / `end(c)` | Free functions; work on containers **and** raw arrays |
| `cbegin/cend`, `rbegin/rend` (members) | Const and reverse iteration |
| `move(first, last, dest)` | Move-assign a range (vs copy) |
| `min/max({...})` | Smallest/largest of an initializer list |
| `minmax({...})`, `minmax_element(f,l)` | Both extremes in one call/pass |

---

## Next Steps
- Practice algorithms with containers
- Combine multiple algorithms
- Use lambdas with algorithms
- Move to [Chapter 12: Memory Management](../12_memory_management/README.md)
