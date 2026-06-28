// Chapter 2: Control Flow & Loops - Runnable Examples
// Compile with: g++ -std=c++11 chapter2_control_flow.cpp -o chapter2_control_flow

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ============================================================
// EXAMPLE 1: If-Else Statements
// ============================================================
void example1_if_else() {
    cout << "\n=== EXAMPLE 1: If-Else Statements ===" << endl;
    
    int score = 85;
    
    if (score >= 90) {
        cout << "Grade: A (Excellent)" << endl;
    } else if (score >= 80) {
        cout << "Grade: B (Good)" << endl;
    } else if (score >= 70) {
        cout << "Grade: C (Fair)" << endl;
    } else {
        cout << "Grade: F (Fail)" << endl;
    }
    
    // Nested if
    int age = 25;
    if (age >= 18) {
        if (age >= 65) {
            cout << "Senior citizen" << endl;
        } else {
            cout << "Adult" << endl;
        }
    } else {
        cout << "Minor" << endl;
    }
}

// ============================================================
// EXAMPLE 2: Switch Statements
// ============================================================
void example2_switch() {
    cout << "\n=== EXAMPLE 2: Switch Statements ===" << endl;
    
    int day = 3;
    
    switch (day) {
        case 1:
            cout << "Monday" << endl;
            break;
        case 2:
            cout << "Tuesday" << endl;
            break;
        case 3:
            cout << "Wednesday" << endl;
            break;
        case 4:
            cout << "Thursday" << endl;
            break;
        case 5:
            cout << "Friday" << endl;
            break;
        case 6:
            cout << "Saturday" << endl;
            break;
        case 7:
            cout << "Sunday" << endl;
            break;
        default:
            cout << "Invalid day" << endl;
    }
    
    // Switch with fall-through
    int grade = 2;
    switch (grade) {
        case 1: cout << "Very Good";
        case 2: cout << " - Good";
        case 3: cout << " - Fair" << endl;
            break;
        case 4: cout << "Poor" << endl;
            break;
    }
}

// ============================================================
// EXAMPLE 3: While Loop
// ============================================================
void example3_while_loop() {
    cout << "\n=== EXAMPLE 3: While Loop ===" << endl;
    
    int counter = 1;
    cout << "Countdown from 5: ";
    while (counter <= 5) {
        cout << counter << " ";
        counter++;
    }
    cout << endl;
    
    // Input validation with while
    int attempts = 0;
    int secret = 7;
    int guess;
    
    cout << "Guess a number (1-10): ";
    cin >> guess;
    
    while (guess != secret && attempts < 2) {
        cout << "Wrong! Try again: ";
        cin >> guess;
        attempts++;
    }
    
    if (guess == secret) {
        cout << "Correct!" << endl;
    } else {
        cout << "Game over. Secret was: " << secret << endl;
    }
}

// ============================================================
// EXAMPLE 4: Do-While Loop
// ============================================================
void example4_do_while() {
    cout << "\n=== EXAMPLE 4: Do-While Loop ===" << endl;
    
    // Executes at least once
    int i = 1;
    do {
        cout << "i = " << i << endl;
        i++;
    } while (i <= 3);
    
    // Menu-driven program
    int choice;
    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Print Hello" << endl;
        cout << "2. Print Goodbye" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: cout << "Hello!" << endl; break;
            case 2: cout << "Goodbye!" << endl; break;
            case 3: cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice" << endl;
        }
    } while (choice != 3);
}

// ============================================================
// EXAMPLE 5: For Loop
// ============================================================
void example5_for_loop() {
    cout << "\n=== EXAMPLE 5: For Loop ===" << endl;
    
    // Basic for loop
    cout << "Numbers 1-5: ";
    for (int i = 1; i <= 5; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    // Reverse loop
    cout << "Countdown: ";
    for (int i = 5; i >= 1; i--) {
        cout << i << " ";
    }
    cout << endl;
    
    // Multiplication table
    cout << "\n5 × 1 to 5 × 10:" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << "5 × " << i << " = " << (5 * i) << endl;
    }
}

// ============================================================
// EXAMPLE 6: Range-Based For Loop (C++11)
// ============================================================
void example6_range_for() {
    cout << "\n=== EXAMPLE 6: Range-Based For Loop (C++11) ===" << endl;
    
    vector<int> numbers = {10, 20, 30, 40, 50};
    
    cout << "Vector elements: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    
    // With reference to modify
    cout << "Doubled: ";
    for (int& num : numbers) {
        num *= 2;
        cout << num << " ";
    }
    cout << endl;
    
    // String iteration
    string text = "Hello";
    cout << "\nCharacters in '" << text << "': ";
    for (char c : text) {
        cout << c << " ";
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 7: Break Statement
// ============================================================
void example7_break() {
    cout << "\n=== EXAMPLE 7: Break Statement ===" << endl;
    
    // Break in for loop
    cout << "Numbers until 4: ";
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            break;  // Exit loop
        }
        cout << i << " ";
    }
    cout << endl;
    
    // Break in switch
    cout << "Break in switch: ";
    for (int i = 1; i <= 3; i++) {
        switch (i) {
            case 1: cout << "One ";
            case 2: cout << "Two ";
            case 3: cout << "Three";
                break;
        }
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 8: Continue Statement
// ============================================================
void example8_continue() {
    cout << "\n=== EXAMPLE 8: Continue Statement ===" << endl;
    
    // Skip even numbers
    cout << "Odd numbers 1-10: ";
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip to next iteration
        }
        cout << i << " ";
    }
    cout << endl;
    
    // Skip specific values
    cout << "Numbers except 3,6,9: ";
    for (int i = 1; i <= 10; i++) {
        if (i % 3 == 0) {
            continue;
        }
        cout << i << " ";
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 9: Nested Loops
// ============================================================
void example9_nested_loops() {
    cout << "\n=== EXAMPLE 9: Nested Loops ===" << endl;
    
    // 3×3 multiplication table
    cout << "Multiplication Table (1-3):" << endl;
    cout << "  ";
    for (int i = 1; i <= 3; i++) cout << i << " ";
    cout << endl;
    
    for (int i = 1; i <= 3; i++) {
        cout << i << ": ";
        for (int j = 1; j <= 3; j++) {
            cout << (i * j) << " ";
        }
        cout << endl;
    }
    
    // Pattern: Triangle
    cout << "\nTriangle Pattern:" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 0; j < i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
}

// ============================================================
// EXAMPLE 10: Complex Control Flow
// ============================================================
void example10_complex_control() {
    cout << "\n=== EXAMPLE 10: Complex Control Flow ===" << endl;
    
    // Find first prime number > 20
    cout << "First prime > 20: ";
    for (int num = 21; num <= 50; num++) {
        bool is_prime = true;
        
        if (num < 2) is_prime = false;
        
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        
        if (is_prime) {
            cout << num << endl;
            break;
        }
    }
    
    // Sum with continue/break
    vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sum = 0;
    
    for (int val : values) {
        if (val % 2 == 0) continue;  // Skip even
        sum += val;
        if (sum > 20) break;         // Stop if sum > 20
    }
    cout << "Sum of odd numbers until > 20: " << sum << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "==================================================" << endl;
    cout << "   CHAPTER 2: CONTROL FLOW & LOOPS (C++11)" << endl;
    cout << "==================================================" << endl;
    
    example1_if_else();
    example2_switch();
    example3_while_loop();
    example4_do_while();
    example5_for_loop();
    example6_range_for();
    example7_break();
    example8_continue();
    example9_nested_loops();
    example10_complex_control();
    
    cout << "\n==================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "==================================================" << endl;
    
    return 0;
}

/* 
LEARNING NOTES:

1. **If-Else**: Most basic control structure
   - Can chain multiple else-if
   - Use for decision making

2. **Switch**: For multiple discrete cases
   - Remember break statements!
   - Fall-through can be intentional

3. **While**: Loop while condition true
   - Condition checked before execution
   - Good for input validation

4. **Do-While**: At least one execution
   - Condition checked after execution
   - Useful for menu systems

5. **For**: When you know iteration count
   - Three parts: init; condition; increment
   - Cleaner than while for iterations

6. **Range-Based For (C++11)**: Iterate containers
   - Automatically handles iterators
   - Use reference for modification

7. **Break**: Immediately exit loop
   - Also works in switch statements
   - Breaks innermost loop only

8. **Continue**: Skip to next iteration
   - Useful for filtering values
   - Skips rest of loop body

9. **Nested Loops**: Loops within loops
   - O(n²) time complexity often
   - Great for matrices and patterns

10. **Complex Control**: Combine all concepts
    - Real-world code uses all these
    - Practice combinations
*/
