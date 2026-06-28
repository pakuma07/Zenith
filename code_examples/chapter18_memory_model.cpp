// Chapter 18: Memory Model and std::memory_order — Runnable Examples
// Compile: g++ -std=c++11 -Wall -Wextra -pthread chapter18_memory_model.cpp -o chapter18_memory_model

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <cassert>
using namespace std;

// ============================================================
// Example 1: Data Race vs Atomic (sequentially consistent default)
// ============================================================
atomic<int> safeCounter(0);

void example1_worker(int n) {
    for (int i = 0; i < n; ++i)
        safeCounter.fetch_add(1);  // seq_cst by default — safe
}

void example1_data_race_vs_atomic() {
    cout << "=== Example 1: Atomic Counter (seq_cst) ===" << endl;
    safeCounter.store(0);

    vector<thread> threads;
    for (int i = 0; i < 4; ++i)
        threads.push_back(thread(example1_worker, 250));
    for (thread& t : threads) t.join();

    cout << "Expected: 1000, Got: " << safeCounter.load() << endl;
    assert(safeCounter.load() == 1000);
}

// ============================================================
// Example 2: Acquire/Release — Message Passing
// ============================================================
atomic<bool> msgReady(false);
int          payload = 0;     // protected by the acquire/release pair

void ex2_producer() {
    payload = 42;                                    // write payload first
    msgReady.store(true, memory_order_release);      // release: publish
}

void ex2_consumer() {
    while (!msgReady.load(memory_order_acquire)) {}  // acquire: subscribe
    // happens-before: payload write is visible here
    cout << "Consumer received payload = " << payload << endl;
    assert(payload == 42);
}

void example2_acquire_release() {
    cout << "\n=== Example 2: Acquire/Release Message Passing ===" << endl;
    msgReady.store(false);
    payload = 0;

    thread producer(ex2_producer), consumer(ex2_consumer);
    producer.join();
    consumer.join();
}

// ============================================================
// Example 3: Relaxed Ordering — Counter
// ============================================================
atomic<long long> relaxedCounter(0);

void ex3_worker(int n) {
    for (int i = 0; i < n; ++i)
        relaxedCounter.fetch_add(1, memory_order_relaxed);
}

void example3_relaxed() {
    cout << "\n=== Example 3: Relaxed Ordering Counter ===" << endl;
    relaxedCounter.store(0);

    vector<thread> threads;
    for (int i = 0; i < 8; ++i)
        threads.push_back(thread(ex3_worker, 1000));
    for (thread& t : threads) t.join();

    // Final value is correct even with relaxed
    cout << "Relaxed counter: " << relaxedCounter.load() << " (expected 8000)" << endl;
    assert(relaxedCounter.load() == 8000);
}

// ============================================================
// Example 4: seq_cst — Total Order Guarantee
// ============================================================
atomic<bool> seqX(false), seqY(false);
atomic<int>  seqZ(0);

void ex4_write_x()       { seqX.store(true); }
void ex4_write_y()       { seqY.store(true); }
void ex4_read_x_then_y() {
    while (!seqX.load()) {}
    if (seqY.load()) ++seqZ;
}
void ex4_read_y_then_x() {
    while (!seqY.load()) {}
    if (seqX.load()) ++seqZ;
}

void example4_seqcst() {
    cout << "\n=== Example 4: Sequentially Consistent Total Order ===" << endl;
    seqX.store(false); seqY.store(false); seqZ.store(0);

    thread t1(ex4_write_x), t2(ex4_write_y),
           t3(ex4_read_x_then_y), t4(ex4_read_y_then_x);
    t1.join(); t2.join(); t3.join(); t4.join();

    // With seq_cst: at least one thread sees both flags, so z >= 1
    cout << "seqZ = " << seqZ.load() << " (must be >= 1 with seq_cst)" << endl;
    assert(seqZ.load() >= 1);
}

// ============================================================
// Example 5: atomic_thread_fence — Group Publish
// ============================================================
int   fenceA = 0, fenceB = 0;
atomic<bool> fenceReady(false);

void ex5_writer() {
    fenceA = 10;
    fenceB = 20;
    atomic_thread_fence(memory_order_release);        // fence covers both fenceA and fenceB
    fenceReady.store(true, memory_order_relaxed);     // just a signal
}

void ex5_reader() {
    while (!fenceReady.load(memory_order_relaxed)) {}
    atomic_thread_fence(memory_order_acquire);        // matching acquire fence
    cout << "fenceA = " << fenceA << " (expected 10)" << endl;
    cout << "fenceB = " << fenceB << " (expected 20)" << endl;
    assert(fenceA == 10 && fenceB == 20);
}

void example5_fence() {
    cout << "\n=== Example 5: atomic_thread_fence ===" << endl;
    fenceA = 0; fenceB = 0; fenceReady.store(false);

    thread writer(ex5_writer), reader(ex5_reader);
    writer.join(); reader.join();
}

// ============================================================
// Example 6: Compare-and-Swap (CAS)
// ============================================================
void example6_cas() {
    cout << "\n=== Example 6: Compare-and-Swap ===" << endl;

    // --- compare_exchange_strong: one-shot initialisation ---
    atomic<int> val(0);
    int expected = 0;

    if (val.compare_exchange_strong(expected, 42,
                                    memory_order_acq_rel,
                                    memory_order_acquire)) {
        cout << "CAS succeeded: val = " << val.load() << endl;
    } else {
        cout << "CAS failed: val already = " << expected << endl;
    }

    // Second attempt — should fail because val is now 42
    expected = 0;
    if (!val.compare_exchange_strong(expected, 99,
                                     memory_order_acq_rel,
                                     memory_order_acquire)) {
        cout << "Second CAS failed as expected; val = " << expected << endl;
    }

    // --- compare_exchange_weak in a loop ---
    atomic<int> counter2(0);
    vector<thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.push_back(thread([&](){
            for (int i = 0; i < 250; ++i) {
                int old = counter2.load(memory_order_relaxed);
                while (!counter2.compare_exchange_weak(old, old + 1,
                                                       memory_order_acq_rel,
                                                       memory_order_relaxed)) {}
            }
        }));
    }
    for (thread& th : threads) th.join();
    cout << "CAS loop counter: " << counter2.load() << " (expected 1000)" << endl;
    assert(counter2.load() == 1000);
}

// ============================================================
// Example 7: Lock-Free Stack
// ============================================================
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
        Node* n = new Node(val, nullptr);
        Node* old = head.load(memory_order_relaxed);
        do { n->next = old; }
        while (!head.compare_exchange_weak(old, n,
                                           memory_order_release,
                                           memory_order_relaxed));
    }

    bool pop(T& out) {
        Node* old = head.load(memory_order_acquire);
        while (old) {
            if (head.compare_exchange_weak(old, old->next,
                                           memory_order_acquire,
                                           memory_order_relaxed)) {
                out = old->value;
                delete old;
                return true;
            }
        }
        return false;
    }

    ~LockFreeStack() {
        T dummy;
        while (pop(dummy)) {}
    }
};

void example7_lock_free_stack() {
    cout << "\n=== Example 7: Lock-Free Stack ===" << endl;
    LockFreeStack<int> stack;

    // Single-threaded test
    for (int i = 1; i <= 5; ++i) stack.push(i);
    int v;
    cout << "Pop order (LIFO): ";
    while (stack.pop(v)) cout << v << " ";  // 5 4 3 2 1
    cout << endl;

    // Multi-threaded push/pop
    LockFreeStack<int> shared;
    atomic<int> popped(0);

    vector<thread> pushers, poppers;
    for (int t = 0; t < 4; ++t) {
        pushers.push_back(thread([&](){ for (int i=0; i<100; ++i) shared.push(i); }));
        poppers.push_back(thread([&](){
            int x;
            for (int i=0; i<100; ++i)
                while (!shared.pop(x)) {}
            popped.fetch_add(100, memory_order_relaxed);
        }));
    }
    for (thread& t : pushers) t.join();
    for (thread& t : poppers) t.join();
    cout << "Total popped by 4 threads: " << popped.load() << " (expected 400)" << endl;
}

// ============================================================
// Example 8: thread_local Storage
// ============================================================
thread_local int tlsCounter = 0;

void ex8_worker(int id) {
    for (int i = 0; i < 5; ++i) ++tlsCounter;
    cout << "Thread " << id << " tlsCounter = " << tlsCounter << endl;  // always 5
}

void example8_thread_local() {
    cout << "\n=== Example 8: thread_local Storage ===" << endl;
    thread t1(ex8_worker, 1), t2(ex8_worker, 2), t3(ex8_worker, 3);
    t1.join(); t2.join(); t3.join();
    // All print 5 — each thread has its own copy
}

// ============================================================
// Main
// ============================================================
int main() {
    example1_data_race_vs_atomic();
    example2_acquire_release();
    example3_relaxed();
    example4_seqcst();
    example5_fence();
    example6_cas();
    example7_lock_free_stack();
    example8_thread_local();

    cout << "\nAll memory model examples completed successfully." << endl;
    return 0;
}
