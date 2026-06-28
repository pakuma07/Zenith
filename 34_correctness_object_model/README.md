# 34 — Correctness: Exception Safety, the Object Model & Undefined Behavior

The earlier chapters taught you to *write* C++; this one teaches you to be *sure it is correct* under exceptions, optimization, and the realities of the object model. These are the topics a reviewer keeps finding scattered and half-stated: "what happens to my container if the copy constructor throws?", "is this `dynamic_cast` cheap enough for the hot path?", "why does the diamond compile but corrupt state?", and the big one — "it works at `-O0` but breaks at `-O2`." That last symptom is almost always **undefined behavior** (UB), and a principal must carry a mental catalog of it. This chapter formalizes the four exception-safety guarantees, dissects RTTI and virtual inheritance at the layout level, and gives you a systematic UB checklist with Symptom/Cause/Fix throughout.

Builds on [Chapter 7: Inheritance & Polymorphism](../07_inheritance_polymorphism/README.md), [Chapter 13: Exception Handling](../13_exception_handling/README.md), and [Chapter 18: Memory Model](../18_memory_model/README.md). Compile with `g++ -std=c++23 -Wall -Wextra`.

---

## 34.1 The Four Exception-Safety Guarantees

When a function can throw, it must still leave the program in a *defined* state. The standard library — and your own APIs — classify every operation into one of four levels.

| Guarantee | Promise on exception | Typical mechanism | Example |
|---|---|---|---|
| **No-throw** | Never throws; always succeeds | `noexcept`, no allocation | `swap`, destructors, `vector::size()` |
| **Strong** | Commit-or-rollback: state is *exactly* as before the call | copy-and-swap, build-then-commit | `vector::push_back`, `v = w` |
| **Basic** | Invariants preserved, no leaks, but state *unspecified* | RAII for cleanup | `sort` after a throwing comparator |
| **None** | No promise — may leak or corrupt | (legacy / to be avoided) | a raw `new` with no cleanup |

```cpp
// No-throw: noexcept is enforced — std::terminate is called if you lie
void clear_flags(int& f) noexcept { f = 0; }

// Basic guarantee: if push_back throws, v leaks nothing and stays usable,
// but whether the element was added is unspecified by THIS function.
void append_basic(std::vector<int>& v, int x) { v.push_back(x); }  // may throw bad_alloc
```

The default target for any non-trivial mutating operation is the **strong guarantee**; reach for **basic** only when strong is too expensive, and treat **none** as a bug.

---

## 34.2 Copy-and-Swap: the Strong Guarantee in One Idiom

The strong guarantee means "if anything throws, it is as if you never called me." The canonical way to get it is **copy-and-swap**: do all the throwing work on a *temporary*, then commit with a **no-throw** `swap`.

```cpp
class Buffer {
    char*  data_;
    size_t size_;
public:
    ~Buffer() { delete[] data_; }

    // ❌ WRONG: if `new` throws AFTER delete[], data_ dangles and *this is
    // corrupt — even the basic guarantee is broken.
    Buffer& operator=(const Buffer& rhs) {
        delete[] data_;                     // commit destruction first...
        data_ = new char[rhs.size_];        // ...then maybe throw. Disaster.
        std::memcpy(data_, rhs.data_, rhs.size_);
        size_ = rhs.size_;
        return *this;
    }

    // ✅ RIGHT: copy-and-swap. All throwing work happens on a copy.
    friend void swap(Buffer& a, Buffer& b) noexcept {   // no-throw swap
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }
    // By value: the copy ctor does the (throwing) allocation. If it throws we
    // never enter the body — *this is untouched (strong guarantee).
    Buffer& operator=(Buffer rhs) noexcept {
        swap(*this, rhs);                   // no-throw commit
        return *this;                       // rhs (old data) dies here
    }
};
```

The pattern generalizes: **(1) build the new state in a local that owns its resources, (2) `swap` it into place, (3) let the old state destruct on scope exit.** The only `noexcept` requirement is the swap itself.

---

## 34.3 Why `noexcept` on Move Matters

`std::vector` grows by allocating a bigger block and *relocating* the elements. If element moves can throw, a half-finished relocation would leave the vector corrupt with no way to roll back — so the library refuses to use move and **falls back to copy** unless your move operations are `noexcept`.

```cpp
struct Slow { Slow(Slow&&) {} };            // move ctor NOT noexcept
struct Fast { Fast(Fast&&) noexcept {} };   // move ctor noexcept

static_assert( std::is_nothrow_move_constructible_v<Fast>);
static_assert(!std::is_nothrow_move_constructible_v<Slow>);

// On reallocation, the library uses std::move_if_noexcept internally:
//   vector<Slow> → elements are COPIED (silent perf cliff; preserves strong guarantee)
//   vector<Fast> → elements are MOVED  (fast, and still strong: moves can't throw)
```

The rule for principals: **mark move constructors and move assignment `noexcept`** (the defaulted ones usually are, if all members are). Verify with `static_assert(std::is_nothrow_move_constructible_v<T>)`.

- **Symptom:** A `vector<MyType>` is mysteriously slow to grow; profiling shows copy constructors firing on `push_back`.
- **Cause:** `MyType`'s move constructor is not `noexcept`, so the strong-guarantee relocation falls back to copy.
- **Fix:** Add `noexcept` to the move ops (or `= default` them if members allow), and assert it.

This is the direct interaction with **RAII** ([Chapter 12](../12_memory_management/README.md)): RAII gives you no-throw destruction; `noexcept` moves let the library *trust* it. Together they make the basic guarantee nearly automatic and the strong guarantee cheap.

---

## 34.4 RTTI: `typeid`, `type_info`, and `dynamic_cast`

Run-Time Type Information lets you query the dynamic type of a polymorphic object. It works only when there is a vtable to consult — i.e. at least one `virtual` function ([Chapter 7](../07_inheritance_polymorphism/README.md)).

```cpp
#include <typeinfo>
#include <iostream>
using namespace std;

struct Animal { virtual ~Animal() = default; };   // virtual → polymorphic
struct Dog : Animal { void bark() {} };
struct Cat : Animal {};

void inspect(Animal& a) {
    // typeid on a polymorphic lvalue returns the DYNAMIC type
    const type_info& ti = typeid(a);
    cout << ti.name() << '\n';          // mangled, e.g. "3Dog" — demangle for humans

    // dynamic_cast on a REFERENCE: throws std::bad_cast on failure
    try { Dog& d = dynamic_cast<Dog&>(a); d.bark(); }
    catch (const std::bad_cast&) { cout << "not a Dog\n"; }

    // dynamic_cast on a POINTER: returns nullptr on failure (no throw)
    if (Dog* d = dynamic_cast<Dog*>(&a)) d->bark();
}
```

`dynamic_cast` does three jobs — **down-cast** (`base* → derived*`, the common, runtime-checked case), **cross-cast** (one base subobject → a sibling base of the same object), and the trivial **up-cast** (`derived* → base*`, which is implicit and free, so needs no cast):

```cpp
struct A { virtual ~A() = default; };
struct B { virtual ~B() = default; };
struct C : A, B {};

A* a = new C;
B* b = dynamic_cast<B*>(a);   // cross-cast: A-subobject → B-subobject of the same C
```

**Cost:** `dynamic_cast` walks the runtime type hierarchy; it is typically tens of nanoseconds — fine for setup, a code smell in a hot loop ([Chapter 24](../24_performance_engineering/README.md)). `typeid(x).name()` returns an *implementation-defined* mangled string; compare `type_info` objects with `==`, do not parse the name.

**When RTTI is the right tool:** plugin/object-graph traversal, deserialization dispatch, debugging/logging, and the rare genuine "is this *actually* a Derived?" check. **When to prefer something else:**

- A closed set of types → `std::variant` + `std::visit` (a compile-time-checked visitor).
- Performance-critical static dispatch → **CRTP** ([Chapter 17](../17_template_metaprogramming/README.md)).
- Hiding the concrete type behind a uniform interface → **type erasure** ([Chapter 30](../30_api_design/README.md)).

**Disabling RTTI** (`-fno-rtti`) shrinks the binary by dropping `type_info` tables, but `dynamic_cast` on polymorphic types and `typeid` both stop working. Common in embedded / game engines by convention — only do it if your design never needs them.

---

## 34.5 Virtual Inheritance & the Diamond Problem

Multiple inheritance can produce a *diamond*, where a base appears twice in the derived object. Without `virtual`, you get two copies and ambiguity.

```cpp
struct Stream { int pos = 0; };
struct InStream  : Stream {};
struct OutStream : Stream {};

// ❌ WRONG: IOStream has TWO Stream subobjects (two `pos` members!)
struct IOStream : InStream, OutStream {};

IOStream io;
// io.pos;            // error: ambiguous — which Stream::pos?
// io.InStream::pos and io.OutStream::pos are DIFFERENT integers.
```

Mark the shared base `virtual` so all paths share **one** subobject:

```cpp
struct Stream { int pos = 0; };
struct InStream  : virtual Stream {};   // virtual base
struct OutStream : virtual Stream {};   // virtual base

// ✅ RIGHT: exactly one shared Stream subobject
struct IOStream : InStream, OutStream {
    // The MOST-DERIVED class (IOStream) is responsible for initializing
    // the virtual base — intermediate ctors' calls to Stream(...) are IGNORED.
    IOStream() : Stream(), InStream(), OutStream() {}
};

IOStream io;
io.pos = 5;          // unambiguous now — one pos
```

**Object layout & cost.** A non-virtual base sits at a fixed compile-time offset. A *virtual* base's location depends on the most-derived type, so the compiler adds indirection: each subobject stores a **virtual-base pointer / offset (`vbptr`)** — analogous to the vptr — and access through the virtual base costs an extra load.

```
Object with virtual base (conceptual):
  [ InStream  | vbptr ----+ ]
  [ OutStream | vbptr ----+ ]  both vbptrs point to →
  [ Stream (shared virtual base) ]  <- located via offset, not fixed
```

**Initialization order:** virtual bases are constructed **first** (before any non-virtual base), in left-to-right depth-first order, and only by the most-derived class. Destruction is the exact reverse.

- **Symptom:** A virtual base's constructor runs with arguments you did not pass / a "default" you didn't expect.
- **Cause:** An intermediate class tried to initialize the virtual base; that initializer is ignored — only the most-derived class's mem-initializer counts.
- **Fix:** Initialize the virtual base explicitly in the most-derived constructor.

**Prefer composition.** Virtual inheritance is the right answer for genuine "is-a *one shared* base" hierarchies (the classic iostream design), but for code reuse it is usually the wrong tool — it adds layout indirection, complicates initialization, and confuses readers. Favor **has-a composition** over a diamond whenever you can.

---

## 34.6 The Undefined Behavior Catalog

The single most important mental model: **the optimizer assumes UB never happens.** It is allowed to delete code, reorder it, or "time-travel" — to hoist an inference *backwards* past the point where the UB occurs. That is why UB famously "works in debug, breaks at `-O2`": at `-O0` the latent assumption is harmless; at `-O2` the optimizer cashes it in.

```cpp
// Time-travel illustration:
int* p = nullptr;
int  v = *p;            // UB: null deref
if (p) { /* ... */ }    // the compiler may DELETE this check — it "proved" p != null
                        // by assuming the deref above was valid. Both branches gone.
```

| # | UB | Why it's UB | What the optimizer may do |
|---|---|---|---|
| 1 | **Signed integer overflow** | `INT_MAX + 1` has no defined value | Assume `x + 1 > x` always; delete overflow checks |
| 2 | **Strict-aliasing violation** | Reading an object through an incompatible pointer type | Assume the two pointers never alias; reorder/cache loads |
| 3 | **Uninitialized read** | Indeterminate value | Treat the variable as any convenient value, even inconsistently |
| 4 | **Dangling pointer / use-after-free** | Object's lifetime ended | Assume the access can't happen; reuse the freed memory |
| 5 | **Out-of-bounds access** | Outside the object's bounds | Assume index is in range; remove bounds checks |
| 6 | **Null dereference** | No object at null | Assume the pointer is non-null everywhere downstream (and upstream!) |
| 7 | **Data race** | Unsynchronized access, one a write ([Ch 18](../18_memory_model/README.md)) | Assume no concurrent access; cache/reorder freely |
| 8 | **Sequencing violation** (`i = i++`) | Unsequenced read+write of `i` | Any result; may produce nonsense |
| 9 | **Invalid downcast** (`static_cast` to wrong type) | Object isn't really that type | Garbage member access, no diagnostic |
| 10 | **ODR violation** | Two differing definitions of one entity | Linker picks one arbitrarily; silent miscompile |
| 11 | **Returning ref/ptr to local** | Storage gone at return | Read of dead stack slot; "works" until it doesn't |
| 12 | **Mismatched new/delete** (`new[]`/`delete`) | Wrong deallocation form | Heap corruption |
| 13 | **Integer→pointer abuse** | Fabricated/misaligned pointer | Misaligned access, provenance loss |

```cpp
// (1) Signed overflow — UB. Use unsigned for wraparound, or check first.
int bad = INT_MAX + 1;                 // ❌ UB
bool ok = (a > 0 && b > INT_MAX - a);  // ✅ check before adding

// (8) Sequencing — UB before C++17 for some, still a bug to write
int i = 0;
i = i++;                               // ❌ UB / unspecified — never write this

// (11) Returning reference to local — dangling
const std::string& name() {
    std::string s = "temp";
    return s;                          // ❌ s dies; caller reads freed stack
}

// (12) Mismatched new/delete
int* arr = new int[10];
delete arr;                            // ❌ must be delete[] arr;  → heap corruption
```

### Strict aliasing — the subtle one

The compiler assumes a `float*` and an `int*` never point at the same bytes (they have incompatible types), so it may cache one across a write through the other.

```cpp
// ❌ WRONG: type-punning through reinterpret_cast violates strict aliasing
float bad(int bits)  { return *reinterpret_cast<float*>(&bits); }   // UB — stale read

// ✅ RIGHT: std::bit_cast (C++20) — well-defined, zero-cost
float good(int bits) { return std::bit_cast<float>(bits); }         // trivially copyable

// ✅ RIGHT (pre-C++20): std::memcpy — the optimizer recognizes & elides it
float copy(int bits) { float f; std::memcpy(&f, &bits, sizeof f); return f; }

// Escape hatch: -fno-strict-aliasing disables the assumption globally (perf cost).
// Prefer bit_cast/memcpy and keep strict aliasing ON.
```

- **Symptom:** A type-punning routine returns correct values at `-O0` and garbage at `-O2`.
- **Cause:** Strict-aliasing violation — the compiler cached the value across a write it assumed couldn't alias.
- **Fix:** Replace the `reinterpret_cast` pun with `std::bit_cast` (or `memcpy`).

---

## 34.7 Catching UB with Tooling

You cannot reason your way to UB-free code at scale; you instrument for it ([Chapter 22](../22_tooling_testing/README.md)).

```bash
# Sanitizers — run your test suite under these in CI:
g++ -std=c++23 -fsanitize=address,undefined -g app.cpp   # ASan + UBSan
g++ -std=c++23 -fsanitize=thread          -g app.cpp     # TSan (data races, Ch 18)

# Warnings catch many bugs before they run:
g++ -std=c++23 -Wall -Wextra -Wshadow -Wconversion app.cpp
```

| Tool | Catches | Catalog # |
|---|---|---|
| **ASan** | use-after-free, out-of-bounds, mismatched new/delete | 4, 5, 12 |
| **UBSan** | signed overflow, null deref, misaligned/bad casts, OOB | 1, 5, 6, 9, 13 |
| **TSan** | data races | 7 |
| **`-Wall -Wextra`** | uninitialized reads, returning local refs, dead code | 3, 11 |

A fourth lever: **`constexpr` evaluation surfaces UB at compile time.** The constant evaluator is required to reject UB, so forcing a computation into a `constexpr` context turns a runtime time-bomb into a build error.

```cpp
constexpr int overflow() { return INT_MAX + 1; }   // ❌ ill-formed: UB in constexpr
// constexpr int x = overflow();   // compile ERROR — UB caught at build time
```

---

## 34.8 const-Correctness in Depth

`const` is a contract about *who may mutate what*, and it propagates: a `const` object exposes only its `const` member functions, and a pointer-to-const forbids writes through that path.

```cpp
struct Widget {
    int value = 0;
    int  get() const { return value; }   // const member: callable on const Widget
    void set(int v)  { value = v; }       // non-const: only on a mutable Widget

    // const-overloading: same name, differ only in const-ness; the compiler
    // picks based on whether the object is const.
    const int& ref() const { return value; }   // chosen for const objects
    int&       ref()       { return value; }    // chosen for mutable objects
};
```

### `mutable`: physical vs logical const

*Logical* const means "observably unchanged"; *physical* const means "no bits change." A cache or mutex is physically modified but logically invisible — that is what `mutable` is for.

```cpp
#include <mutex>
class Lookup {
    mutable std::mutex          m_;     // locking doesn't change logical state
    mutable std::optional<int>  cache_; // memoization is invisible to callers
public:
    int compute() const {               // still const: logically pure
        std::lock_guard lk(m_);         // mutates m_ — allowed via mutable
        if (!cache_) cache_ = expensive();
        return *cache_;
    }
    int expensive() const;
};
```

### The `const_cast` trap

```cpp
void mutate(const Widget& w) {
    const_cast<Widget&>(w).set(5);   // ❌ UB if w refers to a genuinely const object
}
const Widget cw;  mutate(cw);   // ❌ UB: cw is const; the compiler may put it in ROM
Widget mw;        mutate(mw);   // "works": mw is non-const, only viewed const — a smell
```

`const_cast` is legitimate only to interface with legacy/C APIs that forgot `const` on a parameter they don't actually modify. Casting away const to *write* a truly const object is UB.

- **Symptom:** Removing a `const` from a function signature triggers a cascade of new compile errors up the call chain.
- **Cause:** const correctly *propagates* — you were relying on const-ness deep in the stack.
- **Fix:** Don't fight it. Push const as far down as it naturally goes; reach for `const_cast` only at a genuine legacy boundary, never to enable a real mutation.

**const-correct API design** ([Chapter 30](../30_api_design/README.md)): take parameters by `const&` when you only read them; mark every non-mutating member `const`; return `const&` from accessors that expose internals; design types so the *logical* const surface is honest, using `mutable` only for invisible state. A const-correct interface documents intent and lets callers reason — and lets the optimizer help.

---

## Summary

| Concept | Key Points |
|---|---|
| **No-throw** | `noexcept`; destructors and `swap` must never throw |
| **Strong guarantee** | Commit-or-rollback via copy-and-swap; default target |
| **Basic guarantee** | Invariants + no leaks via RAII; state unspecified |
| **`noexcept` moves** | Or `vector` growth silently falls back to copy |
| **RTTI** | `typeid`/`dynamic_cast`; runtime cost; prefer variant/CRTP/erasure when you can |
| **Virtual inheritance** | One shared base via `vbptr`; most-derived initializes it; prefer composition |
| **UB catalog** | Optimizer assumes UB never happens — time-travel optimizations |
| **Strict aliasing** | Fix puns with `std::bit_cast`/`memcpy`, keep the assumption on |
| **Tooling** | ASan/UBSan/TSan + `-Wall -Wextra`; `constexpr` surfaces UB at build time |
| **const-correctness** | Propagation; `mutable` for logical const; `const_cast`-to-write is UB |

---

> **Next:** Loop back to [Chapter 22: Tooling, Testing & Sanitizers](../22_tooling_testing/README.md) to wire ASan/UBSan/TSan into CI as quality gates, and to [Chapter 31: Engineering Practice at Scale](../31_engineering_practice/README.md) for sanitizers-in-CI as a standard.
>
> **Related:** [Chapter 7: Inheritance & Polymorphism](../07_inheritance_polymorphism/README.md) · [Chapter 12: Memory Management](../12_memory_management/README.md) · [Chapter 13: Exception Handling](../13_exception_handling/README.md) · [Chapter 17: Template Metaprogramming](../17_template_metaprogramming/README.md) · [Chapter 18: Memory Model](../18_memory_model/README.md) · [Chapter 24: Performance Engineering](../24_performance_engineering/README.md) · [Chapter 30: API & Interface Design](../30_api_design/README.md)
