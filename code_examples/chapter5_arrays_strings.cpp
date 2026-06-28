// Chapter 5: Arrays & Strings - Runnable Examples
// Compile with: g++ -std=c++11 chapter5_arrays_strings.cpp -o chapter5_arrays_strings

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// ============================================================
// EXAMPLE 1: Array Basics
// ============================================================
void example1_array_basics() {
    cout << "\n=== EXAMPLE 1: Array Basics ===" << endl;
    
    // Array declaration and initialization
    int arr1[5] = {10, 20, 30, 40, 50};
    int arr2[] = {1, 2, 3};  // Size inferred from initializer
    
    // Access and modify
    cout << "Array 1: ";
    for (int i = 0; i < 5; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;
    
    // Array size
    cout << "Size of arr1: " << sizeof(arr1) / sizeof(arr1[0]) << " elements" << endl;
    cout << "Size of arr2: " << sizeof(arr2) / sizeof(arr2[0]) << " elements" << endl;
    
    // Modify array
    arr1[2] = 999;
    cout << "After arr1[2] = 999: " << arr1[2] << endl;
}

// ============================================================
// EXAMPLE 2: Multi-dimensional Arrays
// ============================================================
void example2_multidimensional_arrays() {
    cout << "\n=== EXAMPLE 2: Multi-dimensional Arrays ===" << endl;
    
    // 2D array (3x3 matrix)
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    cout << "3x3 Matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    
    // Main diagonal
    cout << "\nMain diagonal: ";
    for (int i = 0; i < 3; i++) {
        cout << matrix[i][i] << " ";
    }
    cout << endl;
    
    // Row and column sum
    cout << "\nRow 1 sum: ";
    int row_sum = 0;
    for (int j = 0; j < 3; j++) {
        row_sum += matrix[1][j];
    }
    cout << row_sum << endl;
    
    cout << "Column 0 sum: ";
    int col_sum = 0;
    for (int i = 0; i < 3; i++) {
        col_sum += matrix[i][0];
    }
    cout << col_sum << endl;
}

// ============================================================
// EXAMPLE 3: Array Searching & Sorting
// ============================================================
void example3_search_sort() {
    cout << "\n=== EXAMPLE 3: Array Searching & Sorting ===" << endl;
    
    // Create array
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    cout << "Original array: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    
    // Linear search
    int target = 22;
    int pos = -1;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            pos = i;
            break;
        }
    }
    cout << "Linear search for " << target << ": ";
    if (pos != -1) cout << "Found at index " << pos << endl;
    else cout << "Not found" << endl;
    
    // Simple bubble sort
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    cout << "Sorted array: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 4: Vector Container
// ============================================================
void example4_vector() {
    cout << "\n=== EXAMPLE 4: Vector Container ===" << endl;
    
    // Create and initialize vector
    vector<int> v = {10, 20, 30};
    
    cout << "Initial vector: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Add elements
    v.push_back(40);
    v.push_back(50);
    cout << "After push_back: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Access elements
    cout << "First element: " << v[0] << endl;
    cout << "Last element: " << v.back() << endl;
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;
    
    // Remove element
    v.pop_back();
    cout << "After pop_back: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Insert at position
    v.insert(v.begin() + 1, 999);
    cout << "After insert at position 1: ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 5: String Basics
// ============================================================
void example5_string_basics() {
    cout << "\n=== EXAMPLE 5: String Basics ===" << endl;
    
    // String creation
    string s1 = "Hello";
    string s2 = "World";
    string s3;
    
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    
    // String length
    cout << "Length of s1: " << s1.length() << endl;
    cout << "Size of s1: " << s1.size() << endl;
    
    // Access characters
    cout << "First character: " << s1[0] << endl;
    cout << "Last character: " << s1[s1.length() - 1] << endl;
    
    // String concatenation
    string s4 = s1 + " " + s2;
    cout << "Concatenated: " << s4 << endl;
    
    // Modify character
    s1[0] = 'J';
    cout << "After s1[0] = 'J': " << s1 << endl;
    
    // Empty check
    s3 = "";
    cout << "Is s3 empty? " << (s3.empty() ? "Yes" : "No") << endl;
}

// ============================================================
// EXAMPLE 6: String Operations
// ============================================================
void example6_string_operations() {
    cout << "\n=== EXAMPLE 6: String Operations ===" << endl;
    
    string text = "The quick brown fox";
    
    // Find substring
    size_t pos = text.find("quick");
    cout << "Text: " << text << endl;
    cout << "Position of 'quick': " << pos << endl;
    
    // Substring
    string sub = text.substr(4, 5);  // Start at 4, length 5
    cout << "Substring from position 4, length 5: " << sub << endl;
    
    // Replace
    string original = "Hello World";
    original.replace(6, 5, "C++");
    cout << "After replace: " << original << endl;
    
    // Insert
    string s = "Hello World";
    s.insert(5, " Beautiful");
    cout << "After insert: " << s << endl;
    
    // Erase
    s = "Hello World";
    s.erase(5, 6);  // Erase from position 5, length 6
    cout << "After erase: " << s << endl;
    
    // Append
    s = "Hello";
    s.append(" World!");
    cout << "After append: " << s << endl;
}

// ============================================================
// EXAMPLE 7: String Case Conversion
// ============================================================
void example7_case_conversion() {
    cout << "\n=== EXAMPLE 7: String Case Conversion ===" << endl;
    
    string text = "Hello World";
    
    // Convert to uppercase
    string upper = text;
    for (char& c : upper) {
        c = toupper(c);
    }
    cout << "Original: " << text << endl;
    cout << "Uppercase: " << upper << endl;
    
    // Convert to lowercase
    string lower = text;
    for (char& c : lower) {
        c = tolower(c);
    }
    cout << "Lowercase: " << lower << endl;
    
    // Check palindrome
    string word = "racecar";
    string reversed = word;
    reverse(reversed.begin(), reversed.end());
    cout << "\nWord: " << word << endl;
    cout << "Reversed: " << reversed << endl;
    cout << "Is palindrome? " << (word == reversed ? "Yes" : "No") << endl;
}

// ============================================================
// EXAMPLE 8: String Comparison & Search
// ============================================================
void example8_string_comparison() {
    cout << "\n=== EXAMPLE 8: String Comparison & Search ===" << endl;
    
    string s1 = "apple";
    string s2 = "apple";
    string s3 = "banana";
    
    // Equality
    cout << "\"apple\" == \"apple\"? " << (s1 == s2 ? "Yes" : "No") << endl;
    cout << "\"apple\" == \"banana\"? " << (s1 == s3 ? "Yes" : "No") << endl;
    
    // Comparison (lexicographic)
    cout << "\"apple\" < \"banana\"? " << (s1 < s3 ? "Yes" : "No") << endl;
    cout << "\"banana\" < \"apple\"? " << (s3 < s1 ? "Yes" : "No") << endl;
    
    // Find character
    string text = "Hello World";
    pos = text.find('o');
    cout << "\nFirst 'o' in \"" << text << "\" at position: " << pos << endl;
    
    // Count occurrences
    int count = 0;
    string s = "banana";
    for (char c : s) {
        if (c == 'a') count++;
    }
    cout << "Occurrences of 'a' in \"banana\": " << count << endl;
}

// ============================================================
// EXAMPLE 9: Vector vs Array
// ============================================================
void example9_vector_vs_array() {
    cout << "\n=== EXAMPLE 9: Vector vs Array ===" << endl;
    
    // Array: fixed size
    int arr[5] = {1, 2, 3, 4, 5};
    cout << "Array (fixed size 5): ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    // Vector: dynamic size
    vector<int> v;
    cout << "\nVector (dynamic):" << endl;
    for (int i = 1; i <= 5; i++) {
        v.push_back(i);
        cout << "  Added " << i << ", size: " << v.size() << ", capacity: " << v.capacity() << endl;
    }
    
    cout << "\nVector elements: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Iteration
    cout << "\nIteration:" << endl;
    cout << "Array with loop: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl;
    
    cout << "Vector with range-based for: ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 10: Complex Array/String Operations
// ============================================================
void example10_complex_operations() {
    cout << "\n=== EXAMPLE 10: Complex Operations ===" << endl;
    
    // Find longest word in a sentence
    string sentence = "The quick brown fox jumps";
    vector<string> words;
    string word = "";
    
    for (char c : sentence) {
        if (c == ' ') {
            if (!word.empty()) words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    if (!word.empty()) words.push_back(word);
    
    cout << "Sentence: " << sentence << endl;
    cout << "Words: ";
    for (const string& w : words) cout << w << " ";
    cout << endl;
    
    // Find longest
    string longest = words[0];
    for (const string& w : words) {
        if (w.length() > longest.length()) {
            longest = w;
        }
    }
    cout << "Longest word: " << longest << " (length: " << longest.length() << ")" << endl;
    
    // Find anagram pair
    string s1 = "listen";
    string s2 = "silent";
    string sorted1 = s1, sorted2 = s2;
    sort(sorted1.begin(), sorted1.end());
    sort(sorted2.begin(), sorted2.end());
    cout << "\nAre \"" << s1 << "\" and \"" << s2 << "\" anagrams? ";
    cout << (sorted1 == sorted2 ? "Yes" : "No") << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 5: ARRAYS & STRINGS (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_array_basics();
    example2_multidimensional_arrays();
    example3_search_sort();
    example4_vector();
    example5_string_basics();
    example6_string_operations();
    example7_case_conversion();
    example8_string_comparison();
    example9_vector_vs_array();
    example10_complex_operations();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Array Basics**: Fixed-size collection of elements
   - Index starts at 0
   - Size must be known at compile time
   - Direct memory allocation on stack

2. **Multi-dimensional Arrays**: Arrays of arrays
   - 2D arrays like matrices
   - Access: arr[row][col]
   - Natural for grid-like data

3. **Array Search/Sort**: Fundamental algorithms
   - Linear search: O(n)
   - Bubble sort: O(n²) - simple but slow
   - Use STL algorithms for real code

4. **Vector Container**: Dynamic array (C++)
   - Size can change at runtime
   - More convenient than raw arrays
   - Bounds checking with .at()

5. **String Basics**: Text data in C++
   - Use std::string, not C-strings
   - Automatic length management
   - Rich set of operations

6. **String Operations**: Searching and manipulation
   - find(), substr(), replace()
   - insert(), erase(), append()
   - Safe and convenient

7. **Case Conversion**: Uppercase/lowercase
   - Use toupper() and tolower()
   - Range-based for loop for conversion
   - Case-insensitive comparison

8. **String Comparison**: Equality and ordering
   - operator== for exact match
   - operator< for lexicographic order
   - find() for substring search

9. **Vector vs Array**: When to use each
   - Array: compile-time size, slightly faster
   - Vector: runtime size, flexible
   - Prefer vector for most cases

10. **Complex Operations**: Real-world string processing
    - Word splitting and analysis
    - Anagram checking
    - Text manipulation
*/
