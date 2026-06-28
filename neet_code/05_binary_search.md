# Binary Search -- NeetCode 150

---

## Problem 28: Binary Search
**LeetCode #704** | Search for a target in a sorted array.

### Brute Force -- O(N) linear scan
```cpp
int search(vector<int>& nums, int target) {
    for(int i = 0; i < (int)nums.size(); i++)
        if(nums[i] == target) return i;
    return -1;
}
```

### Optimal -- O(log N) time, O(1) space
```cpp
int search(vector<int>& nums, int target) {
    int l = 0, r = nums.size()-1;
    while(l <= r) {
        int m = l + (r-l)/2;
        if(nums[m] == target) return m;
        else if(nums[m] < target) l = m+1;
        else r = m-1;
    }
    return -1;
}
```

---

## Problem 29: Search a 2D Matrix
**LeetCode #74** | Search in a matrix where rows are sorted and first element of each row > last of previous.

### Brute Force -- O(M*N) time
```cpp
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    for(auto& row : matrix)
        for(int x : row)
            if(x == target) return true;
    return false;
}
```

### Optimal -- O(log(M*N)) time (treat as one sorted array)
```cpp
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int m = matrix.size(), n = matrix[0].size();
    int l = 0, r = m*n-1;
    while(l <= r) {
        int mid = l + (r-l)/2;
        int val = matrix[mid/n][mid%n];
        if(val == target) return true;
        else if(val < target) l = mid+1;
        else r = mid-1;
    }
    return false;
}
```

---

## Problem 30: Koko Eating Bananas
**LeetCode #875** | Find minimum eating speed k to eat all piles within h hours.

### Brute Force -- O(max(piles) * N) time (try every speed from 1 upward)
```cpp
int minEatingSpeed(vector<int>& piles, int h) {
    for(int k = 1; ; k++) {
        long long hours = 0;
        for(int p : piles) hours += (p + k - 1) / k;
        if(hours <= h) return k;
    }
}
```

### Optimal -- O(N log(max(piles))) time, binary search on speed
```cpp
int minEatingSpeed(vector<int>& piles, int h) {
    int l = 1, r = *max_element(piles.begin(), piles.end());
    while(l < r) {
        int m = l + (r-l)/2;
        long long hours = 0;
        for(int p : piles) hours += (p + m - 1) / m;
        if(hours <= h) r = m;
        else l = m+1;
    }
    return l;
}
```

---

## Problem 31: Find Minimum in Rotated Sorted Array
**LeetCode #153** | Find the minimum in a rotated sorted array (no duplicates).

### Brute Force -- O(N) linear scan
```cpp
int findMin(vector<int>& nums) {
    return *min_element(nums.begin(), nums.end());
}
```

### Optimal -- O(log N) time, binary search
```cpp
int findMin(vector<int>& nums) {
    int l = 0, r = nums.size()-1;
    while(l < r) {
        int m = l + (r-l)/2;
        if(nums[m] > nums[r]) l = m+1; // min is in right half
        else r = m;                     // min is in left half (including m)
    }
    return nums[l];
}
```

---

## Problem 32: Search in Rotated Sorted Array
**LeetCode #33** | Search for target in rotated sorted array.

### Brute Force -- O(N) linear scan
```cpp
int search(vector<int>& nums, int target) {
    for(int i = 0; i < (int)nums.size(); i++)
        if(nums[i] == target) return i;
    return -1;
}
```

### Optimal -- O(log N) time, modified binary search
```cpp
int search(vector<int>& nums, int target) {
    int l = 0, r = nums.size()-1;
    while(l <= r) {
        int m = l + (r-l)/2;
        if(nums[m] == target) return m;
        // left half is sorted
        if(nums[l] <= nums[m]) {
            if(target >= nums[l] && target < nums[m]) r = m-1;
            else l = m+1;
        } else { // right half is sorted
            if(target > nums[m] && target <= nums[r]) l = m+1;
            else r = m-1;
        }
    }
    return -1;
}
```

---

## Problem 33: Time Based Key-Value Store
**LeetCode #981** | Store key-value pairs with timestamps; get value at or before a given timestamp.

### Brute Force -- O(N) get (linear scan backward)
```cpp
class TimeMap {
    unordered_map<string, vector<pair<int,string>>> mp;
public:
    void set(string key, string value, int timestamp) {
        mp[key].push_back({timestamp, value});
    }
    string get(string key, int timestamp) {
        if(!mp.count(key)) return "";
        for(int i = mp[key].size()-1; i >= 0; i--)
            if(mp[key][i].first <= timestamp) return mp[key][i].second;
        return "";
    }
};
```

### Optimal -- O(log N) get with binary search (timestamps added in order)
```cpp
class TimeMap {
    unordered_map<string, vector<pair<int,string>>> mp;
public:
    void set(string key, string value, int timestamp) {
        mp[key].push_back({timestamp, value});
    }
    string get(string key, int timestamp) {
        if(!mp.count(key)) return "";
        auto& v = mp[key];
        int l = 0, r = v.size()-1, res = -1;
        while(l <= r) {
            int m = l+(r-l)/2;
            if(v[m].first <= timestamp) { res = m; l = m+1; }
            else r = m-1;
        }
        return res == -1 ? "" : v[res].second;
    }
};
```

---

## Problem 34: Median of Two Sorted Arrays
**LeetCode #4** | Find median of two sorted arrays. O(log(min(M,N))) required.

### Brute Force -- O((M+N) log(M+N)) time (merge and find median)
```cpp
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    vector<int> merged;
    merged.insert(merged.end(), nums1.begin(), nums1.end());
    merged.insert(merged.end(), nums2.begin(), nums2.end());
    sort(merged.begin(), merged.end());
    int n = merged.size();
    if(n % 2 == 1) return merged[n/2];
    return (merged[n/2-1] + merged[n/2]) / 2.0;
}
```

### Optimal -- O(log(min(M,N))) time, binary search on partition
```cpp
double findMedianSortedArrays(vector<int>& A, vector<int>& B) {
    if(A.size() > B.size()) swap(A, B);
    int m = A.size(), n = B.size();
    int l = 0, r = m;
    while(l <= r) {
        int i = l + (r-l)/2;      // partition A
        int j = (m+n+1)/2 - i;    // partition B
        int AL = (i > 0) ? A[i-1] : INT_MIN;
        int AR = (i < m) ? A[i]   : INT_MAX;
        int BL = (j > 0) ? B[j-1] : INT_MIN;
        int BR = (j < n) ? B[j]   : INT_MAX;
        if(AL <= BR && BL <= AR) {
            if((m+n) % 2 == 1) return max(AL, BL);
            return (max(AL,BL) + min(AR,BR)) / 2.0;
        } else if(AL > BR) r = i-1;
        else l = i+1;
    }
    return 0;
}
```
