# Chapter 26: Data Validation & Serialization

At every boundary — network messages, config files, RPC payloads, saved state —
data arrives **untrusted and untyped**, and in C++ a parsing mistake is not a
caught exception but potential **memory corruption** (Chapter 27). This chapter
covers turning external bytes into validated, typed C++ objects and back: JSON
(nlohmann/json, simdjson), binary formats (Protobuf, FlatBuffers, Cap'n Proto),
schema evolution, and error handling with `std::expected`. The theme: **parse,
don't validate** — convert input to a well-typed object once, at the edge, then
trust it.

> **C++ raises the stakes:** because there's no runtime memory safety, an
> unvalidated length field or a buffer over-read in a parser is a security
> vulnerability, not a stack trace. Validate at the boundary, and fuzz your parsers
> (Chapter 22).

---

## 26.1 "Parse, Don't Validate"

Convert untrusted input once into a typed object whose existence guarantees
validity; downstream code receives a `User`, not a `json` blob, and never
re-checks.

```text
   untrusted bytes          parse + validate at boundary       trusted object
  JSON / RPC / file   ──▶   reject if malformed/oversized  ──▶  User{...}
```

Two directions: **deserialization** (bytes → object, reject invalid) and
**serialization** (object → bytes for transport/storage).

---

## 26.2 JSON with nlohmann/json

The most ergonomic C++ JSON library — convenient, header-only, with automatic
conversion to/from your types:

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct User { int id; std::string name; };
// Opt in to (de)serialization for your type:
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(User, id, name)

json j = json::parse(input, /*cb=*/nullptr, /*allow_exceptions=*/true);
User u = j.get<User>();                          // typed extraction
std::string out = json{{"id", u.id}, {"name", u.name}}.dump();
```

- **Validate, don't trust:** check required fields and ranges after parsing; use
  `.value("key", default)` for optionals and handle `json::parse` failure.
- **Bound the input** before parsing (max size, max nesting depth) — a deeply nested
  JSON can exhaust the stack (a billion-laughs analog, Chapter 27).

> **nlohmann/json is convenient but not the fastest.** For high-throughput parsing,
> **simdjson** parses gigabytes/second using SIMD (Chapter 24); **RapidJSON** is a
> fast middle ground. Pick by whether parsing is on your hot path.

---

## 26.3 Schema Validation

For untrusted JSON against a contract, validate with **JSON Schema** (e.g.
`json-schema-validator`) rather than hand-rolled checks — declarative, and the
schema doubles as documentation:

```cpp
// Validate parsed JSON against a JSON Schema before using it.
json_validator validator;
validator.set_root_schema(schema);              // types, required, ranges, patterns
validator.validate(parsed);                      // throws with a precise path on failure
```

This is the C++ analogue of Bean Validation / Pydantic — push field constraints
into a declarative schema checked at the boundary.

---

## 26.4 Binary and Interchange Formats

JSON is human-readable but verbose and slow. For performance, size, or strong
cross-language contracts, use a schema'd binary format:

| Format | Schema | Notes |
|---|---|---|
| **JSON** | no | universal, human-readable, slow/large |
| **Protocol Buffers** | yes (`.proto`) | typed, compact, fast; gRPC default (Chapter 30) |
| **FlatBuffers / Cap'n Proto** | yes | **zero-copy**: read fields without a parse step |
| **MessagePack** | no | compact binary "JSON" |

```protobuf
// user.proto — compiled by protoc into C++ classes.
syntax = "proto3";
message User {
  int32  id   = 1;          // field NUMBERS are the contract, not names
  string name = 2;
  repeated string tags = 3;
}
```

```cpp
User u; u.set_id(42); u.set_name("Ada");
std::string bytes = u.SerializeAsString();       // compact wire format
User parsed; parsed.ParseFromString(bytes);      // type-checked deserialization
```

> **Zero-copy formats matter in C++.** FlatBuffers/Cap'n Proto let you access fields
> directly in the received buffer with no parse/allocation step — a real win for
> latency-critical paths (game state, HFT, IPC) where Protobuf's
> serialize/deserialize cost shows up. Trade-off: a less convenient API and a fixed
> wire layout.

---

## 26.5 Schema Evolution

Data outlives code; design wire formats to change without breaking either side
(Chapter 28/30):

- **Backward compatible**: new code reads old data (add fields with defaults).
- **Forward compatible**: old code reads new data (ignore unknown fields).
- **Full**: both — the target for independently-deployed producers/consumers.

**Protobuf** identifies fields by **number**: *never reuse or renumber* a field —
mark removed ones `reserved`; unknown fields are preserved on round-trip. A safe
change adds an optional field; renaming or retyping is breaking. For Kafka-style
streams, a **schema registry** (Chapter 29) enforces compatibility at publish time.

---

## 26.6 Error Handling: Exceptions vs `std::expected`

Validation needs an error strategy. C++ offers two (Chapter 13, 20):

```cpp
// Exception style: throw on invalid input; callers use try/catch.
User parse_or_throw(std::string_view s);          // throws std::runtime_error

// std::expected style (C++23): value-based, explicit, no exceptions on the hot path.
#include <expected>
std::expected<User, ParseError> parse(std::string_view s) {
    if (s.empty()) return std::unexpected(ParseError::Empty);   // error path
    // ... validate ...
    return User{...};                                            // success path
}

if (auto u = parse(input)) use(*u);
else log(u.error());                              // explicit, branch-checked handling
```

- **Exceptions** suit truly exceptional, rare failures and keep the happy path
  clean. They have ~zero cost when not thrown but a real cost when thrown — avoid
  them for *expected* failures in hot loops.
- **`std::expected`** suits *expected* failures (parse/validation), forces callers to
  handle the error at the type level (like Rust's `Result`), and avoids exception
  overhead on the error path. It's increasingly the idiom for validation results.

---

## 26.7 Determinism and Safety

- **Canonical serialization** — for signing, content-addressing, or cache keys you
  need byte-identical output for equal data: sort keys, fix number formatting, pin
  field order.
- **Deserialization is an attack surface** (Chapter 27): enforce **size and depth
  limits** before parsing untrusted input, validate every length/offset field, and
  **fuzz the parser** (Chapter 22). Never `memcpy` based on an unvalidated length
  from the wire.

---

## 26.8 Summary

| Need | Tool |
|---|---|
| Convenient JSON | nlohmann/json (`NLOHMANN_DEFINE_TYPE_*`) |
| Fastest JSON | simdjson (SIMD), RapidJSON |
| Declarative validation | JSON Schema validator |
| Typed cross-language contract | Protobuf (+ gRPC, Chapter 30) |
| Zero-copy / lowest latency | FlatBuffers / Cap'n Proto |
| Error handling | exceptions (rare) vs **`std::expected`** (expected failures) |

- **Parse, don't validate** at every boundary — and in C++ that boundary is a
  *security* boundary: validate lengths, bound size/depth, and fuzz parsers.
- Choose the format by need: nlohmann/simdjson for JSON, **Protobuf** for typed
  contracts, **zero-copy** formats for latency-critical paths; design for **schema
  evolution**.
- Prefer **`std::expected`** for expected validation failures and exceptions for
  truly exceptional ones.

## Next Steps

- Define a type, add nlohmann/json conversion, and validate it against a JSON Schema.
- Rewrite a throwing parser to return `std::expected` and compare call-site clarity.
- Add a libFuzzer harness (Chapter 22) to a binary parser and validate every wire
  length field.
- Continue to **[Chapter 27: Security & Supply Chain](../27_security_supply_chain/README.md)**.
