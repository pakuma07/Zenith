# Math & Geometry -- NeetCode 150

---

## Problem 136: Rotate Image
**LeetCode #48** | Rotate NxN matrix 90 degrees clockwise in-place.

### Brute Force -- O(N^2) copy to new matrix
```cpp
void rotate_bf(vector<vector<int>>& m){int n=m.size();vector<vector<int>>tmp(n,vector<int>(n));for(int r=0;r<n;r++)for(int c=0;c<n;c++)tmp[c][n-1-r]=m[r][c];m=tmp;}
```

### Optimal -- O(N^2) transpose then reverse each row
```cpp
void rotate(vector<vector<int>>& m) {
    int n=m.size();
    for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) swap(m[i][j],m[j][i]);
    for(int i=0;i<n;i++) reverse(m[i].begin(),m[i].end());
}
```

---

## Problem 137: Spiral Matrix
**LeetCode #54** | Return elements in spiral order.

### Brute Force -- O(M*N) direction array simulation with visited grid
### Optimal -- O(M*N) shrink boundaries
```cpp
vector<int> spiralOrder(vector<vector<int>>& m) {
    int top=0,bot=m.size()-1,left=0,right=m[0].size()-1;
    vector<int> res;
    while(top<=bot&&left<=right){
        for(int c=left;c<=right;c++) res.push_back(m[top][c]); top++;
        for(int r=top;r<=bot;r++) res.push_back(m[r][right]); right--;
        if(top<=bot){for(int c=right;c>=left;c--) res.push_back(m[bot][c]); bot--;}
        if(left<=right){for(int r=bot;r>=top;r--) res.push_back(m[r][left]); left++;}
    }
    return res;
}
```

---

## Problem 138: Set Matrix Zeroes
**LeetCode #73** | If element is 0, set its row and column to 0 in-place.

### Brute Force -- O(M*N*(M+N)) for each zero, zero out row+col
### Optimal -- O(M*N) time, O(1) space using first row/col as markers
```cpp
void setZeroes(vector<vector<int>>& m) {
    int rows=m.size(),cols=m[0].size();
    bool row0=false,col0=false;
    for(int c=0;c<cols;c++) if(m[0][c]==0) row0=true;
    for(int r=0;r<rows;r++) if(m[r][0]==0) col0=true;
    for(int r=1;r<rows;r++) for(int c=1;c<cols;c++) if(m[r][c]==0){m[r][0]=m[0][c]=0;}
    for(int r=1;r<rows;r++) for(int c=1;c<cols;c++) if(!m[r][0]||!m[0][c]) m[r][c]=0;
    if(row0) for(int c=0;c<cols;c++) m[0][c]=0;
    if(col0) for(int r=0;r<rows;r++) m[r][0]=0;
}
```

---

## Problem 139: Happy Number
**LeetCode #202** | Sum of squared digits eventually reaches 1 (happy) or cycles.

### Brute Force -- O(log N) with hash set to detect cycle
```cpp
bool isHappy_set(int n){unordered_set<int>seen;while(n!=1){int s=0;while(n){s+=(n%10)*(n%10);n/=10;}n=s;if(seen.count(n))return false;seen.insert(n);}return true;}
```

### Optimal -- O(log N) Floyd's cycle detection (fast/slow pointer)
```cpp
bool isHappy(int n) {
    auto sumSq=[](int x){int s=0;while(x){s+=(x%10)*(x%10);x/=10;}return s;};
    int slow=n,fast=sumSq(n);
    while(fast!=1&&slow!=fast){slow=sumSq(slow);fast=sumSq(sumSq(fast));}
    return fast==1;
}
```

---

## Problem 140: Plus One
**LeetCode #66** | Increment large integer represented as digit array.

### Brute Force -- convert to int, add 1, convert back (overflow risk)
### Optimal -- O(N) add carry from end
```cpp
vector<int> plusOne(vector<int>& d) {
    for(int i=d.size()-1;i>=0;i--){if(d[i]<9){d[i]++;return d;}d[i]=0;}
    d.insert(d.begin(),1); return d;
}
```

---

## Problem 141: Pow(x, n)
**LeetCode #50** | Implement pow(x, n) without TLE.

### Brute Force -- O(N) multiply x n times
```cpp
double myPow_bf(double x,long long n){if(n<0){x=1/x;n=-n;}double r=1;while(n--)r*=x;return r;}
```

### Optimal -- O(log N) fast exponentiation
```cpp
double myPow(double x, long long n) {
    if(n<0){x=1/x;n=-n;}
    double res=1;
    while(n){if(n&1)res*=x;x*=x;n>>=1;}
    return res;
}
```

---

## Problem 142: Multiply Strings
**LeetCode #43** | Multiply two non-negative integers as strings.

### Brute Force -- O(M*N) grade-school multiplication
```cpp
string multiply(string num1, string num2) {
    int m=num1.size(),n=num2.size(); vector<int> pos(m+n,0);
    for(int i=m-1;i>=0;i--) for(int j=n-1;j>=0;j--){
        int mul=(num1[i]-'0')*(num2[j]-'0')+pos[i+j+1];
        pos[i+j+1]=mul%10; pos[i+j]+=mul/10;
    }
    string res; for(int x:pos) if(!(res.empty()&&!x)) res+=('0'+x);
    return res.empty()?"0":res;
}
```

### Optimal -- same O(M*N), above is optimal
```cpp
// FFT-based O(N log N) exists but is overkill for LeetCode constraints
```

---

## Problem 143: Detect Squares
**LeetCode #2013** | Add points; count axis-aligned squares with 3 given points.

### Brute Force -- O(N^3) try all triples of stored points
### Optimal -- O(N) per query using point count map
```cpp
class DetectSquares {
    unordered_map<int,unordered_map<int,int>> cnt;
    vector<pair<int,int>> pts;
public:
    void add(vector<int> p){cnt[p[0]][p[1]]++;pts.push_back({p[0],p[1]});}
    int count(vector<int> p) {
        int res=0,px=p[0],py=p[1];
        for(auto&[x,y]:pts){
            if(abs(px-x)!=abs(py-y)||x==px||y==py) continue;
            res+=cnt[px][y]*cnt[x][py];
        }
        return res;
    }
};
```