// Chapter 4: Pointers & References - Runnable Examples
// Compile with: g++ -std=c++11 chapter4_pointers_references.cpp -o chapter4_pointers_references

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// ============================================================
// EXAMPLE 1: Pointer Basics
// ============================================================
void example1_pointer_basics() {
    cout << "\n=== EXAMPLE 1: Pointer Basics ===" << endl;
    
    int x = 42;
    int* ptr = &x;           // Pointer to x
    
    cout << "Variable x: " << x << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Pointer value: " << ptr << endl;
    cout << "Dereference pointer: " << *ptr << endl;
    
    // Modify through pointer
    *ptr = 100;
    cout << "After *ptr = 100, x = " << x << endl;
    
    // Null pointer (C++11)
    int* null_ptr = nullptr;
    if (null_ptr == nullptr) {
        cout << "null_ptr is null" << endl;
    }
}

// ============================================================
// EXAMPLE 2: Pointer Arithmetic
// ============================================================
void example2_pointer_arithmetic() {
    cout << "\n=== EXAMPLE 2: Pointer Arithmetic ===" << endl;
    
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;  // Points to first element
    
    // Array access using pointers
    cout << "Array elements via pointers:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "arr[" << i << "] = " << *ptr << endl;
        ptr++;  // Move to next element
    }
    
    // Reset pointer
    ptr = arr;
    
    // Pointer subtraction
    cout << "\nPointer arithmetic:" << endl;
    cout << "ptr points to: " << *ptr << endl;
    cout << "ptr+2 points to: " << *(ptr + 2) << endl;
    cout << "ptr+4 points to: " << *(ptr + 4) << endl;
    
    // Pointer difference
    int* ptr1 = &arr[1];
    int* ptr2 = &arr[4];
    cout << "Distance between ptr2 and ptr1: " << (ptr2 - ptr1) << " elements" << endl;
}

// ============================================================
// EXAMPLE 3: Pointers to Pointers
// ============================================================
void example3_pointer_to_pointer() {
    cout << "\n=== EXAMPLE 3: Pointers to Pointers ===" << endl;
    
    int x = 100;
    int* ptr1 = &x;           // Pointer to x
    int** ptr2 = &ptr1;       // Pointer to ptr1
    
    cout << "x = " << x << endl;
    cout << "Address of x (&x): " << &x << endl;
    cout << "ptr1 (points to x): " << ptr1 << endl;
    cout << "*ptr1 (value at ptr1): " << *ptr1 << endl;
    
    cout << "\nAddress of ptr1 (&ptr1): " << &ptr1 << endl;
    cout << "ptr2 (points to ptr1): " << ptr2 << endl;
    cout << "*ptr2 (value at ptr2): " << *ptr2 << endl;
    cout << "**ptr2 (value at *ptr2): " << **ptr2 << endl;
    
    // Modify through double pointer
    **ptr2 = 999;
    cout << "\nAfter **ptr2 = 999, x = " << x << endl;
}

// ============================================================
// EXAMPLE 4: References
// ============================================================
void example4_references() {
    cout << "\n=== EXAMPLE 4: References ===" << endl;
    
    int x = 42;
    int& ref = x;  // Reference to x
    
    cout << "x = " << x << endl;
    cout << "ref = " << ref << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Address of ref: " << &ref << endl;
    
    // References refer to same variable
    ref = 100;
    cout << "\nAfter ref = 100:" << endl;
    cout << "x = " << x << endl;
    cout << "ref = " << ref << endl;
    
    // Can't rebind reference
    int y = 200;
    // ref = y;  // ERROR! This assigns value, not rebinding
    cout << "\nAfter ref = y (assignment, not rebinding):" << endl;
    cout << "x = " << x << " (changed)" << endl;
    cout << "y = " << y << endl;
    cout << "ref = " << ref << " (now equals y value)" << endl;
}

// ============================================================
// EXAMPLE 5: References vs Pointers
// ============================================================
void example5_refs_vs_pointers() {
    cout << "\n=== EXAMPLE 5: References vs Pointers ===" << endl;
    
    int x = 10;
    
    // Reference
    int& ref = x;
    cout << "Reference:" << endl;
    cout << "  ref = " << ref << endl;
    cout << "  No dereference needed" << endl;
    cout << "  Can't be null" << endl;
    cout << "  Can't be reassigned" << endl;
    
    // Pointer
    int* ptr = &x;
    cout << "\nPointer:" << endl;
    cout << "  *ptr = " << *ptr << endl;
    cout << "  Must dereference" << endl;
    cout << "  Can be null" << endl;
    cout << "  Can be reassigned: " << endl;
    
    int y = 20;
    ptr = &y;
    cout << "  After ptr = &y, *ptr = " << *ptr << endl;
}

// ============================================================
// EXAMPLE 6: Pass by Reference
// ============================================================
void swapByValue(int a, int b) {
    cout << "\n  swapByValue: BEFORE swap - a=" << a << ", b=" << b << endl;
    int temp = a;
    a = b;
    b = temp;
    cout << "  swapByValue: AFTER swap - a=" << a << ", b=" << b << endl;
}

void swapByReference(int& a, int& b) {
    cout << "\n  swapByReference: BEFORE swap - a=" << a << ", b=" << b << endl;
    int temp = a;
    a = b;
    b = temp;
    cout << "  swapByReference: AFTER swap - a=" << a << ", b=" << b << endl;
}

void example6_pass_by_reference() {
    cout << "\n=== EXAMPLE 6: Pass by Reference ===" << endl;
    
    int x = 5, y = 10;
    
    cout << "Original: x=" << x << ", y=" << y << endl;
    
    swapByValue(x, y);
    cout << "After swapByValue: x=" << x << ", y=" << y << " (unchanged)" << endl;
    
    swapByReference(x, y);
    cout << "After swapByReference: x=" << x << ", y=" << y << " (changed)" << endl;
}

// ============================================================
// EXAMPLE 7: Function Pointers
// ============================================================
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

int applyOperation(int a, int b, int (*func)(int, int)) {
    return func(a, b);
}

void example7_function_pointers() {
    cout << "\n=== EXAMPLE 7: Function Pointers ===" << endl;
    
    // Function pointer array
    int (*operations[])(int, int) = {add, subtract, multiply};
    
    cout << "Using function pointer array:" << endl;
    cout << "10 + 5 = " << applyOperation(10, 5, operations[0]) << endl;
    cout << "10 - 5 = " << applyOperation(10, 5, operations[1]) << endl;
    cout << "10 * 5 = " << applyOperation(10, 5, operations[2]) << endl;
    
    // Direct function pointer usage
    int (*op)(int, int) = add;
    cout << "\nUsing function pointer directly:" << endl;
    cout << "20 + 3 = " << op(20, 3) << endl;
    
    op = subtract;
    cout << "20 - 3 = " << op(20, 3) << endl;
}

// ============================================================
// EXAMPLE 8: Dynamic Memory Allocation
// ============================================================
void example8_dynamic_memory() {
    cout << "\n=== EXAMPLE 8: Dynamic Memory Allocation ===" << endl;
    
    // Single variable
    int* ptr_int = new int;
    *ptr_int = 42;
    cout << "Single int allocated: " << *ptr_int << endl;
    delete ptr_int;
    ptr_int = nullptr;  // Good practice
    
    // Dynamic array
    int size = 5;
    int* arr = new int[size];
    
    cout << "\nDynamic array:" << endl;
    for (int i = 0; i < size; i++) {
        arr[i] = (i + 1) * 10;
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }
    
    delete[] arr;  // Note: delete[], not delete
    arr = nullptr;
}

// ============================================================
// EXAMPLE 9: Smart Pointers (C++11)
// ============================================================
void example9_smart_pointers() {
    cout << "\n=== EXAMPLE 9: Smart Pointers (C++11) ===" << endl;
    
    {
        // Unique pointer
        unique_ptr<int> ptr1(new int(100));
        cout << "unique_ptr: " << *ptr1 << endl;
        
        // Move ownership
        unique_ptr<int> ptr2 = move(ptr1);
        cout << "After move, ptr2: " << *ptr2 << endl;
        // ptr1 is now null
    }  // Automatic cleanup
    
    cout << "After scope, memory automatically freed" << endl;
    
    {
        // Shared pointer
        shared_ptr<string> sp1(new string("Hello"));
        {
            shared_ptr<string> sp2 = sp1;
            cout << "\nshared_ptr reference count: " << sp1.use_count() << endl;
            cout << "Value: " << *sp2 << endl;
        }
        cout << "After sp2 destroyed, count: " << sp1.use_count() << endl;
    }  // All cleaned up
    
    cout << "After all shared_ptrs destroyed, memory freed" << endl;
}

// ============================================================
// EXAMPLE 10: Const References
// ============================================================
void printVector(const vector<int>& v) {
    cout << "Vector (const reference): ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
}

void example10_const_references() {
    cout << "\n=== EXAMPLE 10: Const References ===" << endl;
    
    vector<int> nums = {1, 2, 3, 4, 5};
    
    // Pass by const reference (efficient, safe)
    printVector(nums);
    
    cout << "Original vector unchanged: ";
    for (int x : nums) {
        cout << x << " ";
    }
    cout << endl;
    
    // Const pointer
    int x = 42;
    const int* const_ptr = &x;
    cout << "\nConst pointer: " << *const_ptr << endl;
    // *const_ptr = 100;  // ERROR: can't modify through const pointer
    
    // Const reference
    const int& const_ref = x;
    cout << "Const reference: " << const_ref << endl;
    // const_ref = 100;  // ERROR: can't modify through const reference
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 4: POINTERS & REFERENCES (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_pointer_basics();
    example2_pointer_arithmetic();
    example3_pointer_to_pointer();
    example4_references();
    example5_refs_vs_pointers();
    example6_pass_by_reference();
    example7_function_pointers();
    example8_dynamic_memory();
    example9_smart_pointers();
    example10_const_references();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Pointer Basics**: Variable holding memory address
   - Declare: int* ptr = &x;
   - Dereference: *ptr gives value
   - Address: &variable gives address

2. **Pointer Arithmetic**: Move through memory
   - ptr++ moves to next element
   - ptr + 2 moves 2 elements forward
   - Works on arrays naturally

3. **Pointer to Pointer**: Pointer holding address of pointer
   - Useful for dynamic allocation
   - **ptr2 dereferences twice

4. **References**: Alias to another variable
   - Like constant pointers, automatically dereferenced
   - Can't be null
   - Can't be reassigned

5. **Refs vs Pointers**: Know when to use each
   - Reference: safer, cleaner syntax
   - Pointer: more flexible, can be null

6. **Pass by Reference**: Modify original variable
   - Use for swaps, in-place modifications
   - More efficient than passing copies

7. **Function Pointers**: Pointers to functions
   - Use for callbacks, operations
   - Can store in arrays

8. **Dynamic Memory**: Allocate at runtime
   - new/delete for single items
   - new[]/delete[] for arrays
   - Remember to deallocate!

9. **Smart Pointers (C++11)**: Automatic memory management
   - unique_ptr: single owner
   - shared_ptr: multiple owners
   - Preferred over raw pointers

10. **Const References**: Safe, efficient parameter passing
    - Can't modify original through reference
    - No copy overhead
    - Use for large objects
*/
