# Linked List -- NeetCode 150

```cpp
// Common node definition used throughout this file
struct ListNode {
    int val; ListNode* next;
    ListNode(int x=0, ListNode* n=nullptr): val(x), next(n){}
};
```

---

## Problem 35: Reverse Linked List
**LeetCode #206** | Reverse a singly linked list.

### Brute Force -- O(N) time, O(N) space (store in array, rebuild)
```cpp
ListNode* reverseList(ListNode* head) {
    vector<int> vals;
    for(ListNode* p = head; p; p = p->next) vals.push_back(p->val);
    ListNode* cur = head;
    for(int i = vals.size()-1; i >= 0; i--) { cur->val = vals[i]; cur = cur->next; }
    return head;
}
```

### Optimal -- O(N) time, O(1) space (iterative)
```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr, *cur = head;
    while(cur) { ListNode* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    return prev;
}
```

---

## Problem 36: Merge Two Sorted Lists
**LeetCode #21** | Merge two sorted linked lists.

### Brute Force -- O(N+M) time, O(N+M) space (collect all, sort, rebuild)
```cpp
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    vector<int> vals;
    for(ListNode* p = l1; p; p = p->next) vals.push_back(p->val);
    for(ListNode* p = l2; p; p = p->next) vals.push_back(p->val);
    sort(vals.begin(), vals.end());
    ListNode dummy; ListNode* cur = &dummy;
    for(int v : vals) { cur->next = new ListNode(v); cur = cur->next; }
    return dummy.next;
}
```

### Optimal -- O(N+M) time, O(1) space (in-place merge)
```cpp
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy; ListNode* cur = &dummy;
    while(l1 && l2) {
        if(l1->val <= l2->val) { cur->next = l1; l1 = l1->next; }
        else                   { cur->next = l2; l2 = l2->next; }
        cur = cur->next;
    }
    cur->next = l1 ? l1 : l2;
    return dummy.next;
}
```

---

## Problem 37: Reorder List
**LeetCode #143** | Reorder list: L0->Ln->L1->Ln-1->L2->Ln-2...

### Brute Force -- O(N) time, O(N) space (store in array)
```cpp
void reorderList(ListNode* head) {
    vector<ListNode*> nodes;
    for(ListNode* p = head; p; p = p->next) nodes.push_back(p);
    int l = 0, r = nodes.size()-1;
    while(l < r) { nodes[l]->next = nodes[r]; l++; if(l==r) break; nodes[r]->next = nodes[l]; r--; }
    nodes[l]->next = nullptr;
}
```

### Optimal -- O(N) time, O(1) space (find mid, reverse second half, merge)
```cpp
void reorderList(ListNode* head) {
    if(!head || !head->next) return;
    // 1. Find mid
    ListNode* slow = head, *fast = head;
    while(fast->next && fast->next->next) { slow = slow->next; fast = fast->next->next; }
    // 2. Reverse second half
    ListNode* prev = nullptr, *cur = slow->next;
    slow->next = nullptr;
    while(cur) { ListNode* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    // 3. Merge two halves
    ListNode* first = head, *second = prev;
    while(second) {
        ListNode* fn = first->next, *sn = second->next;
        first->next = second; second->next = fn;
        first = fn; second = sn;
    }
}
```

---

## Problem 38: Remove Nth Node From End of List
**LeetCode #19** | Remove the nth node from the end.

### Brute Force -- O(N) time, two passes
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    int len = 0;
    for(ListNode* p = head; p; p = p->next) len++;
    ListNode dummy(0, head); ListNode* cur = &dummy;
    for(int i = 0; i < len-n; i++) cur = cur->next;
    cur->next = cur->next->next;
    return dummy.next;
}
```

### Optimal -- O(N) time, O(1) space (one pass, two pointers)
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy, *slow = &dummy;
    for(int i = 0; i <= n; i++) fast = fast->next;
    while(fast) { fast = fast->next; slow = slow->next; }
    slow->next = slow->next->next;
    return dummy.next;
}
```

---

## Problem 39: Copy List with Random Pointer
**LeetCode #138** | Deep copy a linked list where each node has next and random pointer.

### Brute Force -- O(N) time, O(N) space (two passes with map)
```cpp
struct Node { int val; Node* next; Node* random; Node(int v):val(v),next(nullptr),random(nullptr){} };
Node* copyRandomList(Node* head) {
    unordered_map<Node*,Node*> mp;
    for(Node* p = head; p; p = p->next) mp[p] = new Node(p->val);
    for(Node* p = head; p; p = p->next) {
        mp[p]->next   = mp[p->next];
        mp[p]->random = mp[p->random];
    }
    return mp[head];
}
```

### Optimal -- O(N) time, O(1) space (interleave old and new nodes)
```cpp
Node* copyRandomList(Node* head) {
    if(!head) return nullptr;
    // Step 1: interleave: A->A'->B->B'->...
    for(Node* p = head; p; p = p->next->next) {
        Node* copy = new Node(p->val);
        copy->next = p->next; p->next = copy;
    }
    // Step 2: set random pointers
    for(Node* p = head; p; p = p->next->next)
        if(p->random) p->next->random = p->random->next;
    // Step 3: separate lists
    Node* dummy = new Node(0); Node* cur = dummy;
    for(Node* p = head; p; p = p->next) {
        cur->next = p->next; cur = cur->next; p->next = cur->next;
    }
    return dummy->next;
}
```

---

## Problem 40: Add Two Numbers
**LeetCode #2** | Add two numbers represented as reversed linked lists.

### Brute Force -- O(N) time (same as optimal, no simpler approach)
```cpp
// Extract numbers as strings, add, convert back (overflow prone for large nums)
```

### Optimal -- O(max(M,N)) time, O(max(M,N)) space (digit-by-digit with carry)
```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy; ListNode* cur = &dummy;
    int carry = 0;
    while(l1 || l2 || carry) {
        int sum = carry;
        if(l1) { sum += l1->val; l1 = l1->next; }
        if(l2) { sum += l2->val; l2 = l2->next; }
        carry = sum / 10;
        cur->next = new ListNode(sum % 10);
        cur = cur->next;
    }
    return dummy.next;
}
```

---

## Problem 41: Linked List Cycle
**LeetCode #141** | Detect if linked list has a cycle.

### Brute Force -- O(N) time, O(N) space (store visited nodes)
```cpp
bool hasCycle(ListNode* head) {
    unordered_set<ListNode*> seen;
    while(head) { if(seen.count(head)) return true; seen.insert(head); head = head->next; }
    return false;
}
```

### Optimal -- O(N) time, O(1) space (Floyd's cycle detection)
```cpp
bool hasCycle(ListNode* head) {
    ListNode* slow = head, *fast = head;
    while(fast && fast->next) {
        slow = slow->next; fast = fast->next->next;
        if(slow == fast) return true;
    }
    return false;
}
```

---

## Problem 42: Find The Duplicate Number
**LeetCode #287** | Array of N+1 integers where each is in [1,N]. Find the duplicate without modifying array.

### Brute Force -- O(N^2) time, O(1) extra space
```cpp
int findDuplicate(vector<int>& nums) {
    int n = nums.size();
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            if(nums[i] == nums[j]) return nums[i];
    return -1;
}
```

### Optimal -- O(N) time, O(1) space (Floyd's cycle detection on implicit linked list)
```cpp
int findDuplicate(vector<int>& nums) {
    int slow = nums[0], fast = nums[0];
    do { slow = nums[slow]; fast = nums[nums[fast]]; } while(slow != fast);
    slow = nums[0];
    while(slow != fast) { slow = nums[slow]; fast = nums[fast]; }
    return slow;
}
```

---

## Problem 43: LRU Cache
**LeetCode #146** | Implement LRU Cache with get and put in O(1).

### Brute Force -- O(N) put/get (scan list for LRU)
```cpp
// Uses ordered_map approach: O(N) for eviction
```

### Optimal -- O(1) time (doubly linked list + hash map)
```cpp
class LRUCache {
    int cap;
    list<pair<int,int>> lst; // {key, val} most recent at front
    unordered_map<int, list<pair<int,int>>::iterator> mp;
public:
    LRUCache(int capacity) : cap(capacity) {}
    int get(int key) {
        if(!mp.count(key)) return -1;
        lst.splice(lst.begin(), lst, mp[key]); // move to front
        return mp[key]->second;
    }
    void put(int key, int value) {
        if(mp.count(key)) {
            mp[key]->second = value;
            lst.splice(lst.begin(), lst, mp[key]);
        } else {
            if((int)lst.size() == cap) { mp.erase(lst.back().first); lst.pop_back(); }
            lst.push_front({key, value});
            mp[key] = lst.begin();
        }
    }
};
```

---

## Problem 44: Merge K Sorted Lists
**LeetCode #23** | Merge K sorted linked lists into one.

### Brute Force -- O(N*K) time (repeatedly merge pairs one by one)
```cpp
ListNode* mergeTwo(ListNode* a, ListNode* b) {
    ListNode d; ListNode* c = &d;
    while(a&&b){if(a->val<=b->val){c->next=a;a=a->next;}else{c->next=b;b=b->next;}c=c->next;}
    c->next=a?a:b; return d.next;
}
ListNode* mergeKLists(vector<ListNode*>& lists) {
    ListNode* res = nullptr;
    for(auto* l : lists) res = mergeTwo(res, l);
    return res;
}
```

### Optimal -- O(N log K) time (min-heap)
```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b){ return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    for(auto* l : lists) if(l) pq.push(l);
    ListNode dummy; ListNode* cur = &dummy;
    while(!pq.empty()) {
        cur->next = pq.top(); pq.pop();
        cur = cur->next;
        if(cur->next) pq.push(cur->next);
    }
    return dummy.next;
}
```

---

## Problem 45: Reverse Nodes in K-Group
**LeetCode #25** | Reverse the list in groups of k nodes.

### Brute Force -- O(N) time, O(N) space (collect vals, reverse in blocks)
```cpp
ListNode* reverseKGroup(ListNode* head, int k) {
    vector<int> vals;
    for(ListNode* p=head;p;p=p->next) vals.push_back(p->val);
    for(int i=0;i+k<=(int)vals.size();i+=k) reverse(vals.begin()+i,vals.begin()+i+k);
    ListNode* p=head; for(int v:vals){p->val=v;p=p->next;} return head;
}
```

### Optimal -- O(N) time, O(1) space (in-place group reversal)
```cpp
ListNode* reverseKGroup(ListNode* head, int k) {
    // Check if k nodes remain
    ListNode* check = head;
    for(int i=0;i<k;i++) { if(!check) return head; check=check->next; }
    // Reverse k nodes
    ListNode* prev=nullptr, *cur=head;
    for(int i=0;i<k;i++) { ListNode* nxt=cur->next; cur->next=prev; prev=cur; cur=nxt; }
    head->next = reverseKGroup(cur, k); // head is now tail of reversed group
    return prev;
}
```