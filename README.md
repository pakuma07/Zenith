# The Complete Modern C++ Book 📚

A comprehensive guide covering C++ from beginner to **staff/principal** level: the **C++11 foundations** (Chapters 1–18), **modern C++ through C++23** (Chapters 19–20), the **build/tooling/applied** layer (21–23), and the **production, distributed-systems, and engineering-practice** tiers (24–31) a senior engineer works in daily, plus **advanced language deep-dives** — coroutines, modules, and correctness/UB (32–34).

> **Standard:** Chapters 1–18 establish the C++11 core; Chapters 19–20 bring the language current through **C++14/17/20/23**, and later chapters assume a modern toolchain. Compile the modern chapters with `g++ -std=c++23` (or `-std=c++20`).

## 📖 Table of Contents

### [1. Basics & Fundamentals](01_basics/README.md)
- Getting Started with C++
- Data Types & Variables
- Type Conversion & Casting
- Constants & Literals
- Basic Input/Output
- decltype, static_assert, User-Defined Literals (C++11)

### [2. Control Flow & Loops](02_control_flow/README.md)
- If, Else If, Else Statements
- Switch Case
- For Loops
- While & Do-While Loops
- Loop Control (Break, Continue)

### [3. Functions](03_functions/README.md)
- Function Declaration & Definition
- Parameters & Arguments
- Return Values
- Function Overloading
- Default Arguments
- Inline Functions
- Recursion
- Lambdas, std::function & std::bind (C++11)
- Trailing Return Type, constexpr & noexcept (C++11)

### [4. Pointers & References](04_pointers_references/README.md)
- Pointer Basics
- Pointer Arithmetic
- Void Pointers
- Function Pointers
- References vs Pointers
- Null Pointers & nullptr
- Rvalue References & std::move (C++11)

### [5. Arrays & Strings](05_arrays_strings/README.md)
- Arrays (1D, 2D, Multidimensional)
- Array Initialization
- C-Style Strings
- String Class
- String Operations & Methods
- std::array, Unicode & Raw String Literals (C++11)

### [6. Object-Oriented Programming Basics](06_oop_basics/README.md)
- Classes & Objects
- Constructors & Destructors
- Access Modifiers (public, private, protected)
- Member Variables & Functions
- This Pointer
- Static Members

### [7. Inheritance & Polymorphism](07_inheritance_polymorphism/README.md)
- Inheritance Types (public, private, protected)
- Method Overriding
- Virtual Functions
- Abstract Classes & Pure Virtual Functions
- Polymorphism
- Multiple Inheritance
- override, final & Inheriting Constructors (C++11)

### [8. Operator Overloading](08_operator_overloading/README.md)
- Unary Operators
- Binary Operators
- Comparison Operators
- Assignment Operators
- Stream Operators (<<, >>)
- Subscript & Function Call Operators
- Explicit Conversion & Move Assignment Operators (C++11)

### [9. Templates](09_templates/README.md)
- Function Templates
- Class Templates
- Template Specialization
- Template Parameters
- Variadic Templates (C++11)
- Template Aliases & Perfect Forwarding (C++11)

### [10. STL Containers](10_stl_containers/README.md)
- Vector
- List
- Deque
- Queue
- Stack
- Set & Multiset
- Map & Multimap
- Unordered Containers, forward_list & emplace (C++11)

### [11. STL Algorithms](11_stl_algorithms/README.md)
- Iterator Types
- Non-modifying Algorithms (find, count, search)
- Modifying Algorithms (sort, copy, transform)
- Numeric Algorithms
- Range-based For Loops (C++11)
- std::begin/end, move & minmax Algorithms (C++11)

### [12. Memory Management](12_memory_management/README.md)
- Stack vs Heap
- Dynamic Memory Allocation (new/delete)
- Memory Leaks
- Smart Pointers (C++11): unique_ptr, shared_ptr, weak_ptr
- RAII Principle

### [13. Exception Handling](13_exception_handling/README.md)
- Try-Catch Blocks
- Standard Exceptions
- Custom Exceptions
- Exception Specifications
- Stack Unwinding
- noexcept, exception_ptr & nested_exception (C++11)

### [14. File I/O](14_file_io/README.md)
- Input/Output Streams
- File Stream (ifstream, ofstream)
- Text File Operations
- Binary File Operations
- Seekg & Tellg

### [15. Advanced Features](15_advanced_features/README.md)
- Preprocessor Directives
- Macros
- Namespaces
- The Auto Keyword (C++11)
- Lambda Functions (C++11)
- Type Traits (C++11)
- Variadic Arguments
- Move Semantics (C++11)

### [16. Concurrency](16_concurrency/README.md)
- Thread Creation and Joining
- Passing Arguments to Threads
- Mutex, lock_guard, and unique_lock
- Condition Variables
- Atomic Types
- async, future, promise, and packaged_task
- Deadlock Avoidance and Thread Lifecycle

### [17. Template Metaprogramming](17_template_metaprogramming/README.md)
- Compile-Time Computation (Factorial, Fibonacci via struct TMP)
- Type Traits from Scratch (is_same, is_pointer, remove_const, conditional)
- Tag Dispatch and Policy-Based Design
- Expression Templates (eliminate temporaries, lazy evaluation)
- CRTP — Static Polymorphism and Mixins
- Advanced constexpr and SFINAE with the Void-Type Trick
- Variadic Templates and Parameter Packs

### [18. Memory Model](18_memory_model/README.md)
- Why the Memory Model Matters (data races = UB)
- std::memory_order Values (relaxed, acquire, release, acq_rel, seq_cst)
- Acquire/Release Semantics and happens-before
- Sequentially Consistent Operations and Total Order
- std::atomic_thread_fence — Group Variable Ordering
- Lock-Free Programming with Compare-and-Swap (CAS)

### [19. Modern C++ I — C++14 & C++17](19_modern_cpp_14_17/README.md)
- C++14 polish (generic lambdas, `make_unique`, return deduction)
- Structured bindings, `if constexpr`, CTAD
- Vocabulary types: `optional`, `variant`, `string_view`
- Fold expressions, `[[nodiscard]]`, guaranteed copy elision

### [20. Modern C++ II — C++20 & C++23](20_modern_cpp_20_23/README.md)
- Concepts, ranges, coroutines, modules (the "big four")
- Spaceship `<=>`, `std::span`, `std::format`/`std::print`
- `std::expected`, deducing `this`, `std::mdspan` (C++23)

### [21. Build, Linking & the ABI](21_build_linking_abi/README.md)
- Compilation pipeline, translation units, the One Definition Rule
- Name mangling, `extern "C"`, static vs dynamic linking
- The ABI, pImpl, and stable library boundaries; modern CMake

### [22. Tooling, Testing & Sanitizers](22_tooling_testing/README.md)
- Dependency management (vcpkg/Conan); clang-format/clang-tidy
- **Sanitizers** (ASan/UBSan/TSan/MSan); GoogleTest/Catch2; fuzzing
- Debugging/profiling tools; CI for C++

### [23. Time, Text & Filesystem](23_time_text_filesystem/README.md)
- `<chrono>` (type-safe time, C++20 calendars/zones)
- `std::format`/`print`, `<regex>` (and RE2), `<charconv>`
- `std::filesystem` (portable paths; TOCTOU)

### [24. Performance Engineering](24_performance_engineering/README.md)
- Measure-first; mechanical sympathy & the memory hierarchy
- Google Benchmark, perf/VTune flame graphs & hardware counters
- Branchless/SIMD/no-copy; false sharing; tail latency

### [25. Production & Operational Concerns](25_production_operational/README.md)
- Twelve-factor config; structured logging (spdlog) to stdout
- Observability (prometheus-cpp, OpenTelemetry); crash diagnostics
- Graceful `SIGTERM` shutdown; distroless containers; resilience

### [26. Data Validation & Serialization](26_data_validation_serialization/README.md)
- "Parse, don't validate"; nlohmann/json, simdjson; JSON Schema
- Protobuf, FlatBuffers/Cap'n Proto (zero-copy); schema evolution
- Error handling: exceptions vs `std::expected`

### [27. Security & Supply Chain](27_security_supply_chain/README.md)
- Memory safety (70% of CVEs); hardening flags; sanitizers + fuzzing
- Injection, crypto (libsodium), secrets, format-string bugs
- Supply chain: pinning, vendoring, SBOMs, signing (Sigstore/SLSA)

### [28. System Design & Distributed Systems](28_system_design_distributed/README.md)
- Fallacies & latency numbers; CAP/PACELC; consistency models
- Partitioning (consistent hashing), replication & quorums, consensus
- Idempotency, caching, designing for failure (fencing, metastability)

### [29. Data-Intensive Systems](29_data_intensive_systems/README.md)
- Storage engines (B-tree vs LSM — RocksDB/ClickHouse are C++)
- Indexes, transactions/MVCC, SQL vs NoSQL, OLTP vs OLAP
- Messaging & streaming (queues vs Kafka logs); access-pattern modeling

### [30. API & Interface Design](30_api_design/README.md)
- Interface principles; C++ library APIs (RAII, `expected`, the ABI)
- REST vs gRPC; versioning & backward/ABI compatibility
- AuthN/AuthZ, rate limiting, OpenAPI/`.proto` docs

### [31. Engineering Practice at Scale](31_engineering_practice/README.md)
- Design docs/RFCs; code review; sanitizers-in-CI quality gates
- Testing strategy; trunk-based dev; CI/CD with progressive delivery
- Incident response; tech debt; large-scale migrations; leadership

### [32. Coroutines & Asynchronous C++](32_coroutines_async/README.md)
- Stackless coroutines; `co_await`/`co_yield`/`co_return`; the frame & `coroutine_handle`
- The `promise_type` and awaiter protocols built from scratch; symmetric transfer
- Generators (`std::generator`), lazy `task<T>`, async I/O (Asio/stdexec/cppcoro), pitfalls

### [33. Modules in Practice (C++20)](33_modules/README.md)
- The `#include` model's defects vs. named modules; units, partitions, fragments
- Header units, BMI/CMI & dependency-ordered builds; toolchain reality (`import std;`)
- Migrating a large codebase: leaf-first, macro pitfalls, the adopt-deliberately verdict

### [34. Correctness: Exception Safety, the Object Model & UB](34_correctness_object_model/README.md)
- The four exception-safety guarantees; copy-and-swap; noexcept moves & vector growth
- RTTI (typeid/dynamic_cast), virtual inheritance & the diamond (vptr/vbptr layout)
- The undefined-behavior catalog; strict aliasing & `std::bit_cast`; UBSan/ASan/TSan
- const-correctness in depth (mutable, const-overloading, the const_cast trap)

### [35. Data Structures (Complete Module)](data_structures/README.md)
- Linear Structures (Array, Vector, Linked List, Stack, Queue, Deque)
- Hash Tables
- Trees (BST, Heap, Trie, AVL overview)
- Graphs (Adjacency List/Matrix, BFS, DFS)
- Advanced Structures (DSU, Fenwick Tree, Segment Tree)

### [36. Algorithms (Complete Module)](algorithms/README.md)
- Searching and Sorting
- Pattern-Based Techniques (Two Pointers, Sliding Window, Prefix Sum)
- Recursion and Backtracking
- Divide and Conquer, Greedy, Dynamic Programming
- Graph, String, Number Theory, Bit, and Advanced Algorithms

### [37. Competitive Programming](competitive_programming/README.md)
- Fast I/O and Contest Basics
- Time and Space Complexity Analysis
- STL Tricks for CP
- Number Theory (GCD, Sieve, Modular Arithmetic, Matrix Exponentiation)
- Graph Algorithms (Dijkstra, Floyd, MST, DSU, Bridges, SCC)
- Dynamic Programming (LCS, LIS, Knapsack, Bitmask, Digit, Tree DP)
- String Algorithms (KMP, Z-function, Hashing, Trie, Manacher)
- Bit Manipulation and Bitmask DP
- Computational Geometry (Convex Hull, Polygon Area)
- Game Theory (Nim, Sprague-Grundy)
- Contest Strategy, Debugging, Common Mistakes
- Ready-to-Use CP Template (DSU, Segment Tree, Fenwick Tree, Dijkstra)

---

## 🚀 Quick Start

### Learning Path
1. **Beginner**: Chapters 1-3 (Basics, Control Flow, Functions)
2. **Intermediate**: Chapters 4-8 (Pointers, OOP, Operators)
3. **Advanced**: Chapters 9-16 (Templates, STL, Memory, Advanced Features, Concurrency)
4. **Expert (C++11)**: Chapters 17-18 (Template Metaprogramming, Memory Model)
5. **Modern C++**: Chapters 19-20 (C++14/17, then C++20/23)
6. **Build & Applied**: Chapters 21-23 (Build/Linking/ABI, Tooling/Testing, Time/Text/Filesystem)
7. **Production**: Chapters 24-27 (Performance, Production Ops, Serialization, Security)
8. **Architecture / Staff+**: Chapters 28-31 (System Design, Data-Intensive, API Design, Engineering Practice)

### Compiling Examples
```bash
# Using g++ with C++11 standard
g++ -std=c++11 filename.cpp -o output

# Run
./output
```

---

## 📝 Code Examples

Each chapter includes:
- **Detailed explanations** with code comments
- **Complete working examples** in `/code_examples/`
- **Best practices** and common mistakes
- **Performance considerations**

### Example Structure
```
Chapter X
├── README.md          (Full explanation)
├── examples.cpp       (Code examples)
└── best_practices.md  (Tips & tricks)
```

---

## 🎯 Learning Objectives

By completing this book, you will understand:

✅ C++ fundamentals and syntax  
✅ Object-oriented programming principles  
✅ Memory management and pointers  
✅ STL containers and algorithms  
✅ Templates and generic programming  
✅ Advanced C++11 features  
✅ Core concurrency primitives and task-based execution  
✅ Best practices and design patterns  
✅ How to write efficient, maintainable code  

---

## 📊 C++11 Features Covered

This book specifically highlights C++11 enhancements:

- **Auto keyword** for type deduction
- **Lambda functions** for functional programming
- **Smart pointers** for automatic memory management
- **Range-based for loops** for cleaner iteration
- **Variadic templates** for flexible generic code
- **Move semantics** for performance optimization
- **nullptr** for null pointer safety
- **Unordered containers** for better performance
- **Standard concurrency library** for portable multi-threading

---

## 💡 Key Concepts Matrix

| Concept | Level | Chapter | Keywords |
|---------|-------|---------|----------|
| Variables | Beginner | 1 | types, initialization |
| Functions | Beginner | 3 | parameters, return |
| Pointers | Intermediate | 4 | address, dereference |
| Classes | Intermediate | 6 | encapsulation |
| Inheritance | Advanced | 7 | virtual, override |
| Templates | Advanced | 9 | generic, specialization |
| STL | Advanced | 10-11 | containers, algorithms |
| Memory | Advanced | 12 | new, delete, smart_ptr |
| Concurrency | Advanced | 16 | thread, mutex, future |

---

## 📚 Resources

- **Official Standard**: C++11 (ISO/IEC 14882:2011)
- **Compiler Support**: GCC, Clang, MSVC
- **Online Compilers**: ideone.com, repl.it, onlinegdb.com
- **References**: cppreference.com, cplusplus.com

---

## 🔗 Repository Structure

```
cpp_book/
├── 01_basics/
├── 02_control_flow/
├── 03_functions/
├── 04_pointers_references/
├── 05_arrays_strings/
├── 06_oop_basics/
├── 07_inheritance_polymorphism/
├── 08_operator_overloading/
├── 09_templates/
├── 10_stl_containers/
├── 11_stl_algorithms/
├── 12_memory_management/
├── 13_exception_handling/
├── 14_file_io/
├── 15_advanced_features/
├── 16_concurrency/
├── 17_template_metaprogramming/
├── 18_memory_model/
├── 19_modern_cpp_14_17/
├── 20_modern_cpp_20_23/
├── 21_build_linking_abi/
├── 22_tooling_testing/
├── 23_time_text_filesystem/
├── 24_performance_engineering/
├── 25_production_operational/
├── 26_data_validation_serialization/
├── 27_security_supply_chain/
├── 28_system_design_distributed/
├── 29_data_intensive_systems/
├── 30_api_design/
├── 31_engineering_practice/
├── data_structures/
├── algorithms/
├── code_examples/
├── exercises/
├── resources/
└── README.md (this file)
```

---

## 🎓 How to Use This Book

1. **Read the Chapter**: Start with the README.md in each chapter folder
2. **Study Examples**: Look at the detailed code examples
3. **Practice**: Try modifying the examples
4. **Solve Exercises**: Complete practice problems
5. **Build Projects**: Combine concepts to build real programs

---

## 💻 Compilation Flags

### Recommended Compilation
```bash
# Standard C++11
g++ -std=c++11 file.cpp -o output

# With warnings
g++ -std=c++11 -Wall -Wextra file.cpp -o output

# Optimized
g++ -std=c++11 -O2 file.cpp -o output

# Debug
g++ -std=c++11 -g file.cpp -o output
```

---

## 🤔 Common Questions

### Q: Does this cover modern C++ (14/17/20/23)?
A: Yes. Chapters 1–18 build the C++11 foundation; Chapters 19–20 bring you fully current through **C++14/17/20/23** (concepts, ranges, coroutines, modules, `std::expected`, and more), and the later chapters use modern idioms throughout.

### Q: How long to learn?
A: Beginner level (Chapters 1-5): 2-4 weeks  
Intermediate (Chapters 6-8): 3-4 weeks  
Advanced (Chapters 9-15): 4-6 weeks

### Q: Do I need prior programming experience?
A: Not required, but helpful. Chapter 1 starts from the basics.

### Q: Is C++ still relevant?
A: Yes! C++ is used in: Game Engines, OS Kernels, High-Frequency Trading, Embedded Systems, Graphics, etc.

---

## 📈 Progress Tracker

Track your learning progress:

- [ ] Chapter 1: Basics
- [ ] Chapter 2: Control Flow
- [ ] Chapter 3: Functions
- [ ] Chapter 4: Pointers & References
- [ ] Chapter 5: Arrays & Strings
- [ ] Chapter 6: OOP Basics
- [ ] Chapter 7: Inheritance
- [ ] Chapter 8: Operator Overloading
- [ ] Chapter 9: Templates
- [ ] Chapter 10: STL Containers
- [ ] Chapter 11: STL Algorithms
- [ ] Chapter 12: Memory Management
- [ ] Chapter 13: Exception Handling
- [ ] Chapter 14: File I/O
- [ ] Chapter 15: Advanced Features
- [ ] Chapter 16: Concurrency
- [ ] Chapter 17: Template Metaprogramming
- [ ] Chapter 18: Memory Model
- [ ] Chapter 19: Modern C++ I — C++14 & C++17
- [ ] Chapter 20: Modern C++ II — C++20 & C++23
- [ ] Chapter 21: Build, Linking & the ABI
- [ ] Chapter 22: Tooling, Testing & Sanitizers
- [ ] Chapter 23: Time, Text & Filesystem
- [ ] Chapter 24: Performance Engineering
- [ ] Chapter 25: Production & Operational Concerns
- [ ] Chapter 26: Data Validation & Serialization
- [ ] Chapter 27: Security & Supply Chain
- [ ] Chapter 28: System Design & Distributed Systems
- [ ] Chapter 29: Data-Intensive Systems
- [ ] Chapter 30: API & Interface Design
- [ ] Chapter 31: Engineering Practice at Scale

---

## 🤝 Contributing

Found an error or want to add content? Contributions welcome!

---

## 📄 License

This learning material is provided as-is for educational purposes.

---

## 🎉 Let's Get Started!

Begin with [Chapter 1: Basics](01_basics/README.md) and embark on your C++ journey!

**Happy Learning! 🚀**

## C++11 Project-Wide Example

```cpp
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int main() {
    vector<int> practice = {3, 1, 4, 1, 5};
    int total = accumulate(practice.begin(), practice.end(), 0);
    cout << "cpp_book practice sum = " << total << "\n";
    return 0;
}
```
