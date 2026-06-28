# Exercises: Binary Search Tree

## Easy

1. Insert key in BST.
2. Search key in BST.
3. Find minimum and maximum key.
4. Validate BST property.
5. Inorder traversal should return sorted order.

## Medium

1. Delete node from BST.
2. Find inorder predecessor/successor.
3. Kth smallest element.
4. Range sum in BST.
5. Construct BST from sorted array.

## Hard

1. Recover swapped nodes in BST.
2. Largest BST in a binary tree.
3. Merge two BSTs into sorted output.
4. Build BST from preorder only.
5. Count pairs across two BSTs with sum x.

## Challenge

Implement BST iterator with O(h) memory and amortized O(1) next.

## C++11 Exercise Example: BST Search

```cpp
struct Node { int v; Node* l; Node* r; explicit Node(int x): v(x), l(nullptr), r(nullptr) {} };

bool findBST(Node* root, int x) {
    while (root) {
        if (x == root->v) return true;
        root = (x < root->v) ? root->l : root->r;
    }
    return false;
}
```
