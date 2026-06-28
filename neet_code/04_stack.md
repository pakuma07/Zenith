# Stack -- NeetCode 150

---

## Problem 21: Valid Parentheses
**LeetCode #20** | Return true if brackets are correctly matched and nested.

### Brute Force -- O(N^2) time (repeatedly remove valid pairs)
```cpp
bool isValid(string s) {
    while(s.find("()") != string::npos || s.find("[]") != string::npos || s.find("{}") != string::npos) {
        size_t p;
        if((p=s.find("()"))!=string::npos) s.erase(p,2);
        if((p=s.find("[]"))!=string::npos) s.erase(p,2);
        if((p=s.find("{}"))!=string::npos) s.erase(p,2);
    }
    return s.empty();
}
```

### Optimal -- O(N) time, O(N) space
```cpp
bool isValid(string s) {
    stack<char> st;
    for(char c : s) {
        if(c=='(' || c=='[' || c=='{') { st.push(c); continue; }
        if(st.empty()) return false;
        if(c==')' && st.top()!='(') return false;
        if(c==']' && st.top()!='[') return false;
        if(c=='}' && st.top()!='{') return false;
        st.pop();
    }
    return st.empty();
}
```

---

## Problem 22: Min Stack
**LeetCode #155** | Stack that supports push, pop, top, and getMin in O(1).

### Brute Force -- O(N) getMin (scan all elements)
```cpp
class MinStack {
    stack<int> st;
public:
    void push(int val) { st.push(val); }
    void pop()         { st.pop(); }
    int  top()         { return st.top(); }
    int  getMin()      {
        stack<int> tmp = st;
        int m = tmp.top();
        while(!tmp.empty()) { m = min(m, tmp.top()); tmp.pop(); }
        return m;
    }
};
```

### Optimal -- O(1) all operations (parallel min stack)
```cpp
class MinStack {
    stack<int> st, minSt;
public:
    void push(int val) {
        st.push(val);
        minSt.push(minSt.empty() ? val : min(val, minSt.top()));
    }
    void pop()    { st.pop(); minSt.pop(); }
    int  top()    { return st.top(); }
    int  getMin() { return minSt.top(); }
};
```

---

## Problem 23: Evaluate Reverse Polish Notation
**LeetCode #150** | Evaluate an expression in Reverse Polish Notation.

### Brute Force (same as optimal, stack is the natural approach)
```cpp
// No meaningful brute force -- O(N) is already optimal
```

### Optimal -- O(N) time, O(N) space
```cpp
int evalRPN(vector<string>& tokens) {
    stack<long long> st;
    for(auto& t : tokens) {
        if(t=="+"||t=="-"||t=="*"||t=="/") {
            long long b = st.top(); st.pop();
            long long a = st.top(); st.pop();
            if(t=="+") st.push(a+b);
            else if(t=="-") st.push(a-b);
            else if(t=="*") st.push(a*b);
            else st.push(a/b);
        } else {
            st.push(stoll(t));
        }
    }
    return st.top();
}
```

---

## Problem 24: Generate Parentheses
**LeetCode #22** | Generate all combinations of n pairs of valid parentheses.

### Brute Force -- O(2^(2N) * N) -- generate all 2^2n strings, validate each
```cpp
bool valid(string s) {
    int cnt = 0;
    for(char c : s) { if(c=='(') cnt++; else cnt--; if(cnt<0) return false; }
    return cnt == 0;
}
vector<string> generateParenthesis(int n) {
    vector<string> all, res;
    function<void(string)> gen = [&](string cur) {
        if((int)cur.size() == 2*n) { all.push_back(cur); return; }
        gen(cur+'('); gen(cur+')');
    };
    gen("");
    for(auto& s : all) if(valid(s)) res.push_back(s);
    return res;
}
```

### Optimal -- O(4^N / sqrt(N)) time, backtracking with pruning
```cpp
vector<string> generateParenthesis(int n) {
    vector<string> res;
    function<void(string,int,int)> bt = [&](string cur, int open, int close) {
        if((int)cur.size() == 2*n) { res.push_back(cur); return; }
        if(open < n)    bt(cur+'(', open+1, close);
        if(close < open) bt(cur+')', open, close+1);
    };
    bt("", 0, 0);
    return res;
}
```

---

## Problem 25: Daily Temperatures
**LeetCode #739** | For each day, find how many days until a warmer temperature.

### Brute Force -- O(N^2) time, O(1) extra space
```cpp
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> res(n, 0);
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            if(t[j] > t[i]) { res[i] = j-i; break; }
    return res;
}
```

### Optimal -- O(N) time, O(N) space (monotonic decreasing stack)
```cpp
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> res(n, 0);
    stack<int> st; // indices with decreasing temperatures
    for(int i = 0; i < n; i++) {
        while(!st.empty() && t[i] > t[st.top()]) {
            res[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);
    }
    return res;
}
```

---

## Problem 26: Car Fleet
**LeetCode #853** | Find the number of car fleets arriving at the target.

### Brute Force -- O(N^2) time (simulate)
```cpp
int carFleet(int target, vector<int>& pos, vector<int>& speed) {
    int n = pos.size();
    vector<pair<int,int>> cars;
    for(int i = 0; i < n; i++) cars.push_back({pos[i], speed[i]});
    sort(cars.rbegin(), cars.rend()); // sort by position desc
    vector<double> times;
    for(auto& c : cars) times.push_back((double)(target - c.first) / c.second);
    int fleets = 0;
    double maxT = 0;
    for(double t : times) {
        if(t > maxT) { fleets++; maxT = t; }
    }
    return fleets;
}
```

### Optimal -- O(N log N) time, O(N) space (same logic, stack for clarity)
```cpp
int carFleet(int target, vector<int>& pos, vector<int>& speed) {
    int n = pos.size();
    vector<pair<int,int>> cars;
    for(int i = 0; i < n; i++) cars.push_back({pos[i], speed[i]});
    sort(cars.rbegin(), cars.rend());
    stack<double> st;
    for(auto& c : cars) {
        double t = (double)(target - c.first) / c.second;
        if(st.empty() || t > st.top()) st.push(t); // new fleet
        // else: catches up to car ahead, same fleet
    }
    return st.size();
}
```

---

## Problem 27: Largest Rectangle in Histogram
**LeetCode #84** | Find the largest rectangle in the histogram.

### Brute Force -- O(N^2) time
```cpp
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size(), best = 0;
    for(int i = 0; i < n; i++) {
        int minH = heights[i];
        for(int j = i; j < n; j++) {
            minH = min(minH, heights[j]);
            best = max(best, minH * (j-i+1));
        }
    }
    return best;
}
```

### Optimal -- O(N) time, O(N) space (monotonic increasing stack)
```cpp
int largestRectangleArea(vector<int>& heights) {
    stack<int> st; // indices of increasing heights
    int best = 0, n = heights.size();
    for(int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        while(!st.empty() && h < heights[st.top()]) {
            int height = heights[st.top()]; st.pop();
            int width  = st.empty() ? i : i - st.top() - 1;
            best = max(best, height * width);
        }
        st.push(i);
    }
    return best;
}
```
