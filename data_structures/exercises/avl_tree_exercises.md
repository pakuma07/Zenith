# Exercises: AVL Tree

## Easy

1. Compute balance factor of each node.
2. Identify LL, RR, LR, RL cases for given insert sequence.
3. Perform single left rotation.
4. Perform single right rotation.
5. Verify AVL property after insertion.

## Medium

1. AVL insertion with rebalancing.
2. AVL deletion with rebalancing.
3. Count rotations needed for a sequence.
4. Print tree level-order with balance factors.
5. Compare BST vs AVL height on same input.

## Hard

1. Join two AVL trees.
2. Split AVL tree by key.
3. Order-statistics extension on AVL.
4. Persistent AVL (version snapshots).
5. Benchmark rebalancing overhead.

## Challenge

Build a map-like container using AVL as backend.

## C++11 Exercise Example: AVL Height Helpers

```cpp
struct Node { int v, h; Node* l; Node* r; Node(int x): v(x), h(1), l(nullptr), r(nullptr) {} };

int height(Node* n) { return n ? n->h : 0; }
void pull(Node* n) { if (n) n->h = 1 + max(height(n->l), height(n->r)); }
```
