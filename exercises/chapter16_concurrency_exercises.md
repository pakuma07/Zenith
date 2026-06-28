# Chapter 16: Concurrency - Exercises

## Section 1: Thread Basics 🟢

1. Launch a thread that prints a greeting.
2. Pass an integer argument into a worker thread.
3. Create two threads and join both correctly.

## Section 2: Shared State and Mutex 🟡

4. Protect a shared counter with `mutex`.
5. Compare unsafe incrementing vs locked incrementing.
6. Use `lock_guard` to protect vector `push_back`.

## Section 3: `unique_lock` and Lifecycle 🟡

7. Lock and unlock a mutex manually with `unique_lock`.
8. Transfer ownership of a `thread` object using `move`.
9. Detect whether a thread is still joinable.

## Section 4: Condition Variables 🟡

10. Build a one-item producer/consumer example.
11. Wait with a predicate to avoid spurious wakeups.
12. Extend the queue-based consumer to process multiple jobs.

## Section 5: Atomics 🟡

13. Replace a mutex counter with `atomic<int>`.
14. Build a stop flag using `atomic<bool>`.
15. Explain when atomics are not enough for correctness.

## Section 6: Futures and Async 🔴

16. Return a computed value using `async`.
17. Capture exceptions thrown inside `async` through `future::get()`.
18. Compare `launch::async` and `launch::deferred` behavior.

## Section 7: Promise and Packaged Task 🔴

19. Use `promise<int>` to send a result between threads.
20. Use `promise<string>` for status reporting.
21. Wrap a sum function inside `packaged_task`.

## Section 8: Deadlock Awareness 🔴

22. Create a two-mutex deadlock example on paper and explain why it hangs.
23. Fix the deadlock by locking mutexes in a consistent order.
24. Use `std::lock` with `adopt_lock` to acquire two mutexes safely.

## Section 9: Design and Best Practices 🔴

25. Refactor a shared-state design to reduce lock scope.
26. Move I/O outside the critical section in a threaded program.
27. Explain why dangling references in thread lambdas are dangerous.

## Section 10: Integration Challenge 🏆

28. Build a thread-safe work queue with producer and consumer threads.
29. Add an atomic shutdown flag to the work queue.
30. Return final statistics through a `future`.

---

## Tips for Success

- Prefer `join()` unless you truly need `detach()`.
- Use `lock_guard` by default; use `unique_lock` when required.
- Always protect shared mutable state consistently.
- Use predicates with condition-variable waits.
- Keep ownership and lifetime rules explicit.

## Common Pitfalls

- Forgetting to join a thread before destruction.
- Capturing references to variables that go out of scope.
- Holding locks across slow operations.
- Mixing atomics and mutex-protected state carelessly.
- Assuming wakeups only happen when notified.

## Learning Outcomes

After these exercises, you should be able to:
- Launch and manage threads safely
- Protect shared state with mutexes and RAII
- Coordinate threads with condition variables
- Use atomics for simple shared flags and counters
- Return results through futures, promises, and packaged tasks
- Recognize and avoid common deadlock patterns

## C++11 Exercise Example: Atomic Stop Flag

```cpp
#include <atomic>
using namespace std;

atomic<bool> stopRequested(false);

void requestStop() {
    stopRequested.store(true);
}
```
