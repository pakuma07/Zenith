#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using namespace std;

mutex coutMutex;

void printHeader(const string& title) {
    lock_guard<mutex> lock(coutMutex);
    cout << "\n=== " << title << " ===\n";
}

void example1ThreadBasics() {
    printHeader("Example 1: Thread Basics");
    thread worker([]() {
        lock_guard<mutex> lock(coutMutex);
        cout << "Worker thread id: " << this_thread::get_id() << "\n";
    });
    worker.join();
}

void example2PassArguments() {
    printHeader("Example 2: Passing Arguments");
    int value = 21;
    thread worker([&value](int factor) {
        lock_guard<mutex> lock(coutMutex);
        cout << "Computed value: " << value * factor << "\n";
    }, 2);
    worker.join();
}

void example3MutexProtection() {
    printHeader("Example 3: Mutex Protection");
    int sharedCounter = 0;
    mutex counterMutex;

    auto increment = [&sharedCounter, &counterMutex]() {
        for (int i = 0; i < 1000; ++i) {
            lock_guard<mutex> lock(counterMutex);
            ++sharedCounter;
        }
    };

    thread t1(increment);
    thread t2(increment);
    t1.join();
    t2.join();

    lock_guard<mutex> lock(coutMutex);
    cout << "Final protected counter: " << sharedCounter << "\n";
}

void example4LockGuardRAII() {
    printHeader("Example 4: lock_guard RAII");
    mutex localMutex;
    vector<int> values;

    auto pushValue = [&localMutex, &values](int x) {
        lock_guard<mutex> lock(localMutex);
        values.push_back(x);
    };

    thread t1(pushValue, 10);
    thread t2(pushValue, 20);
    t1.join();
    t2.join();

    lock_guard<mutex> lock(coutMutex);
    cout << "Values inserted: ";
    for (size_t i = 0; i < values.size(); ++i) {
        cout << values[i] << (i + 1 == values.size() ? '\n' : ' ');
    }
}

void example5UniqueLockAndConditionVariable() {
    printHeader("Example 5: unique_lock + condition_variable");
    queue<int> jobs;
    mutex jobsMutex;
    condition_variable ready;
    bool done = false;

    thread producer([&]() {
        {
            lock_guard<mutex> lock(jobsMutex);
            jobs.push(7);
            jobs.push(11);
            done = true;
        }
        ready.notify_one();
    });

    thread consumer([&]() {
        unique_lock<mutex> lock(jobsMutex);
        ready.wait(lock, [&]() { return !jobs.empty() || done; });
        while (!jobs.empty()) {
            int job = jobs.front();
            jobs.pop();
            lock.unlock();
            {
                lock_guard<mutex> outLock(coutMutex);
                cout << "Consumed job: " << job << "\n";
            }
            lock.lock();
        }
    });

    producer.join();
    consumer.join();
}

void example6AtomicCounter() {
    printHeader("Example 6: Atomic Counter");
    atomic<int> counter(0);

    auto work = [&counter]() {
        for (int i = 0; i < 5000; ++i) {
            ++counter;
        }
    };

    thread t1(work);
    thread t2(work);
    t1.join();
    t2.join();

    lock_guard<mutex> lock(coutMutex);
    cout << "Atomic counter: " << counter.load() << "\n";
}

int slowSquare(int x) {
    this_thread::sleep_for(chrono::milliseconds(10));
    return x * x;
}

void example7AsyncTask() {
    printHeader("Example 7: async + future");
    future<int> answer = async(launch::async, slowSquare, 9);
    lock_guard<mutex> lock(coutMutex);
    cout << "Square result: " << answer.get() << "\n";
}

void example8PromiseFuture() {
    printHeader("Example 8: promise + future");
    promise<string> message;
    future<string> result = message.get_future();

    thread worker([&message]() {
        message.set_value("promise delivered a value");
    });

    {
        lock_guard<mutex> lock(coutMutex);
        cout << result.get() << "\n";
    }
    worker.join();
}

void example9PackagedTask() {
    printHeader("Example 9: packaged_task");
    packaged_task<int(vector<int>)> task([](vector<int> data) {
        return accumulate(data.begin(), data.end(), 0);
    });

    future<int> total = task.get_future();
    thread worker(move(task), vector<int>{1, 2, 3, 4, 5});
    worker.join();

    lock_guard<mutex> lock(coutMutex);
    cout << "Sum from packaged_task: " << total.get() << "\n";
}

void example10ThreadOwnership() {
    printHeader("Example 10: Thread Ownership Transfer");
    thread original([]() {
        lock_guard<mutex> lock(coutMutex);
        cout << "Thread moved safely between owners\n";
    });

    thread newOwner = move(original);
    if (!original.joinable()) {
        lock_guard<mutex> lock(coutMutex);
        cout << "Original thread object no longer owns the thread\n";
    }
    newOwner.join();
}

int main() {
    cout << "Chapter 16: Concurrency in C++11\n";
    cout << "This file demonstrates core concurrency features safely.\n";

    example1ThreadBasics();
    example2PassArguments();
    example3MutexProtection();
    example4LockGuardRAII();
    example5UniqueLockAndConditionVariable();
    example6AtomicCounter();
    example7AsyncTask();
    example8PromiseFuture();
    example9PackagedTask();
    example10ThreadOwnership();

    cout << "\nAll concurrency examples completed successfully.\n";
    return 0;
}
