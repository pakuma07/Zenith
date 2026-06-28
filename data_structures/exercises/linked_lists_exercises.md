# Exercises: Linked Lists

## Easy

1. Insert node at head, tail, and position k.
2. Delete first occurrence of value x.
3. Find length of linked list.
4. Print middle node.
5. Reverse linked list iteratively.

## Medium

1. Detect cycle in linked list.
2. Remove nth node from end.
3. Merge two sorted linked lists.
4. Check if linked list is palindrome.
5. Reverse nodes in groups of k.

## Hard

1. Clone list with random pointer.
2. Flatten a multilevel linked list.
3. Sort linked list in O(n log n).
4. Find intersection point of two lists.
5. LRU cache using doubly linked list + hash map.

## Challenge

Implement your own doubly linked list class with iterator-style traversal.

## C++11 Exercise Example: Reverse List

```cpp
struct Node { int v; Node* next; Node(int x) : v(x), next(nullptr) {} };

Node* reverseList(Node* head) {
    Node* prev = nullptr;
    while (head) {
        Node* nxt = head->next;
        head->next = prev;
        prev = head;
        head = nxt;
    }
    return prev;
}
```
