# Backtracking -- NeetCode 150

---

## Problem 71: Subsets
**LeetCode #78** | Return all subsets (power set) of distinct integers.

### Brute Force -- O(N * 2^N) bitmask iteration
```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    int n=nums.size(); vector<vector<int>> res;
    for(int mask=0;mask<(1<<n);mask++){
        vector<int> sub;
        for(int i=0;i<n;i++) if((mask>>i)&1) sub.push_back(nums[i]);
        res.push_back(sub);
    }
    return res;
}
```

### Optimal -- O(N * 2^N) backtracking
```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> res; vector<int> cur;
    function<void(int)> bt=[&](int i){
        res.push_back(cur);
        for(int j=i;j<(int)nums.size();j++){cur.push_back(nums[j]);bt(j+1);cur.pop_back();}
    };
    bt(0); return res;
}
```

---

## Problem 72: Combination Sum
**LeetCode #39** | Find all combinations that sum to target (reuse allowed).

### Brute Force -- generate all combinations up to target length, filter
```cpp
// Impractical -- use backtracking directly
```

### Optimal -- O(2^(T/min)) backtracking with pruning
```cpp
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(),candidates.end());
    vector<vector<int>> res; vector<int> cur;
    function<void(int,int)> bt=[&](int i,int rem){
        if(rem==0){res.push_back(cur);return;}
        for(int j=i;j<(int)candidates.size()&&candidates[j]<=rem;j++){
            cur.push_back(candidates[j]);bt(j,rem-candidates[j]);cur.pop_back();
        }
    };
    bt(0,target); return res;
}
```

---

## Problem 73: Combination Sum II
**LeetCode #40** | Each number used once; no duplicate combinations.

### Brute Force -- generate with duplicates, deduplicate using set
```cpp
vector<vector<int>> combinationSum2(vector<int>& c, int t) {
    sort(c.begin(),c.end()); set<vector<int>> res; vector<int> cur;
    function<void(int,int)> bt=[&](int i,int rem){
        if(rem==0){res.insert(cur);return;}
        for(int j=i;j<(int)c.size()&&c[j]<=rem;j++){cur.push_back(c[j]);bt(j+1,rem-c[j]);cur.pop_back();}
    };
    bt(0,t); return vector<vector<int>>(res.begin(),res.end());
}
```

### Optimal -- O(2^N) backtracking, skip duplicates at same level
```cpp
vector<vector<int>> combinationSum2(vector<int>& c, int t) {
    sort(c.begin(),c.end()); vector<vector<int>> res; vector<int> cur;
    function<void(int,int)> bt=[&](int i,int rem){
        if(rem==0){res.push_back(cur);return;}
        for(int j=i;j<(int)c.size()&&c[j]<=rem;j++){
            if(j>i&&c[j]==c[j-1]) continue; // skip duplicates
            cur.push_back(c[j]);bt(j+1,rem-c[j]);cur.pop_back();
        }
    };
    bt(0,t); return res;
}
```

---

## Problem 74: Permutations
**LeetCode #46** | Return all permutations of distinct integers.

### Brute Force -- O(N! * N) using next_permutation
```cpp
vector<vector<int>> permute(vector<int>& nums) {
    sort(nums.begin(),nums.end()); vector<vector<int>> res;
    do { res.push_back(nums); } while(next_permutation(nums.begin(),nums.end()));
    return res;
}
```

### Optimal -- O(N * N!) backtracking with visited array
```cpp
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res; vector<int> cur; vector<bool> used(nums.size(),false);
    function<void()> bt=[&](){
        if((int)cur.size()==(int)nums.size()){res.push_back(cur);return;}
        for(int i=0;i<(int)nums.size();i++){
            if(used[i]) continue;
            used[i]=true; cur.push_back(nums[i]); bt(); cur.pop_back(); used[i]=false;
        }
    };
    bt(); return res;
}
```

---

## Problem 75: Subsets II
**LeetCode #90** | Array may have duplicates; return all unique subsets.

### Brute Force -- generate all, deduplicate with set
```cpp
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(),nums.end()); set<vector<int>> res; vector<int> cur;
    function<void(int)> bt=[&](int i){
        res.insert(cur);
        for(int j=i;j<(int)nums.size();j++){cur.push_back(nums[j]);bt(j+1);cur.pop_back();}
    };
    bt(0); return vector<vector<int>>(res.begin(),res.end());
}
```

### Optimal -- sort + skip same element at same depth level
```cpp
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(),nums.end()); vector<vector<int>> res; vector<int> cur;
    function<void(int)> bt=[&](int i){
        res.push_back(cur);
        for(int j=i;j<(int)nums.size();j++){
            if(j>i&&nums[j]==nums[j-1]) continue;
            cur.push_back(nums[j]); bt(j+1); cur.pop_back();
        }
    };
    bt(0); return res;
}
```

---

## Problem 76: Word Search
**LeetCode #79** | Find if word exists in board using adjacent cells.

### Brute Force -- O(M*N*4^L) DFS from every cell
```cpp
bool exist(vector<vector<char>>& board, string word) {
    int m=board.size(),n=board[0].size();
    function<bool(int,int,int)> dfs=[&](int r,int c,int i)->bool{
        if(i==(int)word.size()) return true;
        if(r<0||r>=m||c<0||c>=n||board[r][c]!=word[i]) return false;
        char tmp=board[r][c]; board[r][c]='#';
        bool ok=dfs(r+1,c,i+1)||dfs(r-1,c,i+1)||dfs(r,c+1,i+1)||dfs(r,c-1,i+1);
        board[r][c]=tmp; return ok;
    };
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) if(dfs(r,c,0)) return true;
    return false;
}
```

### Optimal -- same DFS but with early termination; this is already optimal O(M*N*4^L)
```cpp
// Add pruning: check character frequency first
bool exist(vector<vector<char>>& board, string word) {
    int m=board.size(),n=board[0].size();
    int cnt[128]={}; for(auto& row:board) for(char c:row) cnt[(int)c]++;
    for(char c:word) if(--cnt[(int)c]<0) return false; // frequency pruning
    if(word[0]!=word.back()) { // reverse if last char rarer (fewer starts)
        int fc=0,lc=0;
        for(auto& row:board)for(char c:row){if(c==word[0])fc++;if(c==word.back())lc++;}
        if(lc<fc) reverse(word.begin(),word.end());
    }
    function<bool(int,int,int)> dfs=[&](int r,int c,int i)->bool{
        if(i==(int)word.size()) return true;
        if(r<0||r>=m||c<0||c>=n||board[r][c]!=word[i]) return false;
        char tmp=board[r][c]; board[r][c]='#';
        bool ok=dfs(r+1,c,i+1)||dfs(r-1,c,i+1)||dfs(r,c+1,i+1)||dfs(r,c-1,i+1);
        board[r][c]=tmp; return ok;
    };
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) if(dfs(r,c,0)) return true;
    return false;
}
```

---

## Problem 77: Palindrome Partitioning
**LeetCode #131** | Partition s so every substring is a palindrome; return all partitions.

### Brute Force -- O(N * 2^N) generate all partitions, check each piece
```cpp
bool isPalin(string& s,int l,int r){while(l<r)if(s[l++]!=s[r--])return false;return true;}
vector<vector<string>> partition(string s) {
    vector<vector<string>> res; vector<string> cur;
    function<void(int)> bt=[&](int i){
        if(i==(int)s.size()){res.push_back(cur);return;}
        for(int j=i;j<(int)s.size();j++){
            if(isPalin(s,i,j)){cur.push_back(s.substr(i,j-i+1));bt(j+1);cur.pop_back();}
        }
    };
    bt(0); return res;
}
```

### Optimal -- precompute palindrome DP, then backtrack O(N * 2^N)
```cpp
vector<vector<string>> partition(string s) {
    int n=s.size(); vector<vector<bool>> dp(n,vector<bool>(n,false));
    for(int i=n-1;i>=0;i--) for(int j=i;j<n;j++)
        dp[i][j]=(s[i]==s[j])&&(j-i<=2||dp[i+1][j-1]);
    vector<vector<string>> res; vector<string> cur;
    function<void(int)> bt=[&](int i){
        if(i==n){res.push_back(cur);return;}
        for(int j=i;j<n;j++) if(dp[i][j]){cur.push_back(s.substr(i,j-i+1));bt(j+1);cur.pop_back();}
    };
    bt(0); return res;
}
```

---

## Problem 78: Letter Combinations of a Phone Number
**LeetCode #17**

### Brute Force -- O(4^N * N) iterative product
```cpp
vector<string> letterCombinations(string digits) {
    if(digits.empty()) return {};
    string mp[]={"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    vector<string> res={""}; 
    for(char d:digits){
        vector<string> tmp;
        for(auto& s:res) for(char c:mp[d-'0']) tmp.push_back(s+c);
        res=tmp;
    }
    return res;
}
```

### Optimal -- O(4^N * N) backtracking (same complexity, less memory)
```cpp
vector<string> letterCombinations(string digits) {
    if(digits.empty()) return {};
    string mp[]={"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    vector<string> res; string cur;
    function<void(int)> bt=[&](int i){
        if(i==(int)digits.size()){res.push_back(cur);return;}
        for(char c:mp[digits[i]-'0']){cur+=c;bt(i+1);cur.pop_back();}
    };
    bt(0); return res;
}
```

---

## Problem 79: N-Queens
**LeetCode #51** | Place N queens on NxN board so no two attack each other.

### Brute Force -- O(N! * N) try all permutations of column placements, validate
```cpp
// Generate all permutations of 0..N-1 as column positions, check diagonals
```

### Optimal -- O(N!) backtracking with 3 sets for cols/diags/anti-diags
```cpp
vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> res;
    vector<string> board(n,string(n,'.'));
    set<int> cols,diag,anti;
    function<void(int)> bt=[&](int r){
        if(r==n){res.push_back(board);return;}
        for(int c=0;c<n;c++){
            if(cols.count(c)||diag.count(r-c)||anti.count(r+c)) continue;
            cols.insert(c);diag.insert(r-c);anti.insert(r+c);board[r][c]='Q';
            bt(r+1);
            cols.erase(c);diag.erase(r-c);anti.erase(r+c);board[r][c]='.';
        }
    };
    bt(0); return res;
}
```