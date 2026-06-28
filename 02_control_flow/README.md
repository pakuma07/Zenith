# Chapter 2: Control Flow & Loops

**Control flow** is what turns a straight-line list of statements into a program that makes decisions and repeats work. By default a C++ program executes top-to-bottom, one statement after another; control-flow constructs let you alter that order based on data. There are two broad families:

- **Selection** (`if`/`else`, `switch`, the ternary `?:`) — choose *which* code runs.
- **Iteration** (`for`, `while`, `do-while`, range-based `for`) — choose *how many times* code runs.

Two ideas underpin all of them. First, every condition is evaluated as a **`bool`**: a `true`/`false` value. Non-boolean values are *contextually converted* — any non-zero number or non-null pointer is `true`, while `0` and `nullptr` are `false`. Second, a **block** `{ ... }` groups statements and introduces a scope; variables declared inside a block are destroyed when the block ends. Always brace your conditionals and loops even when the body is a single line — it prevents a whole class of bugs when the body later grows.

---

## 2.1 If-Else Statements

The `if` statement is the most fundamental decision tool: it evaluates a boolean condition and runs its block only when the condition is `true`. Adding `else` provides an alternative block for the `false` case, and chaining `else if` lets you test a sequence of mutually exclusive conditions — the first one that is `true` wins, and the rest are skipped. This short-circuiting matters: order your conditions from most specific to most general, because once a branch matches, evaluation stops.

### Basic If Statement
```cpp
#include <iostream>
using namespace std;

int main() {
  int age = 20;
  
  if (age >= 18) {
    cout << "You are an adult" << endl;
  }
  
  return 0;
}
```

### If-Else Statement
```cpp
if (condition) {
  // Code executed if condition is true
} else {
  // Code executed if condition is false
}

// Example
int score = 75;

if (score >= 90) {
  cout << "Grade: A" << endl;
} else if (score >= 80) {
  cout << "Grade: B" << endl;
} else if (score >= 70) {
  cout << "Grade: C" << endl;
} else if (score >= 60) {
  cout << "Grade: D" << endl;
} else {
  cout << "Grade: F" << endl;
}
```

### Nested If Statements

A `if` placed inside another `if` lets you test a second condition only after the first has already passed — useful when one check is meaningful only in the context of another. Nesting more than two or three levels deep quickly becomes hard to read; when you find yourself deeply nested, consider combining conditions with `&&`, returning early, or extracting a helper function.

```cpp
int age = 25;
int income = 50000;

if (age >= 18) {
  if (income >= 40000) {
    cout << "Eligible for loan" << endl;
  } else {
    cout << "Income too low" << endl;
  }
} else {
  cout << "Too young" << endl;
}
```

### Ternary Operator

The ternary (conditional) operator `?:` is the only operator in C++ that takes three operands. It is an **expression**, not a statement, so it produces a value you can assign or pass directly — making it ideal for picking between two values in a single line. Prefer it over a full `if`/`else` only when both branches are short and side-effect-free; nesting ternaries (as shown below) is legal but should be used sparingly, since it sacrifices readability for brevity.

```cpp
condition ? value_if_true : value_if_false

// Example
int x = 10, y = 20;
int max = (x > y) ? x : y;  // max = 20

// Can be nested but use sparingly
string result = (x > y) ? "x is larger" : 
                (x < y) ? "y is larger" : 
                "equal";
```

---

## 2.2 Switch Statement

A `switch` compares a single **integral** expression (an `int`, `char`, `enum`, etc. — *not* a `string`, `double`, or arbitrary object) against a list of constant `case` labels and jumps to the matching one. Compilers can often implement this as a **jump table**, making it faster than a long `if`/`else if` chain when there are many discrete values. Two rules are essential: each `case` falls through to the next unless you end it with `break`, and the optional `default` label catches any value that matched no case. Because all cases share one scope, declaring a variable inside a case without its own `{ }` block can trigger a compiler error.

### Basic Switch
```cpp
int day = 3;
string dayName;

switch (day) {
  case 1:
    dayName = "Monday";
    break;
  case 2:
    dayName = "Tuesday";
    break;
  case 3:
    dayName = "Wednesday";
    break;
  case 4:
    dayName = "Thursday";
    break;
  case 5:
    dayName = "Friday";
    break;
  case 6:
    dayName = "Saturday";
    break;
  case 7:
    dayName = "Sunday";
    break;
  default:
    dayName = "Invalid day";
}

cout << dayName << endl;  // Output: Wednesday
```

### Switch with Fall-Through

**Fall-through** — execution continuing into the next case when `break` is omitted — is usually a bug, but it is occasionally the goal: stacking several `case` labels with no code between them lets multiple values share one block of code. Use this deliberately and, ideally, leave a comment so readers know the missing `break` is intentional rather than an oversight.

```cpp
char grade = 'B';
int points = 0;

switch (grade) {
  case 'A':
  case 'B':
    points = 10;  // Both A and B get 10 points
    break;
  case 'C':
    points = 5;
    break;
  case 'D':
    points = 2;
    break;
  case 'F':
    points = 0;
    break;
  default:
    cout << "Invalid grade" << endl;
}
```

### Switch vs If-Else
```cpp
// Switch: Better for multiple discrete values
// - Cleaner syntax for many cases
// - Potentially faster (jump table)
// - Only works with integral types

// If-Else: Better for ranges or complex conditions
// - Works with any data type
// - Can express complex boolean logic
// - More flexible

// Example: Use switch
int choice = 2;
switch (choice) {
  case 1: process1(); break;
  case 2: process2(); break;
  case 3: process3(); break;
}

// Example: Use if-else
if (age < 13) {
  childActivity();
} else if (age < 18) {
  teenActivity();
} else {
  adultActivity();
}
```

---

## 2.3 For Loops

The `for` loop is the workhorse of counted iteration. Its header packs three parts into one line: an **initialization** (run once, before the loop), a **condition** (checked before each pass — the loop ends when it becomes `false`), and an **increment** (run after each pass). Any of the three may be omitted. Because the loop variable is declared in the initialization, its scope is confined to the loop, which keeps surrounding code clean. Use a `for` loop when you know the iteration count or need an explicit index; reach for the range-based form below when you simply want to visit every element of a container.

### Traditional For Loop
```cpp
// for (initialization; condition; increment)

for (int i = 0; i < 5; i++) {
  cout << i << " ";  // Output: 0 1 2 3 4
}

// Counting down
for (int i = 5; i > 0; i--) {
  cout << i << " ";  // Output: 5 4 3 2 1
}

// Multiple iterations
for (int i = 1; i <= 3; i++) {
  for (int j = 1; j <= 3; j++) {
    cout << i << "," << j << " ";
  }
  cout << endl;
}
// Output:
// 1,1 1,2 1,3
// 2,1 2,2 2,3
// 3,1 3,2 3,3
```

### Range-Based For Loop (C++11)

C++11's range-based `for` iterates directly over the elements of any range — arrays, STL containers, `std::initializer_list`, or any type providing `begin()` and `end()`. It removes the index arithmetic and bounds checks that cause off-by-one errors. The choice of loop variable matters for both correctness and performance: use `auto value` to work on a **copy** of each element, `auto& value` to **modify** the originals in place, and `const auto& value` to **read** elements efficiently without copying (important for large objects like strings or vectors).

```cpp
#include <vector>
using namespace std;

// For containers
vector<int> vec = {1, 2, 3, 4, 5};

for (int num : vec) {
  cout << num << " ";  // Output: 1 2 3 4 5
}

// With auto type deduction
for (auto num : vec) {
  cout << num << " ";
}

// For arrays
int arr[] = {10, 20, 30};
for (int val : arr) {
  cout << val << " ";  // Output: 10 20 30
}

// Using reference to modify elements
for (int& num : vec) {
  num *= 2;  // Modify original
}

// Using const reference for efficiency (no copy)
for (const auto& num : vec) {
  cout << num << " ";
}
```

---

## 2.4 While & Do-While Loops

When you don't know in advance how many iterations are needed, a `while` loop is the natural choice: it repeats as long as its condition stays `true`, checking *before* each pass. This makes it ideal for reading input until a sentinel value, polling until a state changes, or any loop driven by a condition rather than a counter. The one responsibility you carry is ensuring the condition will eventually become `false` — otherwise the loop never ends (see 2.6).

### While Loop
```cpp
int i = 0;

while (i < 5) {
  cout << i << " ";
  i++;
}
// Output: 0 1 2 3 4

// Reading input
int num = 0;
while (num != -1) {
  cout << "Enter number (-1 to quit): ";
  cin >> num;
  
  if (num != -1) {
    cout << "You entered: " << num << endl;
  }
}
```

### Do-While Loop

The `do-while` loop is the mirror image of `while`: it runs its body **first** and checks the condition **afterward**, guaranteeing the body executes at least once. This fits situations where you must do something before you can decide whether to repeat — most commonly prompting for input and re-prompting until it is valid, or driving a menu that should display at least once. Note the required semicolon after the closing `while (...)`.

```cpp
// Executes body at least once, then checks condition

int i = 0;

do {
  cout << i << " ";
  i++;
} while (i < 5);
// Output: 0 1 2 3 4

// Menu example
int choice = 0;
do {
  cout << "\n1. Add\n2. Delete\n3. Exit\n";
  cout << "Choice: ";
  cin >> choice;
  
  if (choice == 1) addItem();
  else if (choice == 2) deleteItem();
  
} while (choice != 3);
```

### While vs Do-While
```cpp
// While: checks condition first
int i = 10;
while (i < 5) {
  cout << i;  // Never executes
}

// Do-While: executes body first
int j = 10;
do {
  cout << j;  // Executes once!
} while (j < 5);
```

---

## 2.5 Loop Control

Sometimes the normal start-to-finish flow of a loop isn't enough and you need to alter it mid-stream. Two keywords do this: `break` **terminates** the loop entirely and resumes execution after it, while `continue` **abandons the current iteration** and jumps straight to the next condition check (or increment, in a `for` loop). Both act only on the **innermost** loop that encloses them — a critical limitation to remember when working with nested loops. Used well, they flatten otherwise-tangled conditions; overused, they make a loop's exit points hard to trace.

### Break Statement
```cpp
// Exits loop immediately

for (int i = 0; i < 10; i++) {
  if (i == 5) {
    break;  // Exit when i == 5
  }
  cout << i << " ";
}
// Output: 0 1 2 3 4

// Search example
vector<int> vec = {1, 2, 3, 4, 5, 6};
bool found = false;

for (int i = 0; i < vec.size(); i++) {
  if (vec[i] == 4) {
    found = true;
    break;  // Stop searching
  }
}
```

### Continue Statement

`continue` is the tool for "skip the ones I don't care about." Instead of wrapping the body of a loop in a large `if` that indents everything, you can test for the unwanted case at the top and `continue` past it — leaving the main logic at a single, readable indentation level. This "guard clause" style is especially common when validating input or filtering elements.

```cpp
// Skips current iteration, continues with next

for (int i = 0; i < 10; i++) {
  if (i % 2 == 0) {
    continue;  // Skip even numbers
  }
  cout << i << " ";
}
// Output: 1 3 5 7 9

// Skip invalid input
while (true) {
  cout << "Enter positive number: ";
  int num;
  cin >> num;
  
  if (num <= 0) {
    cout << "Invalid! Try again.\n";
    continue;
  }
  
  process(num);
  break;
}
```

### Nested Loop Control

Because `break` and `continue` affect only the innermost loop, escaping *all* the way out of nested loops takes extra thought. C++ has no labeled `break` (unlike Java), so the common patterns are: set a boolean flag and test it in each loop's condition, move the loops into a function and `return`, or restructure so the inner work is a separate function. The flag approach shown below is the most portable; a `return` from a helper function is often the cleanest.

```cpp
// Break only exits innermost loop
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    if (j == 1) {
      break;  // Exits inner loop only
    }
    cout << "(" << i << "," << j << ") ";
  }
}
// Output: (0,0) (1,0) (2,0)

// To exit outer loop, use flag or restructure
bool done = false;
for (int i = 0; i < 3 && !done; i++) {
  for (int j = 0; j < 3; j++) {
    if (condition) {
      done = true;
      break;
    }
  }
}
```

---

## 2.6 Infinite Loops

A loop whose condition never becomes `false` runs forever. This is sometimes exactly what you want — event loops in games, servers accepting connections, and embedded firmware all rely on a deliberate `while (true)` (or the equivalent `for (;;)`) that runs until the program is told to stop, typically via an internal `break` or `return`. The danger lies in the *accidental* infinite loop: a missing increment, a condition that can never be met, or an unsigned counter that wraps around. When a program hangs and pins the CPU, an unintended infinite loop is the first thing to suspect.

### Intentional Infinite Loops
```cpp
// Event loop (common in games, servers)
while (true) {
  handleEvent();
  render();
}

// Server loop
for (;;) {
  Socket socket = server.accept();
  handleClient(socket);
}

// With break to exit
while (true) {
  cout << "Enter command: ";
  string cmd;
  cin >> cmd;
  
  if (cmd == "quit") break;
  
  processCommand(cmd);
}
```

### Accidental Infinite Loops (Avoid!)
```cpp
// ❌ Forgot to increment
for (int i = 0; i < 10; ) {  // Missing i++
  cout << i;
  // Will loop forever!
}

// ❌ Wrong condition
while (i >= 0) {
  i++;  // Will never become < 0
}

// ✅ Correct
for (int i = 0; i < 10; i++) {
  cout << i;
}
```

---

## 2.7 Best Practices

Control-flow code is read far more often than it is written, and small habits compound into big differences in clarity and speed. The guidelines below cluster around three themes: keep the **logic obvious** (favor the simplest construct that expresses the intent, and prefer range-based `for` when you're just visiting elements), avoid **hidden costs** inside loop bodies and conditions (don't recompute `size()` every pass, don't build strings by repeated concatenation), and lean on a handful of **recognizable patterns** (sum, find-max, count, all-match) that readers can identify at a glance. Note that several of these hand-written patterns have direct STL equivalents covered in [Chapter 11](../11_stl_algorithms/README.md) — `std::accumulate`, `std::max_element`, `std::count`, and `std::all_of` — which are usually preferable in production code.

### Readability
```cpp
// ✅ Clear loop logic
for (int i = 0; i < arr.size(); i++) {
  process(arr[i]);
}

// ✅ Range-based for (simpler when available)
for (int val : arr) {
  process(val);
}

// ❌ Complex loop logic
for (int i = 0; i < 100; i += (i % 2 == 0) ? 1 : 3) {
  // Confusing!
}
```

### Performance
```cpp
// ❌ Inefficient: string concatenation in loop
string result = "";
for (int i = 0; i < 1000000; i++) {
  result += to_string(i);  // Creates new string each iteration
}

// ✅ Efficient: use efficient string building
stringstream ss;
for (int i = 0; i < 1000000; i++) {
  ss << i;
}
string result = ss.str();

// ❌ Inefficient: function call in loop condition
for (int i = 0; i < vec.size(); i++) {
  // size() called every iteration
}

// ✅ Efficient: cache size
int size = vec.size();
for (int i = 0; i < size; i++) {
  process(vec[i]);
}

// ✅ Or use range-based for
for (auto item : vec) {
  process(item);
}
```

### Common Patterns
```cpp
// Sum elements
int sum = 0;
for (int val : vec) {
  sum += val;
}

// Find maximum
int max_val = INT_MIN;
for (int val : vec) {
  if (val > max_val) max_val = val;
}

// Count occurrences
int count = 0;
for (int val : vec) {
  if (val == target) count++;
}

// Check if all meet condition
bool all_positive = true;
for (int val : vec) {
  if (val <= 0) {
    all_positive = false;
    break;
  }
}
```

---

## Summary

| Construct | Use Case |
|-----------|----------|
| **if-else** | Boolean conditions |
| **switch** | Discrete value matching |
| **for** | Fixed repetitions |
| **while** | Condition-based repetition |
| **do-while** | At least one execution needed |
| **break** | Exit loop early |
| **continue** | Skip to next iteration |
| **range-for** | Iterate containers (C++11) |

---

## Next Steps
- Practice writing conditional logic
- Build loops for various patterns (sums, searches, etc.)
- Combine nested loops and control flow
- Move to [Chapter 3: Functions](../03_functions/README.md)
