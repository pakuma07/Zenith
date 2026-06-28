# Chapter 22: Tooling, Testing & Sanitizers

C++ gives you no runtime safety net — no garbage collector, no bounds checks by
default, no managed memory. The discipline that makes large C++ codebases survivable
is **tooling**: dependency management, static analysis, runtime **sanitizers**, unit
tests, and fuzzing, all wired into CI. A staff C++ engineer treats these as
non-negotiable infrastructure, not optional extras. This chapter is the modern C++
quality toolchain.

> These are table stakes at FAANG-scale C++: a memory bug that a sanitizer catches
> in CI in seconds can otherwise be a multi-day production incident (Chapter 27).

---

## 22.1 Dependency Management: vcpkg and Conan

Unlike Maven/pip, C++ historically had no standard package manager — you vendored
sources or fought system packages. Modern projects use **vcpkg** or **Conan**:

```bash
# vcpkg (Microsoft) — manifest mode: declare deps in vcpkg.json, integrate with CMake.
vcpkg install                            # reads vcpkg.json, builds deps
cmake -B build -DCMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake

# Conan (Python-based, profiles for cross-compilation):
conan install . --output-folder=build --build=missing
```

Both pin versions for reproducible builds (Chapter 27 supply chain). **CMake
`FetchContent`** is a lighter option for small dependency sets (fetches and builds
at configure time). Pin exact versions — "latest" is not reproducible.

---

## 22.2 Formatting and Static Analysis

Push everything mechanical into tools so humans review judgment, not whitespace
(Chapter 31):

| Tool | Role |
|---|---|
| **clang-format** | enforce one formatting style (`.clang-format`), auto-applied |
| **clang-tidy** | lint: bug-prone patterns, modernization, naming, core guidelines |
| **cppcheck** | additional static analysis |
| **include-what-you-use** | prune/fix `#include`s |
| **Clang Static Analyzer / PVS-Studio / Coverity** | deep path-sensitive analysis |

```bash
clang-format -i src/*.cpp                          # format in place
clang-tidy src/foo.cpp -checks='modernize-*,bugprone-*,performance-*' -- -std=c++23
```

clang-tidy's `modernize-*` checks will, for instance, rewrite raw loops to
range-`for`, `NULL` to `nullptr`, and typedef to `using` — invaluable when
upgrading a C++11 codebase to the idioms of Chapters 19–20. Enforce both in CI
(`--dry-run --Werror` for format).

---

## 22.3 Sanitizers: Catching Undefined Behavior at Runtime

The single most valuable C++ tooling. Sanitizers instrument the binary to detect at
*runtime* the bugs the language otherwise leaves as silent undefined behavior:

| Sanitizer | Flag | Catches |
|---|---|---|
| **AddressSanitizer** | `-fsanitize=address` | use-after-free, buffer overflow, leaks, use-after-scope |
| **UndefinedBehaviorSanitizer** | `-fsanitize=undefined` | signed overflow, bad shifts, null deref, misaligned access |
| **ThreadSanitizer** | `-fsanitize=thread` | data races (Chapter 18) |
| **MemorySanitizer** | `-fsanitize=memory` | reads of uninitialized memory |

```bash
# Build tests with ASan+UBSan and run them — failures abort with a precise report.
g++ -std=c++23 -fsanitize=address,undefined -g -O1 tests.cpp -o tests && ./tests
```

```cpp
int main() {
    int* a = new int[10];
    a[10] = 1;                            // heap-buffer-overflow — ASan pinpoints it
    delete[] a;
    return a[0];                          // heap-use-after-free — ASan pinpoints it
}
```

> **Run your whole test suite under ASan+UBSan in CI**, and TSan for concurrent
> code. ASan/UBSan can share a build (combine flags); TSan and MSan must run
> separately. They add ~2× slowdown — acceptable for tests, not for shipping
> binaries. This is how you get Java-like safety feedback on a native language.

---

## 22.4 Unit Testing: GoogleTest and Catch2

C++ has no built-in test framework; the two standards are **GoogleTest** (gtest,
+gmock for mocking) and **Catch2** (header-only, expressive).

```cpp
// GoogleTest
#include <gtest/gtest.h>

TEST(MathTest, Squares) {
    EXPECT_EQ(square(3), 9);              // non-fatal: continues on failure
    ASSERT_NE(divide(10, 2), 0);         // fatal: aborts this test on failure
}

TEST(VectorTest, Throws) {
    std::vector<int> v;
    EXPECT_THROW(v.at(5), std::out_of_range);
}
```

```cmake
# CTest integration in CMake — `ctest` runs them, CI gates on the result.
enable_testing()
add_executable(unit_tests tests/math_test.cpp)
target_link_libraries(unit_tests PRIVATE core GTest::gtest_main)
add_test(NAME unit COMMAND unit_tests)
```

The testing *strategy* (the pyramid, integration vs unit, flakiness) is in Chapter
31; gmock provides mocking, and GoogleTest's `TEST_P` gives parameterized tests.

---

## 22.5 Fuzzing: Finding the Inputs You Didn't Think Of

For any code that parses untrusted input (Chapters 26, 27), **coverage-guided
fuzzing** finds crashing/UB inputs automatically — it mutates inputs and follows
new code paths, catching bugs human-written tests miss.

```cpp
// libFuzzer entry point; build with -fsanitize=fuzzer,address.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    parse_packet(std::span<const uint8_t>(data, size));   // must not crash/UB on ANY input
    return 0;
}
```

```bash
clang++ -std=c++23 -fsanitize=fuzzer,address parser_fuzz.cpp -o fuzz && ./fuzz
```

Run fuzzers in CI (OSS-Fuzz is the model for open source). Fuzzing + ASan is the
most effective way to harden a C++ parser.

---

## 22.6 Debugging and Profiling Tools

- **gdb / lldb** — interactive debuggers (`bt`, `break`, `watch`); pair with
  `-g` debug info and avoid `-O2` while stepping.
- **Valgrind (memcheck)** — finds leaks/invalid memory without recompiling, but
  ~10–50× slower; ASan is preferred when you can rebuild.
- **perf / VTune / Instruments** — sampling profilers and flame graphs for finding
  CPU hotspots (Chapter 24).
- **`-Wall -Wextra -Wpedantic -Werror`** — the warnings *are* free static analysis;
  enable them and treat warnings as errors. Many bugs are caught here for zero cost.

---

## 22.7 Continuous Integration for C++

A C++ CI pipeline (Chapter 31) should, on every push:

```text
1. Configure & build with -Wall -Wextra -Werror (Debug and Release)
2. clang-format --dry-run --Werror   +   clang-tidy        (style/lint gate)
3. Run the test suite under AddressSanitizer + UBSan       (memory/UB gate)
4. Run the test suite under ThreadSanitizer (concurrent code)
5. Run fuzzers (smoke or corpus replay) for parsers
6. Dependency/vuln scan (Chapter 27); build the release artifact
```

Build across **multiple compilers** (GCC, Clang, MSVC) and standards — portability
bugs surface only under a second toolchain (Chapter 21's ABI/mangling differences).

---

## 22.8 Summary

| Area | Tooling |
|---|---|
| Dependencies | vcpkg / Conan / CMake FetchContent (pinned) |
| Formatting / lint | clang-format, clang-tidy (`modernize-*`), cppcheck |
| Runtime safety | **ASan, UBSan, TSan, MSan** — run tests under them in CI |
| Unit testing | GoogleTest (+gmock) / Catch2, via CTest |
| Fuzzing | libFuzzer/AFL++ + ASan for untrusted-input code |
| Debug / profile | gdb/lldb, Valgrind, perf/VTime; `-Wall -Wextra -Werror` |

- C++ has no runtime safety net, so **sanitizers + tests + fuzzing in CI** are the
  substitute — non-negotiable at scale.
- Use a real **package manager** (vcpkg/Conan), enforce **clang-format/clang-tidy**,
  and treat **warnings as errors**.
- Build and test across **multiple compilers** under multiple sanitizers.

## Next Steps

- Add `-fsanitize=address,undefined` to your test build and fix whatever it finds.
- Wire clang-format + clang-tidy + CTest into CMake and run them in CI.
- Write a libFuzzer harness for any function that parses external bytes.
- Continue to **[Chapter 23: Time, Text & Filesystem](../23_time_text_filesystem/README.md)**.
