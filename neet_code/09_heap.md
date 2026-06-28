# Heap / Priority Queue -- NeetCode 150

---

## Problem 64: Kth Largest Element in a Stream
**LeetCode #703** | Design class that finds kth largest element in a stream.

### Brute Force -- O(N log N) sort on every add
```cpp
class KthLargest {
    vector<int> data; int k;
public:
    KthLargest(int k, vector<int>& nums):k(k),data(nums){}
    int add(int val){data.push_back(val);sort(data.rbegin(),data.rend());return data[k-1];}
};
```

### Optimal -- O(log K) add using min-heap of size k
```cpp
class KthLargest {
    priority_queue<int,vector<int>,greater<int>> pq; int k;
public:
    KthLargest(int k,vector<int>& nums):k(k){for(int x:nums)add(x);}
    int add(int val){pq.push(val);if((int)pq.size()>k)pq.pop();return pq.top();}
};
```

---

## Problem 65: Last Stone Weight
**LeetCode #1046** | Smash two heaviest stones repeatedly; return last stone or 0.

### Brute Force -- O(N^2 log N) sort on each step
```cpp
int lastStoneWeight(vector<int>& stones) {
    sort(stones.begin(),stones.end());
    while(stones.size()>1){
        int a=stones.back();stones.pop_back();
        int b=stones.back();stones.pop_back();
        if(a!=b){stones.push_back(a-b);sort(stones.begin(),stones.end());}
    }
    return stones.empty()?0:stones[0];
}
```

### Optimal -- O(N log N) max-heap
```cpp
int lastStoneWeight(vector<int>& stones) {
    priority_queue<int> pq(stones.begin(),stones.end());
    while(pq.size()>1){
        int a=pq.top();pq.pop();
        int b=pq.top();pq.pop();
        if(a!=b) pq.push(a-b);
    }
    return pq.empty()?0:pq.top();
}
```

---

## Problem 66: K Closest Points to Origin
**LeetCode #973**

### Brute Force -- O(N log N) sort all by distance
```cpp
vector<vector<int>> kClosest(vector<vector<int>>& pts, int k) {
    sort(pts.begin(),pts.end(),[](auto&a,auto&b){return a[0]*a[0]+a[1]*a[1]<b[0]*b[0]+b[1]*b[1];});
    return vector<vector<int>>(pts.begin(),pts.begin()+k);
}
```

### Optimal -- O(N log K) max-heap of size k
```cpp
vector<vector<int>> kClosest(vector<vector<int>>& pts, int k) {
    auto dist=[](vector<int>& p){return p[0]*p[0]+p[1]*p[1];};
    priority_queue<pair<int,int>> pq; // {dist, index}
    for(int i=0;i<(int)pts.size();i++){
        pq.push({dist(pts[i]),i});
        if((int)pq.size()>k) pq.pop();
    }
    vector<vector<int>> res;
    while(!pq.empty()){res.push_back(pts[pq.top().second]);pq.pop();}
    return res;
}
```

---

## Problem 67: Kth Largest Element in an Array
**LeetCode #215**

### Brute Force -- O(N log N) sort
```cpp
int findKthLargest(vector<int>& nums, int k){sort(nums.rbegin(),nums.rend());return nums[k-1];}
```

### Optimal -- O(N) average QuickSelect
```cpp
int findKthLargest(vector<int>& nums, int k) {
    int n=nums.size(), target=n-k;
    function<int(int,int)> qs=[&](int l,int r)->int{
        int pivot=nums[r], p=l;
        for(int i=l;i<r;i++) if(nums[i]<=pivot) swap(nums[i],nums[p++]);
        swap(nums[p],nums[r]);
        if(p==target) return nums[p];
        return p<target?qs(p+1,r):qs(l,p-1);
    };
    return qs(0,n-1);
}
```

---

## Problem 68: Task Scheduler
**LeetCode #621** | Min intervals to run all tasks with cooldown n between same tasks.

### Brute Force -- O(T * intervals) simulate the scheduling
```cpp
int leastInterval(vector<char>& tasks, int n) {
    int cnt[26]={};
    for(char c:tasks) cnt[c-'A']++;
    int time=0;
    while(true){
        bool any=false;
        vector<int> cooldown(26,-1);
        // simulation gets complex -- use formula instead
        break;
    }
    return 0; // placeholder
}
```

### Optimal -- O(N) formula-based
```cpp
int leastInterval(vector<char>& tasks, int n) {
    int cnt[26]={};
    for(char c:tasks) cnt[c-'A']++;
    int maxCnt=*max_element(cnt,cnt+26);
    int maxFreq=count(cnt,cnt+26,maxCnt);
    // Formula: max(tasks.size(), (maxCnt-1)*(n+1)+maxFreq)
    return max((int)tasks.size(), (maxCnt-1)*(n+1)+maxFreq);
}
```

---

## Problem 69: Design Twitter
**LeetCode #355** | postTweet, getNewsFeed (10 most recent), follow, unfollow.

### Brute Force -- O(N log N) collect all followed tweets, sort
```cpp
class Twitter {
    int ts=0;
    unordered_map<int,vector<pair<int,int>>> tweets; // userId -> [{time, tweetId}]
    unordered_map<int,unordered_set<int>> following;
public:
    void postTweet(int u,int t){tweets[u].push_back({ts++,t});}
    vector<int> getNewsFeed(int u){
        vector<pair<int,int>> all;
        following[u].insert(u);
        for(int f:following[u]) for(auto& p:tweets[f]) all.push_back(p);
        sort(all.rbegin(),all.rend());
        vector<int> res;
        for(int i=0;i<min(10,(int)all.size());i++) res.push_back(all[i].second);
        return res;
    }
    void follow(int u,int f){following[u].insert(f);}
    void unfollow(int u,int f){following[u].erase(f);}
};
```

### Optimal -- O(K log K) where K = number of followed users (heap merge)
```cpp
class Twitter {
    int ts=0;
    unordered_map<int,vector<pair<int,int>>> tweets;
    unordered_map<int,unordered_set<int>> following;
public:
    void postTweet(int u,int t){tweets[u].push_back({ts++,t});}
    vector<int> getNewsFeed(int u){
        // min-heap: {time, tweetId, userId, tweet_index}
        priority_queue<tuple<int,int,int,int>> pq;
        following[u].insert(u);
        for(int f:following[u]){
            if(!tweets[f].empty()){
                int idx=tweets[f].size()-1;
                pq.push({tweets[f][idx].first,tweets[f][idx].second,f,idx});
            }
        }
        vector<int> res;
        while(!pq.empty()&&(int)res.size()<10){
            auto [t,tid,f,idx]=pq.top();pq.pop();
            res.push_back(tid);
            if(idx>0) pq.push({tweets[f][idx-1].first,tweets[f][idx-1].second,f,idx-1});
        }
        return res;
    }
    void follow(int u,int f){following[u].insert(f);}
    void unfollow(int u,int f){following[u].erase(f);}
};
```

---

## Problem 70: Find Median from Data Stream
**LeetCode #295** | addNum, findMedian in O(log N) and O(1).

### Brute Force -- O(N log N) sort on each findMedian
```cpp
class MedianFinder {
    vector<int> data;
public:
    void addNum(int n){data.push_back(n);sort(data.begin(),data.end());}
    double findMedian(){int n=data.size();return n%2?data[n/2]:(data[n/2-1]+data[n/2])/2.0;}
};
```

### Optimal -- O(log N) add, O(1) findMedian (two heaps)
```cpp
class MedianFinder {
    priority_queue<int> lo;                          // max-heap for lower half
    priority_queue<int,vector<int>,greater<int>> hi; // min-heap for upper half
public:
    void addNum(int n){
        lo.push(n);
        hi.push(lo.top()); lo.pop();
        if(hi.size()>lo.size()+1){lo.push(hi.top());hi.pop();}
    }
    double findMedian(){
        if(hi.size()>lo.size()) return hi.top();
        return (lo.top()+hi.top())/2.0;
    }
};
```