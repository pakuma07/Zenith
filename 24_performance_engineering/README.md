# Chapter 24: Performance Engineering

C++ is chosen when performance matters — game engines, HFT, databases, browsers.
But "C++ is fast" is not automatic; it's the *result* of understanding the machine
and measuring relentlessly. This chapter is the disciplined workflow: **measure
first**, exploit **mechanical sympathy** (cache, branches, the memory hierarchy),
benchmark correctly, and reach for lock-free and SIMD only when the data says so.
At C++'s level, performance is largely about **memory layout**, not clever code.

> **The golden rule:** measure, don't guess. And measure a **`-O2`/`-O3` Release
> build** — benchmarking a `Debug` build (Chapter 21) measures nothing real, because
> the optimizer that defines C++ performance is turned off.

---

## 24.1 The Optimization Workflow

```text
1. Set a goal        ── "p99 < 1 ms" / "process 1 GB/s"
2. Measure baseline  ── Release build, realistic data
3. Profile           ── find the ONE hotspot (perf/VTune flame graph)
4. Hypothesize & fix ── target the hotspot; often a data-layout change
5. Re-measure        ── kept only if it moved the goal metric
6. Repeat or stop    ── stop at the goal; don't pessimize readability for nothing
```

**Amdahl's law:** optimizing 5% of runtime caps your gain at 5%. And trust the
compiler first — modern optimizers inline, vectorize, and elide aggressively; many
hand "optimizations" only obstruct them.

---

## 24.2 Mechanical Sympathy: The Memory Hierarchy

The dominant fact of modern performance: **the CPU is starved for data**. A cache
miss to main memory costs ~100 ns — hundreds of instructions' worth of stall.
Performance is mostly about feeding the cache.

```text
L1 ~1 ns  ·  L2 ~4 ns  ·  L3 ~15 ns  ·  RAM ~100 ns   (≈100× cliff to RAM)
```

The practical consequence: **contiguous, cache-friendly data beats clever
algorithms with poor locality.**

```cpp
// std::vector (contiguous) vs std::list (pointer-chasing): for traversal/search,
// vector wins by 10–50× even when Big-O says otherwise — every list node is a
// likely cache miss, while vector streams through cache lines.
std::vector<int> v;     // contiguous  -> cache-friendly, prefetchable  ◀ default
std::list<int>   l;     // node-per-element -> a cache miss per hop      ◀ rarely right
```

> **Data-oriented design:** lay out data by *how it's accessed*, not by OO entity
> boundaries. Struct-of-arrays (SoA) often beats array-of-structs (AoS) because a
> hot loop touches one field across many elements — SoA packs that field
> contiguously. This is the core insight behind high-performance C++.

---

## 24.3 Benchmarking Correctly

Hand-rolled timing lies — the optimizer deletes unused work, and noise swamps small
differences. Use **Google Benchmark**, which warms up, iterates adaptively, and
provides `DoNotOptimize` to defeat dead-code elimination:

```cpp
#include <benchmark/benchmark.h>

static void BM_VectorSum(benchmark::State& state) {
    std::vector<int> v(state.range(0), 1);
    for (auto _ : state) {                          // the timed loop, auto-iterated
        long sum = std::accumulate(v.begin(), v.end(), 0L);
        benchmark::DoNotOptimize(sum);              // stop the optimizer eliding it
    }
}
BENCHMARK(BM_VectorSum)->Range(1<<10, 1<<20);       // sweep input sizes
BENCHMARK_MAIN();
```

- **Always benchmark a Release build** on a quiet machine; pin to a core
  (`taskset`), disable turbo/frequency scaling, and report a distribution, not one
  number.
- A microbenchmark can be 10× off from real workload behavior (cache state, data
  sizes) — confirm with an end-to-end measurement.

---

## 24.4 Profiling

Find *where* time goes before changing anything:

- **`perf`** (Linux): `perf record ./app && perf report`; turn it into a **flame
  graph** to see the hot call stack at a glance. Low overhead, sampling-based —
  usable on production builds.
- **VTune / Instruments / `perf stat`** expose hardware counters: cache-miss rate,
  branch-misprediction rate, IPC — the *why* behind a hotspot.
- **Compiler Explorer (godbolt.org)** to inspect the actual assembly for a hot
  function — did it vectorize? did the call inline?

```bash
perf stat ./app        # cache-misses, branch-misses, instructions-per-cycle
perf record -g ./app && perf report   # sampled call-graph profile
```

A high cache-miss rate points at **layout** (24.2); a high branch-miss rate points
at unpredictable branches (24.5).

---

## 24.5 Lower-Level Levers

Once layout is right, the finer levers:

- **Branch prediction:** unpredictable branches stall the pipeline (~3–15 ns each).
  Sort data to make branches predictable, or go **branchless** (e.g. `std::min`,
  arithmetic masks). `[[likely]]`/`[[unlikely]]` (C++20) hint the compiler.
- **Inlining & `constexpr`:** small hot functions should inline (the compiler
  usually decides; keep them in headers / mark `constexpr` to compute at compile
  time, Chapters 19–20).
- **Move semantics & avoiding copies** (Chapters 4, 15): pass by `const&` or
  `string_view`/`span`, `reserve()` vectors, `emplace_back` over `push_back`, and
  return by value (guaranteed copy elision, Chapter 19).
- **SIMD/vectorization:** prefer letting the compiler auto-vectorize (clean
  contiguous loops, `-O3 -march=native`); reach for intrinsics or `std::simd`
  (future) only for verified hot kernels.
- **Allocation is expensive:** `new`/`malloc` hit the allocator and hurt locality.
  Reuse buffers, use arena/pool allocators or PMR (`std::pmr`) for many small
  objects, and prefer stack/`std::array` for small fixed sizes.

---

## 24.6 Concurrency for Throughput

Chapters 16 and 18 cover the primitives and memory model; for *performance*:

- **Parallel algorithms** (C++17): `std::sort(std::execution::par, ...)` parallelizes
  with one argument — the easiest multicore win.
- **Thread-per-core + work stealing** (e.g. Intel TBB, a task scheduler) beats
  thread-per-task for CPU-bound work; threads ≈ cores, not thousands.
- **False sharing** is a top concurrency-perf bug: two threads writing different
  variables that share one 64-byte cache line ping-pong it between cores. Pad/align
  hot per-thread data to a cache line (`alignas(std::hardware_destructive_interference_size)`).
- **Lock-free** (Chapter 18, CAS) avoids lock contention but is *extremely* hard to
  get right — measure that a lock is actually the bottleneck first; a well-placed
  `std::mutex` is often faster and always safer.

---

## 24.7 Tail Latency

For services, the number that matters is a **high percentile**, not the average
(the same principle as managed languages, minus GC):

- Report and target **p50/p95/p99/p999**. In C++ the tail is driven by allocation
  spikes, lock contention, page faults, and NUMA effects — not a garbage collector.
- **Fan-out amplifies the tail:** a request hitting 100 services waits on the
  slowest; each service's p99 becomes the typical end-to-end latency.
- Bound the tail with **preallocation** (no `malloc` on the hot path), **huge pages**
  / NUMA-aware allocation, and avoiding lock contention.

---

## 24.8 Summary

| Lever | Impact |
|---|---|
| **Measure first (Release build)** | avoid optimizing the wrong thing / a Debug build |
| **Memory layout / cache locality** | usually the biggest win — contiguous > clever |
| **Google Benchmark + `DoNotOptimize`** | benchmarks that don't lie |
| **perf / VTune flame graphs + counters** | find hotspots and *why* (cache/branch) |
| Branchless, inlining, move/no-copy, SIMD | finer gains after layout is right |
| Parallel algorithms, avoid false sharing | multicore throughput |
| Preallocation, percentiles | bound **tail latency** |

- C++ performance is **mechanical sympathy**: feed the cache, minimize allocation,
  keep branches predictable — *measured* on a Release build with real tools.
- Trust the optimizer; change **data layout** before code; reach for lock-free/SIMD
  only when the profiler proves it's worth the complexity.

## Next Steps

- Benchmark `std::vector` vs `std::list` traversal with Google Benchmark and explain
  the gap with `perf stat` cache-miss counts.
- Flame-graph a real workload with `perf` and attack the top frame.
- Convert a hot AoS structure to SoA and re-measure.
- Continue to **[Chapter 25: Production & Operational Concerns](../25_production_operational/README.md)**.
