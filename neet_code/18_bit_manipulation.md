# Bit Manipulation -- NeetCode 150

---

## Problem 144: Single Number
**LeetCode #136** | Every element appears twice except one. Find it.

### Brute Force -- O(N) hash map count
```cpp
int singleNumber_map(vector<int>& nums){unordered_map<int,int>c;for(int x:nums)c[x]++;for(auto&[k,v]:c)if(v==1)return k;return -1;}
```

### Optimal -- O(N) time, O(1) space: XOR all numbers
```cpp
int singleNumber(vector<int>& nums){int res=0;for(int x:nums)res^=x;return res;}
```

---

## Problem 145: Number of 1 Bits
**LeetCode #191** | Count set bits (popcount).

### Brute Force -- O(32) check each bit
```cpp
int hammingWeight_bf(uint32_t n){int cnt=0;while(n){cnt+=n&1;n>>=1;}return cnt;}
```

### Optimal -- O(k) where k = number of set bits, using n & (n-1) trick
```cpp
int hammingWeight(uint32_t n){int cnt=0;while(n){n&=n-1;cnt++;}return cnt;}
// or: return __builtin_popcount(n);
```

---

## Problem 146: Counting Bits
**LeetCode #338** | Return array where ans[i] = number of 1s in binary of i, for i in [0,n].

### Brute Force -- O(N log N) count bits for each number individually
```cpp
vector<int> countBits_bf(int n){vector<int>res(n+1);for(int i=0;i<=n;i++)res[i]=__builtin_popcount(i);return res;}
```

### Optimal -- O(N) DP: dp[i] = dp[i>>1] + (i&1)
```cpp
vector<int> countBits(int n){vector<int>dp(n+1,0);for(int i=1;i<=n;i++)dp[i]=dp[i>>1]+(i&1);return dp;}
```

---

## Problem 147: Reverse Bits
**LeetCode #190** | Reverse bits of 32-bit unsigned integer.

### Brute Force -- O(32) swap bits one by one using string
```cpp
uint32_t reverseBits_bf(uint32_t n){uint32_t res=0;for(int i=0;i<32;i++){res=(res<<1)|(n&1);n>>=1;}return res;}
```

### Optimal -- O(1) same approach but with explicit 32-bit reversal
```cpp
uint32_t reverseBits(uint32_t n) {
    uint32_t res=0;
    for(int i=0;i<32;i++){res=(res<<1)|(n&1);n>>=1;}
    return res;
}
// Or use __builtin_bswap32 + per-byte reversal, but above is clearest
```

---

## Problem 148: Missing Number
**LeetCode #268** | Find missing number in [0, n].

### Brute Force -- O(N) hash set
```cpp
int missingNumber_set(vector<int>& nums){unordered_set<int>s(nums.begin(),nums.end());for(int i=0;i<=(int)nums.size();i++)if(!s.count(i))return i;return -1;}
```

### Optimal -- O(N) XOR all indices and values
```cpp
int missingNumber(vector<int>& nums) {
    int res=nums.size();
    for(int i=0;i<(int)nums.size();i++) res^=i^nums[i];
    return res;
    // Or: return n*(n+1)/2 - accumulate(nums.begin(),nums.end(),0);
}
```

---

## Problem 149: Sum of Two Integers
**LeetCode #371** | Add two integers without using + or -.

### Brute Force -- O(N) increment one by one (trivially works but defeats purpose)
### Optimal -- O(1) bit addition: XOR for sum, AND+shift for carry
```cpp
int getSum(int a, int b) {
    while(b){
        unsigned carry=((unsigned)a&(unsigned)b)<<1;
        a=a^b; b=carry;
    }
    return a;
}
```

---

## Problem 150: Reverse Integer
**LeetCode #7** | Reverse digits of 32-bit signed integer; return 0 on overflow.

### Brute Force -- O(log N) convert to string, reverse, parse
```cpp
int reverse_str(int x){string s=to_string(abs(x));::reverse(s.begin(),s.end());long long r=stoll(s)*(x<0?-1:1);return (r>INT_MAX||r<INT_MIN)?0:(int)r;}
```

### Optimal -- O(log N) extract digits with modulo
```cpp
int reverse(int x) {
    long long res=0;
    while(x){res=res*10+(x%10);x/=10;}
    return (res>INT_MAX||res<INT_MIN)?0:(int)res;
}
```