# Linked Lists

This chapter has been split into micro-files.

## Structure Files

1. [Singly Linked List](structures/singly_linked_list.md)
2. [Doubly Linked List](structures/doubly_linked_list.md)
3. [Linear Complexity Table](structures/linear_complexity_table.md)

## Practice

- ../exercises/linked_lists_exercises.md

## C++11 Example: Singly Linked List Insert Front

```cpp
struct Node {
    int val;
    Node* next;
    Node(int v, Node* n = nullptr) : val(v), next(n) {}
};

Node* pushFront(Node* head, int x) {
    return new Node(x, head);
}
```