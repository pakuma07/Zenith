# Chapter 18: C++ Memory Model and std::memory_order

The **C++ memory model** is a formal specification of how reads and writes to shared variables interact across threads. Without understanding it, even `std::atomic` operations can silently produce incorrect concurrent programs. This chapter covers everything from *C++ Concurrency in Action* Chapter 5 — the expert-level counterpart to Chapter 16's practical primitives.

---

## 18.1 Why the Memory Model Matters

Modern CPUs and compilers can reorder memory operations for performance. This is transparent in single-threaded programs but **visible across threads**.

```
Thread A                Thread B
--------                --------
x = 1;                  if (ready) {
ready = true;             // sees x == 0 ???
                        }
```

Without explicit ordering, the compiler or CPU may reorder `x = 1` and `ready = true`. Thread B can see `ready == true` while still reading `x == 0`. This is undefined behaviour in C++ — a **data race**.

The C++ memory model defines:
1. What constitutes a data race (and that data races are undefined behaviour)
2. How `std::atomic` operations create ordering guarantees
3. The **happens-before** relation that makes concurrent programs correct

```cpp
#include <atomic>
#include <thread>
#include <iostream>
using namespace std;

// ❌ Data race — undefined behaviour (not fixed by volatile)
int shared = 0;  // NOT atomic

void writerBad() { shared = 42; }
void readerBad() { cout << shared; }  // may see 0, 42, or garbage

// ✅ Data race eliminated — atomic guarantees no torn reads/writes
atomic<int> safeShared(0);

void writerGood() { safeShared.store(42); }
void readerGood() { cout << safeShared.load(); }
```

---

## 18.2 The memory_order Values

`std::memory_order` controls what ordering guarantees accompany an atomic operation. It is an enum with six values.

```cpp
#include <atomic>
using namespace std;

// The six memory_order values:
// memory_order_relaxed    — no ordering guarantees beyond atomicity
// memory_order_consume    — (rarely used, deprecated in practice)
// memory_order_acquire    — this load sees all stores before the paired release
// memory_order_release    — all stores before this are visible to a paired acquire
// memory_order_acq_rel    — combines acquire + release (for read-modify-write)
// memory_order_seq_cst    — sequentially consistent: total global order (default)

atomic<int> x(0);

// Default (no memory_order argument) = memory_order_seq_cst
x.store(1);               // seq_cst store
int v = x.load();         // seq_cst load

// Explicit memory_order
x.store(1, memory_order_relaxed);
x.store(1, memory_order_release);
v = x.load(memory_order_relaxed);
v = x.load(memory_order_acquire);
v = x.exchange(2, memory_order_acq_rel);    // read-modify-write
bool ok = x.compare_exchange_weak(v, 3, memory_order_acq_rel, memory_order_acquire);
```

| `memory_order` | Strength | Cost (approx.) | Use case |
|---|---|---|---|
| `relaxed` | Weakest | Lowest | Counters where only the final value matters |
| `acquire` | Medium | Medium | Load that starts a "critical section" |
| `release` | Medium | Medium | Store that ends a "critical section" |
| `acq_rel` | Medium | Medium | Read-modify-write (fetch_add, CAS) |
| `seq_cst` | Strongest | Highest | Default; reasoning is simplest |

---

## 18.3 Sequentially Consistent Operations (seq_cst)

`memory_order_seq_cst` is the default and the strongest guarantee. All `seq_cst` operations appear to execute in a **single total order** that is the same from every thread's perspective.

```cpp
#include <atomic>
#include <thread>
#include <cassert>
using namespace std;

atomic<bool> x(false), y(false);
atomic<int>  z(0);

// Thread 1
void write_x()  { x.store(true); }   // seq_cst by default

// Thread 2
void write_y()  { y.store(true); }

// Thread 3
void read_x_then_y() {
  while (!x.load()) {}   // spin until x is true
  if (y.load())          // is y also true?
    ++z;
}

// Thread 4
void read_y_then_x() {
  while (!y.load()) {}   // spin until y is true
  if (x.load())          // is x also true?
    ++z;
}

// With seq_cst: guaranteed that at least one of thread 3 or thread 4 sees
// both flags set — z must be at least 1 after all threads complete.
// This guarantee does NOT hold with weaker memory orders.

thread t1(write_x), t2(write_y), t3(read_x_then_y), t4(read_y_then_x);
t1.join(); t2.join(); t3.join(); t4.join();
assert(z.load() != 0);  // always passes with seq_cst
cout << "z = " << z.load() << "\n";  // 1 or 2
```

**When to use seq_cst:** Whenever you are not sure which order to use. It is the most expensive but easiest to reason about. Profile before switching to a weaker order.

---

## 18.4 Acquire/Release Semantics

The acquire/release pair is the building block of most lock-free algorithms. A **release store** publishes data; an **acquire load** subscribes to it.

```
Thread A (writer)                 Thread B (reader)
-----------------                 ----------------
data = 42;                        while (!ready.load(acquire)) {}
ready.store(true, release);  -->  assert(data == 42);  // guaranteed!
```

The release fence on A's store ensures that all stores **before** the release are visible to any thread that sees the release via an acquire load.

```cpp
#include <atomic>
#include <thread>
#include <iostream>
using namespace std;

atomic<bool> ready(false);
int          payload = 0;      // NOT atomic — protected by the release/acquire

void producer() {
  payload = 99;                              // must happen before the release
  ready.store(true, memory_order_release);   // publish: "payload is ready"
}

void consumer() {
  while (!ready.load(memory_order_acquire)) {}  // subscribe: spin until published
  // happens-before: everything before the release store is visible here
  cout << payload << "\n";   // guaranteed to print 99
}

thread t1(producer), t2(consumer);
t1.join(); t2.join();

// --- Message passing pattern: generalised ---
struct Message {
  int  id;
  char text[64];
};

atomic<Message*> msgPtr(nullptr);

void sender() {
  Message* m = new Message();
  m->id = 1;
  m->text[0] = 'H'; m->text[1] = '\0';
  // release store: all writes to *m are visible after acquire load sees non-null
  msgPtr.store(m, memory_order_release);
}

void receiver() {
  Message* m = nullptr;
  while ((m = msgPtr.load(memory_order_acquire)) == nullptr) {}
  cout << "Got message id=" << m->id << " text=" << m->text << "\n";
  delete m;
}

thread s(sender), r(receiver);
s.join(); r.join();
```

**Key rule:** Pair every `memory_order_release` store with a `memory_order_acquire` load on the same atomic variable in the consumer thread.

---

## 18.5 The happens-before Relationship

**happens-before** is the formal ordering relation that determines which memory operations are guaranteed to be visible.

```
Operation A  happens-before  Operation B
means: B is guaranteed to see the effect of A.
```

```cpp
#include <atomic>
#include <thread>
#include <iostream>
using namespace std;

// happens-before rules:
//
// 1. Sequenced-before (within a single thread):
//    Statement A executes before statement B in source order →
//    A happens-before B (for that thread)
//
// 2. Synchronizes-with (across threads via atomics):
//    release-store(x) in thread A synchronizes-with acquire-load(x) in thread B
//    → A's release-store happens-before B's acquire-load
//
// 3. Transitivity:
//    If A hb B and B hb C, then A hb C
//
// 4. No happens-before → no guarantee:
//    Two operations without an hb path between them may be seen in any order

atomic<int> sync(0);
int a = 0, b = 0;

void threadA() {
  a = 1;          // (1) sequenced-before (2) within thread A
  b = 2;          // (2) sequenced-before (3) within thread A
  sync.store(1, memory_order_release);  // (3) synchronizes-with (4) in thread B
}

void threadB() {
  while (sync.load(memory_order_acquire) != 1) {}  // (4) acquire
  // happens-before chain: (1),(2) hb (3) hb (4)
  // Therefore (1) and (2) happen-before here:
  cout << a << " " << b << "\n";   // guaranteed: 1 2
}

thread tA(threadA), tB(threadB);
tA.join(); tB.join();

// --- Common mistake: wrong atomic variable ---
atomic<int> sync2(0);
int c = 0;

void wrongWriter() {
  c = 42;
  sync2.store(1, memory_order_release);  // releases c via sync2
}

void wrongReader() {
  while (sync.load(memory_order_acquire) != 1) {}  // acquire on sync, NOT sync2!
  // ❌ No synchronizes-with between sync2 store and sync acquire
  // c is not guaranteed to be 42 here
  cout << c << "\n";   // undefined behaviour territory
}
```

---

## 18.6 Relaxed Ordering

`memory_order_relaxed` provides **only atomicity** — no ordering guarantees relative to other operations. Use it only when you need a thread-safe counter and don't care about the order of other memory operations.

```cpp
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>
using namespace std;

// ✅ Correct use: global counter that accumulates hits
// We only care about the final total, not order of increments
atomic<int> hitCount(0);

void worker(int iterations) {
  for (int i = 0; i < iterations; ++i) {
    // relaxed: no synchronization with other operations
    // each increment is atomic (no torn writes), but ordering vs other ops unspecified
    hitCount.fetch_add(1, memory_order_relaxed);
  }
}

vector<thread> threads;
for (int i = 0; i < 8; ++i)
  threads.push_back(thread(worker, 1000));
for (thread& t : threads) t.join();

// Final value is correct (8000), but individual increments had no ordering guarantee
cout << "Hits: " << hitCount.load(memory_order_relaxed) << "\n";  // 8000

// ❌ WRONG use: using relaxed to publish data to another thread
atomic<bool> flag(false);
int data2 = 0;

void bad_writer() {
  data2 = 42;
  flag.store(true, memory_order_relaxed);  // NO release — data2 write may not be visible!
}

void bad_reader() {
  while (!flag.load(memory_order_relaxed)) {}   // NO acquire
  // ❌ data2 might still be 0 — relaxed provides no ordering guarantee
  cout << data2 << "\n";   // UNDEFINED BEHAVIOUR
}
// Use memory_order_release/acquire here instead
```

| | `relaxed` | `acquire`/`release` | `seq_cst` |
|---|---|---|---|
| Atomicity | Yes | Yes | Yes |
| Ordering relative to other ops | No | Yes (pair) | Yes (global) |
| Cost on x86 | Same as `seq_cst` | Cheaper on ARM | Full fence on ARM |
| Use case | Counters, flags with no side effects | Message passing, lock-free structures | General-purpose default |

---

## 18.7 std::atomic_thread_fence

A **fence** imposes ordering on non-atomic memory operations without attaching the ordering to a specific atomic variable. Use fences when multiple variables need to be published atomically.

```cpp
#include <atomic>
#include <thread>
#include <iostream>
using namespace std;

int   x2 = 0, y2 = 0;   // non-atomic payload
atomic<bool> guard(false);

// Release fence: all writes BEFORE the fence are visible to any thread
// that does an acquire fence and then sees the guard store
void writer() {
  x2 = 10;
  y2 = 20;
  atomic_thread_fence(memory_order_release);   // fence covers x2 AND y2
  guard.store(true, memory_order_relaxed);     // just signals "I'm done"
}

void reader() {
  while (!guard.load(memory_order_relaxed)) {} // spin
  atomic_thread_fence(memory_order_acquire);   // acquire fence
  // Now x2 and y2 are guaranteed to be 10 and 20
  cout << x2 << " " << y2 << "\n";  // guaranteed: 10 20
}

thread tw(writer), tr(reader);
tw.join(); tr.join();

// Fence vs atomic operation on the same variable:
// atomic<int> x(0); x.store(v, release); — release attached to ONE variable
// atomic_thread_fence(release);           — release covers ALL preceding stores
```

---

## 18.8 Lock-Free Programming Patterns

Lock-free means **at least one thread always makes progress** even if others are delayed. The key primitive is **compare-and-swap (CAS)**.

```cpp
#include <atomic>
#include <iostream>
using namespace std;

// --- Lock-free stack ---
template<typename T>
class LockFreeStack {
  struct Node {
    T     value;
    Node* next;
    Node(const T& v, Node* n) : value(v), next(n) {}
  };

  atomic<Node*> head;
public:
  LockFreeStack() : head(nullptr) {}

  void push(const T& val) {
    Node* newNode = new Node(val, nullptr);
    // CAS loop: retry until we successfully set head
    Node* oldHead = head.load(memory_order_relaxed);
    do {
      newNode->next = oldHead;
    } while (!head.compare_exchange_weak(
                 oldHead, newNode,
                 memory_order_release,    // success: release the new node
                 memory_order_relaxed));  // failure: just retry
  }

  bool pop(T& result) {
    Node* oldHead = head.load(memory_order_acquire);
    while (oldHead != nullptr) {
      Node* newHead = oldHead->next;
      if (head.compare_exchange_weak(
              oldHead, newHead,
              memory_order_acquire,
              memory_order_relaxed)) {
        result = oldHead->value;
        delete oldHead;
        return true;
      }
      // oldHead was updated by compare_exchange_weak on failure — retry
    }
    return false;  // stack was empty
  }

  ~LockFreeStack() {
    T dummy;
    while (pop(dummy)) {}
  }
};

LockFreeStack<int> stack;
stack.push(1); stack.push(2); stack.push(3);

int v;
while (stack.pop(v)) cout << v << " ";  // 3 2 1
cout << "\n";

// --- compare_exchange_weak vs compare_exchange_strong ---
// weak: may spuriously fail on some platforms (returns false even if equal)
//       → use in a retry loop (cheaper on ARM/POWER)
// strong: never spuriously fails
//         → use when a single attempt is sufficient

atomic<int> counter(0);
int expected = 0;
// strong CAS: if counter == 0, set to 1 (one-shot initialisation)
if (counter.compare_exchange_strong(expected, 1)) {
  cout << "Initialised\n";
} else {
  cout << "Already set to " << expected << "\n";
}
```

---

## 18.9 Memory Model Summary Table

```cpp
// Quick reference: which memory_order to use

// Publishing data from one thread to another (most common case):
data = 42;
flag.store(true, memory_order_release);   // writer: release
// ------
while (!flag.load(memory_order_acquire)) {} // reader: acquire

// Thread-safe counter (final value only, no ordering needed):
counter.fetch_add(1, memory_order_relaxed);

// Mutual exclusion (similar to what mutex does internally):
// acquire on lock, release on unlock

// Sequential reasoning (default, safest):
// Just use default (seq_cst) — change to weaker only after profiling

// Read-modify-write (CAS, fetch_add, exchange):
// Use acq_rel for the operation itself
old = x.fetch_add(1, memory_order_acq_rel);
```

---

## 18.10 Common Pitfalls

```cpp
#include <atomic>
using namespace std;

// ❌ Pitfall 1: Using volatile instead of atomic
// volatile prevents compiler caching but does NOT prevent CPU reordering
// and does NOT guarantee atomic read-modify-write
volatile int bad = 0;   // NOT thread-safe for ++bad

// ✅ Use atomic<int> bad(0); and bad.fetch_add(1);

// ❌ Pitfall 2: Assuming seq_cst on non-atomic variables
int nonatomic = 0;
atomic<bool> sync3(false);
// Writing nonatomic then storing sync3 with release IS correct:
//   nonatomic = 1; sync3.store(true, release); — works
// But reading nonatomic without first doing an acquire load of sync3 is NOT:
//   if (nonatomic == 1) { } — data race if another thread writes it!

// ❌ Pitfall 3: Wrong pairing of release/acquire
atomic<int> a2(0), b2(0);
// writer stores to a2 with release
// reader acquires from b2 (DIFFERENT variable!)
// → NO synchronizes-with → NO happens-before guarantee

// ✅ Always pair release store and acquire load on THE SAME atomic variable

// ❌ Pitfall 4: compare_exchange in a non-loop
atomic<int> val(0);
int exp = 0;
// compare_exchange_weak can spuriously fail — MUST be in a loop
val.compare_exchange_weak(exp, 1);  // may fail even if val == 0!
// ✅ while (!val.compare_exchange_weak(exp, 1)) {}

// ❌ Pitfall 5: Lock-free ABA problem
// If pop returns a node, frees it, and another thread pushes a new node
// at the same address, CAS may succeed incorrectly.
// Solution: hazard pointers or epoch-based reclamation (advanced topic)
```

---

## Summary

| Concept | Key Points |
|---|---|
| **Data race** | Any unsynchronised concurrent access to non-atomic data = UB |
| **happens-before** | Formal ordering: B sees effect of A iff A hb B |
| **synchronizes-with** | release-store + acquire-load on same atomic → hb across threads |
| **seq_cst** | Total global order; strongest and safest default |
| **acquire/release** | Efficient message-passing pattern; cheaper than seq_cst on ARM |
| **relaxed** | Atomicity only; for counters with no side-effect ordering |
| **fence** | Imposes ordering on a group of variables at once |
| **CAS loop** | Foundation of lock-free data structures |
| **volatile** | NOT a threading primitive; does not prevent reordering |

---

## Next Steps

- Study the C++11 memory model specification (ISO 1.10, 29.x)
- Read *C++ Concurrency in Action* (Williams) Chapter 5 for the formal model
- Apply acquire/release in all lock-free flags and message-passing patterns
- Return to [Chapter 16: Concurrency](../16_concurrency/README.md) for practical high-level primitives
- Return to [Chapter 17: Template Metaprogramming](../17_template_metaprogramming/README.md) for the other expert topic
