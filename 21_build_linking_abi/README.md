# Chapter 21: Build, Linking & the ABI

Java has the JVM; C++ has the **toolchain**. To work at staff level you must
understand what happens between your `.cpp` files and a running binary — the
preprocess → compile → assemble → link pipeline, the **One Definition Rule**, name
mangling, and the **ABI** (Application Binary Interface) that determines whether two
separately-compiled pieces can talk. These mechanics explain most "it compiles but
won't link" and "it links but crashes" mysteries, and they are the foundation for
the build tooling in Chapter 22.

> **C++ has no runtime VM.** A C++ program is native machine code; there is no class
> loading or JIT. The cost of that performance is that *you* own the build and the
> binary-compatibility problem the JVM solves for you.

---

## 21.1 The Compilation Pipeline

Each `.cpp` is a **translation unit (TU)** compiled independently into an object
file, then the linker stitches the objects into one binary:

```text
foo.cpp ─preprocess─▶ foo.i ─compile─▶ foo.s ─assemble─▶ foo.o ─┐
bar.cpp ─preprocess─▶ bar.i ─compile─▶ bar.s ─assemble─▶ bar.o ─┼─link─▶ a.out
                                          libs (.a / .so) ───────┘
```

1. **Preprocess** — `#include` is *textual paste*; macros expand. A header is copied
   into every TU that includes it (why headers bloat build times — Chapter 20's
   modules fix this).
2. **Compile** — each TU becomes assembly. The compiler sees only *this* TU plus the
   declarations it included; it does not see other TUs.
3. **Assemble** — assembly → an object file (`.o`) of machine code + a symbol table.
4. **Link** — resolve every symbol reference to a definition across all objects and
   libraries; produce the executable or shared library.

```bash
g++ -std=c++23 -c foo.cpp -o foo.o     # compile only (stop before link)
g++ foo.o bar.o -o app                  # link the objects into a binary
g++ -E foo.cpp                          # see preprocessor output (expanded includes)
```

---

## 21.2 Declaration vs Definition, and the ODR

The compiler needs a **declaration** (the signature) to compile a use; the linker
needs exactly one **definition** (the body) across the whole program. This split is
why C++ separates headers (declarations) from sources (definitions).

The **One Definition Rule (ODR)**:

- Every entity used must be defined **exactly once** across the program.
- A `class`/`template`/`inline` function may be defined in *each* TU (via a header),
  but every definition must be **token-for-token identical** — violating this is
  undefined behavior (often a silent, baffling crash).

```cpp
// widget.hpp
struct Widget { int value; };           // class definition: OK in every TU (identical)
int compute(int);                        // DECLARATION only — defined elsewhere once
inline int twice(int x){ return 2*x; }   // `inline` -> may be defined per-TU (header-safe)

// widget.cpp
int compute(int x) { return x + 1; }     // the single DEFINITION
```

The two classic errors: **"undefined reference"** = you declared and used a symbol
but never linked its definition; **"multiple definition"** = you put a non-`inline`
definition in a header included by multiple TUs.

---

## 21.3 Name Mangling and `extern "C"`

C++ encodes the full signature (namespace, types, const-ness) into the linker symbol
— **name mangling** — which is how overloading works at the binary level:

```bash
$ nm app | c++filt
0000 T add(int, int)          # mangled _Z3addii  -> demangled by c++filt
0000 T add(double, double)    # a DIFFERENT symbol -> overloading works via mangling
```

Mangling is **not standardized across compilers**, so MSVC and GCC objects don't
link together. To expose a stable, C-compatible symbol (for FFI, plugin ABIs, or
cross-language linking), suppress mangling:

```cpp
extern "C" int plugin_entry(int);       // unmangled symbol "plugin_entry"
```

This is the basis of every stable C++ plugin/library boundary (and how other
languages call into C++ — the analogue of Java's JNI).

---

## 21.4 Static vs Dynamic Linking

| | Static (`.a` / `.lib`) | Dynamic (`.so` / `.dll` / `.dylib`) |
|---|---|---|
| Code location | copied into the binary at link time | loaded at run time, shared across processes |
| Binary size | larger (self-contained) | smaller |
| Updates | rebuild to pick up a lib fix | swap the `.so` (if ABI-compatible) |
| Startup | no resolution cost | dynamic-loader symbol resolution |
| Deployment | one file, no surprises | must ship/locate the right `.so` versions |

```bash
ldd app                                  # which shared libs a binary needs (Linux)
g++ app.o -L/path -lfoo                   # link against libfoo.so / libfoo.a
```

> **Dynamic linking is where the ABI bites.** A program built against `libfoo.so.1`
> will crash or misbehave if loaded against an ABI-incompatible `libfoo.so.2` even
> though it "links" — this is the C++ analogue of Java's `NoSuchMethodError`, but at
> raw memory-layout level. Hence library **soname** versioning.

---

## 21.5 The ABI: Why Binary Compatibility Is Hard

The **ABI** is the binary contract between compiled components: struct layout,
calling conventions, vtable layout, name mangling, exception-handling tables, and
the standard library's own layout. Two pieces can interoperate only if they agree
on all of it.

ABI-**breaking** changes to a shipped library (any of these silently corrupts
callers compiled against the old version):

- Adding/removing/reordering **data members** (changes object size and offsets).
- Adding a **virtual function** (changes the vtable layout).
- Changing a function signature, inline function body, or a template.
- Changing `std` types across an incompatible standard-library version.

```cpp
// The "pImpl" idiom: hide the layout behind a pointer so you can change internals
// WITHOUT breaking the ABI of a shipped shared library.
// widget.hpp
class Widget {
public:
    Widget(); ~Widget();
    void doIt();
private:
    struct Impl;                         // opaque — size/members hidden from callers
    std::unique_ptr<Impl> p_;            // only a pointer crosses the ABI boundary
};
```

> **ABI stability is a multi-year commitment.** It's why the C++ standard library
> itself rarely makes breaking changes, why distributions pin compiler versions, and
> why you compile your whole dependency graph with one toolchain/stdlib. For a
> stable plugin boundary, expose a **C ABI** (`extern "C"` + POD structs), not a C++
> one (Chapter 30).

---

## 21.6 Building with CMake

Hand-invoking `g++` doesn't scale; **CMake** is the de-facto build-system generator
for C++. You describe targets and dependencies; CMake generates Makefiles/Ninja/IDE
projects.

```cmake
cmake_minimum_required(VERSION 3.20)
project(myapp CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(core src/widget.cpp)              # a library target
target_include_directories(core PUBLIC include)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE core)       # `app` depends on `core`
```

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

- **Targets and usage requirements** (`PUBLIC`/`PRIVATE`/`INTERFACE`) propagate
  include dirs, flags, and dependencies — "modern CMake" is target-based, not
  variable/global-flag-based.
- **`CMAKE_BUILD_TYPE`** (Debug/Release/RelWithDebInfo) controls optimization and
  debug info — a `Debug` benchmark is meaningless (Chapter 24).

---

## 21.7 Summary

| Concept | Why it matters |
|---|---|
| TU / compilation pipeline | each `.cpp` compiles in isolation; the linker unites them |
| Declaration vs definition, ODR | source of "undefined reference" / "multiple definition" |
| Name mangling / `extern "C"` | how overloading works; how to expose stable symbols |
| Static vs dynamic linking | size/update/deployment trade-offs; `.so` versioning |
| ABI | the binary contract; pImpl & C ABI for stable boundaries |
| CMake | target-based builds; correct flags and build types |

- C++ is **native, no VM** — you own the build and the binary-compatibility problem.
- Most link errors are **ODR / declaration-vs-definition** issues; most crashes
  across `.so` boundaries are **ABI** mismatches.
- Use **pImpl** or a **C ABI** for stable library boundaries, and **modern,
  target-based CMake** to manage real projects.

## Next Steps

- Run `g++ -E` on a file to see how much code one `#include` pastes in.
- Use `nm` + `c++filt` to inspect mangled symbols and find an "undefined reference".
- Convert a small project to target-based CMake with a library + executable.
- Continue to **[Chapter 22: Tooling, Testing & Sanitizers](../22_tooling_testing/README.md)**.
