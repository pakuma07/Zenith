// Chapter 12: Memory Management - Runnable Examples
// Compile with: g++ -std=c++11 chapter12_memory_management.cpp -o chapter12_memory_management

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// ============================================================
// EXAMPLE 1: Raw Pointers (The Old Way)
// ============================================================
void example1_raw_pointers() {
    cout << "\n=== EXAMPLE 1: Raw Pointers (The Old Way) ===" << endl;
    
    int* ptr = new int(42);
    cout << "Allocated int with value: " << *ptr << endl;
    
    int* arr = new int[5];
    for (int i = 0; i < 5; i++) arr[i] = i * 10;
    
    cout << "Array: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl;
    
    delete ptr;      // Must delete single allocation
    delete[] arr;    // Must use delete[] for arrays
    
    cout << "Memory freed (note: this is error-prone!)" << endl;
}

// ============================================================
// EXAMPLE 2: Unique Pointer (Single Ownership)
// ============================================================
class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
    void use() { cout << "Using resource" << endl; }
};

void example2_unique_ptr() {
    cout << "\n=== EXAMPLE 2: Unique Pointer (Single Ownership) ===" << endl;
    
    {
        unique_ptr<Resource> ptr1(new Resource());
        ptr1->use();
        
        // Transfer ownership
        unique_ptr<Resource> ptr2 = move(ptr1);
        // ptr1 is now null, ptr2 owns the resource
        
        if (ptr1 == nullptr) cout << "ptr1 is null after move" << endl;
        ptr2->use();
    }  // ptr2 destroyed, resource freed automatically
    
    cout << "Automatic cleanup - no manual delete needed!" << endl;
}

// ============================================================
// EXAMPLE 3: Shared Pointer (Shared Ownership)
// ============================================================
void example3_shared_ptr() {
    cout << "\n=== EXAMPLE 3: Shared Pointer (Shared Ownership) ===" << endl;
    
    {
        shared_ptr<Resource> sp1(new Resource());
        {
            shared_ptr<Resource> sp2 = sp1;
            cout << "Reference count: " << sp1.use_count() << endl;
            sp2->use();
        }
        cout << "After sp2 destroyed, count: " << sp1.use_count() << endl;
        sp1->use();
    }
    cout << "All owners destroyed, resource freed" << endl;
}

// ============================================================
// EXAMPLE 4: Make_unique and Make_shared
// ============================================================
void example4_make_functions() {
    cout << "\n=== EXAMPLE 4: Make_unique and Make_shared (Preferred) ===" << endl;
    
    {
        // Preferred over unique_ptr<T>(new T(...))
        auto ptr1 = make_unique<int>(100);
        cout << "Made unique int: " << *ptr1 << endl;
        
        auto ptr2 = make_shared<Resource>();
        ptr2->use();
        cout << "Shared pointer count: " << ptr2.use_count() << endl;
    }
    cout << "Automatic cleanup" << endl;
}

// ============================================================
// EXAMPLE 5: Weak Pointer (Prevent Circular References)
// ============================================================
class Node {
public:
    int value;
    shared_ptr<Node> next;
    weak_ptr<Node> prev;  // Weak to break circular reference
    
    Node(int v) : value(v) {}
    ~Node() { cout << "Node " << value << " destroyed" << endl; }
};

void example5_weak_ptr() {
    cout << "\n=== EXAMPLE 5: Weak Pointer (Prevent Circular Refs) ===" << endl;
    
    {
        auto n1 = make_shared<Node>(1);
        auto n2 = make_shared<Node>(2);
        
        n1->next = n2;
        n2->prev = n1;  // Weak pointer - doesn't count as owner
        
        cout << "n1 use_count: " << n1.use_count() << endl;
        cout << "n2 use_count: " << n2.use_count() << endl;
    }
    cout << "Both nodes destroyed (no circular reference!)" << endl;
}

// ============================================================
// EXAMPLE 6: RAII (Resource Acquisition Is Initialization)
// ============================================================
class FileHandle {
private:
    FILE* handle;
public:
    FileHandle(const char* filename) {
        handle = fopen(filename, "r");
        cout << "File opened (or attempted)" << endl;
    }
    
    ~FileHandle() {
        if (handle) {
            fclose(handle);
            cout << "File closed" << endl;
        }
    }
    
    // Prevent copying
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};

void example6_raii() {
    cout << "\n=== EXAMPLE 6: RAII Pattern ===" << endl;
    
    {
        FileHandle file("nonexistent.txt");
        // File handle is open (or would be)
    }
    // File automatically closed by destructor
}

// ============================================================
// EXAMPLE 7: Move Semantics
// ============================================================
class BigData {
private:
    int* data;
    int size;
public:
    BigData(int sz) : size(sz) {
        data = new int[sz];
        cout << "BigData allocated" << endl;
    }
    
    // Move constructor - efficient transfer
    BigData(BigData&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "BigData moved (efficient!)" << endl;
    }
    
    ~BigData() {
        delete[] data;
        cout << "BigData destroyed" << endl;
    }
};

void example7_move_semantics() {
    cout << "\n=== EXAMPLE 7: Move Semantics ===" << endl;
    
    vector<BigData> vec;
    
    BigData temp(100);
    vec.push_back(move(temp));  // Move, not copy
    
    cout << "Vector push_back used move!" << endl;
}

// ============================================================
// EXAMPLE 8: Smart Pointers with Arrays
// ============================================================
void example8_smart_ptr_arrays() {
    cout << "\n=== EXAMPLE 8: Smart Pointers with Arrays ===" << endl;
    
    {
        // C++20 has unique_ptr<T[]>, for C++11 use vector
        unique_ptr<int[]> arr(new int[5]);
        
        for (int i = 0; i < 5; i++) arr[i] = i * 2;
        cout << "Array via unique_ptr: ";
        for (int i = 0; i < 5; i++) cout << arr[i] << " ";
        cout << endl;
    }
    cout << "Array freed automatically" << endl;
}

// ============================================================
// EXAMPLE 9: Smart Pointers in Containers
// ============================================================
void example9_smart_ptr_containers() {
    cout << "\n=== EXAMPLE 9: Smart Pointers in Containers ===" << endl;
    
    {
        vector<unique_ptr<Resource>> vec;
        
        vec.push_back(make_unique<Resource>());
        vec.push_back(make_unique<Resource>());
        vec.push_back(make_unique<Resource>());
        
        for (auto& res : vec) {
            res->use();
        }
    }
    cout << "All resources in vector freed automatically" << endl;
}

// ============================================================
// EXAMPLE 10: Delete and Nullptr Assignment
// ============================================================
void example10_best_practices() {
    cout << "\n=== EXAMPLE 10: Best Practices ===" << endl;
    
    cout << "Use smart pointers instead of raw pointers:" << endl;
    cout << "  ✓ automatic cleanup" << endl;
    cout << "  ✓ exception-safe" << endl;
    cout << "  ✓ prevents memory leaks" << endl;
    cout << "  ✓ clear ownership semantics" << endl;
    
    cout << "\nPrefer:" << endl;
    cout << "  - unique_ptr for single ownership" << endl;
    cout << "  - shared_ptr for shared ownership" << endl;
    cout << "  - make_unique/make_shared for creation" << endl;
    cout << "  - weak_ptr to break circular refs" << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 12: MEMORY MANAGEMENT (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_raw_pointers();
    example2_unique_ptr();
    example3_shared_ptr();
    example4_make_functions();
    example5_weak_ptr();
    example6_raii();
    example7_move_semantics();
    example8_smart_ptr_arrays();
    example9_smart_ptr_containers();
    example10_best_practices();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Raw Pointers**: Traditional approach (error-prone)
   - Manual new/delete pairing
   - Easy to forget or mismatch
   - Ownership unclear

2. **Unique Pointer**: Single ownership
   - Only one owner at a time
   - Move semantics for transfer
   - Automatic cleanup via destructor

3. **Shared Pointer**: Multiple owners
   - Reference counted
   - Last owner cleanup
   - Slight overhead for reference count

4. **Make Functions**: Recommended creation
   - make_unique, make_shared (C++11)
   - Exception safe
   - More efficient

5. **Weak Pointer**: Avoid circular references
   - Doesn't increment reference count
   - Must convert to shared_ptr to use
   - Crucial for graph-like structures

6. **RAII Pattern**: Core C++ principle
   - Resource = initialization
   - Automatic release = destruction
   - Works with smart pointers

7. **Move Semantics**: Efficient transfer
   - Rvalue references (&&)
   - Move constructor/assignment
   - Avoids unnecessary copies

8. **Smart Pointers with Arrays**: Modern C++
   - unique_ptr<T[]> for arrays
   - Or prefer vector<T>
   - Automatic [] deletion

9. **Containers with Smart Pointers**: Best practice
   - Polymorphic storage
   - Automatic memory management
   - Clean abstraction

10. **Best Practices Summary**:
    - Always use smart pointers
    - Avoid raw new/delete
    - Use RAII for resources
    - move() when appropriate
    - Containers > arrays
*/
