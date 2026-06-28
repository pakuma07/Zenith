# Two Pointers -- NeetCode 150

---

## Problem 10: Valid Palindrome
**LeetCode #125** | A phrase is a palindrome if it reads the same after lowercasing and removing non-alphanumeric chars.

### Brute Force -- O(N) time, O(N) space
```cpp
bool isPalindrome(string s) {
    string clean;
    for(char c : s) if(isalnum(c)) clean += tolower(c);
    string rev = clean;
    reverse(rev.begin(), rev.end());
    return clean == rev;
}
```

### Optimal -- O(N) time, O(1) space
```cpp
bool isPalindrome(string s) {
    int l = 0, r = s.size()-1;
    while(l < r) {
        while(l < r && !isalnum(s[l])) l++;
        while(l < r && !isalnum(s[r])) r--;
        if(tolower(s[l]) != tolower(s[r])) return false;
        l++; r--;
    }
    return true;
}
```

---

## Problem 11: Two Sum II (Input Array is Sorted)
**LeetCode #167** | Find two numbers that sum to target in a sorted array. Return 1-indexed positions.

### Brute Force -- O(N^2) time, O(1) space
```cpp
vector<int> twoSum(vector<int>& numbers, int target) {
    int n = numbers.size();
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            if(numbers[i]+numbers[j] == target) return {i+1, j+1};
    return {};
}
```

### Optimal -- O(N) time, O(1) space (two pointers on sorted array)
```cpp
vector<int> twoSum(vector<int>& numbers, int target) {
    int l = 0, r = numbers.size()-1;
    while(l < r) {
        int s = numbers[l] + numbers[r];
        if(s == target) return {l+1, r+1};
        else if(s < target) l++;
        else r--;
    }
    return {};
}
```

---

## Problem 12: 3Sum
**LeetCode #15** | Find all unique triplets that sum to zero.

### Brute Force -- O(N^3) time, O(N) space
```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    int n = nums.size();
    set<vector<int>> res;
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            for(int k = j+1; k < n; k++)
                if(nums[i]+nums[j]+nums[k] == 0) {
                    vector<int> t = {nums[i],nums[j],nums[k]};
                    sort(t.begin(),t.end());
                    res.insert(t);
                }
    return vector<vector<int>>(res.begin(), res.end());
}
```

### Optimal -- O(N^2) time, O(1) extra space (sort + two pointers)
```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();
    for(int i = 0; i < n-2; i++) {
        if(i > 0 && nums[i] == nums[i-1]) continue; // skip duplicates
        int l = i+1, r = n-1;
        while(l < r) {
            int s = nums[i]+nums[l]+nums[r];
            if(s == 0) {
                res.push_back({nums[i],nums[l],nums[r]});
                while(l < r && nums[l]==nums[l+1]) l++;
                while(l < r && nums[r]==nums[r-1]) r--;
                l++; r--;
            } else if(s < 0) l++;
            else r--;
        }
    }
    return res;
}
```

---

## Problem 13: Container With Most Water
**LeetCode #11** | Find two lines that contain the most water.

### Brute Force -- O(N^2) time, O(1) space
```cpp
int maxArea(vector<int>& height) {
    int n = height.size(), best = 0;
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            best = max(best, min(height[i],height[j]) * (j-i));
    return best;
}
```

### Optimal -- O(N) time, O(1) space (greedy two pointers: always move the shorter side)
```cpp
int maxArea(vector<int>& height) {
    int l = 0, r = height.size()-1, best = 0;
    while(l < r) {
        best = max(best, min(height[l],height[r]) * (r-l));
        if(height[l] < height[r]) l++;
        else r--;
    }
    return best;
}
```

---

## Problem 14: Trapping Rain Water
**LeetCode #42** | Compute how much water can be trapped after raining.

### Brute Force -- O(N^2) time, O(1) space
```cpp
int trap(vector<int>& height) {
    int n = height.size(), water = 0;
    for(int i = 1; i < n-1; i++) {
        int lmax = *max_element(height.begin(), height.begin()+i+1);
        int rmax = *max_element(height.begin()+i, height.end());
        water += min(lmax, rmax) - height[i];
    }
    return water;
}
```

### Optimal -- O(N) time, O(1) space (two pointers)
```cpp
int trap(vector<int>& height) {
    int l = 0, r = height.size()-1;
    int lmax = 0, rmax = 0, water = 0;
    while(l < r) {
        if(height[l] <= height[r]) {
            lmax = max(lmax, height[l]);
            water += lmax - height[l];
            l++;
        } else {
            rmax = max(rmax, height[r]);
            water += rmax - height[r];
            r--;
        }
    }
    return water;
}
```
