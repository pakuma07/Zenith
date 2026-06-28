# Chapter 25: Production & Operational Concerns

C++ powers latency-critical backends — trading systems, databases, game servers, ad
exchanges — that must run reliably for years. Operating a C++ service has the same
shape as any other (config, logging, observability, graceful shutdown, resilience),
but with C++-specific edges: there is no GC to hide leaks, a crash is a `SIGSEGV`
not a stack trace, and the binary owns its own memory and signals. This chapter is
the operational layer for production C++.

> **C++'s operational double-edge:** no GC pauses and tiny, predictable footprints
> (great for tail latency, Chapter 24) — but a memory bug is a *crash or
> corruption*, not an exception. Observability and the safety tooling of Chapter 22
> are how you keep a native service trustworthy.

---

## 25.1 The Twelve-Factor App (C++ Edition)

| Factor | C++ specifics |
|---|---|
| **Config** | from environment / parsed config file into a typed struct (25.2) |
| **Dependencies** | pinned via vcpkg/Conan (Chapter 22); static-link for hermetic deploys |
| **Backing services** | DB/cache/queue clients as attachable URLs |
| **Processes** | stateless; no shared in-process state across instances |
| **Port binding** | the binary *is* the server (embed an HTTP/RPC server) |
| **Concurrency** | scale out with instances; thread-per-core within (Chapter 24) |
| **Disposability** | fast startup (native — no VM warmup), graceful shutdown (25.5) |
| **Logs** | event stream to **stdout** (25.3) |

C++'s native compilation gives **instant startup** (no JVM warmup, Chapter 24's
Java contrast) and **small memory footprint** — real advantages for scale-to-zero
and dense packing.

---

## 25.2 Configuration

Read config **once** at startup into an immutable, validated struct — never scatter
`getenv` through the code:

```cpp
struct Config {
    std::string database_url;
    uint16_t    port        = 8080;
    int         log_level   = 2;
};

Config load_config() {
    Config c;
    if (const char* u = std::getenv("DATABASE_URL")) c.database_url = u;
    else throw std::runtime_error("DATABASE_URL is required");      // fail fast
    if (const char* p = std::getenv("PORT")) c.port = parse_port(p);
    return c;                                                        // immutable after
}
```

Use a real config library for anything non-trivial — **TOML** (toml++), **YAML**
(yaml-cpp), or JSON (Chapter 26) — and **validate at startup** so a misconfigured
deploy fails immediately, not at first request. Secrets come from the environment /
a secret manager (Chapter 27), never the binary or repo.

---

## 25.3 Structured Logging

Emit **structured (JSON) logs to stdout** with context, so they're queryable at
scale. **spdlog** is the de-facto fast logging library:

```cpp
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

auto log = spdlog::stdout_logger_mt("app");
log->set_pattern(R"({"ts":"%Y-%m-%dT%H:%M:%S.%e","level":"%l","msg":"%v"})");
log->set_level(spdlog::level::info);

log->info("order placed id={} sku={}", order.id, order.sku);   // async, level-gated
```

- **spdlog is fast and asynchronous** — formatting is deferred and level-gated, so a
  disabled `debug` call costs nearly nothing (the C++ analogue of lazy logging).
- Attach a **correlation/trace id** to every line (a thread-local or context field)
  so one request is traceable across services.
- Never log secrets/PII; log at appropriate levels (`info` business events, `warn`
  recoverable, `error` failures).

---

## 25.4 Observability: Metrics, Traces, Health

- **Metrics:** the **prometheus-cpp** client exposes a `/metrics` endpoint for
  scraping — counters, gauges, histograms (use histograms for latency percentiles,
  Chapter 24).
- **Traces:** **OpenTelemetry C++** provides distributed tracing; propagate the
  trace context (`traceparent`) across service calls (Chapter 28).
- **Crash diagnostics (C++-specific):** install a signal handler to dump a
  **stacktrace** on `SIGSEGV`/`SIGABRT` (C++23 `std::stacktrace`, or Boost.Stacktrace),
  and collect **core dumps**. A native crash gives you no automatic traceback — you
  must arrange one.

```cpp
#include <prometheus/counter.h>            // prometheus-cpp
auto& requests = prometheus::BuildCounter()
    .Name("http_requests_total").Register(*registry)
    .Add({{"method","GET"}, {"status","200"}});
requests.Increment();
```

> **Bounded metric cardinality** is just as important in C++: never use unbounded
> label values (user id, request id) — they explode the time series. Define **SLOs**
> and measure the **golden signals** (latency, traffic, errors, saturation), same as
> any service.

### Health checks

Expose **liveness** (process healthy — restart if not) and **readiness** (can serve
traffic — drain if not) endpoints, so an orchestrator restarts a hung process but
merely drains one whose database is briefly unreachable.

---

## 25.5 Graceful Shutdown and Signals

A C++ service must handle `SIGTERM` to drain in-flight work before exit. **Signal
handlers are extremely restricted** — only async-signal-safe operations are legal
inside them — so the correct pattern is to set an atomic flag and act in the main
loop:

```cpp
#include <atomic>
#include <csignal>
std::atomic<bool> g_stop{false};

extern "C" void on_signal(int) { g_stop.store(true); }   // ONLY async-signal-safe ops

int main() {
    std::signal(SIGTERM, on_signal);
    std::signal(SIGINT,  on_signal);
    while (!g_stop.load()) {
        serve_one_request();          // finish current work
    }
    drain_and_close();                // close pools, flush logs/metrics, then exit 0
}
```

The container drain sequence is the same as any service: flip **readiness** to fail,
finish in-flight requests within the grace period, then exit — and ensure the
process is **PID 1 signal-aware** (exec form / `tini`) so `SIGTERM` reaches it.

---

## 25.6 Containerizing C++

C++ containers can be remarkably small — a key operational advantage:

```dockerfile
# Multi-stage: build with the toolchain, ship only the binary on a minimal base.
FROM gcc:14 AS build
WORKDIR /src
COPY . .
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j

FROM gcr.io/distroless/cc-debian12        # tiny, no shell, fewer CVEs
COPY --from=build /src/build/app /app
USER nonroot
ENTRYPOINT ["/app"]                        # exec form -> signals reach the process
```

- **Static linking** (or carefully bundled `.so`s, Chapter 21) makes the binary
  self-contained — pair with a **distroless** or `scratch` base for a minimal attack
  surface (Chapter 27) and tiny images.
- Set **container memory/CPU limits**; a C++ process exceeding the memory limit is
  **OOMKilled** by the kernel. Without a GC, unbounded growth is a *leak* — find it
  with ASan/Valgrind (Chapter 22), not by tuning a heap.

---

## 25.7 Resilience

The same patterns as any distributed service (Chapter 28), implemented in C++:

| Pattern | Purpose |
|---|---|
| **Timeouts** | bound every remote call (set socket + RPC deadlines) |
| **Retries + backoff + jitter** | survive transient failures without a thundering herd |
| **Circuit breaker** | stop hammering a dead dependency; fail fast |
| **Bulkhead** | isolate thread/connection pools per dependency |
| **Idempotency** | make retried writes safe (Chapter 28) |

C++ RPC frameworks (gRPC C++, Chapter 30) provide deadlines, cancellation, and
retry policies; wrap dependencies with these rather than calling raw and hoping.

---

## 25.8 Summary

| Concern | C++ approach |
|---|---|
| Config | parse once into a validated immutable struct; fail fast |
| Logging | **spdlog** structured JSON to stdout, async, with trace ids |
| Metrics/traces | prometheus-cpp + OpenTelemetry C++; bounded cardinality; SLOs |
| Crash diagnostics | signal handler + `std::stacktrace`/Boost.Stacktrace, core dumps |
| Shutdown | `SIGTERM` -> atomic flag -> drain in main loop (async-signal-safe) |
| Containers | multi-stage + **distroless/static**; mind OOMKilled (= leak) |
| Resilience | timeouts, retries+backoff, circuit breakers, idempotency |

- C++ services start instantly and run lean (great for tail latency), but a bug is a
  **crash, not an exception** — invest in crash diagnostics, observability, and the
  Chapter 22 safety tooling.
- Config, structured logging, metrics/traces/health, graceful `SIGTERM` handling,
  and resilience are the same disciplines as any service — with C++-specific signal
  and memory caveats.

## Next Steps

- Add spdlog JSON logging and a prometheus-cpp `/metrics` endpoint to a service.
- Implement a `SIGTERM` atomic-flag drain loop and verify in-flight work completes.
- Build a distroless multi-stage image and compare its size to a full-base image.
- Continue to **[Chapter 26: Data Validation & Serialization](../26_data_validation_serialization/README.md)**.
