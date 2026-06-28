# Hashing Complexity Table

## Concept

This reference compares the hashing structures from this chapter: separate chaining, open addressing (linear probing), and the standard `std::unordered_map`/`std::unordered_set`. All three give average O(1) search, insert, and delete by mapping keys to slots with a hash function, and all degrade to O(n) in the worst case when collisions pile up. The key differences are how they resolve collisions and how they behave under load. Chaining stores collisions in per-bucket lists and tolerates load factors above 1; open addressing keeps everything in one array (cache-friendly, compact) but must resize well before it fills and needs tombstones for deletion. Use this table to reason about the speed/memory trade-offs.

## Mermaid

```mermaid
flowchart TD
    ROOT["Collision resolution"]
    ROOT --> CHAIN["Separate chaining"]
    ROOT --> OPEN["Open addressing"]
    CHAIN --> CN["buckets -> linked lists; load factor can exceed 1"]
    OPEN --> ON["single array + probing; needs low load factor + tombstones"]
    CHAIN --> STD["std::unordered_map / set use chaining"]
```

## Complexity

| Structure / scheme        | Search (avg) | Insert (avg) | Delete (avg) | Worst case | Notes                                      |
|---------------------------|--------------|--------------|--------------|------------|--------------------------------------------|
| Separate chaining         | O(1)         | O(1)         | O(1)         | O(n)       | per-bucket lists; high load factor OK      |
| Open addressing (linear)  | O(1)         | O(1)         | O(1)         | O(n)       | one array; tombstones; resize before full  |
| std::unordered_map / set  | O(1)         | O(1)         | O(1)         | O(n)       | chaining internally; auto-rehash; C++11    |
| (compare) std::map / set  | O(log n)     | O(log n)     | O(log n)     | O(log n)   | balanced BST; ordered iteration            |

- Space: hashing structures O(n) plus bucket/slot overhead; load factor governs performance.

## C++11 Code

```cpp
// Choosing a hashing approach:
#include <unordered_map>
#include <map>
using namespace std;

void chooseHashing() {
    unordered_map<int,int> fast;   // average O(1), unordered  -> default choice
    map<int,int>           sorted; // O(log n) but ordered iteration / range queries
    (void)fast; (void)sorted;
    // Custom chaining  -> tolerate high load factor, simple deletion.
    // Custom open addr -> maximize cache locality, keep load factor low (<0.7).
}
```

## Mini Usage Example

```cpp
// "Need average O(1) lookups, order irrelevant?" -> std::unordered_map.
// "Need keys visited in sorted order / range scans?" -> std::map (O(log n)).
// "Building one from scratch, deletes common?" -> chaining (no tombstones).
```

## Code Snippet Flow

```mermaid
flowchart LR
    A{"Need ordered iteration?"} -- yes --> B["std::map / set (O(log n))"]
    A -- no --> C{"Use the STL?"}
    C -- yes --> D["std::unordered_map / set"]
    C -- no --> E{"Deletes frequent / high load?"}
    E -- yes --> F["chaining"]
    E -- no --> G["open addressing"]
```
