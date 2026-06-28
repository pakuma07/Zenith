# Algorithms Code Examples

Runnable C++11 examples for the algorithms module.

## Navigation

- Start at [Searching and Sorting Example](#1-searching_sortingcpp)
- Pair examples with [../README.md](../README.md)
- Keep [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md) open while reviewing

## Files

1. `searching_sorting.cpp`
   - Linear search
   - Binary search
   - Merge sort

   Next steps:
   - Matching theory: [../02_searching_sorting/searching.md](../02_searching_sorting/searching.md)
   - Matching theory: [../02_searching_sorting/sorting.md](../02_searching_sorting/sorting.md)
   - Matching exercises: [../exercises/searching_exercises.md](../exercises/searching_exercises.md)
   - Matching exercises: [../exercises/sorting_exercises.md](../exercises/sorting_exercises.md)
   - Next example: `pattern_based.cpp`

2. `pattern_based.cpp`
   - Two pointers
   - Sliding window

   Next steps:
   - Matching theory: [../03_pattern_based/two_pointers_sliding_window_prefix.md](../03_pattern_based/two_pointers_sliding_window_prefix.md)
   - Matching exercises: [../exercises/pattern_based_exercises.md](../exercises/pattern_based_exercises.md)
   - Previous example: `searching_sorting.cpp`
   - Next example: `recursion_backtracking.cpp`

3. `recursion_backtracking.cpp`
   - Recursive factorial
   - Subset generation

   Next steps:
   - Matching theory: [../04_recursion_backtracking/recursion_backtracking.md](../04_recursion_backtracking/recursion_backtracking.md)
   - Matching exercises: [../exercises/recursion_backtracking_exercises.md](../exercises/recursion_backtracking_exercises.md)
   - Previous example: `pattern_based.cpp`
   - Next example: `divide_and_conquer.cpp`

4. `divide_and_conquer.cpp`
   - Maximum subarray via divide and conquer

   Next steps:
   - Matching theory: [../05_divide_and_conquer/divide_and_conquer.md](../05_divide_and_conquer/divide_and_conquer.md)
   - Matching exercises: [../exercises/divide_and_conquer_exercises.md](../exercises/divide_and_conquer_exercises.md)
   - Previous example: `recursion_backtracking.cpp`
   - Next example: `greedy.cpp`

5. `greedy.cpp`
   - Activity selection

   Next steps:
   - Matching theory: [../06_greedy/greedy_algorithms.md](../06_greedy/greedy_algorithms.md)
   - Matching exercises: [../exercises/greedy_exercises.md](../exercises/greedy_exercises.md)
   - Previous example: `divide_and_conquer.cpp`
   - Next example: `dynamic_programming.cpp`

6. `dynamic_programming.cpp`
   - Fibonacci tabulation
   - 0/1 knapsack

   Next steps:
   - Matching theory: [../07_dynamic_programming/dynamic_programming.md](../07_dynamic_programming/dynamic_programming.md)
   - Matching exercises: [../exercises/dynamic_programming_exercises.md](../exercises/dynamic_programming_exercises.md)
   - Previous example: `greedy.cpp`
   - Next example: `graph_algorithms.cpp`

7. `graph_algorithms.cpp`
   - Dijkstra shortest path

   Next steps:
   - Matching theory: [../08_graph_algorithms/graph_algorithms.md](../08_graph_algorithms/graph_algorithms.md)
   - Matching exercises: [../exercises/graph_algorithms_exercises.md](../exercises/graph_algorithms_exercises.md)
   - Previous example: `dynamic_programming.cpp`
   - Next example: `string_algorithms.cpp`

8. `string_algorithms.cpp`
   - KMP LPS construction

   Next steps:
   - Matching theory: [../09_string_algorithms/string_algorithms.md](../09_string_algorithms/string_algorithms.md)
   - Matching exercises: [../exercises/string_algorithms_exercises.md](../exercises/string_algorithms_exercises.md)
   - Previous example: `graph_algorithms.cpp`
   - Next example: `number_theory_bit.cpp`

9. `number_theory_bit.cpp`
   - Fast exponentiation
   - Sieve of Eratosthenes

   Next steps:
   - Matching theory: [../10_number_theory_bit/number_theory_bit.md](../10_number_theory_bit/number_theory_bit.md)
   - Matching exercises: [../exercises/number_theory_bit_exercises.md](../exercises/number_theory_bit_exercises.md)
   - Previous example: `string_algorithms.cpp`
   - Next example: `advanced_algorithms.cpp`

10. `advanced_algorithms.cpp`
   - Floyd-Warshall

   Next steps:
   - Matching theory: [../11_advanced_algorithms/advanced_algorithms.md](../11_advanced_algorithms/advanced_algorithms.md)
   - Matching exercises: [../exercises/advanced_algorithms_exercises.md](../exercises/advanced_algorithms_exercises.md)
   - Previous example: `number_theory_bit.cpp`
   - Return to [Algorithms Module Overview](../README.md)

## Compilation

```bash
g++ -std=c++11 -Wall -Wextra searching_sorting.cpp -o searching_sorting
g++ -std=c++11 -Wall -Wextra pattern_based.cpp -o pattern_based
g++ -std=c++11 -Wall -Wextra recursion_backtracking.cpp -o recursion_backtracking
g++ -std=c++11 -Wall -Wextra divide_and_conquer.cpp -o divide_and_conquer
g++ -std=c++11 -Wall -Wextra greedy.cpp -o greedy
g++ -std=c++11 -Wall -Wextra dynamic_programming.cpp -o dynamic_programming
g++ -std=c++11 -Wall -Wextra graph_algorithms.cpp -o graph_algorithms
g++ -std=c++11 -Wall -Wextra string_algorithms.cpp -o string_algorithms
g++ -std=c++11 -Wall -Wextra number_theory_bit.cpp -o number_theory_bit
g++ -std=c++11 -Wall -Wextra advanced_algorithms.cpp -o advanced_algorithms
```

## Suggested Usage

- Read the matching chapter first.
- Compile and run the example.
- Modify inputs and test edge cases.
- Reimplement from memory afterward.

---

## Flow

- Theory path: [../README.md](../README.md)
- Practice path: [../exercises/README.md](../exercises/README.md)
- Quick cheat sheet: [../QUICK_REFERENCE.md](../QUICK_REFERENCE.md)

## C++11 Example Selector

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Try: g++ -std=c++11 graph_algorithms.cpp -o graph_algorithms\n";
    cout << "Then run: ./graph_algorithms (or graph_algorithms.exe on Windows)\n";
    return 0;
}
```