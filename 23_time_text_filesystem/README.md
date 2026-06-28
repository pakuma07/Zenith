# Chapter 23: Time, Text & Filesystem

Beyond the core language, real programs handle dates, parse text, and touch the
filesystem. Modern C++ has first-class standard-library support for all three:
`<chrono>` (a type-safe time library, extended with calendars and time zones in
C++20), `<regex>`, `<filesystem>` (C++17), and the formatting from Chapter 20.
This chapter covers the applied standard library a staff engineer uses for
everyday work — and the footguns in each.

> **Compile with:** `g++ -std=c++23`. `<filesystem>` is C++17; chrono calendars/zones
> and `std::format` are C++20; `std::print` is C++23.

---

## 23.1 `<chrono>`: Type-Safe Time

`<chrono>` makes durations and time points **strongly typed**, so unit mistakes are
compile errors — not the silent "is this seconds or milliseconds?" bug of raw
integers.

```cpp
#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;

auto timeout = 500ms;                       // a typed duration; 5s, 2h also work
auto total = timeout + 2s;                   // 2500ms — units convert automatically
// auto bad = timeout + 3;                   // COMPILE ERROR: 3 has no unit

// Measure elapsed time with a STEADY clock (never goes backward):
auto start = steady_clock::now();
do_work();
auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start);
```

The three clocks, and which to use:

| Clock | Property | Use for |
|---|---|---|
| `steady_clock` | monotonic, never adjusted | **measuring durations / timeouts** |
| `system_clock` | wall-clock, can jump (NTP) | timestamps, calendar time |
| `high_resolution_clock` | aliases one of the above | avoid — ambiguous; prefer `steady_clock` |

> **Never measure elapsed time with `system_clock`** — an NTP correction can make it
> jump backward and yield a negative or wildly wrong duration (the C++ analogue of
> the `System.currentTimeMillis` warning in other languages). Use `steady_clock`.

### Calendars and time zones (C++20)

```cpp
auto today = year_month_day{floor<days>(system_clock::now())};   // 2026y/June/23
zoned_time local{"America/Los_Angeles", system_clock::now()};    // tz-aware
std::println("{:%Y-%m-%d %H:%M}", local);                         // formatted (Ch 20)
```

C++20 finally brought a real date/calendar and IANA time-zone database into the
standard library — before it, you reached for Howard Hinnant's `date` library
(which became this).

---

## 23.2 `std::format` and `std::print` (Recap)

Type-safe formatting (Chapter 20) is the modern replacement for `printf` and
`<iostream>` for assembling text:

```cpp
#include <format>
std::string row = std::format("{:>6} | {:<20} | {:8.2f}", id, name, price);
std::println("{} items, total {:.2f}", n, total);     // C++23, type-checked
```

`std::format` validates the format string against argument types **at compile time**
— eliminating the format/argument mismatch class of `printf` bugs. Make custom types
formattable by specializing `std::formatter<T>`.

---

## 23.3 `<regex>`: Pattern Matching

`std::regex` provides standard regular expressions. It's convenient and portable —
but know its performance reputation (23.4):

```cpp
#include <regex>
std::string text = "user=ada id=42";
std::regex  re(R"(id=(\d+))");                 // raw string literal: no \\ escaping

std::smatch m;
if (std::regex_search(text, m, re))
    int id = std::stoi(m[1]);                  // m[1] = first capture group

// Replace and iterate all matches:
std::string out = std::regex_replace(text, std::regex(R"(\d+)"), "#");
for (std::sregex_iterator it{text.begin(), text.end(), re}, end; it != end; ++it)
    use((*it)[1]);
```

- Use **raw string literals** `R"(...)"` (Chapter 5) so backslashes aren't
  double-escaped.
- **Compile the `std::regex` once** and reuse it — constructing it is expensive
  (it's not cached like some languages do).

> **`std::regex` is notoriously slow** and bloats binaries; for hot paths or large
> inputs prefer **RE2** (linear-time, ReDoS-safe — Chapter 27) or PCRE2. And beware
> **catastrophic backtracking** (ReDoS): a pattern like `(a+)+$` on adversarial
> input can hang — RE2 is immune by design.

---

## 23.4 `<filesystem>`: Portable Paths and Files (C++17)

`std::filesystem` gives portable path manipulation and directory operations,
replacing platform `#ifdef`s and POSIX calls:

```cpp
#include <filesystem>
namespace fs = std::filesystem;

fs::path p = fs::path("data") / "logs" / "app.log";   // portable separator
if (fs::exists(p) && fs::file_size(p) > 1'000'000)
    fs::rename(p, p.replace_extension(".log.old"));

for (const auto& entry : fs::recursive_directory_iterator("src"))
    if (entry.path().extension() == ".cpp")
        process(entry.path());

fs::create_directories("out/cache");
std::uintmax_t freed = fs::remove_all("out/tmp");      // returns count removed
```

- Operations have **two overloads**: one that throws `filesystem_error`, and one
  taking a `std::error_code&` for exception-free handling — choose per call site
  (Chapter 26's error-handling discussion).
- **TOCTOU caution** (Chapter 27): `exists()` then open is a race — prefer to open
  and handle failure, rather than check-then-act, for security-sensitive paths.

---

## 23.5 String Handling Recap

Pulling together the modern string toolkit (Chapters 5, 19, 20):

- **`std::string_view`** for non-owning read-only parameters — no copies (mind the
  borrow/lifetime rule, Chapter 19).
- **`std::format`** to build strings type-safely.
- **`<charconv>`** (`std::from_chars`/`std::to_chars`, C++17) for the **fastest,
  locale-independent, non-throwing** numeric conversions — prefer over `stoi`/
  `stringstream` on hot paths:

```cpp
#include <charconv>
int value;
auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
if (ec == std::errc{}) use(value);            // no exceptions, no allocation, fast
```

---

## 23.6 Summary

| Need | Modern tool |
|---|---|
| Measure durations / timeouts | `chrono::steady_clock` (never `system_clock`) |
| Calendar / time zones | C++20 `chrono` (`year_month_day`, `zoned_time`) |
| Build text | `std::format` / `std::print` (type-safe, not `printf`) |
| Pattern matching | `std::regex` (small jobs) → **RE2/PCRE2** (hot paths, ReDoS-safe) |
| Paths & files | `std::filesystem` (portable; mind TOCTOU & error_code) |
| Fast number parsing | `<charconv>` `from_chars`/`to_chars` |

- `<chrono>` makes time **strongly typed** — use `steady_clock` for elapsed time and
  C++20 calendars/zones for wall-clock work.
- Prefer `std::format`/`print` over `printf`/iostreams, `<charconv>` over `stoi` on
  hot paths, and a linear-time regex engine over `std::regex` for untrusted/large
  input.
- `std::filesystem` replaces platform-specific path code — but watch error handling
  and TOCTOU races.

## Next Steps

- Replace a `printf`/`stringstream` formatting block with `std::format`.
- Switch elapsed-time measurement from `system_clock` to `steady_clock` and a typed
  `duration`.
- Benchmark `std::from_chars` vs `std::stoi` on a parsing hot path (Chapter 24).
- Continue to **[Chapter 24: Performance Engineering](../24_performance_engineering/README.md)**.
