# Stack, Queue, and Deque

This chapter has been split into micro-files.

## Structure Files

1. [Stack](structures/stack.md)
2. [Queue](structures/queue.md)
3. [Deque](structures/deque.md)
4. [Linear Complexity Table](structures/linear_complexity_table.md)

## Practice

- ../exercises/stack_queue_deque_exercises.md

## C++11 Example: Queue with std::deque

```cpp
#include <deque>
using namespace std;

struct IntQueue {
    deque<int> d;
    void push(int x) { d.push_back(x); }
    void pop() { if (!d.empty()) d.pop_front(); }
    int front() const { return d.front(); }
};
```