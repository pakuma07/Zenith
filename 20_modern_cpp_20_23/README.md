# Chapter 20: Modern C++ II — C++20 & C++23

C++20 is the largest language release since C++11 — it delivers the "big four"
that change how you architect C++: **concepts**, **ranges**, **coroutines**, and
**modules**, plus the spaceship operator and `std::span`. C++23 then rounds off
the rough edges (`std::expected`, `std::print`, `mdspan`, deducing `this`). A staff
engineer in 2020s C++ reaches for these daily; this chapter is the working tour.

> **Compile with:** `g++ -std=c++23` (or `-std=c++20`). Feature support varies by
> compiler/stdlib — `import std;`, full modules, and some C++23 library pieces are
> still maturing; check cppreference's compiler-support tables.

---

## 20.1 Concepts: Constraints That Read Like English

Concepts constrain template parameters with named, checkable requirements —
replacing SFINAE (Chapter 17) and turning incomprehensible template errors into
clear messages.

```cpp
#include <concepts>

template <class T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>                          // constrained — only numbers compile
T square(T x) { return x * x; }

// Inline with requires, and custom requirements:
template <class T>
concept Hashable = requires(T x) {
    { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;   // must be hashable
};

auto sum(std::ranges::range auto&& r) { /* ... */ }   // abbreviated, constrained
```

The payoff: `square("hello")` now fails with *"constraint Numeric not satisfied"*
instead of a 40-line template backtrace. Constrain your templates.

---

## 20.2 Ranges: Composable, Lazy Algorithms

The Ranges library (`<ranges>`) replaces iterator-pair algorithms with composable
**views** and pipelines — lazy, allocation-free, and far more readable:

```cpp
#include <ranges>
namespace rv = std::views;

std::vector<int> nums{1,2,3,4,5,6,7,8,9,10};

auto result = nums
    | rv::filter([](int n){ return n % 2 == 0; })   // lazy: nothing computed yet
    | rv::transform([](int n){ return n * n; })
    | rv::take(3);                                   // 4, 16, 36 — computed on demand

for (int x : result) use(x);                         // evaluation happens here

std::ranges::sort(nums);                             // no .begin()/.end() needed
auto pos = std::ranges::find(nums, 7);
```

Views are **lazy and non-owning** (composition is free; nothing materializes until
iterated). C++23 adds `std::ranges::to<std::vector>()` to materialize a pipeline
and `views::zip`, `enumerate`, and `fold_left`.

---

## 20.3 Coroutines: Suspendable Functions

A function containing `co_await`, `co_yield`, or `co_return` is a **coroutine** —
it can suspend and resume, enabling generators and async I/O without callbacks.

```cpp
#include <generator>                          // C++23 std::generator

std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;                           // suspend, hand a value to the caller
        std::tie(a, b) = std::make_pair(b, a + b);
    }
}

for (int n : fibonacci() | std::views::take(10)) std::cout << n << ' ';
```

> **Coroutines are low-level machinery in C++20.** The language provides the
> mechanism (`promise_type`, awaitables); ergonomic types came later (`std::generator`
> in C++23) or from libraries (cppcoro, Boost.Asio's `awaitable`, libunifex). For
> async I/O, use a library's coroutine support (Chapter 25) rather than hand-rolling
> a `promise_type`.

---

## 20.4 Modules: Beyond the `#include` Model

Modules replace textual `#include` (Chapter 21) with a real component boundary —
faster builds (no re-parsing headers per TU), no macro leakage, and explicit
exports:

```cpp
// math.ixx — a module interface unit
export module math;                            // declares the module

export int add(int a, int b) { return a + b; } // exported: visible to importers
int helper() { return 42; }                    // not exported: module-private

// main.cpp
import math;                                    // not #include — no preprocessor
int main() { return add(2, 3); }
```

C++23 adds `import std;` to import the entire standard library as a module. Modules
are the long-term future, but **tooling/build support is still stabilizing** across
compilers and build systems — adopt deliberately, not reflexively, in 2020s code.

---

## 20.5 The Spaceship Operator (`<=>`)

Three-way comparison generates all six relational operators from one definition —
the end of writing `==`, `!=`, `<`, `<=`, `>`, `>=` by hand (Chapter 8):

```cpp
#include <compare>
struct Version {
    int major, minor, patch;
    auto operator<=>(const Version&) const = default;   // all 6 operators, memberwise
    bool operator==(const Version&) const = default;
};
// Version{1,2,0} < Version{1,3,0}  -> true, for free
```

---

## 20.6 `std::span`: A View Over Contiguous Memory

`std::span<T>` is a non-owning view over a contiguous sequence (array, `vector`,
C array) — the array analogue of `string_view`, replacing `(ptr, len)` parameter
pairs:

```cpp
#include <span>
double average(std::span<const double> data) {        // accepts any contiguous source
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}
std::vector<double> v{1,2,3};   double arr[]{4,5,6};
average(v);   average(arr);                            // both work, no copy
```

> Like `string_view`, a `span` is a **borrow** — it must not outlive the buffer it
> views (Chapter 4 lifetimes). Use it for parameters; never return one to a local.

---

## 20.7 Formatting and Output: `std::format` / `std::print`

Type-safe, fast, Python-style formatting replaces `printf` (unsafe) and iostreams
(verbose):

```cpp
#include <format>
#include <print>                               // C++23

std::string s = std::format("{} scored {:.1f}%", name, 91.25);   // type-checked
std::print("{:>8} | {:<12}\n", id, name);      // C++23: direct, fast, no <<
std::println("done");                          // adds the newline
```

`std::format` checks the format string against the argument types *at compile time*
in C++20+ — no more `printf` format/argument mismatches.

---

## 20.8 C++23 Highlights

```cpp
// std::expected<T, E>: a value OR an error, without exceptions (Ch 13, 26).
#include <expected>
std::expected<int, std::string> parse(std::string_view s);
if (auto r = parse("42")) use(*r);
else log(r.error());                           // explicit, exception-free error flow

// Deducing this: write one member function instead of const/non-const/&&/&  overloads.
struct Widget {
    template <class Self>
    auto&& value(this Self&& self) { return std::forward<Self>(self).value_; }
};

// Also: std::mdspan (multidimensional view), std::flat_map/flat_set (cache-friendly),
//       std::stacktrace, if consteval, and multidimensional operator[].
```

`std::expected` is especially significant: it brings a first-class, value-based
error-handling style (like Rust's `Result`) to the standard library — see Chapter
26 for when to prefer it over exceptions.

---

## 20.9 Summary

| Feature (std) | What it gives you |
|---|---|
| **Concepts** (20) | readable template constraints; sane error messages |
| **Ranges** (20/23) | lazy, composable algorithm pipelines |
| **Coroutines** (20) | generators & async without callbacks (use a library) |
| **Modules** (20/23) | real components, faster builds (tooling maturing) |
| **`<=>`** (20) | all comparison operators from one default |
| **`std::span`** (20) | non-owning contiguous view (borrow semantics) |
| **`std::format`/`print`** (20/23) | type-safe, fast formatting |
| **`std::expected`** (23) | value-based error handling without exceptions |

- C++20's **concepts, ranges, coroutines, modules** are the defining modern tools;
  concepts and ranges are production-ready today, modules/coroutines need library
  and toolchain support.
- Prefer `<=>` for comparisons, `std::span`/`string_view` for borrowed views,
  `std::format`/`print` over `printf`/iostreams, and `std::expected` for
  exception-free error paths.

## Next Steps

- Constrain a template with a `concept` and observe the improved error message.
- Rewrite a manual filter/transform loop as a `std::views` pipeline.
- Replace hand-written comparison operators with `= default` `<=>`.
- Try `std::expected` for a parse/validate function and compare to throwing.
- Continue to **[Chapter 21: Build, Linking & the ABI](../21_build_linking_abi/README.md)**.
