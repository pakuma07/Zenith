# Intervals -- NeetCode 150

---

## Problem 130: Insert Interval
**LeetCode #57** | Insert a new interval into sorted non-overlapping intervals.

### Brute Force -- O(N log N) append + merge
```cpp
vector<vector<int>> insert_bf(vector<vector<int>>& intervals, vector<int>& newInterval) {
    intervals.push_back(newInterval);
    sort(intervals.begin(),intervals.end());
    vector<vector<int>> res; res.push_back(intervals[0]);
    for(int i=1;i<(int)intervals.size();i++){
        if(intervals[i][0]<=res.back()[1]) res.back()[1]=max(res.back()[1],intervals[i][1]);
        else res.push_back(intervals[i]);
    }
    return res;
}
```

### Optimal -- O(N) three-phase single pass
```cpp
vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> res; int i=0,n=intervals.size();
    while(i<n && intervals[i][1]<newInterval[0]) res.push_back(intervals[i++]);
    while(i<n && intervals[i][0]<=newInterval[1]){
        newInterval[0]=min(newInterval[0],intervals[i][0]);
        newInterval[1]=max(newInterval[1],intervals[i][1]); i++;
    }
    res.push_back(newInterval);
    while(i<n) res.push_back(intervals[i++]);
    return res;
}
```

---

## Problem 131: Merge Intervals
**LeetCode #56**

### Brute Force -- O(N^2) repeatedly merge any two overlapping intervals
### Optimal -- O(N log N) sort by start, merge
```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(),intervals.end());
    vector<vector<int>> res; res.push_back(intervals[0]);
    for(int i=1;i<(int)intervals.size();i++){
        if(intervals[i][0]<=res.back()[1]) res.back()[1]=max(res.back()[1],intervals[i][1]);
        else res.push_back(intervals[i]);
    }
    return res;
}
```

---

## Problem 132: Non-overlapping Intervals
**LeetCode #435** | Min intervals to remove to make rest non-overlapping.

### Brute Force -- O(2^N) try all subsets of intervals
### Optimal -- O(N log N) sort by end, greedy (similar to activity selection)
```cpp
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(),intervals.end(),[](auto&a,auto&b){return a[1]<b[1];});
    int cnt=0,end=INT_MIN;
    for(auto& iv:intervals){
        if(iv[0]>=end) end=iv[1];
        else cnt++;
    }
    return cnt;
}
```

---

## Problem 133: Meeting Rooms
**LeetCode #252** | Can a person attend all meetings? (no overlap check)

### Brute Force -- O(N^2) check all pairs for overlap
### Optimal -- O(N log N) sort by start, check consecutive
```cpp
bool canAttendMeetings(vector<vector<int>>& intervals) {
    sort(intervals.begin(),intervals.end());
    for(int i=1;i<(int)intervals.size();i++) if(intervals[i][0]<intervals[i-1][1]) return false;
    return true;
}
```

---

## Problem 134: Meeting Rooms II
**LeetCode #253** | Min number of conference rooms required.

### Brute Force -- O(N^2) simulate with room tracking
### Optimal -- O(N log N) min-heap of room end times
```cpp
int minMeetingRooms(vector<vector<int>>& intervals) {
    sort(intervals.begin(),intervals.end());
    priority_queue<int,vector<int>,greater<int>> pq; // min-heap of end times
    for(auto& iv:intervals){
        if(!pq.empty()&&pq.top()<=iv[0]) pq.pop();
        pq.push(iv[1]);
    }
    return pq.size();
}
```

---

## Problem 135: Minimum Interval to Include Each Query
**LeetCode #1851** | For each query, find the smallest interval containing it.

### Brute Force -- O(N * Q) check all intervals for each query
```cpp
vector<int> minInterval_bf(vector<vector<int>>& intervals, vector<int>& queries) {
    vector<int> res;
    for(int q:queries){
        int best=INT_MAX;
        for(auto& iv:intervals) if(iv[0]<=q&&q<=iv[1]) best=min(best,iv[1]-iv[0]+1);
        res.push_back(best==INT_MAX?-1:best);
    }
    return res;
}
```

### Optimal -- O((N+Q) log(N+Q)) sort intervals and queries, sweep with min-heap
```cpp
vector<int> minInterval(vector<vector<int>>& intervals, vector<int>& queries) {
    sort(intervals.begin(),intervals.end());
    int n=queries.size(); vector<int> idx(n); iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int a,int b){return queries[a]<queries[b];});
    // min-heap: {size, end}
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
    vector<int> res(n,-1); int i=0;
    for(int qi:idx){
        int q=queries[qi];
        while(i<(int)intervals.size()&&intervals[i][0]<=q){
            int sz=intervals[i][1]-intervals[i][0]+1;
            pq.push({sz,intervals[i][1]}); i++;
        }
        while(!pq.empty()&&pq.top().second<q) pq.pop();
        if(!pq.empty()) res[qi]=pq.top().first;
    }
    return res;
}
```