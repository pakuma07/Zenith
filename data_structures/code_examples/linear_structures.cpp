// linear_structures.cpp
// Demonstrates linear data structures: C-array, std::vector, a hand-written
// singly linked list, and stack/queue/deque adapters built on STL containers.

// A single node of a singly linked list: a value plus a pointer to the next node.
struct Node {
    int data;
    Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

// Singly linked list with head-insertion. Nodes are heap-allocated and chained
// one direction. Invariant: `head` points to the first node (or null if empty).
// push_front/print are O(1)/O(n); space O(n) for n nodes.
class SinglyLinkedList {
private:
    Node* head;
public:
    SinglyLinkedList() : head(nullptr) {}

    // Insert at the front: link the new node ahead of the old head, then
    // make it the new head. O(1) (no traversal needed).
    void push_front(int value) {
        Node* n = new Node(value);
        n->next = head;  // new node points to former first element
        head = n;        // head now references the new node
    }

    // Traverse from head to tail printing each value. O(n).
    void print() const {
        Node* cur = head;
        while (cur) {
            cout << cur->data << " ";
            cur = cur->next;  // advance the walking pointer
        }
        cout << endl;
    }

    // Destructor: free every node to avoid leaks. O(n).
    ~SinglyLinkedList() {
        while (head) {
            Node* tmp = head;      // remember current node
            head = head->next;     // unlink it by advancing head
            delete tmp;            // then free the detached node
        }
    }
};

int main() {
    cout << "== Array and Vector ==" << endl;
    // Fixed-size C array: contiguous, O(1) random access, size fixed at compile time.
    int arr[5] = {10, 20, 30, 40, 50};
    cout << "arr[2] = " << arr[2] << endl;

    // std::vector: dynamic contiguous array. push_back is amortized O(1);
    // insert in the middle is O(n) because trailing elements shift right.
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.insert(v.begin() + 1, 99);  // insert 99 at index 1, shifting 2 and 3 over

    cout << "vector: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    cout << "\n== Singly Linked List ==" << endl;
    SinglyLinkedList list;
    list.push_front(30);
    list.push_front(20);
    list.push_front(10);
    list.print();

    cout << "\n== Stack using vector ==" << endl;
    // Stack (LIFO): use the vector's back as the top. push/pop/top all O(1).
    vector<int> stackv;
    stackv.push_back(10);
    stackv.push_back(20);
    stackv.push_back(30);
    cout << "stack top: " << stackv.back() << endl;
    stackv.pop_back();  // remove the most recently pushed element
    cout << "stack top after pop: " << stackv.back() << endl;

    cout << "\n== Queue using deque ==" << endl;
    // Queue (FIFO): enqueue at the back, dequeue from the front. Both O(1) on a deque.
    deque<int> q;
    q.push_back(100);
    q.push_back(200);
    q.push_back(300);
    cout << "queue front: " << q.front() << endl;
    q.pop_front();  // dequeue: remove the oldest element
    cout << "queue front after dequeue: " << q.front() << endl;

    cout << "\n== Deque ==" << endl;
    // Double-ended queue: O(1) insert/remove at both ends.
    deque<int> d;
    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    cout << "deque: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    return 0;
}
