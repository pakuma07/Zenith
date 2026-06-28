// trees.cpp
// Tree-based structures: an unbalanced Binary Search Tree (insert/search/inorder),
// a min-heap via std::priority_queue, and a 26-way Trie for lowercase strings.
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

// BSTNode: a single binary-search-tree node holding a key and two child pointers.
struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Insert key into the BST, returning the (possibly new) subtree root.
// Invariant: left keys < node key < right keys; duplicates ignored.
// O(h) time, h = tree height (O(log n) if balanced, O(n) worst case).
BSTNode* insertBST(BSTNode* root, int key) {
    if (!root) return new BSTNode(key);                              // empty spot: create the node
    if (key < root->key) root->left = insertBST(root->left, key);    // go left for smaller keys
    else if (key > root->key) root->right = insertBST(root->right, key); // go right for larger
    return root;                                                     // equal key: no-op, return current root
}

// Search for key, following the BST ordering to prune half the tree each step. O(h).
bool searchBST(BSTNode* root, int key) {
    if (!root) return false;                              // fell off the tree: not present
    if (root->key == key) return true;
    if (key < root->key) return searchBST(root->left, key);  // smaller -> left subtree
    return searchBST(root->right, key);                      // larger -> right subtree
}

// In-order traversal (left, node, right) prints keys in sorted ascending order. O(n).
void inorder(BSTNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}

// Post-order delete: free both subtrees before the node itself to avoid dangling pointers. O(n).
void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;  // safe to delete now that children are gone
}

// TrieNode: node in a prefix tree; one child slot per lowercase letter ('a'..'z').
// isEnd marks that some inserted word terminates here.
struct TrieNode {
    bool isEnd;
    TrieNode* children[26];
    TrieNode() : isEnd(false) {
        for (int i = 0; i < 26; ++i) children[i] = nullptr;  // start with no children
    }
};

// Insert word by walking/creating one node per character. O(L) for word length L.
void trieInsert(TrieNode* root, const string& word) {
    TrieNode* cur = root;
    for (char c : word) {
        int idx = c - 'a';                                       // map letter to child slot 0..25
        if (!cur->children[idx]) cur->children[idx] = new TrieNode();  // create branch if missing
        cur = cur->children[idx];                                // descend
    }
    cur->isEnd = true;  // mark the final node as a complete word
}

// Search for an exact word: follow each character, then require an end marker. O(L).
bool trieSearch(TrieNode* root, const string& word) {
    TrieNode* cur = root;
    for (char c : word) {
        int idx = c - 'a';
        if (!cur->children[idx]) return false;  // missing branch -> word absent
        cur = cur->children[idx];
    }
    return cur->isEnd;  // path exists; true only if it marks a full word (not just a prefix)
}

// Post-order delete of all trie nodes. O(total nodes).
void freeTrie(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < 26; ++i) freeTrie(root->children[i]);  // free every child subtree first
    delete root;
}

// Driver: exercises the BST, a min-heap, and the Trie, printing sample results.
int main() {
    cout << "== Binary Search Tree ==" << endl;
    BSTNode* root = nullptr;
    vector<int> keys = {15, 8, 22, 4, 10, 18, 30};
    for (int k : keys) root = insertBST(root, k);

    cout << "BST inorder: ";
    inorder(root);
    cout << endl;
    cout << "search 10: " << (searchBST(root, 10) ? "found" : "not found") << endl;

    cout << "\n== Min Heap (priority_queue) ==" << endl;
    // greater<int> comparator flips the default max-heap into a min-heap (smallest on top).
    // push/pop are O(log n); top is O(1).
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(5);
    minHeap.push(2);
    minHeap.push(8);
    minHeap.push(1);
    cout << "heap pop order: ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";
        minHeap.pop();
    }
    cout << endl;

    cout << "\n== Trie ==" << endl;
    TrieNode* trieRoot = new TrieNode();
    trieInsert(trieRoot, "cat");
    trieInsert(trieRoot, "car");
    trieInsert(trieRoot, "dog");

    cout << "search cat: " << (trieSearch(trieRoot, "cat") ? "yes" : "no") << endl;
    cout << "search cap: " << (trieSearch(trieRoot, "cap") ? "yes" : "no") << endl;

    freeTrie(trieRoot);
    freeBST(root);
    return 0;
}
