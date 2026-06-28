# Trees -- NeetCode 150

```cpp
struct TreeNode { int val; TreeNode *left, *right; TreeNode(int x=0,TreeNode*l=nullptr,TreeNode*r=nullptr):val(x),left(l),right(r){} };
```

---

## Problem 46: Invert Binary Tree
**LeetCode #226**

### Brute Force -- O(N) BFS (queue-based)
```cpp
TreeNode* invertTree(TreeNode* root) {
    if(!root) return nullptr;
    queue<TreeNode*> q; q.push(root);
    while(!q.empty()) {
        TreeNode* n = q.front(); q.pop();
        swap(n->left, n->right);
        if(n->left)  q.push(n->left);
        if(n->right) q.push(n->right);
    }
    return root;
}
```

### Optimal -- O(N) time, O(H) space (recursive DFS)
```cpp
TreeNode* invertTree(TreeNode* root) {
    if(!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left); invertTree(root->right);
    return root;
}
```

---

## Problem 47: Maximum Depth of Binary Tree
**LeetCode #104**

### Brute Force -- O(N) BFS level counting
```cpp
int maxDepth(TreeNode* root) {
    if(!root) return 0;
    queue<TreeNode*> q; q.push(root); int depth=0;
    while(!q.empty()) { depth++; for(int i=q.size();i>0;i--) { auto n=q.front();q.pop(); if(n->left)q.push(n->left); if(n->right)q.push(n->right); } }
    return depth;
}
```

### Optimal -- O(N) time, O(H) space (DFS)
```cpp
int maxDepth(TreeNode* root) {
    if(!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

---

## Problem 48: Diameter of Binary Tree
**LeetCode #543** | Longest path (number of edges) between any two nodes.

### Brute Force -- O(N^2) (compute height for each node separately)
```cpp
int height(TreeNode* r){return r?1+max(height(r->left),height(r->right)):0;}
int diameterOfBinaryTree(TreeNode* root) {
    if(!root) return 0;
    int through = height(root->left) + height(root->right);
    return max({through, diameterOfBinaryTree(root->left), diameterOfBinaryTree(root->right)});
}
```

### Optimal -- O(N) time (single DFS, track global max)
```cpp
int diameterOfBinaryTree(TreeNode* root) {
    int best = 0;
    function<int(TreeNode*)> dfs = [&](TreeNode* n) -> int {
        if(!n) return 0;
        int l = dfs(n->left), r = dfs(n->right);
        best = max(best, l+r);
        return 1 + max(l, r);
    };
    dfs(root);
    return best;
}
```

---

## Problem 49: Balanced Binary Tree
**LeetCode #110** | Determine if tree is height-balanced (left/right heights differ by at most 1).

### Brute Force -- O(N^2) (compute height at each node)
```cpp
int ht(TreeNode* r){return r?1+max(ht(r->left),ht(r->right)):0;}
bool isBalanced(TreeNode* root) {
    if(!root) return true;
    return abs(ht(root->left)-ht(root->right))<=1 && isBalanced(root->left) && isBalanced(root->right);
}
```

### Optimal -- O(N) time (DFS returns -1 on imbalance)
```cpp
bool isBalanced(TreeNode* root) {
    function<int(TreeNode*)> dfs = [&](TreeNode* n) -> int {
        if(!n) return 0;
        int l = dfs(n->left); if(l==-1) return -1;
        int r = dfs(n->right); if(r==-1) return -1;
        if(abs(l-r)>1) return -1;
        return 1+max(l,r);
    };
    return dfs(root) != -1;
}
```

---

## Problem 50: Same Tree
**LeetCode #100**

### Brute Force -- O(N) serialize both trees, compare strings
```cpp
string ser(TreeNode* r){return r? to_string(r->val)+","+ser(r->left)+","+ser(r->right):"#";}
bool isSameTree(TreeNode* p, TreeNode* q){return ser(p)==ser(q);}
```

### Optimal -- O(N) time, O(H) space (recursive comparison)
```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if(!p && !q) return true;
    if(!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
```

---

## Problem 51: Subtree of Another Tree
**LeetCode #572**

### Brute Force -- O(S*T) check isSameTree at every node of s
```cpp
bool isSame(TreeNode* a,TreeNode* b){if(!a&&!b)return true;if(!a||!b||a->val!=b->val)return false;return isSame(a->left,b->left)&&isSame(a->right,b->right);}
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if(!root) return false;
    if(isSame(root, subRoot)) return true;
    return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}
```

### Optimal -- O(S*T) worst case same, but subtree check is clean single pass
```cpp
// Same as above -- O(S*T) is accepted; S, T <= 2000
// Truly optimal uses tree hashing/serialization: O(S+T) -- for reference:
// Serialize both trees (null markers included), check if sub-serialization of subRoot
// is a substring of root serialization.
```

---

## Problem 52: Lowest Common Ancestor of BST
**LeetCode #235**

### Brute Force -- O(N) generic LCA without using BST property
```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if(!root || root==p || root==q) return root;
    TreeNode* l = lowestCommonAncestor(root->left,p,q);
    TreeNode* r = lowestCommonAncestor(root->right,p,q);
    if(l&&r) return root;
    return l?l:r;
}
```

### Optimal -- O(H) time using BST property
```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while(root) {
        if(p->val < root->val && q->val < root->val) root = root->left;
        else if(p->val > root->val && q->val > root->val) root = root->right;
        else return root;
    }
    return nullptr;
}
```

---

## Problem 53: Binary Tree Level Order Traversal
**LeetCode #102**

### Brute Force -- O(N) DFS with depth tracking
```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    function<void(TreeNode*,int)> dfs=[&](TreeNode* n,int d){
        if(!n) return; if(d==(int)res.size()) res.push_back({});
        res[d].push_back(n->val); dfs(n->left,d+1); dfs(n->right,d+1);
    };
    dfs(root,0); return res;
}
```

### Optimal -- O(N) time, O(N) space (BFS)
```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if(!root) return res;
    queue<TreeNode*> q; q.push(root);
    while(!q.empty()) {
        res.push_back({});
        for(int i=q.size();i>0;i--) {
            auto n=q.front();q.pop(); res.back().push_back(n->val);
            if(n->left)q.push(n->left); if(n->right)q.push(n->right);
        }
    }
    return res;
}
```

---

## Problem 54: Binary Tree Right Side View
**LeetCode #199**

### Brute Force -- O(N) BFS, collect rightmost of each level
```cpp
vector<int> rightSideView(TreeNode* root) {
    vector<int> res;
    if(!root) return res;
    queue<TreeNode*> q; q.push(root);
    while(!q.empty()) {
        TreeNode* last=nullptr;
        for(int i=q.size();i>0;i--){auto n=q.front();q.pop();last=n;if(n->left)q.push(n->left);if(n->right)q.push(n->right);}
        res.push_back(last->val);
    }
    return res;
}
```

### Optimal -- O(N) DFS (right child first, record first node per depth)
```cpp
vector<int> rightSideView(TreeNode* root) {
    vector<int> res;
    function<void(TreeNode*,int)> dfs=[&](TreeNode* n,int d){
        if(!n)return; if(d==(int)res.size())res.push_back(n->val);
        dfs(n->right,d+1); dfs(n->left,d+1);
    };
    dfs(root,0); return res;
}
```

---

## Problem 55: Count Good Nodes in Binary Tree
**LeetCode #1448** | Node X is "good" if no node on path from root to X has value > X.

### Brute Force -- O(N * H) (for each node, trace path to root)
```cpp
// Impractical without parent pointers; use DFS with max instead
```

### Optimal -- O(N) DFS tracking max value on path
```cpp
int goodNodes(TreeNode* root) {
    int cnt = 0;
    function<void(TreeNode*,int)> dfs=[&](TreeNode* n,int mx){
        if(!n) return;
        if(n->val >= mx) cnt++;
        mx = max(mx, n->val);
        dfs(n->left,mx); dfs(n->right,mx);
    };
    dfs(root, INT_MIN);
    return cnt;
}
```

---

## Problem 56: Validate Binary Search Tree
**LeetCode #98**

### Brute Force -- O(N^2) (for each node verify all left < node < all right)
```cpp
bool allLess(TreeNode* r,int v){return r?r->val<v&&allLess(r->left,v)&&allLess(r->right,v):true;}
bool allGreater(TreeNode* r,int v){return r?r->val>v&&allGreater(r->left,v)&&allGreater(r->right,v):true;}
bool isValidBST(TreeNode* root){if(!root)return true;return allLess(root->left,root->val)&&allGreater(root->right,root->val)&&isValidBST(root->left)&&isValidBST(root->right);}
```

### Optimal -- O(N) DFS with valid range [min, max]
```cpp
bool isValidBST(TreeNode* root) {
    function<bool(TreeNode*,long,long)> dfs=[&](TreeNode* n,long lo,long hi)->bool{
        if(!n) return true;
        if(n->val<=lo || n->val>=hi) return false;
        return dfs(n->left,lo,n->val) && dfs(n->right,n->val,hi);
    };
    return dfs(root, LONG_MIN, LONG_MAX);
}
```

---

## Problem 57: Kth Smallest Element in BST
**LeetCode #230**

### Brute Force -- O(N) collect all values in vector, sort, return k-th
```cpp
int kthSmallest(TreeNode* root, int k) {
    vector<int> vals;
    function<void(TreeNode*)> dfs=[&](TreeNode* n){if(!n)return;dfs(n->left);vals.push_back(n->val);dfs(n->right);};
    dfs(root); return vals[k-1];
}
```

### Optimal -- O(H+k) inorder traversal, stop at k-th node
```cpp
int kthSmallest(TreeNode* root, int k) {
    int cnt=0, res=0;
    function<void(TreeNode*)> inorder=[&](TreeNode* n){
        if(!n||cnt>=k)return;
        inorder(n->left);
        if(++cnt==k){res=n->val;return;}
        inorder(n->right);
    };
    inorder(root); return res;
}
```

---

## Problem 58: Construct Binary Tree from Preorder and Inorder Traversal
**LeetCode #105**

### Brute Force -- O(N^2) (linear search for root in inorder each time)
```cpp
TreeNode* buildTree(vector<int>& pre, vector<int>& in) {
    function<TreeNode*(int,int,int,int)> build=[&](int ps,int pe,int is,int ie)->TreeNode*{
        if(ps>pe)return nullptr;
        TreeNode* root=new TreeNode(pre[ps]);
        int idx=find(in.begin()+is,in.begin()+ie+1,pre[ps])-in.begin();
        int leftSz=idx-is;
        root->left=build(ps+1,ps+leftSz,is,idx-1);
        root->right=build(ps+leftSz+1,pe,idx+1,ie);
        return root;
    };
    return build(0,pre.size()-1,0,in.size()-1);
}
```

### Optimal -- O(N) with hash map for inorder index lookup
```cpp
TreeNode* buildTree(vector<int>& pre, vector<int>& in) {
    unordered_map<int,int> idx;
    for(int i=0;i<(int)in.size();i++) idx[in[i]]=i;
    int pi=0;
    function<TreeNode*(int,int)> build=[&](int l,int r)->TreeNode*{
        if(l>r) return nullptr;
        TreeNode* root=new TreeNode(pre[pi++]);
        int mid=idx[root->val];
        root->left=build(l,mid-1); root->right=build(mid+1,r);
        return root;
    };
    return build(0,in.size()-1);
}
```

---

## Problem 59: Binary Tree Maximum Path Sum
**LeetCode #124** | Path can start/end at any node; find max sum.

### Brute Force -- O(N^2) (try every pair of nodes as path endpoints)
```cpp
// Difficult to implement cleanly; DFS approach is both correct and optimal
```

### Optimal -- O(N) DFS, at each node try including both children
```cpp
int maxPathSum(TreeNode* root) {
    int best = INT_MIN;
    function<int(TreeNode*)> dfs=[&](TreeNode* n)->int{
        if(!n) return 0;
        int l=max(0,dfs(n->left)), r=max(0,dfs(n->right));
        best = max(best, n->val+l+r); // path through n
        return n->val + max(l,r);     // return best single-branch to parent
    };
    dfs(root); return best;
}
```

---

## Problem 60: Serialize and Deserialize Binary Tree
**LeetCode #297**

### Brute Force -- O(N) level-order (BFS) serialization
```cpp
string serialize(TreeNode* root) {
    if(!root) return "";
    string res; queue<TreeNode*> q; q.push(root);
    while(!q.empty()){auto n=q.front();q.pop();if(n){res+=to_string(n->val)+",";q.push(n->left);q.push(n->right);}else res+="#,";}
    return res;
}
TreeNode* deserialize(string data) {
    if(data.empty()) return nullptr;
    istringstream ss(data); string tok; vector<string> tokens;
    while(getline(ss,tok,',')) tokens.push_back(tok);
    TreeNode* root=new TreeNode(stoi(tokens[0]));
    queue<TreeNode*> q; q.push(root); int i=1;
    while(!q.empty()&&i<(int)tokens.size()){
        auto n=q.front();q.pop();
        if(tokens[i++]!="#"){n->left=new TreeNode(stoi(tokens[i-1]));q.push(n->left);}
        if(i<(int)tokens.size()&&tokens[i++]!="#"){n->right=new TreeNode(stoi(tokens[i-1]));q.push(n->right);}
    }
    return root;
}
```

### Optimal -- O(N) preorder DFS serialization (cleaner, same complexity)
```cpp
string serialize(TreeNode* root) {
    if(!root) return "#,";
    return to_string(root->val)+","+serialize(root->left)+serialize(root->right);
}
TreeNode* deserialize(string data) {
    istringstream ss(data);
    function<TreeNode*()> build=[&]()->TreeNode*{
        string tok; getline(ss,tok,',');
        if(tok=="#") return nullptr;
        TreeNode* n=new TreeNode(stoi(tok));
        n->left=build(); n->right=build(); return n;
    };
    return build();
}
```