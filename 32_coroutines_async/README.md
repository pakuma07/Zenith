# 32 — Coroutines & Asynchronous C++

[Chapter 20](../20_modern_cpp_20_23/README.md) introduced coroutines in two pages —
enough to recognize `co_yield` and run a generator, not enough to *build* one or to
reason about lifetimes, allocation, and symmetric transfer when production async code
breaks. This is the authoritative coroutine chapter for a staff/principal engineer:
the compiler transformation, the `promise_type` and awaiter protocols built from
scratch, lazy `task<T>`, the ecosystem (Asio, stdexec, cppcoro), and the pitfalls
that page you at 3am.

> **Compile with:** `g++ -std=c++23` (or `-std=c++20`). The core machinery is C++20;
> `std::generator` is C++23. Coroutines are a *language mechanism* — most ergonomic
> types come from libraries or the C++23 stdlib, not C++20's `<coroutine>` alone.

---

## 32.1 What a Coroutine Actually Is

A coroutine is a function that can **suspend** (pause, returning control to its
caller) and later **resume** from exactly where it left off, with its locals intact.
A normal function has one entry and one exit; a coroutine has one entry and *many*
suspend/resume points.

In C++20 coroutines are **stackless**: the suspended state lives in a heap-allocated
**coroutine frame**, not on a separate call stack (contrast with fibers/green threads,
which are *stackful*). Stackless means suspension only happens at the coroutine's own
`co_await`/`co_yield` points — you cannot suspend from a nested ordinary call.

**Any function whose body contains `co_await`, `co_yield`, or `co_return` is a
coroutine** — there is no `coroutine` keyword. The presence of one of those keywords
is what triggers the compiler transformation.

```cpp
// These three keywords MAKE a function a coroutine:
co_await expr;          // suspend until 'expr' (an awaitable) is ready; get its result
co_yield value;         // suspend and produce 'value' to the consumer (sugar over co_await)
co_return value;        // finish the coroutine, producing a final result
```

> A coroutine cannot use plain `return`, cannot be `constexpr`/`consteval`, cannot be
> `main`, a constructor/destructor, and cannot have a plain-`...` variadic signature.
> The *return type* of the function is what selects the coroutine machinery (below).

---

## 32.2 The Compiler Transformation: Frame, Handle, Resume

When the compiler sees a coroutine, it rewrites it roughly like this. Understanding
this transformation is the key to everything else.

```cpp
// You write:
Task foo(int x) {
    int local = x * 2;
    co_await something();
    co_return local;
}

// The compiler generates (conceptually):
Task foo(int x) {
    // 1. Allocate the coroutine frame on the heap (holds: promise, params, locals,
    //    and a resume-point index). May be ELIDED — see HALO below.
    auto* frame = new __foo_frame{ /* copies of params: x */ };
    auto& promise = frame->promise;

    // 2. Build the return object handed back to the caller.
    Task ret = promise.get_return_object();

    // 3. co_await promise.initial_suspend();  -> decide: run now, or suspend at the top.
    // 4. Run the body until the first suspend point, then return 'ret' to the caller.
    //    On resume, jump back to the saved resume-point and continue.
    // 5. At co_return: promise.return_value(local); (or return_void()).
    // 6. co_await promise.final_suspend();  -> cleanup / resume a continuation.
    // (exceptions in the body route to promise.unhandled_exception())
}
```

The handle to that frame is `std::coroutine_handle<Promise>` — a non-owning,
trivially-copyable pointer-like object:

```cpp
#include <coroutine>
std::coroutine_handle<Promise> h = /* ... */;
h.resume();   // or h() — resume at the saved suspend point
h.done();     // true once the coroutine ran past final_suspend
h.destroy();  // free the frame (RAII wrappers do this in their destructor)
Promise& p = h.promise();                                    // access the promise
auto h2 = std::coroutine_handle<Promise>::from_promise(p);   // recover handle from promise
```

**HALO (Heap Allocation eLision Optimization):** the frame allocation is *observable as
`new`*, but the standard permits the optimizer to elide it (coroutine-frame elision)
when the coroutine's lifetime is provably bounded by the caller and the size is known —
typically a generator consumed in the same function; inlining unlocks it. **Do not
assume elision happens** across TU boundaries or behind indirection; if allocation cost
matters, measure (32.7) and/or supply a custom `operator new` on the promise.

---

## 32.3 The `promise_type` Protocol — Built From Scratch

The compiler finds the promise via `std::coroutine_traits<ReturnType, Args...>`, which
by default looks for a nested `ReturnType::promise_type`. The promise is the
**customization point** controlling every step. Here is a minimal but complete
fire-and-forget coroutine type:

```cpp
#include <coroutine>
#include <exception>
#include <utility>

struct Task {
    struct promise_type {                          // the protocol the compiler calls
        Task get_return_object() {                 // (1) build the caller's return value
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_never  initial_suspend() noexcept { return {}; }  // (2) start eagerly
        std::suspend_always final_suspend()   noexcept { return {}; }  // (3) MUST be noexcept
        void return_void() {}                                          // (4) co_return; (no value)
        void unhandled_exception() { std::terminate(); }               // (5) escaped exception
    };

    std::coroutine_handle<promise_type> h_;
    explicit Task(std::coroutine_handle<promise_type> h) : h_(h) {}
    Task(Task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ~Task() { if (h_) h_.destroy(); }              // RAII: own and free the frame
};
```

The compiler-called members, in full:

| Member | When called | Meaning |
|---|---|---|
| `get_return_object()` | once, at start | produces the object returned to the caller |
| `initial_suspend()` | once, before the body | `suspend_never` = **eager** (run now); `suspend_always` = **lazy** (run on first resume) |
| `final_suspend() noexcept` | once, after `co_return`/fall-off | last suspend; usually `suspend_always` so the frame survives for the caller to read the result, then `destroy()` it |
| `return_value(v)` *or* `return_void()` | at `co_return` | exactly one must exist; mixing both is ill-formed |
| `yield_value(v)` | at each `co_yield v` | returns an awaitable; how generators hand out values |
| `unhandled_exception()` | on escaped exception | called inside the catch; store/rethrow or `terminate` |

**Why `final_suspend()` is `noexcept`:** at final suspend the coroutine is essentially
done; allowing it to throw would mean an exception with nowhere sane to go (the body's
`catch` already ran). The standard *requires* `final_suspend` be `noexcept`.

**Why `suspend_always` at final suspend:** if you return `suspend_never`, the frame is
destroyed immediately on completion — and any handle the caller holds (to read the
result) instantly dangles. Lazy/result-bearing types return `suspend_always` and let
the owning wrapper call `destroy()`.

`std::suspend_always` and `std::suspend_never` are the two trivial awaiters: the first
always suspends, the second never does (32.4).

---

## 32.4 The Awaitable / Awaiter Protocol

`co_await expr` drives an **awaiter** with three methods. The compiler obtains the
awaiter from `expr` (via `operator co_await`, or the promise's `await_transform`, or
`expr` itself), then generates:

```cpp
auto&& awaiter = /* from expr */;
if (!awaiter.await_ready()) {                  // (1) already done? skip suspension
    // ... save resume point, then:
    awaiter.await_suspend(this_coro_handle);   // (2) we are now suspended
    // control returns to whoever resumed us
}
auto result = awaiter.await_resume();          // (3) produce co_await's value on resume
```

```cpp
struct Awaiter {
    bool await_ready() const noexcept;         // true => don't suspend (fast path)

    // await_suspend has three legal return types:
    void await_suspend(std::coroutine_handle<> h);   // (a) just suspend; caller regains control
    bool await_suspend(std::coroutine_handle<> h);   // (b) false => resume immediately (don't suspend)
    std::coroutine_handle<>                          // (c) SYMMETRIC TRANSFER: resume THIS handle next
        await_suspend(std::coroutine_handle<> h);    //     (a tail-call style hand-off, no stack growth)

    T await_resume();                          // value of the whole `co_await` expression
};
```

`operator co_await` lets a type customize how it is awaited without being an awaiter
itself:

```cpp
struct Event {
    Awaiter operator co_await() const noexcept { return Awaiter{/*...*/}; }
};
```

The **symmetric transfer** form (c) is the load-bearing one for async: instead of
`h.resume()` calling into the next coroutine (growing the C++ stack on every hop), you
*return* the next handle and the coroutine machinery performs a guaranteed tail-call.
This is what prevents stack overflow in long `co_await` chains (32.6, 32.9).

---

## 32.5 Generators — Lazy Sequences with `co_yield`

`co_yield v` is sugar for `co_await promise.yield_value(v)`. A hand-rolled lazy
generator:

```cpp
#include <coroutine>
#include <exception>
#include <utility>

template <class T>
struct Generator {
    struct promise_type {
        T current_;
        Generator get_return_object() {
            return Generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }   // LAZY: produce nothing yet
        std::suspend_always final_suspend()   noexcept { return {}; }
        std::suspend_always yield_value(T v) noexcept {                 // co_yield v lands here
            current_ = std::move(v);
            return {};                                                  // suspend, value now visible
        }
        void return_void() {}
        void unhandled_exception() { throw; }                           // rethrow to the consumer
    };

    std::coroutine_handle<promise_type> h_;
    explicit Generator(std::coroutine_handle<promise_type> h) : h_(h) {}
    Generator(Generator&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ~Generator() { if (h_) h_.destroy(); }

    // Minimal input-iterator interface so range-for works:
    struct iterator {
        std::coroutine_handle<promise_type> h_;
        iterator& operator++() { h_.resume(); return *this; }
        const T& operator*() const { return h_.promise().current_; }
        bool operator==(std::default_sentinel_t) const { return !h_ || h_.done(); }
    };
    iterator begin() { if (h_) h_.resume(); return iterator{h_}; }   // resume to first co_yield
    std::default_sentinel_t end() { return {}; }
};

Generator<int> count_to(int n) {
    for (int i = 1; i <= n; ++i) co_yield i;     // one value per resume — nothing buffered
}

// for (int x : count_to(3)) std::cout << x;     // 1 2 3, produced lazily
```

In real code, prefer **`std::generator<T>`** (C++23, header [`<generator>`](../23_time_text_filesystem/README.md)).
It is the standard, move-only, allocator-aware, recursion-friendly (`co_yield
std::ranges::elements_of(inner)`) generator and composes with [ranges](../20_modern_cpp_20_23/README.md):

```cpp
#include <generator>
#include <ranges>

std::generator<int> fib() {
    int a = 0, b = 1;
    while (true) { co_yield a; std::tie(a, b) = std::pair{b, a + b}; }
}
for (int n : fib() | std::views::take(10)) std::cout << n << ' ';   // 0 1 1 2 3 5 ...
```

---

## 32.6 Tasks & Async — Lazy `task<T>`, Continuations, Symmetric Transfer

A generator *produces values*; a **`task<T>`** *represents an async computation* whose
result you `co_await`. The defining design choices:

- **Lazy vs eager.** A *lazy* task starts suspended (`initial_suspend()` returns
  `suspend_always`) and runs only when awaited. This is the modern default — it makes
  composition and cancellation sane. An *eager* task runs immediately on call; simpler,
  but it can complete (or throw) before anyone is listening.
- **Continuation.** When task `A` does `co_await B`, `B` stores `A`'s handle and, at its
  `final_suspend`, **symmetric-transfers** back to `A`. That hand-off is the whole game.

```cpp
#include <coroutine>
#include <exception>
#include <utility>

template <class T>
struct task {
    struct promise_type {
        T result_;
        std::coroutine_handle<> continuation_;          // who resumes us when we finish
        std::exception_ptr error_;

        task get_return_object() {
            return task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }   // LAZY

        struct final_awaiter {                                          // symmetric transfer home
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(
                    std::coroutine_handle<promise_type> h) noexcept {
                auto cont = h.promise().continuation_;
                return cont ? cont : std::noop_coroutine();             // tail-call the awaiter
            }
            void await_resume() noexcept {}
        };
        final_awaiter final_suspend() noexcept { return {}; }

        void return_value(T v) { result_ = std::move(v); }
        void unhandled_exception() { error_ = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> h_;
    explicit task(std::coroutine_handle<promise_type> h) : h_(h) {}
    task(task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ~task() { if (h_) h_.destroy(); }

    // Make `co_await someTask` work: register the awaiting coroutine and start this one.
    auto operator co_await() noexcept {
        struct awaiter {
            std::coroutine_handle<promise_type> h_;
            bool await_ready() noexcept { return !h_ || h_.done(); }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) noexcept {
                h_.promise().continuation_ = awaiting;   // resume the caller when we're done
                return h_;                               // symmetric transfer: start running us
            }
            T await_resume() {
                if (h_.promise().error_) std::rethrow_exception(h_.promise().error_);
                return std::move(h_.promise().result_);
            }
        };
        return awaiter{h_};
    }
};
```

`std::noop_coroutine()` is the standard no-op handle: a valid target for symmetric
transfer when there is no continuation, so the resume chain terminates cleanly instead
of branching.

**Integrating with real I/O.** You rarely write the above by hand. You adopt an
executor/event loop that provides awaitables:

| Library | Model | What you `co_await` |
|---|---|---|
| **Boost.Asio / standalone Asio** | event loop + completion handlers | `asio::awaitable<T>` with `co_spawn`, `use_awaitable`; sockets/timers are awaitable |
| **stdexec (P2300 senders/receivers)** | structured concurrency, the std-track async model | senders via `co_await`; `task` integrates with schedulers |
| **libunifex** | senders/receivers (the stdexec precursor) | `task<T>`, algorithms (`when_all`, `let_value`) |
| **cppcoro** (Lewis Baker) | reference coroutine types | `task<T>`, `generator`, `async_generator`, `when_all`, `sync_wait` |

```cpp
// Asio flavor — readable async I/O, no callback nesting:
asio::awaitable<void> echo(tcp::socket sock) {
    char data[1024];
    for (;;) {
        std::size_t n = co_await sock.async_read_some(asio::buffer(data), asio::use_awaitable);
        co_await asio::async_write(sock, asio::buffer(data, n), asio::use_awaitable);
    }
}
```

---

## 32.7 Pitfalls (Symptom / Cause / Fix)

**Dangling reference — coroutine outlives a by-reference argument.** The classic
footgun: a coroutine frame copies *parameters by value*, but a reference parameter
copies only the reference. If the coroutine suspends and the referent dies, resuming
reads freed memory.

```cpp
// WRONG — 's' is a dangling reference after the temporary string is destroyed:
task<int> length_of(const std::string& s) {     // reference param survives in the frame
    co_await some_async_op();                    // suspends; caller's temporary may die here
    co_return (int)s.size();                     // use-after-free on resume
}
length_of(std::string("temp"));                  // temporary gone before resume — UB

// RIGHT — take by value so the frame owns a copy:
task<int> length_of(std::string s) {             // value param is stored in the frame
    co_await some_async_op();
    co_return (int)s.size();                      // safe: 's' lives in the frame
}
```

- **Symptom:** ASan use-after-free / garbage results only after a suspension point.
- **Cause:** reference/pointer params (or captured `this`) outlived by the frame.
- **Fix:** pass coroutine arguments **by value**; for `this`, keep the owning object
  alive for the coroutine's whole lifetime (or capture a `shared_ptr`).

**The lambda-coroutine capture trap.** A coroutine lambda's *captures* live in the
**closure object**, not the frame. If the closure is a temporary, the captures die at
the first suspension.

```cpp
// WRONG — captures live in the lambda temporary, which is destroyed immediately:
auto t = [v = std::vector<int>{1,2,3}]() -> task<int> {
    co_await x();                                // closure (and 'v') already gone
    co_return v[0];                              // use-after-free
}();                                             // <-- lambda destroyed right here

// RIGHT — pass state as a by-value parameter, or keep the closure alive for the task.
auto make = [](std::vector<int> v) -> task<int> { co_await x(); co_return v[0]; };
```

**Allocation cost.** Every coroutine *may* heap-allocate its frame.
- **Symptom:** allocator pressure / latency spikes in hot async paths.
- **Cause:** frames not elided (HALO didn't fire — indirection, separate TU, type-erased
  handle).
- **Fix:** keep generators/tasks inlinable and locally scoped; give the promise a custom
  `operator new`/`operator delete` (e.g. pool or arena); profile before assuming.

**"Nothing runs until awaited."** A lazy `task<T>` is inert until `co_await`ed (or driven
by `sync_wait`).
- **Symptom:** an async function "isn't being called"; side effects never happen.
- **Cause:** the returned `task` was created but never awaited/spawned.
- **Fix:** `co_await` it, `co_spawn`/`sync_wait` it, or `[[nodiscard]]` your task type so
  the compiler flags ignored results.

**Exceptions across suspension.** An exception thrown in the body goes to
`unhandled_exception()`; it is *not* propagated to the original caller (who already
returned at the first suspend). Store it (`std::current_exception()`) and rethrow it in
`await_resume()` of whoever awaits — the `task` above does this. Forgetting to rethrow
silently swallows errors.

---

## 32.8 Coroutines vs Threads vs Callbacks

| Approach | Concurrency kind | Cost | Use when |
|---|---|---|---|
| **Threads** ([Ch 16](../16_concurrency/README.md)) | parallelism (multi-core) | OS stack (~MB), context switch | CPU-bound work across cores; blocking calls you can't make async |
| **Callbacks** | async, single-thread | cheap, but "callback hell" | simple one-shot async; legacy/C APIs |
| **Coroutines** | async, cooperative | tiny frame (often elided) | I/O-bound concurrency (many in-flight ops), generators, linear-looking async code |

Coroutines give you **callbacks' efficiency with synchronous-looking code**. They are
*concurrency, not parallelism* — a coroutine runs on whatever thread resumes it; combine
with a thread pool/executor for parallelism. For CPU-bound parallel work, threads
([Chapter 16](../16_concurrency/README.md)) and the [memory model](../18_memory_model/README.md)
still rule; for high-fan-out I/O (servers, proxies, pipelines), coroutines win on both
clarity and footprint.

---

## 32.9 Ecosystem Maturity (2020s reality)

- **C++20** ships only the *machinery* (`<coroutine>`): no ready-to-use task/generator.
  Hand-rolling a `promise_type` is for library authors and for understanding — not for
  application code.
- **C++23** adds **`std::generator`** — production-ready, use it. There is still **no
  standard `task<T>`**; that is expected to arrive with senders/receivers (P2300 /
  stdexec) in a future standard.
- For async I/O **today**, use a battle-tested library: **Asio** (`awaitable`),
  **cppcoro**, **libunifex**, or **stdexec**. See production integration in
  [Chapter 25](../25_production_operational/README.md) and serialization pipelines in
  [Chapter 26](../26_data_validation_serialization/README.md).
- Tooling: ASan/UBSan ([Chapter 22](../22_tooling_testing/README.md)) catch the
  dangling-frame bugs in 32.7 — run coroutine code under sanitizers in CI.

---

## 32.10 Summary

| Concept | Takeaway |
|---|---|
| **Coroutine** | any function using `co_await`/`co_yield`/`co_return`; stackless, suspendable |
| **Frame** | heap-allocated state (params by value, locals, resume index); HALO *may* elide it |
| **`coroutine_handle<>`** | non-owning handle: `resume()`, `done()`, `destroy()`, `promise()` |
| **`promise_type`** | the customization protocol; `final_suspend` must be `noexcept` |
| **Awaiter** | `await_ready` / `await_suspend` / `await_resume`; return a handle for symmetric transfer |
| **Generator** | lazy sequence via `co_yield`; prefer C++23 `std::generator` |
| **`task<T>`** | lazy async result; continuation + symmetric transfer; use a library |
| **Pitfalls** | by-value params, keep `this`/closures alive, lazy-never-awaited, rethrow exceptions |

- Pass coroutine arguments **by value**; reference params and captured `this`/closures
  are the #1 source of use-after-free.
- Use **symmetric transfer** (handle-returning `await_suspend`) in any chained task to
  keep stack depth O(1).
- In application code, reach for **`std::generator`** (C++23) and an **async library**
  (Asio/stdexec/cppcoro) — hand-write a `promise_type` only to learn or to author a
  library.
- Coroutines are **concurrency, not parallelism**: pair them with an executor for
  multi-core work; use [threads](../16_concurrency/README.md) for CPU-bound parallelism.

## Next Steps

- Build the `Generator<T>` from 32.5 and step through `begin()`/`operator++` in a debugger.
- Add a custom `operator new` to a `promise_type` and confirm (via a counter) when HALO elides it.
- Convert a callback-based read loop to an `asio::awaitable` coroutine and compare readability.
- Run a coroutine that captures a reference under ASan and watch 32.7 fire.

> **Related:** [Chapter 16: Concurrency](../16_concurrency/README.md) ·
> [Chapter 18: Memory Model](../18_memory_model/README.md) ·
> [Chapter 20: Modern C++ II — C++20 & C++23](../20_modern_cpp_20_23/README.md) ·
> [Chapter 25: Production & Operational Concerns](../25_production_operational/README.md)
