# Chapter 17: Template Metaprogramming — Exercises

Compile all exercises with:
```bash
g++ -std=c++11 -Wall -Wextra your_file.cpp -o out
```

---

## Section 1: Compile-Time Integer Computation 🟢

1. Write a `Power<Base, Exp>` struct that computes `Base^Exp` at compile time using struct TMP.
2. Write a `Sum<N>` struct that computes `1 + 2 + ... + N` at compile time.
3. Write `IsPrime<N>` that sets `value = true` if N is prime, false otherwise. Use a helper `IsPrimeHelper<N, D>` with D as the current divisor.
4. Convert `Factorial<N>` to use `constexpr` instead of struct TMP. Verify you get the same result.
5. Write `GCD<A, B>` struct using template specialization to compute GCD at compile time. Check `GCD<48, 18>::value == 6`.

---

## Section 2: Type Traits from Scratch 🟢

6. Write `is_integral<T>` that is `true` for `int`, `long`, `short`, `char` and `false` for `float`, `double`, `string`.  
   *Hint: use explicit full specializations for each integral type.*
7. Write `is_reference<T>` that detects `T&`. Add a specialization for `const T&`.
8. Write `add_const<T>` that produces `const T` (the reverse of `remove_const`).
9. Write `decay<T>` that strips one level of reference and const:  
   `decay<const int&>::type` → `int`.
10. Write `enable_if<bool, T>` with a nested `type` only when the bool is true.  
    Verify it causes a substitution failure (compile error) when used with false.

---

## Section 3: Tag Dispatch 🟡

11. Implement `my_swap` using tag dispatch:
    - For trivially-copyable types (a tag you define), use a bitwise `memcpy` swap.
    - For non-trivial types, use the three-way swap with a temporary.
12. Implement `printContainer` using tag dispatch on `has_random_access` tag:
    - If the container has random access, print elements by index.
    - Otherwise, use an iterator loop.
13. Define `IntegralTag`, `FloatingTag` and implement `absoluteValue` using tag dispatch to call `abs()` vs `fabs()`.
14. Write `serialize(value, stream)` dispatched by `IsArithmetic` / `IsClass` tags. For arithmetic types, write directly; for class types, call `value.serialize(stream)`.

---

## Section 4: Policy-Based Design 🟡

15. Create a `Logger` class template with a `OutputPolicy` (ConsoleOutput, FileOutput stub) and a `TimestampPolicy` (WithTimestamp, NoTimestamp). Combine them for four variants.
16. Design a `Stack<T, StoragePolicy>` where `StoragePolicy` can be `VectorStorage<T>` or `ArrayStorage<T, N>`. Both provide `push`, `pop`, `top`, `empty`.
17. Create a `SmartCache<Key, Value, EvictionPolicy>` with two eviction policies: `NoEviction` (grows forever) and `LRUEviction` (evict oldest). Use `map` internally.
18. Write a `Formatter<T, PaddingPolicy>` where `PaddingPolicy` provides `pad(string, width)`: one pads with spaces, one with zeros.

---

## Section 5: Expression Templates 🔴

19. Extend the Vec expression template example to support subtraction (`operator-`) and scalar multiplication (`operator*(Vec, double)`). Verify a single-pass evaluation.
20. Write a `LazyConcat<L, R>` expression template for `std::string` concatenation that avoids creating temporaries. `a + b + c + d` should build the final string in one allocation.
21. Implement a `MatrixExpr` mini-library (2×2 matrices) using expression templates so `A + B + C` computes in one loop over elements.

---

## Section 6: Advanced constexpr 🟡

22. Write a `constexpr` function `ispalindrome(const char* s, int len)` that returns true if a string is a palindrome. Use `static_assert` to verify at compile time.
23. Write `constexpr bool isPrime(int n)` and use it to define a compile-time array of the first 10 primes (fill via a constexpr helper).
24. Write `constexpr int log2Floor(int n)` that computes floor(log2(n)) at compile time. Verify `log2Floor(8) == 3`, `log2Floor(9) == 3`.
25. Write a compile-time string hasher: `constexpr size_t hash(const char* s, size_t len)` using the FNV-1a algorithm. Verify two strings get different hashes.

---

## Section 7: CRTP 🔴

26. Create a `Printable<Derived>` CRTP base that provides `print()` calling `Derived::toString()`. Apply it to `Circle` and `Rectangle`.
27. Create a `Singleton<Derived>` CRTP base that prevents direct construction and provides `instance()`. Apply to two different classes.
28. Implement `Counter<Derived>` that counts how many objects of each derived type have been constructed (using a `static int` member). Verify counts separately for two different derived types.
29. Use CRTP to implement a `VectorArithmetic<Derived, N>` base providing `operator+`, `operator-`, `operator*(scalar)` for any fixed-size vector class.

---

## Section 8: Template Template Parameters 🔴

30. Write `ContainerAdapter<T, template Container>` that wraps any sequence container and adds `sorted_insert(T)` — inserts and keeps the container sorted.
31. Create `DoubleContainer<template C>` that stores elements in two `C<T>` containers (primary and backup) and always writes to both.
32. Write `print_all<T, Container>` as a function template template taking any standard sequence container and printing all elements.

---

## Integration Challenges 🏆

**Challenge 1:** Build a compile-time `TypeMap` (mapping types to integer IDs):

```cpp
static_assert(TypeId<int>::value    == 0, "");
static_assert(TypeId<double>::value == 1, "");
static_assert(TypeId<string>::value == 2, "");
```

**Challenge 2:** Write a full `StaticVector<T, N>` (compile-time max capacity, runtime size) with:
- Policy for bounds checking (`Checked` / `Unchecked`)
- CRTP-injected comparison operators
- `initializer_list` constructor

**Challenge 3:** Implement a simplified `tuple<T...>` from scratch using recursive inheritance. Support `get<N>(t)` and `tuple_size<Tuple>::value`.

---

## Key Concepts Checklist

```
✓ Struct TMP for compile-time integers
✓ Template specialization as compile-time if/else
✓ Type traits via partial specialization
✓ Tag dispatch for zero-overhead algorithm selection
✓ Policy templates for composable, swappable behaviour
✓ Expression templates for lazy arithmetic evaluation
✓ constexpr for type-safe compile-time computation
✓ CRTP for static polymorphism and mixin behaviour
✓ Template template parameters for container-generic code
✓ typename and template disambiguators for dependent names
```

---

## Expected Difficulty

- **Easy (🟢)**: 15-20 min each — 5 exercises
- **Medium (🟡)**: 25-45 min each — 11 exercises  
- **Hard (🔴)**: 45-90 min each — 11 exercises
- **Challenges (🏆)**: 60-120+ min — 3 exercises
