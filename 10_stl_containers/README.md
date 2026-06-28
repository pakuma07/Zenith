# Chapter 10: STL Containers

The Standard Template Library (STL) containers are generic, type-safe data structures that manage collections of objects for you, handling memory allocation, growth, and cleanup automatically. They fall into three broad families: *sequence containers* (`vector`, `list`, `deque`, `array`, `forward_list`) that store elements in a linear order, *associative containers* (`map`, `set`, `multimap`, `multiset`) that keep elements sorted in a balanced tree for fast lookup, and *unordered associative containers* (`unordered_map`, `unordered_set`, and their `multi` variants) that use hashing for average O(1) access. Container *adaptors* (`stack`, `queue`, `priority_queue`) restrict an underlying container to a specific access pattern.

Choosing the right container is mostly about matching the access pattern of your problem to the performance guarantees of the container: whether you need random access, fast insertion in the middle, sorted iteration, or constant-time key lookup. This chapter surveys each container with its core operations and complexity characteristics, then covers the C++11 additions (`array`, `forward_list`, the unordered families, `emplace`, `tuple`) and important practical topics like iterator invalidation. Keep the comparison tables in sections 10.9 and the Summary handy as a quick reference when deciding which one to reach for.

## 10.1 Vector

`std::vector` is a dynamically resizable array and is the default container you should reach for unless you have a specific reason not to. Its elements live in a single contiguous block of memory, which gives O(1) random access via `[]` and excellent cache performance when iterating. Appending with `push_back` is amortized O(1), but inserting or erasing anywhere except the end is O(n) because following elements must shift. The catch is reallocation: when the vector outgrows its `capacity` it allocates a larger buffer and moves everything, which invalidates all existing iterators, pointers, and references — call `reserve()` up front when you know the final size. Prefer `at()` over `[]` when you want a bounds check (it throws `std::out_of_range`).

```cpp
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5};

// Access
v[0];                     // 1
v.at(2);                  // 3 (with bounds check)
v.front();                // 1
v.back();                 // 5

// Size
v.size();                 // 5
v.capacity();             // >= 5
v.empty();                // false

// Modification
v.push_back(6);           // Add to end
v.pop_back();             // Remove from end
v.insert(v.begin() + 2, 99);  // Insert 99 at index 2
v.erase(v.begin() + 1);   // Erase at index 1
v.clear();                // Remove all

// Iteration
for (int x : v) cout << x;
for (auto it = v.begin(); it != v.end(); ++it) cout << *it;
```

## 10.2 List

`std::list` is a doubly linked list: each element sits in its own node holding a value plus pointers to the previous and next nodes. Because nodes are not contiguous it has no random access (no `operator[]`), and searching is O(n), but insertion and deletion at a known position are O(1) — no elements are shifted, only a couple of pointers are rewired. Crucially, iterators, pointers, and references to other elements stay valid across insertions and erasures (only the erased element's iterator is invalidated), making it the safest container when you must hold iterators while mutating. Choose `list` when you do many splices or middle insertions/removals and rarely need indexed access; otherwise a `vector` is usually faster in practice due to cache locality.

```cpp
#include <list>
using namespace std;

list<int> l = {1, 2, 3, 4, 5};

// Cannot random access
// l[0];  // ERROR

// Forward iteration
l.push_back(6);           // Add to end
l.pop_back();             // Remove from end
l.push_front(0);          // Add to front
l.pop_front();            // Remove from front

// Efficient insertion/deletion
l.insert(l.begin(), 99);  // Insert
l.erase(l.begin());       // Erase

// Iteration
for (int x : l) cout << x;
```

## 10.3 Deque

`std::deque` (double-ended queue) supports fast O(1) insertion and removal at *both* the front and the back, which `vector` cannot offer at the front. It also provides O(1) random access via `[]` and `at()`, but unlike `vector` its storage is not a single contiguous block — it is built from several fixed-size chunks, so element addresses are not guaranteed contiguous. Use a `deque` when you need queue-like behavior with growth at both ends, or as the default underlying container for `stack` and `queue`. Note that any insertion at the ends may invalidate all iterators (though references to existing elements survive front/back growth), so do not rely on iterator stability the way you would with a `list`.

```cpp
#include <deque>
using namespace std;

deque<int> dq = {1, 2, 3};

// Efficient at both ends
dq.push_front(0);         // Add to front
dq.push_back(4);          // Add to back
dq.pop_front();           // Remove from front
dq.pop_back();            // Remove from back

// Random access
cout << dq[0];
cout << dq.at(2);
```

## 10.4 Map

`std::map` is an ordered associative container that stores unique key-value pairs sorted by key, implemented as a balanced binary search tree (typically a red-black tree). Insertion, lookup, and erasure are all O(log n), and iterating walks the keys in sorted order. The `operator[]` is convenient but has a subtle trap: accessing a missing key *inserts* a default-constructed value rather than failing, so use `find()` (which returns `end()` when absent) when you only want to read. Iterators yield `std::pair<const Key, Value>`, so you access the key via `it->first` and the value via `it->second`. Reach for `map` when you need keys kept in sorted order or range queries; if you only need fast lookup and don't care about order, prefer `unordered_map`.

```cpp
#include <map>
using namespace std;

map<string, int> ages;

// Insert
ages["Alice"] = 25;
ages["Bob"] = 30;
ages.insert({"Charlie", 35});

// Access
cout << ages["Alice"];    // 25

// Find
auto it = ages.find("Bob");
if (it != ages.end()) {
  cout << it->second;    // 30
}

// Remove
ages.erase("Bob");

// Iterate (sorted by key)
for (map<string, int>::const_iterator it = ages.begin(); it != ages.end(); ++it) {
  cout << it->first << ": " << it->second << endl;
}
```

## 10.5 Unordered_map (C++11)

`std::unordered_map`, introduced in C++11, stores unique key-value pairs in a hash table rather than a tree. This gives average O(1) insertion, lookup, and erasure — typically faster than `map`'s O(log n) — at the cost of *no* ordering when iterating. Worst-case operations degrade to O(n) if many keys hash to the same bucket, and inserting may trigger a *rehash* that invalidates all iterators (though references to elements remain valid); calling `reserve()` ahead of time avoids repeated rehashing. The key type must be hashable, which is built in for the standard types but requires a custom hash functor for your own types. Choose `unordered_map` for pure lookup speed; choose `map` when you need sorted traversal or ordered range queries.

```cpp
#include <unordered_map>
using namespace std;

unordered_map<string, int> map;

// Same operations as map, but unordered
map["key1"] = 10;
map["key2"] = 20;

// Faster average access: O(1) vs map's O(log n)
// But not sorted

for (auto& [key, value] : map) {
  cout << key << ": " << value << endl;
}
```

## 10.6 Set

`std::set` is an ordered associative container holding *unique* elements (no duplicates), kept sorted, and backed by a balanced binary search tree. Insertion, lookup via `find`/`count`, and erasure are each O(log n), and iteration visits elements in sorted order. Because each element acts as its own key, the stored values are effectively `const` — you cannot modify an element in place in a way that would change its sort position; erase and re-insert instead. Use `set` when you need a collection of unique items maintained in order or want to test membership while iterating in sorted sequence; if ordering is irrelevant and you want faster average lookup, prefer `unordered_set`.

```cpp
#include <set>
using namespace std;

set<int> s = {3, 1, 4, 1, 5};  // Duplicates ignored

// Size
s.size();                 // 4 (1 duplicate removed)

// Insert
s.insert(2);

// Find
auto it = s.find(3);
if (it != s.end()) {
  cout << *it << endl;
}

// Iteration (sorted)
for (int x : s) {
  cout << x << " ";      // 1 2 3 4 5
}

// Remove
s.erase(2);
```

## 10.7 Queue & Stack

`std::queue` and `std::stack` are *container adaptors*: they wrap an underlying container (a `deque` by default) and expose only a restricted interface enforcing a specific access discipline. A `queue` is FIFO (first-in, first-out) — you `push` to the back and `pop`/`front` from the front, like a line of people. A `stack` is LIFO (last-in, first-out) — you `push` and `pop`/`top` from the same end, like a stack of plates. Both offer O(1) push and pop and deliberately provide no iterators or random access, so you cannot traverse them; if you need to inspect all elements, use the underlying container directly. Use them to express intent clearly when the algorithm genuinely requires only stack or queue semantics (e.g. BFS uses a queue, DFS uses a stack).

```cpp
#include <queue>
#include <stack>
using namespace std;

// Queue (FIFO)
queue<int> q;
q.push(1);
q.push(2);
q.push(3);
cout << q.front();       // 1
q.pop();                 // Remove 1
cout << q.front();       // 2

// Stack (LIFO)
stack<int> st;
st.push(1);
st.push(2);
st.push(3);
cout << st.top();        // 3
st.pop();                // Remove 3
cout << st.top();        // 2
```

## 10.8 Unordered_set (C++11)

`std::unordered_set`, added in C++11, holds unique elements in a hash table, giving average O(1) insertion, lookup (`count`/`find`), and erasure — the hashed counterpart to `set`. Iteration order is unspecified and may change after a rehash, so never rely on it. As with `unordered_map`, worst-case operations are O(n) under heavy hash collisions, inserts can trigger a rehash that invalidates iterators (references stay valid), and `reserve()` helps when the size is known in advance. It is the go-to choice for fast membership testing — "have I seen this value before?" — when sorted order does not matter; use `set` instead when you need ordered traversal.

```cpp
#include <unordered_set>
using namespace std;

unordered_set<int> s = {3, 1, 4, 1, 5};

s.insert(2);
s.erase(3);

// Find: O(1) average vs set's O(log n)
if (s.count(4) > 0) {
  cout << "Found" << endl;
}

// Iteration (unordered)
for (int x : s) {
  cout << x << " ";
}
```

---

## 10.9 Container Comparison

The table below summarizes the asymptotic complexity and ordering guarantees of every container in this chapter so you can pick one at a glance. Read it by first asking what operation dominates your workload — random access, middle insertion, search, or ordered iteration — then choosing the container whose cheapest column matches it. Remember that these are asymptotic *Big-O* figures: a `vector` with its contiguous memory often beats node-based containers in real timings even where the Big-O looks worse, thanks to cache locality, so treat the table as a starting point rather than the final word.

| Container | Random Access | Insert/Delete | Search | Order |
|-----------|---------------|---------------|--------|-------|
| **Vector** | O(1) | O(n) | O(n) | Insertion |
| **List** | O(n) | O(1) | O(n) | Insertion |
| **Deque** | O(1) | O(n) | O(n) | Insertion |
| **Map** | - | O(log n) | O(log n) | Sorted key |
| **Set** | - | O(log n) | O(log n) | Sorted |
| **Unordered_map** | - | O(1) avg | O(1) avg | Unordered |
| **Unordered_set** | - | O(1) avg | O(1) avg | Unordered |
| **Queue** | - | O(1) | - | FIFO |
| **Stack** | - | O(1) | - | LIFO |
| **Array** | O(1) | - | O(n) | Insertion (fixed) |
| **Priority_queue** | - | O(log n) | - | Max-heap |
| **Multiset** | - | O(log n) | O(log n) | Sorted (dup OK) |
| **Multimap** | - | O(log n) | O(log n) | Sorted key (dup OK) |

---

## 10.10 std::array (C++11)

`std::array` is a fixed-size, stack-allocated array. Unlike raw arrays it knows its own size, supports iterator-based algorithms, and provides bounds-checked access.

```cpp
#include <array>
#include <algorithm>
using namespace std;

// Declaration & initialization
array<int, 5> arr = {10, 20, 30, 40, 50};

// Element access
cout << arr[0];          // 10 (no bounds check)
cout << arr.at(2);       // 30 (throws out_of_range if index invalid)
cout << arr.front();     // 10
cout << arr.back();      // 50

// Size is a compile-time constant
cout << arr.size();      // 5

// Fill all elements with a value
arr.fill(0);             // All elements become 0

// Fully compatible with STL algorithms
array<int, 5> nums = {5, 3, 1, 4, 2};
sort(nums.begin(), nums.end());
for (int x : nums) {
  cout << x << " ";     // 1 2 3 4 5
}
```

| Feature | Raw array | `std::array` | `std::vector` |
|---------|-----------|--------------|---------------|
| Knows its own size | No (decays to pointer) | Yes — `.size()` | Yes — `.size()` |
| Bounds-checked access | No | Yes — `.at()` | Yes — `.at()` |
| Heap allocation | No | No | Yes |
| Resize at runtime | No | No | Yes |
| STL algorithm compatible | Partial | Full | Full |

---

## 10.11 priority_queue

`priority_queue` is a heap-based container adaptor. By default it is a **max-heap** (largest element on top).

```cpp
#include <queue>
using namespace std;

// Max-heap (default) — largest on top
priority_queue<int> maxpq;
maxpq.push(3);
maxpq.push(1);
maxpq.push(4);
maxpq.push(5);

cout << maxpq.top();    // 5 (largest)
maxpq.pop();            // Remove 5
cout << maxpq.top();    // 4

// Min-heap — smallest on top
// Note: space before > > required in C++11 to avoid >> token
priority_queue<int, vector<int>, greater<int> > minpq;
minpq.push(3);
minpq.push(1);
minpq.push(4);

cout << minpq.top();    // 1 (smallest)
minpq.pop();
cout << minpq.top();    // 3

// Custom comparator for structs
struct Task {
  int priority;
  string name;
};

struct TaskCmp {
  bool operator()(const Task& a, const Task& b) {
    return a.priority < b.priority;  // larger priority value = higher priority
  }
};

priority_queue<Task, vector<Task>, TaskCmp> taskQueue;
taskQueue.push({3, "Low priority task"});
taskQueue.push({10, "Urgent task"});
taskQueue.push({5, "Medium task"});

cout << taskQueue.top().name;  // "Urgent task"
```

| Operation | Complexity |
|-----------|-----------|
| `push(x)` | O(log n) |
| `top()` | O(1) |
| `pop()` | O(log n) |
| `empty()` / `size()` | O(1) |

**Common uses:** Dijkstra's algorithm, scheduling, top-K elements.

---

## 10.12 multiset and multimap

Unlike `set` and `map`, the `multi` variants **allow duplicate keys**.

```cpp
#include <set>
#include <map>
using namespace std;

// --- multiset: sorted set that allows duplicates ---
multiset<int> ms = {3, 1, 4, 1, 5, 9, 2, 6, 5};
// Stored sorted, duplicates retained: 1 1 2 3 4 5 5 6 9

// Count occurrences
cout << ms.count(5);    // 2
cout << ms.count(7);    // 0

// Insert
ms.insert(1);           // Now three 1s in the set

// Erase ONE occurrence (erase by iterator)
multiset<int>::iterator it = ms.find(5);
if (it != ms.end()) {
  ms.erase(it);         // Removes exactly one 5
}

// Erase ALL occurrences of a value
ms.erase(1);            // Removes all 1s

// Iterate a range of equal elements
pair<multiset<int>::iterator, multiset<int>::iterator> range = ms.equal_range(6);
for (multiset<int>::iterator r = range.first; r != range.second; ++r) {
  cout << *r << " ";
}

// --- multimap: key-value map that allows duplicate keys ---
multimap<string, int> mm;
mm.insert(make_pair("alice", 90));
mm.insert(make_pair("alice", 85));  // duplicate key — both entries stored
mm.insert(make_pair("bob",   72));

cout << mm.count("alice");   // 2

// Iterate all values for one key using equal_range
pair<multimap<string,int>::iterator, multimap<string,int>::iterator> r2 = mm.equal_range("alice");
for (multimap<string,int>::iterator it2 = r2.first; it2 != r2.second; ++it2) {
  cout << it2->second << " ";  // 90 85
}
```

| Container | Duplicate keys | Order | Typical use |
|-----------|---------------|-------|-------------|
| `set` | No | Sorted | Unique sorted elements |
| `multiset` | Yes | Sorted | Frequency count, sorted bag |
| `map` | No | Sorted by key | Unique key-value pairs |
| `multimap` | Yes | Sorted by key | Grouping, one-to-many mapping |

---

## 10.13 std::pair and std::tuple (C++11)

`std::pair` holds exactly two values; `std::tuple` holds any number of values of potentially different types. Both are in `<utility>` / `<tuple>`.

```cpp
#include <utility>
#include <tuple>
#include <map>
#include <iostream>
using namespace std;

// --- std::pair ---
pair<string, int> person = make_pair("Alice", 30);

cout << person.first;   // Alice
cout << person.second;  // 30

// Modify in-place
person.second = 31;

// Pairs are returned by map iterators
map<string, int> scores;
scores["Alice"] = 95;
scores["Bob"]   = 82;
for (map<string,int>::iterator it = scores.begin(); it != scores.end(); ++it) {
  // it->first = key (string), it->second = value (int)
  cout << it->first << ": " << it->second << endl;
}

// Pairs compare lexicographically
pair<int,int> p1 = make_pair(1, 5);
pair<int,int> p2 = make_pair(1, 8);
cout << (p1 < p2);  // true (same first, 5 < 8)

// --- std::tuple ---
tuple<string, int, double> rec = make_tuple("Bob", 25, 4.0);

// Access by compile-time index
cout << get<0>(rec);  // Bob
cout << get<1>(rec);  // 25
cout << get<2>(rec);  // 4.0

// Modify via get
get<1>(rec) = 26;

// tie: unpack all values into named variables
string name;
int    age;
double gpa;
tie(name, age, gpa) = rec;
cout << name << " age=" << age;  // Bob age=26

// Ignore a field during unpacking
tie(name, ignore, gpa) = rec;   // skip age

// Query size at compile time
cout << tuple_size<tuple<int, string, double> >::value;  // 3

// Return multiple values from a function
tuple<bool, int, string> parseInput(const string& s) {
  if (s.empty())
    return make_tuple(false, 0, string("empty"));
  return make_tuple(true, static_cast<int>(s.size()), s);
}

bool   ok;
int    len;
string val;
tie(ok, len, val) = parseInput("hello");
cout << ok << " " << len << " " << val;  // 1 5 hello
```

| | `pair` | `tuple` |
|---|---|---|
| Elements | Exactly 2 | Any number |
| Access | `.first` / `.second` | `get<N>(t)` |
| Unpack | Direct assign or `tie` | `tie(...)` |
| Comparison | Lexicographic | Lexicographic |
| Common use | Map entries, return 2 values | Return 3+ values, heterogeneous records |

---

## Summary

| Container | Best For |
|-----------|----------|
| **Vector** | General purpose, random access |
| **List** | Frequent insertions/deletions |
| **Deque** | Add/remove both ends |
| **Map** | Key-value pairs, sorted |
| **Set** | Unique elements, sorted |
| **Unordered_map** | Key-value pairs, fast lookup |
| **Unordered_set** | Unique elements, fast lookup |
| **Queue** | FIFO operations |
| **Stack** | LIFO operations |
| **Array** | Fixed-size, stack-allocated, STL-compatible |
| **Priority_queue** | Heap-ordered access (max/min), top-K problems |
| **Multiset** | Sorted bag allowing duplicate elements |
| **Multimap** | One-to-many sorted key mapping |
| **pair** | Two heterogeneous values; returned by map iterators |
| **tuple** | N heterogeneous values; returning multiple values |
| **forward_list** | Singly linked list, minimal memory, forward-only |
| **unordered_multiset/map** | Hashed duplicates, O(1) average lookup |

---

## 10.14 Iterator Invalidation Rules

Every STL container has rules about which operations **invalidate** existing iterators, pointers, and references. Using an invalidated iterator is undefined behaviour — a frequent source of hard-to-debug crashes.

```cpp
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <iostream>
using namespace std;

// --- vector ---
vector<int> v = {1, 2, 3, 4, 5};
vector<int>::iterator it = v.begin() + 2;  // points to 3
cout << *it;  // 3

v.push_back(6);            // MAY reallocate — it is now INVALID if realloc happened
// cout << *it;            // UNDEFINED BEHAVIOUR

// Safe pattern: reserve first to prevent reallocation
vector<int> v2;
v2.reserve(10);            // reserve before filling
vector<int>::iterator it2 = v2.end();
v2.push_back(1);           // no realloc (capacity was reserved) — it2 stays valid
// BUT: it2 was end() before push_back; it still points to old end, not new element

// erase invalidates at-and-after the erased position
vector<int> v3 = {1, 2, 3, 4};
vector<int>::iterator it3 = v3.begin() + 1;  // points to 2
it3 = v3.erase(it3);    // correct: erase returns next valid iterator
cout << *it3;           // 3

// --- list ---
list<int> l = {1, 2, 3, 4};
list<int>::iterator lit = l.begin();
l.push_back(5);   // list never reallocates — lit remains valid
l.erase(lit);     // only the erased iterator is invalidated, all others survive
// cout << *lit;  // INVALID: lit was erased

// --- deque ---
deque<int> d = {1, 2, 3};
deque<int>::iterator dit = d.begin() + 1;
d.push_back(4);   // invalidates ALL iterators (may reorganise memory)
d.push_front(0);  // invalidates ALL iterators
// dit is now INVALID

// --- map / set ---
map<int, string> m;
m[1] = "one"; m[2] = "two";
map<int,string>::iterator mit = m.find(1);
m[3] = "three";  // insertion does NOT invalidate existing iterators
m.erase(2);      // erases only the pointed-to iterator; mit (pointing to 1) is valid
cout << mit->second;  // "one" — still valid
```

| Container | insert / push | erase | Notes |
|-----------|--------------|-------|-------|
| `vector` | Invalidates ALL if reallocation; else only iterators ≥ insert point | All iterators ≥ erase point | Use `reserve()` to prevent reallocation |
| `deque` | Invalidates ALL | All iterators ≥ erase point (or ALL if at front/back) | Avoid holding iterators across inserts |
| `list` | None | Only erased iterator | Safest for iterator stability |
| `map` / `set` | None (tree rebalancing doesn't move nodes) | Only erased iterator | Very stable |
| `unordered_map` | Invalidates ALL on rehash | Only erased iterator | Use `reserve()` to prevent rehash |

**Golden rules:**
1. After any mutating operation on `vector` or `deque`, assume all iterators are invalid unless you know no reallocation occurred.
2. Always use the return value of `erase()` — it returns the next valid iterator.
3. Never erase elements in a range-based for loop over a vector; use an index loop with `erase()` instead.

---

## 10.15 `std::tie` and `std::ignore` (C++11)

`std::tie` creates a tuple of lvalue references, enabling structured assignment
from a `std::tuple` or a function returning multiple values.

```cpp
#include <tuple>
#include <string>
#include <iostream>

// Unpacking a tuple into existing variables
int x; double y; std::string name;
std::tie(x, y, name) = std::make_tuple(42, 3.14, "Alice");
std::cout << x << " " << y << " " << name << "\n";  // 42 3.14 Alice

// std::ignore: skip elements you don't need
std::tie(x, std::ignore, name) = std::make_tuple(99, 2.71, "Bob");
std::cout << x << " " << name << "\n";  // 99 Bob  (y unchanged)

// Idiomatic use: unpack std::pair (returned by map::insert etc.)
std::map<std::string, int> m;
bool inserted;
std::map<std::string,int>::iterator it;
std::tie(it, inserted) = m.insert({"key", 1});
std::cout << "Inserted: " << inserted << "\n";  // 1

// Lexicographic comparison using tie (common trick for structs)
struct Point { int x, y, z; };
bool operator<(const Point& a, const Point& b) {
    return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z);
    // equivalent to: x<b.x || (x==b.x && (y<b.y || (y==b.y && z<b.z)))
}

// Sort array of structs lexicographically:
// std::sort(pts.begin(), pts.end());
// works automatically once operator< uses tie.
```

**`std::tie` for comparison is the idiomatic C++11 way to compare structs
without writing multi-level boolean expressions by hand.**

---

## 10.16 emplace and emplace_back (C++11)

`push_back`/`insert` take an **already-constructed** object and then copy or move it into the container. The `emplace` family instead **constructs the element in place**, forwarding its arguments directly to the element's constructor — saving a temporary and a move/copy.

```cpp
#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) { cout << "construct\n"; }
  Point(const Point& o) : x(o.x), y(o.y) { cout << "copy\n"; }
};

vector<Point> pts;

// push_back: build a temporary Point, then move/copy it in
pts.push_back(Point(1, 2));      // construct + (move/copy)

// emplace_back: forward args, construct ONCE directly inside the vector
pts.emplace_back(3, 4);          // construct only — no temporary

// emplace at an arbitrary position
pts.emplace(pts.begin(), 0, 0);  // builds Point(0,0) in place at the front

// emplace into associative containers — args go to the value_type constructor
map<string, Point> m;
m.emplace("origin", Point(0, 0));         // construct value in place
m.emplace(piecewise_construct,            // construct BOTH key and value in place
          forward_as_tuple("p1"),
          forward_as_tuple(5, 6));

// Other emplace members:
//   vector/deque/list: emplace_back, emplace_front (deque/list), emplace
//   map/set:           emplace, emplace_hint
//   queue/stack:       emplace
```

| Call | Behaviour |
|---|---|
| `v.push_back(T(args))` | Construct temporary, then move/copy into container |
| `v.emplace_back(args)` | Forward `args`, construct element in place (no temporary) |

**Use `emplace_back` when** you would otherwise construct a temporary just to insert it. For an already-existing object, `push_back(obj)` (or `push_back(std::move(obj))`) is equally good. (Emplace relies on perfect forwarding — see [Chapter 9.14](../09_templates/README.md).)

---

## 10.17 std::forward_list (C++11)

`std::forward_list` is a **singly linked list** — each node points only to the next. It is the most memory-efficient node-based container (one pointer per node instead of two) but supports only forward traversal.

```cpp
#include <forward_list>
#include <iostream>
using namespace std;

forward_list<int> fl = {1, 2, 3, 4};

// Only front operations are O(1); there is NO push_back and NO size()
fl.push_front(0);          // {0,1,2,3,4}
fl.pop_front();            // {1,2,3,4}

// Insertion/erasure happen AFTER a given position ("_after" methods)
forward_list<int>::iterator it = fl.begin();
fl.insert_after(it, 99);   // {1,99,2,3,4}
fl.erase_after(it);        // removes the element after 'it' -> {1,2,3,4}

// before_begin(): a special iterator for inserting at the very front
fl.insert_after(fl.before_begin(), -1);  // {-1,1,2,3,4}

for (int x : fl) cout << x << " ";   // -1 1 2 3 4
```

| | `list` (doubly linked) | `forward_list` (singly linked) |
|---|---|---|
| Memory per node | 2 pointers | 1 pointer |
| Traversal | Forward & backward | Forward only |
| `size()` | Yes | **No** (would cost storage) |
| `push_back` | Yes | **No** — front + `*_after` only |
| Use when | General linked list | Memory-critical, forward-only |

---

## 10.18 unordered_multimap and unordered_multiset (C++11)

Like `multimap`/`multiset`, these allow **duplicate keys**, but they are **hash-based** (unordered, O(1) average) rather than tree-based (sorted, O(log n)).

```cpp
#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;

// --- unordered_multiset: hashed bag allowing duplicates ---
unordered_multiset<int> ums = {1, 2, 2, 3, 3, 3};
cout << ums.count(3);          // 3
ums.insert(2);                 // duplicates allowed

// --- unordered_multimap: hashed one-to-many mapping ---
unordered_multimap<string, int> umm;
umm.insert({"alice", 90});
umm.insert({"alice", 85});     // duplicate key kept
umm.insert({"bob",   72});

cout << umm.count("alice");    // 2

// All values for one key via equal_range (order within a key is unspecified)
auto range = umm.equal_range("alice");
for (auto it = range.first; it != range.second; ++it) {
  cout << it->second << " ";   // 90 85 (in some order)
}
```

| Container | Duplicates | Ordering | Lookup |
|---|---|---|---|
| `unordered_set` / `unordered_map` | No | Unordered (hashed) | O(1) average |
| `unordered_multiset` / `unordered_multimap` | Yes | Unordered (hashed) | O(1) average |
| `multiset` / `multimap` | Yes | Sorted | O(log n) |

---

## Next Steps
- Choose appropriate containers
- Use iterators for manipulation
- Combine containers as needed
- Move to [Chapter 11: STL Algorithms](../11_stl_algorithms/README.md)
