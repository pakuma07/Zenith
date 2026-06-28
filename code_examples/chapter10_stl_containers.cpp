// Chapter 10: STL Containers - Runnable Examples
// Compile with: g++ -std=c++11 chapter10_stl_containers.cpp -o chapter10_stl_containers

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

// ============================================================
// EXAMPLE 1: Vector
// ============================================================
void example1_vector() {
    cout << "\n=== EXAMPLE 1: Vector ===" << endl;
    
    vector<int> v = {1, 2, 3};
    
    v.push_back(4);
    v.push_back(5);
    
    cout << "Vector: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    v.insert(v.begin() + 2, 99);
    cout << "After insert at position 2: ";
    for (int x : v) cout << x << " ";
    cout << endl;
    
    v.erase(v.begin() + 2);
    cout << "After erase: " << v.size() << " elements" << endl;
}

// ============================================================
// EXAMPLE 2: Deque
// ============================================================
void example2_deque() {
    cout << "\n=== EXAMPLE 2: Deque (Double-Ended Queue) ===" << endl;
    
    deque<int> dq;
    
    dq.push_back(2);
    dq.push_back(3);
    dq.push_front(1);
    dq.push_front(0);
    
    cout << "Deque: ";
    for (int x : dq) cout << x << " ";
    cout << endl;
    
    cout << "Front: " << dq.front() << ", Back: " << dq.back() << endl;
    
    dq.pop_front();
    dq.pop_back();
    cout << "After pop_front and pop_back: ";
    for (int x : dq) cout << x << " ";
    cout << endl;
}

// ============================================================
// EXAMPLE 3: List
// ============================================================
void example3_list() {
    cout << "\n=== EXAMPLE 3: List (Linked List) ===" << endl;
    
    list<int> l = {1, 2, 3};
    
    l.push_back(4);
    l.push_front(0);
    
    cout << "List: ";
    for (int x : l) cout << x << " ";
    cout << endl;
    
    l.insert(next(l.begin(), 2), 99);
    cout << "After insert at position 2: ";
    for (int x : l) cout << x << " ";
    cout << endl;
    
    l.remove(99);
    cout << "After remove(99): " << l.size() << " elements" << endl;
}

// ============================================================
// EXAMPLE 4: Set
// ============================================================
void example4_set() {
    cout << "\n=== EXAMPLE 4: Set (Unique, Sorted) ===" << endl;
    
    set<int> s;
    
    s.insert(5);
    s.insert(2);
    s.insert(8);
    s.insert(2);  // Duplicate ignored
    s.insert(1);
    
    cout << "Set (automatically sorted): ";
    for (int x : s) cout << x << " ";
    cout << endl;
    
    cout << "Contains 5? " << (s.count(5) ? "Yes" : "No") << endl;
    cout << "Contains 3? " << (s.count(3) ? "Yes" : "No") << endl;
    
    s.erase(2);
    cout << "After erase(2): " << s.size() << " elements" << endl;
}

// ============================================================
// EXAMPLE 5: Map
// ============================================================
void example5_map() {
    cout << "\n=== EXAMPLE 5: Map (Key-Value, Sorted) ===" << endl;
    
    map<string, int> m;
    
    m["Alice"] = 25;
    m["Bob"] = 30;
    m["Charlie"] = 28;
    
    cout << "Map contents:" << endl;
    for (auto& [name, age] : m) {
        cout << "  " << name << ": " << age << endl;
    }
    
    cout << "Alice's age: " << m["Alice"] << endl;
    
    m.erase("Bob");
    cout << "After erase(\"Bob\"): " << m.size() << " entries" << endl;
}

// ============================================================
// EXAMPLE 6: Unordered_Set
// ============================================================
void example6_unordered_set() {
    cout << "\n=== EXAMPLE 6: Unordered_Set (Unique, Unordered) ===" << endl;
    
    unordered_set<int> us;
    
    us.insert(5);
    us.insert(2);
    us.insert(8);
    us.insert(2);  // Duplicate ignored
    us.insert(1);
    
    cout << "Unordered_set (not necessarily sorted): ";
    for (int x : us) cout << x << " ";
    cout << endl;
    
    cout << "Size: " << us.size() << endl;
    cout << "Contains 8? " << (us.count(8) ? "Yes" : "No") << endl;
}

// ============================================================
// EXAMPLE 7: Unordered_Map
// ============================================================
void example7_unordered_map() {
    cout << "\n=== EXAMPLE 7: Unordered_Map (Key-Value, Unordered) ===" << endl;
    
    unordered_map<string, int> um;
    
    um["apple"] = 5;
    um["banana"] = 3;
    um["orange"] = 7;
    
    cout << "Unordered_map (iteration order undefined):" << endl;
    for (auto& [fruit, count] : um) {
        cout << "  " << fruit << ": " << count << endl;
    }
    
    cout << "Lookup 'banana': " << um["banana"] << endl;
}

// ============================================================
// EXAMPLE 8: Queue
// ============================================================
void example8_queue() {
    cout << "\n=== EXAMPLE 8: Queue (FIFO) ===" << endl;
    
    queue<int> q;
    
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    
    cout << "Queue size: " << q.size() << endl;
    cout << "Front: " << q.front() << endl;
    
    cout << "Dequeue all: ";
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 9: Stack
// ============================================================
void example9_stack() {
    cout << "\n=== EXAMPLE 9: Stack (LIFO) ===" << endl;
    
    stack<string> st;
    
    st.push("first");
    st.push("second");
    st.push("third");
    
    cout << "Stack size: " << st.size() << endl;
    cout << "Top: " << st.top() << endl;
    
    cout << "Pop all: ";
    while (!st.empty()) {
        cout << st.top() << " ";
        st.pop();
    }
    cout << endl;
}

// ============================================================
// EXAMPLE 10: Multimap and Multiset
// ============================================================
void example10_multi_containers() {
    cout << "\n=== EXAMPLE 10: Multimap & Multiset ===" << endl;
    
    multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    ms.insert(2);
    ms.insert(2);
    ms.insert(3);
    
    cout << "Multiset (allows duplicates): ";
    for (int x : ms) cout << x << " ";
    cout << endl;
    cout << "Count of 2: " << ms.count(2) << endl;
    
    multimap<string, int> mm;
    mm.insert({"fruit", 5});
    mm.insert({"vegetable", 3});
    mm.insert({"fruit", 7});
    mm.insert({"vegetable", 2});
    
    cout << "Multimap (key 'fruit' appears twice):" << endl;
    for (auto& [key, value] : mm) {
        cout << "  " << key << ": " << value << endl;
    }
}

// ============================================================
// Container Characteristics Comparison
// ============================================================
void container_comparison() {
    cout << "\n=== CONTAINER CHARACTERISTICS ===" << endl;
    cout << "Vector: Fast random access, slow middle insertion" << endl;
    cout << "Deque: Fast front/back, good random access" << endl;
    cout << "List: Fast middle operations, no random access" << endl;
    cout << "Set: Unique sorted, O(log n) insert/find" << endl;
    cout << "Map: Key-value pairs, sorted by key" << endl;
    cout << "Unordered_set: Unique, O(1) average insert/find" << endl;
    cout << "Unordered_map: Key-value, fast lookup" << endl;
    cout << "Queue: FIFO data structure" << endl;
    cout << "Stack: LIFO data structure" << endl;
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 10: STL CONTAINERS (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_vector();
    example2_deque();
    example3_list();
    example4_set();
    example5_map();
    example6_unordered_set();
    example7_unordered_map();
    example8_queue();
    example9_stack();
    example10_multi_containers();
    
    container_comparison();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Vector**: Resizable array
   - Random access: O(1)
   - Insert/delete at end: O(1)
   - Insert/delete middle: O(n)

2. **Deque**: Double-ended queue
   - Efficient front and back operations
   - Random access like vector
   - Insert/delete middle: O(n)

3. **List**: Doubly linked list
   - No random access (must iterate)
   - Efficient middle insertion: O(1)
   - Insert/delete any position: O(1) if have iterator

4. **Set**: Sorted unique values
   - Automatic sorting
   - No duplicates
   - O(log n) insert, find, erase

5. **Map**: Key-value pairs
   - Sorted by key
   - O(log n) operations
   - Access via key: m[key]

6. **Unordered_set**: Hash set
   - Unique values (unordered)
   - O(1) average insert/find
   - Order undefined

7. **Unordered_map**: Hash map
   - Key-value pairs (unordered)
   - O(1) average lookup
   - Faster than map in practice

8. **Queue**: FIFO structure
   - push() adds to back
   - front(), pop() from front
   - Common for scheduling

9. **Stack**: LIFO structure
   - push() adds to top
   - top(), pop() from top
   - Common for parsing

10. **Multi-containers**: Duplicates allowed
    - multiset: set with duplicates
    - multimap: map with duplicate keys
    - count() returns occurrence count
*/
