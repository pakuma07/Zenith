# Chapter 30: API & Interface Design

An API is a **contract** — the most expensive thing you ship, because once others
depend on it you can rarely take it back. This chapter covers designing interfaces
that are correct, evolvable, and pleasant: C++ library APIs (headers, the ABI), and
network APIs (REST, gRPC) between services. In C++ the contract has an extra layer —
the **ABI** (Chapter 21) — so "designing for change" means source *and* binary
compatibility. The staff skill is shipping a v1 that grows to v5 without breaking
callers.

> **Hyrum's Law:** with enough users, every observable behavior of your interface
> will be depended upon — including behaviors you never promised. In C++ that
> includes struct layout and exception specifications, not just function names.

---

## 30.1 Principles of Good Interface Design

Apply equally to a function signature and a service endpoint:

- **Easy to use correctly, hard to use incorrectly** (Scott Meyers' maxim, born in
  C++). Make the common case the default; make the dangerous case require effort.
- **Small surface area** — every public symbol is a forever-commitment; expose the
  minimum, keep the rest in an unnamed namespace / `detail` namespace / `.cpp`.
- **Least astonishment** and **consistency** — same concepts, names, parameter order
  throughout.
- **Express intent in the type system** — the C++ superpower.

```cpp
// Use types to make misuse impossible. A raw (ptr, len) invites overruns; a span
// (Chapter 20) carries its own bounds. Strong types prevent argument swaps.
void send(std::span<const std::byte> data);          // not (const void*, size_t)
struct Port { uint16_t v; };  struct Timeout { std::chrono::milliseconds v; };
Connection connect(Host h, Port p, Timeout t);       // can't swap port and timeout
```

---

## 30.2 Designing C++ Library APIs

- **Header = the contract.** Put declarations in the header, definitions in `.cpp`;
  keep implementation details out of public headers (faster builds, stable ABI).
- **RAII ownership in the signature** (Chapters 12, 13): take/return `unique_ptr`/
  `shared_ptr` to express ownership transfer; take `const&`/`span`/`string_view` for
  borrowed inputs. Ownership is *visible in the type*.
- **Const-correctness and `[[nodiscard]]`** (Chapter 19): mark observers `const`, and
  results that must be checked `[[nodiscard]]`.
- **Return `std::optional`/`std::expected`** (Chapter 20) instead of sentinels or
  out-params for "maybe"/"or-error".
- **Concepts** (Chapter 20) constrain template APIs with readable requirements and
  good error messages.

```cpp
class [[nodiscard]] FileHandle {                 // can't accidentally ignore the result
public:
    static std::expected<FileHandle, std::error_code> open(std::string_view path);
    std::span<const std::byte> read(size_t n);   // borrowed view out
    // move-only: ownership is unique and explicit (Chapters 4, 12)
    FileHandle(FileHandle&&) noexcept;
    FileHandle(const FileHandle&) = delete;
};
```

> **ABI stability is part of the C++ contract** (Chapter 21). For a *shipped shared
> library*, use **pImpl** to hide layout, and expose a **C ABI** (`extern "C"` + POD
> structs) for a truly stable plugin boundary — C++ name mangling and layout are not
> portable across compilers/versions.

---

## 30.3 Deprecation, Not Deletion

Evolve a published API in stages, like the standard library does:

```cpp
[[deprecated("use open() instead; removed in 3.0")]]
FileHandle openFile(const char* path);           // compiler warns every caller
```

Mark the old API `[[deprecated]]` with the replacement and removal version, keep it
working for at least one release, and remove only on a **major version** (and, for a
library, a soname bump — Chapter 21).

---

## 30.4 REST API Design

For network services, REST models the domain as **resources** (nouns) with HTTP
**methods** (verbs). C++ frameworks: Drogon, Crow, Pistache, oat++.

```text
GET    /orders        list   (safe, idempotent, cacheable)
POST   /orders        create (not idempotent -> idempotency key)
GET    /orders/{id}   fetch one
PUT    /orders/{id}   replace (idempotent)
DELETE /orders/{id}   delete  (idempotent)
```

- **Resource-oriented URLs** (plural nouns); verbs live in the method.
- **Correct status codes:** `200/201/204`; `400/401/403/404/409/422/429`; `500/503`.
  Don't return `200` with an error body — proxies and clients rely on the code.
- **Method semantics:** `GET` safe + cacheable; `PUT`/`DELETE` idempotent; `POST`
  neither — protect with an **idempotency key** (Chapter 28).
- **Cursor pagination** over offset; consistent error envelope (RFC 9457).

---

## 30.5 RPC and gRPC

For action-oriented service-to-service calls, **gRPC** is the common choice and a
natural fit for C++: Protobuf (Chapter 26) over HTTP/2, generated C++ stubs,
streaming, small fast payloads, built-in deadlines/cancellation (Chapter 25).

```protobuf
service OrderService {
  rpc CreateOrder (CreateOrderRequest) returns (Order);
  rpc WatchOrders (WatchRequest) returns (stream Order);   // server streaming
}
```

| | REST/JSON | gRPC/Protobuf |
|---|---|---|
| Payload | text, large | binary, small, fast |
| Streaming | limited | first-class bi-directional |
| Contract | OpenAPI (often after) | `.proto` (contract-first, codegen) |
| Best for | public/partner, browsers | internal high-throughput (C++'s sweet spot) |

The staff pattern is often **both**: gRPC between internal C++ services for speed and
a typed contract; REST at the public edge for reach.

---

## 30.6 Versioning and Backward Compatibility

The heart of API design — change the contract without breaking callers. Classify
every change:

- **Backward-compatible:** add an optional field/endpoint, add a tolerated enum value,
  relax a constraint.
- **Breaking:** remove/rename a field, change a type, make optional required, change
  semantics — **or, in C++, change struct layout / add a virtual / change a signature
  in a shipped library** (an ABI break, Chapter 21).

Strategies: **additive evolution** (the Protobuf philosophy, Chapter 26 — design so
most changes are backward-compatible), **tolerant reader** (ignore unknown fields),
URL/header versioning for network APIs, and **pImpl / C ABI** for binary stability.
When you must break: run versions in parallel, announce a deprecation timeline, and
instrument old-version usage to know when it's safe to retire.

> **Consumer-driven contract tests** (Pact, Chapter 31) verify a provider never
> breaks the shape a consumer depends on — catching breaking changes in CI, across
> team boundaries, before deploy.

---

## 30.7 Auth, Rate Limiting, and Docs

- **AuthN/AuthZ** (Chapter 27): validate tokens/JWTs with explicit algorithms and
  claims; enforce authorization **server-side on every request** — never trust the
  client.
- **Rate limiting:** token-bucket per client; return `429` with `Retry-After` so
  well-behaved clients back off (Chapter 28 load shedding).
- **Docs as source of truth:** OpenAPI for REST, `.proto` for gRPC — humans and
  codegen both consume them; document errors, idempotency, pagination, and auth, not
  just the happy path.

---

## 30.8 Summary

| Layer | Key practices |
|---|---|
| Principles | small, consistent, hard-to-misuse, type-expressed intent |
| C++ library API | header = contract; RAII ownership in signatures; `[[nodiscard]]`; `optional`/`expected`; concepts |
| **ABI** | pImpl / C ABI for stable shipped-library boundaries (Chapter 21) |
| Deprecation | `[[deprecated]]` → one release → remove on major + soname bump |
| Network | REST (resources, status codes, cursors) vs gRPC (typed, fast, streaming) |
| Evolution | additive changes, tolerant reader, parallel versions, contract tests |
| Cross-cutting | server-side authZ, `429` rate limiting, OpenAPI/`.proto` docs |

- An API is a **forever contract**; in C++ it includes the **ABI** — design for both
  source and binary evolution.
- Express intent in the **type system**, own resources via **RAII**, and return
  `optional`/`expected`; stage **deprecations**.
- For services, choose **gRPC** for fast internal contracts and **REST** at the edge;
  evolve **additively** and guard boundaries with **contract tests**.

## Next Steps

- Add `[[nodiscard]]` and `std::expected` returns to a library API and observe the
  caller-side safety.
- Convert a shipped class to **pImpl** and confirm you can change internals without
  recompiling callers.
- Define a `.proto` service and generate C++ gRPC stubs; add deadlines (Chapter 25).
- Continue to **[Chapter 31: Engineering Practice at Scale](../31_engineering_practice/README.md)**.
