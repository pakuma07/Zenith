# Sliding Window -- NeetCode 150

---

## Problem 15: Best Time to Buy and Sell Stock
**LeetCode #121** | Find max profit from one buy/sell (buy before sell).

### Brute Force -- O(N^2) time, O(1) space
```cpp
int maxProfit(vector<int>& prices) {
    int n = prices.size(), best = 0;
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            best = max(best, prices[j]-prices[i]);
    return best;
}
```

### Optimal -- O(N) time, O(1) space
```cpp
int maxProfit(vector<int>& prices) {
    int minPrice = INT_MAX, best = 0;
    for(int p : prices) {
        best = max(best, p - minPrice);
        minPrice = min(minPrice, p);
    }
    return best;
}
```

---

## Problem 16: Longest Substring Without Repeating Characters
**LeetCode #3** | Find length of the longest substring with all unique characters.

### Brute Force -- O(N^2) or O(N^3) time
```cpp
int lengthOfLongestSubstring(string s) {
    int n = s.size(), best = 0;
    for(int i = 0; i < n; i++) {
        set<char> seen;
        for(int j = i; j < n; j++) {
            if(seen.count(s[j])) break;
            seen.insert(s[j]);
            best = max(best, j-i+1);
        }
    }
    return best;
}
```

### Optimal -- O(N) time, O(1) space (128 ASCII chars)
```cpp
int lengthOfLongestSubstring(string s) {
    int last[128] = {};  // last seen index+1 of each char (0 = not seen)
    fill(last, last+128, -1);
    int l = 0, best = 0;
    for(int r = 0; r < (int)s.size(); r++) {
        if(last[s[r]] >= l) l = last[s[r]] + 1;
        last[s[r]] = r;
        best = max(best, r-l+1);
    }
    return best;
}
```

---

## Problem 17: Longest Repeating Character Replacement
**LeetCode #424** | Replace at most k characters; find longest substring with all same character.

### Brute Force -- O(N^2) time
```cpp
int characterReplacement(string s, int k) {
    int n = s.size(), best = 0;
    for(int i = 0; i < n; i++) {
        int cnt[26] = {}, maxCnt = 0;
        for(int j = i; j < n; j++) {
            cnt[s[j]-'A']++;
            maxCnt = max(maxCnt, cnt[s[j]-'A']);
            if((j-i+1) - maxCnt <= k) best = max(best, j-i+1);
            else break;
        }
    }
    return best;
}
```

### Optimal -- O(N) time, O(1) space (sliding window)
```cpp
int characterReplacement(string s, int k) {
    int cnt[26] = {}, l = 0, maxCnt = 0, best = 0;
    for(int r = 0; r < (int)s.size(); r++) {
        cnt[s[r]-'A']++;
        maxCnt = max(maxCnt, cnt[s[r]-'A']);
        // window size - max freq > k: shrink left
        if((r-l+1) - maxCnt > k) {
            cnt[s[l]-'A']--;
            l++;
        }
        best = max(best, r-l+1);
    }
    return best;
}
```

---

## Problem 18: Permutation in String
**LeetCode #567** | Return true if s2 contains a permutation of s1.

### Brute Force -- O(N1! * N2) -- generate all permutations of s1, search in s2
```cpp
bool checkInclusion(string s1, string s2) {
    sort(s1.begin(), s1.end());
    do {
        if(s2.find(s1) != string::npos) return true;
    } while(next_permutation(s1.begin(), s1.end()));
    return false;
}
```

### Optimal -- O(N) time, O(1) space (fixed-size window)
```cpp
bool checkInclusion(string s1, string s2) {
    if(s1.size() > s2.size()) return false;
    int need[26]={}, have[26]={};
    for(char c : s1) need[c-'a']++;
    int k = s1.size(), matches = 0;
    // matches = number of chars where have[c] == need[c]
    for(int r = 0; r < (int)s2.size(); r++) {
        int c = s2[r]-'a';
        have[c]++;
        if(have[c] == need[c]) matches++;
        if(r >= k) {
            int lc = s2[r-k]-'a';
            if(have[lc] == need[lc]) matches--;
            have[lc]--;
        }
        if(matches == 26) return true;
    }
    return false;
}
```

---

## Problem 19: Minimum Window Substring
**LeetCode #76** | Find minimum window in s that contains all chars of t.

### Brute Force -- O(N^2) time
```cpp
string minWindow(string s, string t) {
    int n = s.size(), best = INT_MAX, start = 0;
    for(int i = 0; i < n; i++) {
        unordered_map<char,int> need;
        for(char c : t) need[c]++;
        int cnt = t.size();
        for(int j = i; j < n; j++) {
            if(need.count(s[j]) && need[s[j]]-- > 0) cnt--;
            if(cnt == 0 && j-i+1 < best) { best = j-i+1; start = i; break; }
        }
    }
    return best == INT_MAX ? "" : s.substr(start, best);
}
```

### Optimal -- O(N) time, O(1) space (sliding window with have/need counts)
```cpp
string minWindow(string s, string t) {
    if(t.empty()) return "";
    unordered_map<char,int> need, have;
    for(char c : t) need[c]++;
    int formed = 0, required = need.size();
    int l = 0, best = INT_MAX, bstart = 0;
    for(int r = 0; r < (int)s.size(); r++) {
        char c = s[r];
        have[c]++;
        if(need.count(c) && have[c] == need[c]) formed++;
        while(formed == required) {
            if(r-l+1 < best) { best = r-l+1; bstart = l; }
            have[s[l]]--;
            if(need.count(s[l]) && have[s[l]] < need[s[l]]) formed--;
            l++;
        }
    }
    return best == INT_MAX ? "" : s.substr(bstart, best);
}
```

---

## Problem 20: Sliding Window Maximum
**LeetCode #239** | Return max of every window of size k.

### Brute Force -- O(N*K) time, O(N) space
```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> res;
    for(int i = 0; i+k <= n; i++)
        res.push_back(*max_element(nums.begin()+i, nums.begin()+i+k));
    return res;
}
```

### Optimal -- O(N) time, O(K) space (monotonic deque)
```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;  // stores indices; front = index of max
    vector<int> res;
    for(int r = 0; r < (int)nums.size(); r++) {
        // remove indices out of window
        if(!dq.empty() && dq.front() < r-k+1) dq.pop_front();
        // maintain decreasing order: remove smaller elements from back
        while(!dq.empty() && nums[dq.back()] < nums[r]) dq.pop_back();
        dq.push_back(r);
        if(r >= k-1) res.push_back(nums[dq.front()]);
    }
    return res;
}
```
