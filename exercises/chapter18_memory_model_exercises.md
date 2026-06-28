# Chapter 18: Memory Model and std::memory_order — Exercises

Compile all exercises with:
```bash
g++ -std=c++11 -Wall -Wextra -pthread your_file.cpp -o out
```

---

## Section 1: Data Races and Atomics 🟢

1. Write a program where two threads simultaneously increment a plain `int` counter 100,000 times each. Observe the incorrect final value. Then fix it with `atomic<int>`.
2. Replace the following with an atomic alternative:
   ```cpp
   volatile bool done = false;
   // Thread A: done = true;
   // Thread B: while (!done) {}
   ```
   Use `atomic<bool>` with appropriate `memory_order`.
3. Write a thread-safe reference counter class `RefCount` using `atomic<int>` with `fetch_add` and `fetch_sub`. Add a method `isUnique()` that returns true when count == 1.
4. Implement a thread-safe `AtomicMax(atomic<int>& var, int candidate)` using a CAS loop: atomically sets `var = max(var, candidate)`.

---

## Section 2: memory_order Basics 🟢

5. Write a producer/consumer pair using `memory_order_release` / `memory_order_acquire` to publish a `struct Message { int id; double value; }` from one thread to another.
6. Modify exercise 5 to use `memory_order_seq_cst` (the default). Measure (or reason about) any difference in correctness.
7. Write a thread-safe lazy initializer using `atomic<bool>` with release/acquire:
   ```cpp
   atomic<bool> initialized(false);
   Data* data = nullptr;
   void init();          // called once from any thread
   Data* get();          // returns initialized data
   ```
8. Write a `SpinLock` class using `atomic<bool>` with `test_and_set` / `clear`. Use `memory_order_acquire` on lock and `memory_order_release` on unlock. Test with 4 threads sharing a counter.

---

## Section 3: Acquire/Release Patterns 🟡

9. Implement a single-producer single-consumer (SPSC) queue using `atomic<int>` for head and tail indices, with acquire/release ordering. Capacity = 16 ints.
10. Write a **lock-free flag** that:
    - Can be set by any thread (once)
    - Can be tested by any thread without locking
    - Uses `memory_order_release` on set and `memory_order_acquire` on test
    - Guarantees that all memory written before `set()` is visible after `test()` returns true
11. Design a `ReadyFlag<T>` class template:
    ```cpp
    ReadyFlag<int> rf;
    rf.publish(42);           // thread A
    int v = rf.wait();        // thread B — blocks until published, returns 42
    ```
    Use acquire/release internally (no mutex).

---

## Section 4: Relaxed Ordering 🟡

12. Write a benchmarking counter that is incremented by 8 threads using `memory_order_relaxed`. Verify the final count is correct. Add a note explaining why relaxed is safe here.
13. Implement a `statistics` struct updated by workers:
    ```cpp
    atomic<long long> totalOps;
    atomic<long long> totalTime;
    ```
    Use `memory_order_relaxed` for increments. Use `memory_order_seq_cst` for the final read. Explain the difference.
14. Write a program that demonstrates why you cannot use relaxed ordering for the producer/consumer pattern in exercise 5 (write a comment explaining what could go wrong).

---

## Section 5: Fences 🟡

15. Rewrite exercise 5 (producer/consumer) using `atomic_thread_fence` with `memory_order_release` / `memory_order_acquire` instead of per-operation ordering. Use a `relaxed` store/load on the flag itself.
16. Write a function `publish_all(vars[], n, flag)` that uses a single release fence to make `n` non-atomic variables visible to a reader who does an acquire fence then reads the flag.
17. Explain (in code comments) why a fence covers all preceding stores while a per-operation `release` only covers the ordering relative to that specific atomic variable.

---

## Section 6: Compare-and-Swap 🔴

18. Implement `atomic_min(atomic<int>& a, int b)` using `compare_exchange_weak` in a loop. Verify it works correctly with 8 threads each contributing a random value.
19. Write a lock-free `AtomicQueue<int>` (MPMC — multiple producer, multiple consumer) using a CAS loop on an internal linked list. Support `push` and `pop`.
20. Implement a `TreiberStack<T>` (the classic lock-free stack from Section 18.8) and test it with 4 pusher threads and 4 popper threads, each doing 1000 operations. Assert all pushes are eventually popped.
21. Write a `HazardPointer` stub that prevents the ABA problem: before accessing a node's `next`, the thread registers the node pointer as "in use", preventing deallocation. Outline the design (full implementation is a research topic).

---

## Section 7: thread_local and call_once 🟡

22. Write a program where each of 4 threads accumulates a running sum in a `thread_local` variable. At the end, collect all sums using a mutex-protected global vector and print the total.
23. Implement thread-safe `Logger::instance()` using `call_once` + `once_flag`. Verify that the constructor runs exactly once even when 8 threads call `instance()` simultaneously.
24. Write a per-thread random engine:
    ```cpp
    mt19937& getEngine();   // returns this thread's mt19937, seeded once
    ```
    Use `thread_local` so each thread gets an independent, properly seeded engine.
25. Implement `runOnce(func)` — a function that takes a callable and guarantees it is executed exactly once across all calls from any thread, using `call_once` internally.

---

## Section 8: Lock-Free Data Structures 🔴

26. Implement a lock-free `AtomicCounter` supporting `increment()`, `decrement()`, `get()` using only `atomic<int>` and `fetch_add`.
27. Write a lock-free single-linked stack with a **counted pointer** (combine pointer + counter in a single `atomic<uint64_t>`) to mitigate the ABA problem on 64-bit platforms.
28. Design and implement a lock-free bitset of 64 bits using `atomic<uint64_t>`. Provide `set(bit)`, `clear(bit)`, `test(bit)`, and `flip(bit)` — all thread-safe.
29. Write a `SpinBarrier` for N threads: all threads must call `arrive()` before any can proceed. Use `atomic<int>` and a spin-wait loop.
30. Build a thread-safe `EventBus` where publishers call `publish(event)` and subscribers register handlers. Use `atomic` for the count of registered handlers (no mutex for the count).

---

## Integration Challenges 🏆

**Challenge 1 — Sequence Lock (seqlock):**  
Implement a `SeqLock<T>` that allows many concurrent readers and one writer:
- Writer increments a sequence counter (odd = writing), updates data, increments again (even = done)
- Reader reads sequence, copies data, reads sequence again — if unchanged and even, data is valid
- Use `atomic<int>` for the sequence counter with appropriate memory orders
- Verify correctness with 1 writer and 4 readers

**Challenge 2 — Lock-Free Ring Buffer (SPSC):**  
Write a single-producer single-consumer ring buffer of capacity 256. Use `atomic<size_t>` for head and tail with `memory_order_release` / `memory_order_acquire`. Ensure zero blocking under non-full/non-empty conditions.

**Challenge 3 — Memory Model Quiz:**  
Without running the code, predict the possible output of each snippet:
```cpp
// Snippet A: two atomics, relaxed, two threads
// Snippet B: message passing, wrong pairing (write to a, read from b)
// Snippet C: seq_cst store, seq_cst load, different threads
```
Then run them and verify your predictions.

---

## Key Concepts Checklist

```
✓ Data race = UB; use atomic<T> for shared mutable data
✓ seq_cst: total global order; default; safest
✓ acquire/release: paired; establishes happens-before across threads
✓ relaxed: atomicity only; for counters with no side-effect ordering
✓ fence: covers a group of variables; decoupled from specific atomic
✓ CAS loop (compare_exchange_weak): foundation of lock-free structures
✓ thread_local: per-thread independent storage
✓ call_once: exactly-once initialization across threads
✓ volatile: NOT a threading primitive; does not prevent reordering
✓ happens-before: the formal relation that makes concurrent programs correct
```

---

## Expected Difficulty

- **Easy (🟢)**: 15-25 min each — 7 exercises
- **Medium (🟡)**: 30-50 min each — 12 exercises
- **Hard (🔴)**: 60-120 min each — 8 exercises
- **Challenges (🏆)**: 90-180 min — 3 exercises
