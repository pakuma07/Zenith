# 33 — Modules in Practice (C++20)

[Chapter 20](../20_modern_cpp_20_23/README.md) named modules as one of C++20's "big
four" but only sketched them. This chapter is the authoritative treatment: what
modules replace, how the units compose, and — critically for a staff/principal
engineer — whether and *how* to adopt them in a large legacy codebase. The honest
2026 answer is "yes, but deliberately, leaf-first, and with your eyes open about
toolchain maturity." Code-first, ending in a cost/benefit verdict and the real
failure modes.

> **Modules are a build-model change, not just a syntax change.** Unlike concepts or
> ranges you can't turn them on with a flag — they alter how translation units depend
> on each other and force your build system to scan and order compilation. That fact
> drives most of the friction below.

---

## 33.1 The Problem: the Textual `#include` Model

[Chapter 21](../21_build_linking_abi/README.md) showed that `#include` is *textual
paste* by the preprocessor. Every header is copied verbatim into every translation
unit (TU) that includes it, then parsed again from scratch. That model has four
chronic defects:

```cpp
// math.hpp  — a "normal" header
#define FAST 1                 // macro leaks into EVERY includer (and their includers)
inline int square(int x){ return x*x; }

// a.cpp and b.cpp both #include "math.hpp"
// -> <vector>, <string> etc. parsed AGAIN per TU (repeated parsing -> slow builds)
// -> FAST is now defined in a.cpp and b.cpp whether they wanted it or not
```

| Defect | Cause | Consequence |
|---|---|---|
| Slow builds | each header re-parsed per TU; `<vector>` pulled in 100s of times | build time scales with `headers × TUs` |
| Macro leakage | `#define` has no scope; it flows transitively | `min`/`max` from `<windows.h>` break unrelated code |
| ODR fragility | identical token streams required across TUs | one stray `#define` before an include = silent UB ([Ch 21.2](../21_build_linking_abi/README.md)) |
| Include-order bugs | a header's meaning depends on what was `#define`d before it | reordering includes changes behavior |

Modules fix all four: an interface is **compiled once** into a binary artifact,
**macros do not cross** the boundary, names have proper **linkage**, and there is no
include order because there is no text paste.

---

## 33.2 Named Modules — the Minimal Example

A **named module** declares an interface with `export module name;`. Only
`export`ed declarations are visible to importers; everything else has module-internal
linkage (invisible outside, even without `static`).

```cpp
// math.ixx  (MSVC) / math.cppm (Clang) — the module INTERFACE unit
export module math;            // module declaration; opens the "module purview"

export int square(int x){ return x * x; }   // visible to importers
int helper(int x){ return x + 1; }          // NOT exported -> internal linkage
export constexpr double PI = 3.14159;        // exported value
```

```cpp
// main.cpp — a consumer
import math;                   // no path, no text paste; uses the compiled interface
#include <print>

int main(){
    std::print("{} {}\n", square(7), PI);    // OK
    // helper(3);              // ERROR: not exported, not even visible
    // FAST is undefined here  // macros do NOT cross module boundaries (by design)
}
```

Everything after `export module math;` is the **module purview**. Names there default
to internal linkage unless `export`ed — the opposite of the header world, where every
non-`static` name leaks.

---

## 33.3 Module Units: Interface, Implementation, Partitions

A module is built from one or more **module units**:

- **Primary module interface unit** — exactly one per module; contains
  `export module foo;`. It is the public face.
- **Module implementation unit** — `module foo;` (no `export`). Holds definitions you
  don't want to recompile into every consumer. It is *not* visible to importers.
- **Module partitions** — split a large module internally. **Interface partitions**
  (`export module foo:part;`) are re-exported by the primary; **internal partitions**
  (`module foo:detail;`) are private helpers.

```cpp
// foo.cppm — primary interface unit
export module foo;
export import :shapes;         // re-export an interface partition to consumers
import :detail;                // use an internal partition (NOT re-exported)
export void run();
```

```cpp
// foo-shapes.cppm — interface partition
export module foo:shapes;      // ":shapes" belongs to module foo
export struct Circle { double r; };
```

```cpp
// foo-detail.cppm — internal partition
module foo:detail;             // helpers, invisible outside module foo
int seed();
```

```cpp
// foo-impl.cpp — implementation unit (definitions only)
module foo;                    // attaches to foo; recompiled here, not per-consumer
void run(){ /* ... uses Circle, seed() ... */ }
```

> **Partition vs implementation.** Use **implementation units** to move *definitions*
> out of the interface (faster consumer builds, the modules analogue of pImpl).
> Use **partitions** to organize a *large interface* into files without exposing the
> split to consumers.

---

## 33.4 The Global and Private Module Fragments

Modules must still consume legacy headers. The **global module fragment** is a region
*before* the module declaration, opened by a bare `module;`, where `#include` is
allowed. Its contents are attached to the global module, not your module.

```cpp
module;                        // GLOBAL MODULE FRAGMENT — only preprocessor lines here
#include <cmath>               // legacy header pulled in textually, as before
#include "legacy_c_api.h"

export module geometry;        // module purview begins here

export double hypot2(double a, double b){
    return std::sqrt(a*a + b*b);   // std::sqrt comes from the included <cmath>
}
```

The **private module fragment** (`module :private;`) appears at the *end* of a
single-file module and holds definitions invisible to importers — changing it does
not force consumers to recompile.

```cpp
export module widget;
export class Widget { public: void doIt(); };

module :private;               // PRIVATE FRAGMENT — only legal in a single-file module
void Widget::doIt(){ /* edit freely without rebuilding consumers */ }
```

---

## 33.5 Header Units — the Migration Stepping-Stone

You don't have to modularize a header to get *some* benefit. A **header unit**
imports an existing, unmodified header as a precompiled artifact:

```cpp
import <vector>;               // header UNIT: compiled once, imported (not re-parsed)
import "my_legacy.hpp";        // your own header, importable as a unit
```

The header is parsed once into an artifact and reused. Crucially — and unlike named
modules — **macros defined in a header unit *do* leak to the importer**, because a
header unit preserves header semantics. This makes header units a low-risk first step
for an `#include`-heavy codebase (see the migration ladder in 33.7).

> **Header units are an importable header, not a module.** They give you build-cache
> reuse with header semantics intact. Named modules give you isolation but require
> rewriting the header's interface.

---

## 33.6 The Build Model: BMI/CMI and Dependency Scanning

Compiling a module interface produces a **BMI** (Built Module Interface; MSVC calls it
a **CMI** / `.ifc`, GCC uses `.gcm`, Clang uses `.pcm`). Consumers read the BMI
instead of re-parsing source.

```bash
# Clang, by hand — note the two-step "produce interface, then consume" model
clang++ -std=c++20 --precompile math.cppm -o math.pcm   # interface -> BMI
clang++ -std=c++20 -fmodule-file=math=math.pcm main.cpp math.pcm -o app
```

This is the structural change. With headers, every `.cpp` was independent and could
compile in any order ([Ch 21.1](../21_build_linking_abi/README.md)). With modules:

- `import foo;` requires `foo`'s BMI to **already exist**, so consumers must build
  *after* their dependencies — a true compile-time dependency graph.
- The build system can no longer just glob `*.cpp` and fan out. It must first
  **scan** every source for `import`/`export module` lines (`clang-scan-deps`,
  `cl /scanDependencies`) to discover the order, then build in topological order.
- BMIs are **not portable**: tied to the compiler, version, and flags. They are a
  build cache, never a distribution format.

```text
# header world: embarrassingly parallel
a.cpp ─▶ a.o ┐
b.cpp ─▶ b.o ┼─link─▶ app     (any order)

# module world: ordered DAG (scan first, then build leaves -> roots)
math.cppm ─▶ math.bmi ─▶ (needed before) ─▶ main.cpp ─▶ app
```

That scan-then-order requirement is why modules demand build-system support — you
cannot bolt them onto a naive Makefile.

---

## 33.7 Toolchain Reality (the Honest Part)

As of 2026 the core language feature is broadly usable, but the *ecosystem* still has
sharp edges. Be honest in a design doc:

| Component | Status (mid-2020s) | Notes |
|---|---|---|
| MSVC | Most mature; named modules + `import std;` | Visual Studio drives much of the tooling |
| Clang | Solid named modules; `import std;` in recent releases | needs explicit BMI/dep-scan plumbing |
| GCC | Usable; historically the laggard, steadily improving | partition/`import std` support trails |
| CMake | `FILE_SET CXX_MODULES` (3.28+) with Ninja | the only well-supported build path |
| `import std;` | C++23; uneven across compilers/stdlibs | the headline payoff; verify per toolchain |

```cmake
cmake_minimum_required(VERSION 3.28)
project(modapp CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(math)
target_sources(math
  PUBLIC FILE_SET CXX_MODULES FILES   # declares module interface units to CMake
    src/math.cppm)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE math)   # CMake derives the module build order
```

```bash
cmake -S . -B build -G Ninja          # Ninja is the supported generator for modules
cmake --build build
```

`import std;` (C++23) replaces dozens of standard-library `#include`s with a single
import and is the most compelling reason to care — but its availability is
toolchain-specific; gate it behind a feature check, do not assume it.

> **Rough edges to expect:** generator restricted to Ninja; non-portable BMIs that
> invalidate on flag changes; `import std;` gaps; weaker IDE/static-analysis support
> than headers; package managers (vcpkg/Conan, [Ch 22](../22_tooling_testing/README.md))
> still maturing their module story.

---

## 33.8 Migration Strategy for a Large Legacy Codebase

Do **not** attempt a big-bang rewrite. Modules interoperate with headers, so migrate
incrementally up the dependency graph:

1. **Upgrade the toolchain & build system first.** CMake ≥ 3.28 + Ninja, a compiler
   with stable module support. Without this, stop — modules are not viable.
2. **Header units next.** Replace hot `#include <...>` with `import <...>;` (or
   `import std;` if available). Lowest risk, real build-time wins, header semantics
   intact (33.5).
3. **Modularize leaf libraries first.** Convert libraries with *no* internal C++
   dependencies into named modules; wrap their legacy `#include`s in the global module
   fragment (33.4). Work upward toward roots.
4. **Bridge the two worlds.** Modular code can `#include` headers (via the global
   module fragment) and import header units; non-modular code keeps including the old
   headers. Keep a header shim during transition so both consumers compile.
5. **Hold the line in CI.** Add the module build to CI early ([Ch 22](../22_tooling_testing/README.md));
   module build failures are ordering/scanning issues a naive setup won't catch.

**What breaks — internalize this:** macros do not cross a named-module boundary. Any
configuration macro, feature-toggle `#define`, or header-only library that relies on
the consumer defining a macro *before* include will silently stop working when the
consumer becomes a module. Inventory your macros before you start.

```cpp
// WRONG — assuming a config macro reaches into a module
import logging;                // logging is a named module
#define LOG_LEVEL 3            // has NO effect inside module `logging`
```

```cpp
// RIGHT — pass configuration through the interface, not the preprocessor
import logging;
logging::set_level(3);         // a real exported function; macros stay TU-local
```

---

## 33.9 Benefits Realized — and the Verdict

What modules actually deliver:

- **Build speed.** Interfaces compile once into a BMI instead of being re-parsed per
  TU. In theory large; in practice gated by dependency *serialization* (33.6) and
  toolchain overhead — real wins on `import std;`-heavy code, modest on small projects.
- **Isolation.** No macro leakage, no include-order bugs, internal-linkage-by-default.
- **Stronger ODR.** A single compiled interface removes the "identical token streams"
  footgun ([Ch 21.2](../21_build_linking_abi/README.md)) that causes silent UB.
- **Cleaner interfaces.** `export` makes the public surface explicit instead of
  "whatever the header happened to declare."

| Force | For adopting now | Against / wait |
|---|---|---|
| Build times | big multi-TU codebases, `import std;` | small projects see little gain |
| Toolchain | MSVC/Clang + CMake 3.28 + Ninja | GCC-only or old CMake shops |
| Team | greenfield, leaf libraries | deep macro-config dependence |
| Tooling | basic builds work | IDE/analysis/package-manager gaps |

> **Verdict: adopt deliberately, not reflexively.** Modules are the future of C++
> compilation and worth piloting on a new leaf library or via header units today.
> But the payoff is dominated by toolchain maturity and your dependency shape — treat
> adoption as a migration project ([Ch 31](../31_engineering_practice/README.md)) with
> a CI gate, not a flag flip. (Note: modules are orthogonal to runtime async — they
> change how [coroutine](../32_coroutines_async/README.md) libraries are *delivered*,
> not how they run.)

---

## 33.10 Symptom / Cause / Fix

- **Symptom:** a macro from a module isn't visible in the consumer.
  - **Cause:** named modules do not export macros — only declarations. `#define` is a
    preprocessor construct and is TU-local across the boundary.
  - **Fix:** expose configuration as exported `constexpr`/functions, or `import` the
    macro's *header unit* (header units preserve macros, 33.5) instead of the module.

- **Symptom:** build fails with "module X not found" / circular-dependency / wrong
  order errors.
  - **Cause:** consumers were compiled before the dependency's BMI existed, or two
    modules `import` each other (modules cannot form an import cycle).
  - **Fix:** use a module-aware build (CMake `FILE_SET CXX_MODULES` + Ninja) so order
    is derived from dependency scanning (33.6); break cycles by moving shared
    declarations into a separate leaf module or an interface partition.

- **Symptom:** linker reports duplicate/double definitions when mixing modules and
  headers.
  - **Cause:** the same entity is reachable both through a `#include`d header *and*
    through an `import` of the module that owns it — two definitions, one program.
  - **Fix:** pick one path per entity. While migrating, ensure a translation unit gets
    a symbol either via the header *or* via the module, never both; remove the legacy
    `#include` once the importing TU has switched to `import`.

---

## 33.11 Summary

| Concept | Why it matters |
|---|---|
| `#include` defects | repeated parsing, macro leakage, ODR fragility, order bugs |
| Named modules | `export`/`import`; internal linkage by default; macros don't leak |
| Units & partitions | primary interface, implementation units, interface/internal partitions |
| Fragments | global fragment consumes legacy headers; private fragment hides definitions |
| Header units | `import <vector>;` — low-risk migration step, macros preserved |
| BMI/CMI + dep-scan | build once, but compilation is now an *ordered* DAG |
| Toolchain / migration | MSVC/Clang + CMake 3.28+Ninja; header units → leaf modules → up the graph |

- Modules fix the four chronic `#include` defects but impose an **ordered build model**
  that your build system must understand.
- **Header units first, leaf libraries first** — incremental, never big-bang.
- The big gotcha: **macros do not cross named-module boundaries** — pass config
  through the interface.
- Adopt **deliberately**: the value tracks toolchain maturity and dependency shape.

> **Next:** the book proper continues at the applied tiers — revisit
> [Chapter 22: Tooling, Testing & Sanitizers](../22_tooling_testing/README.md) to put
> the module build under CI.
> **Related:** [20 — Modern C++ II (C++20/23)](../20_modern_cpp_20_23/README.md) ·
> [21 — Build, Linking & the ABI](../21_build_linking_abi/README.md) ·
> [32 — Coroutines & Async](../32_coroutines_async/README.md)
