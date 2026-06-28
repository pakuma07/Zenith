# Exercises: Heap

## Easy

1. Build min-heap from array.
2. Insert and extract-min.
3. Convert min-heap to max-heap.
4. Check if array satisfies heap property.
5. Kth largest using heap.

## Medium

1. Heap sort implementation.
2. Merge k sorted arrays.
3. Running median with two heaps.
4. Top-k frequent numbers.
5. Reorganize string with max-heap.

## Hard

1. Design indexed priority queue.
2. Dijkstra with binary heap.
3. Sliding window median.
4. Merge k sorted linked lists.
5. Min cost to connect ropes.

## Challenge

Implement custom binary heap class with decrease-key.

## C++11 Exercise Example: Min-Heap Top K

```cpp
#include <queue>
#include <vector>
using namespace std;

vector<int> kSmallest(const vector<int>& a, int k) {
    priority_queue<int> pq;
    for (int x : a) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();
    }
    vector<int> out;
    while (!pq.empty()) { out.push_back(pq.top()); pq.pop(); }
    return out;
}
```
