# Arrays & Hashing -- NeetCode 150

---

## Problem 1: Contains Duplicate
**LeetCode #217** | Given an integer array, return `true` if any value appears at least twice.

### Brute Force -- O(N^2) time, O(1) space
```cpp
bool containsDuplicate(vector<int>& nums) {
    int n = nums.size();
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            if(nums[i] == nums[j]) return true;
    return false;
}
```

### Optimal -- O(N) time, O(N) space
```cpp
bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen;
    for(int x : nums) {
        if(seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
}
```

---

## Problem 2: Valid Anagram
**LeetCode #242** | Return `true` if `t` is an anagram of `s`.

### Brute Force -- O(N log N) time, O(N) space
```cpp
bool isAnagram(string s, string t) {
    if(s.size() != t.size()) return false;
    sort(s.begin(), s.end());
    sort(t.begin(), t.end());
    return s == t;
}
```

### Optimal -- O(N) time, O(1) space (26-letter alphabet)
```cpp
bool isAnagram(string s, string t) {
    if(s.size() != t.size()) return false;
    int cnt[26] = {};
    for(int i = 0; i < (int)s.size(); i++) {
        cnt[s[i]-'a']++;
        cnt[t[i]-'a']--;
    }
    for(int c : cnt) if(c != 0) return false;
    return true;
}
```

---

## Problem 3: Two Sum
**LeetCode #1** | Return indices of two numbers that add up to target.

### Brute Force -- O(N^2) time, O(1) space
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    int n = nums.size();
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            if(nums[i]+nums[j] == target) return {i, j};
    return {};
}
```

### Optimal -- O(N) time, O(N) space
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int,int> seen; // value -> index
    for(int i = 0; i < (int)nums.size(); i++) {
        int need = target - nums[i];
        if(seen.count(need)) return {seen[need], i};
        seen[nums[i]] = i;
    }
    return {};
}
```

---

## Problem 4: Group Anagrams
**LeetCode #49** | Group strings that are anagrams of each other.

### Brute Force -- O(N^2 * K log K) time
```cpp
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    int n = strs.size();
    vector<bool> used(n, false);
    vector<vector<string>> res;
    for(int i = 0; i < n; i++) {
        if(used[i]) continue;
        vector<string> group = {strs[i]};
        string si = strs[i]; sort(si.begin(), si.end());
        for(int j = i+1; j < n; j++) {
            string sj = strs[j]; sort(sj.begin(), sj.end());
            if(si == sj) { group.push_back(strs[j]); used[j] = true; }
        }
        res.push_back(group);
    }
    return res;
}
```

### Optimal -- O(N * K log K) time, O(N*K) space
```cpp
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> mp;
    for(string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        mp[key].push_back(s);
    }
    vector<vector<string>> res;
    for(auto& p : mp) res.push_back(p.second);
    return res;
}
```

---

## Problem 5: Top K Frequent Elements
**LeetCode #347** | Return the k most frequent elements.

### Brute Force -- O(N log N) time, O(N) space
```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> cnt;
    for(int x : nums) cnt[x]++;
    vector<pair<int,int>> freq(cnt.begin(), cnt.end());
    sort(freq.begin(), freq.end(), [](auto& a, auto& b){ return a.second > b.second; });
    vector<int> res;
    for(int i = 0; i < k; i++) res.push_back(freq[i].first);
    return res;
}
```

### Optimal -- O(N) time (bucket sort), O(N) space
```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> cnt;
    for(int x : nums) cnt[x]++;
    // bucket[i] = list of numbers with frequency i
    vector<vector<int>> bucket(nums.size()+1);
    for(auto& p : cnt) bucket[p.second].push_back(p.first);
    vector<int> res;
    for(int i = bucket.size()-1; i >= 0 && (int)res.size() < k; i--)
        for(int x : bucket[i]) { res.push_back(x); if((int)res.size()==k) break; }
    return res;
}
```

---

## Problem 6: Encode and Decode Strings
**LeetCode #271** | Encode a list of strings into a single string; decode back.

### Brute Force (delimiter-based, fragile with special chars)
```cpp
string encode(vector<string>& strs) {
    string res;
    for(auto& s : strs) res += s + "\x01"; // hope no 0x01 in input
    return res;
}
vector<string> decode(string s) {
    vector<string> res;
    string cur;
    for(char c : s) {
        if(c == '\x01') { res.push_back(cur); cur = ""; }
        else cur += c;
    }
    return res;
}
```

### Optimal -- length-prefix encoding, O(N) time
```cpp
// Format: "4#hell5#world" -- length, #, content
string encode(vector<string>& strs) {
    string res;
    for(auto& s : strs) res += to_string(s.size()) + "#" + s;
    return res;
}
vector<string> decode(string s) {
    vector<string> res;
    int i = 0;
    while(i < (int)s.size()) {
        int j = i;
        while(s[j] != '#') j++;
        int len = stoi(s.substr(i, j-i));
        res.push_back(s.substr(j+1, len));
        i = j + 1 + len;
    }
    return res;
}
```

---

## Problem 7: Product of Array Except Self
**LeetCode #238** | Return array where output[i] = product of all elements except nums[i]. No division.

### Brute Force -- O(N^2) time, O(1) extra space
```cpp
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(i != j) res[i] *= nums[j];
    return res;
}
```

### Optimal -- O(N) time, O(1) extra space (output array not counted)
```cpp
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    // left pass: res[i] = product of nums[0..i-1]
    int prefix = 1;
    for(int i = 0; i < n; i++) { res[i] = prefix; prefix *= nums[i]; }
    // right pass: multiply by product of nums[i+1..n-1]
    int suffix = 1;
    for(int i = n-1; i >= 0; i--) { res[i] *= suffix; suffix *= nums[i]; }
    return res;
}
```

---

## Problem 8: Valid Sudoku
**LeetCode #36** | Determine if a 9x9 board is valid (no duplicates in rows, cols, boxes).

### Brute Force -- O(1) time (fixed 9x9), clear but verbose
```cpp
bool isValidSudoku(vector<vector<char>>& board) {
    // Check rows
    for(int r = 0; r < 9; r++) {
        set<char> s;
        for(int c = 0; c < 9; c++) {
            if(board[r][c] == '.') continue;
            if(s.count(board[r][c])) return false;
            s.insert(board[r][c]);
        }
    }
    // Check columns
    for(int c = 0; c < 9; c++) {
        set<char> s;
        for(int r = 0; r < 9; r++) {
            if(board[r][c] == '.') continue;
            if(s.count(board[r][c])) return false;
            s.insert(board[r][c]);
        }
    }
    // Check 3x3 boxes
    for(int br = 0; br < 3; br++) for(int bc = 0; bc < 3; bc++) {
        set<char> s;
        for(int r = 0; r < 3; r++) for(int c = 0; c < 3; c++) {
            char ch = board[br*3+r][bc*3+c];
            if(ch == '.') continue;
            if(s.count(ch)) return false;
            s.insert(ch);
        }
    }
    return true;
}
```

### Optimal -- O(1) time, single pass with bit masks
```cpp
bool isValidSudoku(vector<vector<char>>& board) {
    int rows[9]={}, cols[9]={}, boxes[9]={};
    for(int r = 0; r < 9; r++) for(int c = 0; c < 9; c++) {
        if(board[r][c] == '.') continue;
        int bit = 1 << (board[r][c]-'1');
        int box = (r/3)*3 + c/3;
        if((rows[r]|cols[c]|boxes[box]) & bit) return false;
        rows[r] |= bit; cols[c] |= bit; boxes[box] |= bit;
    }
    return true;
}
```

---

## Problem 9: Longest Consecutive Sequence
**LeetCode #128** | Find the length of the longest consecutive elements sequence. O(N) required.

### Brute Force -- O(N^2) time
```cpp
int longestConsecutive(vector<int>& nums) {
    int best = 0;
    unordered_set<int> s(nums.begin(), nums.end());
    for(int x : nums) {
        int len = 1;
        while(s.count(x + len)) len++;
        best = max(best, len);
    }
    return best; // still O(N^2) worst case without start-check
}
```

### Optimal -- O(N) time, O(N) space (only start sequences)
```cpp
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int best = 0;
    for(int x : nums) {
        if(s.count(x-1)) continue; // x is not the start of a sequence
        int len = 1;
        while(s.count(x+len)) len++;
        best = max(best, len);
    }
    return best;
}
```
