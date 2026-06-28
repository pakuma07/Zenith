# Chapter 19: Modern C++ I — C++14 & C++17

Chapters 1–18 cover C++11, the foundation of modern C++. But the language did not
stop in 2011: C++14 polished the rough edges, and C++17 added the **vocabulary
types** (`optional`, `variant`, `string_view`) and ergonomics (structured
bindings, `if constexpr`) that define how staff-level C++ is written today. This
chapter brings you from C++11 to C++17 — the baseline every modern codebase
assumes.

> **Compile with:** `g++ -std=c++17` (or `-std=c++14`). Most features below are
> compiler-version sensitive; check support on cppreference.

---

## 19.1 C++14: Polishing C++11

C++14 is a small, incremental release that removes friction from C++11:

```cpp
// Return-type deduction: no trailing return type needed.
auto add(int a, int b) { return a + b; }          // C++14

// Generic lambdas: auto parameters -> the lambda is templated.
auto twice = [](auto x) { return x + x; };          // works for int, double, string
twice(21);  twice(std::string("ab"));

// std::make_unique (C++11 shipped make_shared but forgot this one):
auto p = std::make_unique<Widget>(args);            // never write `new` again (Ch 12)

// Variable templates and binary/digit-separator literals:
template <class T> constexpr T pi = T(3.1415926535);
int mask = 0b1010'1100;                             // binary + ' separators
```

Also: **relaxed `constexpr`** (loops and locals inside `constexpr` functions) and
`[[deprecated]]`. The upgrade from C++11 to C++14 is nearly free — turn it on.

---

## 19.2 Structured Bindings (C++17)

Destructure tuples, pairs, structs, and arrays into named variables — the end of
`std::get<0>` and `.first`/`.second`:

```cpp
std::map<std::string,int> scores{{"ada",90}};

for (const auto& [name, score] : scores)            // not .first / .second
    std::cout << name << ": " << score << "\n";

auto [iter, inserted] = scores.insert({"bob", 80}); // self-documenting names

struct Point { double x, y; };
Point pt{1.0, 2.0};
auto [px, py] = pt;                                 // works on aggregates too
```

This pairs with **init-statements in `if`/`switch`** to scope a result tightly:

```cpp
if (auto [it, ok] = scores.insert({"cy", 70}); ok)  // `it`, `ok` scoped to the if
    use(it);
```

---

## 19.3 `if constexpr`: Compile-Time Branching

`if constexpr` discards the untaken branch *at compile time* — replacing most
SFINAE and tag-dispatch (Chapter 17) with readable code:

```cpp
template <class T>
auto stringify(const T& v) {
    if constexpr (std::is_arithmetic_v<T>)
        return std::to_string(v);                   // only compiled for numbers
    else
        return std::string(v);                      // only compiled for string-like
}
```

The discarded branch need not even be valid for the other type — a massive
simplification over C++11 template metaprogramming.

---

## 19.4 Class Template Argument Deduction (CTAD)

C++17 deduces class template parameters from constructor arguments, so you stop
spelling them out:

```cpp
std::pair p{1, 2.0};                  // std::pair<int,double> — no <...> needed
std::vector v{1, 2, 3};               // std::vector<int>
std::lock_guard lk{mtx};              // std::lock_guard<std::mutex> (Ch 16)
std::array a{1, 2, 3};                // std::array<int,3>
```

---

## 19.5 The Vocabulary Types

C++17 standardized the types that finally let APIs express intent precisely
(replacing `new`/null/`void*` conventions). These are the most impactful additions
for everyday design:

```cpp
#include <optional>
std::optional<User> find(int id);     // "maybe a User" — no sentinel, no out-param
if (auto u = find(42)) use(*u);       // contextually bool; deref to access
int port = cfg.value_or(8080);        // default if empty

#include <variant>
std::variant<int, std::string> v = 42;          // a type-safe tagged union
std::visit([](auto&& x){ process(x); }, v);     // exhaustive dispatch
std::holds_alternative<int>(v);

#include <string_view>
size_t count_spaces(std::string_view s);        // non-owning view: no copy, no alloc
// Accepts const char*, std::string, substrings — all without copying.
```

| Type | Replaces | Use for |
|---|---|---|
| `std::optional<T>` | null pointers, sentinel values, out-params | "maybe absent" return values |
| `std::variant<Ts...>` | unions, `void*` + tag, inheritance hacks | a value that is one of N types |
| `std::any` | `void*` | type-erased storage (rarely the right tool) |
| `std::string_view` | `const std::string&`, `const char*` | non-owning read-only string params |

> **`string_view` is a borrow, not an owner.** It points at memory it does not
> manage — never return a `string_view` to a temporary or a local string; that's a
> dangling view (the same lifetime trap as references, Chapter 4). Use it for
> *parameters*, store an owning `std::string`.

---

## 19.6 Fold Expressions

Variadic templates (Chapter 9) got dramatically simpler — no recursive helper:

```cpp
template <class... Args>
auto sum(Args... args) { return (args + ...); }     // fold over operator+

template <class... Args>
void print(Args&&... args) {
    ((std::cout << args << ' '), ...);              // fold over the comma operator
}
```

---

## 19.7 Other High-Value C++17 Features

```cpp
// Guaranteed copy elision: returning a prvalue constructs in place (no move/copy).
Widget make() { return Widget{}; }     // no temporary, even for non-movable types

// inline variables: define a global/static in a header with no ODR violation.
struct Config { static inline int version = 3; };   // header-only constant

// [[nodiscard]]: warn if a return value is ignored (errors, handles, statuses).
[[nodiscard]] Error connect();         // caller MUST check the result

// std::byte: a real byte type (not char), for raw memory.
// Parallel algorithms: std::sort(std::execution::par, v.begin(), v.end());
```

`std::filesystem` also arrived in C++17 — covered with `chrono` and `regex` in
Chapter 23.

---

## 19.8 Summary

| Feature | What it gives you |
|---|---|
| C++14 return deduction / generic lambdas | less boilerplate, generic callables |
| `std::make_unique` | safe ownership, no raw `new` (Ch 12) |
| Structured bindings | destructure tuples/structs into named vars |
| `if constexpr` | readable compile-time branching (replaces SFINAE) |
| CTAD | drop redundant template arguments |
| `optional`/`variant`/`string_view` | express "maybe", "one-of", and "borrowed string" |
| Fold expressions | concise variadic code |
| `[[nodiscard]]`, guaranteed copy elision, `inline` vars | safer, faster, header-friendly |

- C++14/17 is the **modern baseline** — vocabulary types and `if constexpr` change
  how you design APIs, not just how you write loops.
- Prefer `optional`/`variant` over sentinels and `void*`; prefer `string_view` for
  read-only string parameters (mind its **borrow** semantics).
- Reach for structured bindings, CTAD, and fold expressions to cut boilerplate.

## Next Steps

- Recompile a C++11 codebase with `-std=c++17` and replace `std::get`/`.first` with
  structured bindings and sentinel returns with `std::optional`.
- Replace a tag-dispatch or SFINAE helper (Chapter 17) with `if constexpr`.
- Audit string parameters and switch read-only ones to `std::string_view`.
- Continue to **[Chapter 20: Modern C++ II — C++20 & C++23](../20_modern_cpp_20_23/README.md)**.
