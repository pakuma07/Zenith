# 04 — Number Theory

The backbone of math-heavy competitive programming problems.

---

## 4.1 GCD and LCM

```cpp
// Euclidean algorithm — O(log min(a,b))
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

// LCM — always divide first to prevent overflow
long long lcm(long long a, long long b) { return a / gcd(a, b) * b; }

// Extended Euclidean: finds x, y such that a*x + b*y = gcd(a,b)
int extgcd(int a, int b, int &x, int &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int x1, y1;
    int g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
```

---

## 4.2 Prime Testing

```cpp
// O(√N) primality test
bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}
```

---

## 4.3 Sieve of Eratosthenes — O(N log log N)

```cpp
const int MAXN = 1000001;
bool isComposite[MAXN];   // false = prime
vector<int> primes;

void sieve(int n) {
    fill(isComposite, isComposite + n + 1, false);
    isComposite[0] = isComposite[1] = true;
    for (int i = 2; i <= n; ++i) {
        if (!isComposite[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= n; j += i)
                isComposite[j] = true;
        }
    }
}
// After sieve(1e6): primes contains all primes up to 1,000,000
```

### Linear Sieve — O(N) (each composite crossed out exactly once)

```cpp
int lp[MAXN];   // lp[i] = smallest prime factor of i
vector<int> primes;

void linearSieve(int n) {
    fill(lp, lp + n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        if (lp[i] == 0) { lp[i] = i; primes.push_back(i); }
        for (int j = 0; j < (int)primes.size() && primes[j] <= lp[i] && (ll)i * primes[j] <= n; ++j)
            lp[i * primes[j]] = primes[j];
    }
}
// Use lp[i] to factor any i in O(log i)
vector<int> factorize(int n) {
    vector<int> f;
    while (n > 1) { f.push_back(lp[n]); n /= lp[n]; }
    return f;
}
```

---

## 4.4 Prime Factorisation

```cpp
// O(√N) factorisation
map<int,int> factorize(int n) {
    map<int,int> f;
    for (int p = 2; (long long)p * p <= n; ++p) {
        while (n % p == 0) { f[p]++; n /= p; }
    }
    if (n > 1) f[n]++;
    return f;
}

// Number of divisors from factorisation
// If n = p1^e1 * p2^e2 * ..., then #divisors = (e1+1)(e2+1)...
int numDivisors(int n) {
    int count = 1;
    for (int p = 2; (long long)p * p <= n; ++p) {
        int e = 0;
        while (n % p == 0) { e++; n /= p; }
        count *= (e + 1);
    }
    if (n > 1) count *= 2;
    return count;
}
```

---

## 4.5 Modular Arithmetic

**Golden rules**:
1. Always reduce after **every** operation, not just at the end.
2. Subtraction can go negative → add MOD before reducing.
3. Never take `%` of a negative number directly.

```cpp
const long long MOD = 1e9 + 7;

long long addmod(long long a, long long b) { return (a + b) % MOD; }
long long submod(long long a, long long b) { return (a - b % MOD + MOD) % MOD; }
long long mulmod(long long a, long long b) { return (a % MOD) * (b % MOD) % MOD; }
```

---

## 4.6 Fast Modular Exponentiation — O(log exp)

```cpp
// Computes base^exp % mod
long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Usage
long long x = power(2, 30, MOD);    // 2^30 mod 10^9+7
```

---

## 4.7 Modular Inverse

The modular inverse of `a` under modulo `m` is `x` such that `a * x ≡ 1 (mod m)`.

**Method 1 — Fermat's Little Theorem** (m must be prime):
```cpp
// inv(a) = a^(m-2) mod m   (when m is prime)
long long modInverse(long long a, long long m) {
    return power(a, m - 2, m);
}
```

**Method 2 — Extended Euclidean** (works for any coprime a, m):
```cpp
long long modInverseExt(long long a, long long m) {
    long long x, y;
    long long g = extgcd(a, m, x, y);
    if (g != 1) return -1;   // no inverse
    return (x % m + m) % m;
}
```

**Method 3 — Precompute inverses 1..N in O(N)**:
```cpp
vector<long long> inv(n + 1);
inv[1] = 1;
for (int i = 2; i <= n; ++i)
    inv[i] = (MOD - (MOD / i) * inv[MOD % i] % MOD) % MOD;
```

---

## 4.8 Combinations with Modular Arithmetic

```cpp
const int MAXN = 200005;
long long fact[MAXN], inv_fact[MAXN];

void precompute(int n) {
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i-1] * i % MOD;
    inv_fact[n] = power(fact[n], MOD - 2, MOD);
    for (int i = n-1; i >= 0; --i) inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
}

long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] % MOD * inv_fact[n-k] % MOD;
}
```

---

## 4.9 Euler's Totient Function

φ(n) = count of integers in [1, n] coprime to n.

```cpp
// Single value — O(√N)
int phi(int n) {
    int result = n;
    for (int p = 2; (long long)p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// Sieve for all values 1..N
vector<int> eulerSieve(int n) {
    vector<int> phi(n + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int i = 2; i <= n; ++i) {
        if (phi[i] == i) {   // i is prime
            for (int j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    return phi;
}
```

---

## 4.10 Chinese Remainder Theorem (CRT)

Given: x ≡ r1 (mod m1),  x ≡ r2 (mod m2)   [m1, m2 coprime]  
Find: x (mod m1·m2)

```cpp
// Returns {remainder, modulus} for the combined congruence
pair<long long,long long> crt(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    long long g = extgcd(m1, m2, x, y);
    if ((r2 - r1) % g != 0) return {-1, -1};  // no solution
    long long lcm = m1 / g * m2;
    long long sol = (r1 + m1 * ((r2 - r1) / g % (m2/g) * (x % (m2/g)) % (m2/g))) % lcm;
    return {(sol + lcm) % lcm, lcm};
}
```

---

## 4.11 Matrix Exponentiation — O(K³ log N)

Used for computing Nth term of linear recurrences in O(log N).

```cpp
typedef vector<vector<long long>> Mat;

Mat multiply(const Mat& A, const Mat& B, long long mod) {
    int n = A.size();
    Mat C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) if (A[i][k])
            for (int j = 0; j < n; ++j)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
    return C;
}

Mat matpow(Mat A, long long p, long long mod) {
    int n = A.size();
    Mat R(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) R[i][i] = 1;  // identity
    for (; p > 0; p >>= 1) {
        if (p & 1) R = multiply(R, A, mod);
        A = multiply(A, A, mod);
    }
    return R;
}

// Example: Fibonacci F(n) using matrix exponentiation
// | F(n+1) |   | 1 1 |^n   | F(1) |
// | F(n)   | = | 1 0 |   × | F(0) |
long long fib(long long n, long long mod) {
    if (n == 0) return 0;
    Mat M = {{1,1},{1,0}};
    Mat R = matpow(M, n-1, mod);
    return R[0][0];  // F(n)
}
```

---

## 4.12 Number Theory Summary

| Problem | Tool |
|---------|------|
| GCD/LCM | Euclidean algorithm |
| Is N prime? | O(√N) trial division |
| All primes ≤ N | Sieve of Eratosthenes |
| Smallest prime factor of all ≤ N | Linear sieve |
| Factorize N | O(√N) or linear sieve |
| a^b mod m | Fast power |
| Division under mod | Modular inverse (Fermat or extGCD) |
| C(n,k) mod p | Precomputed factorials + inverse factorials |
| Linear recurrence Nth term | Matrix exponentiation |
| System of congruences | Chinese Remainder Theorem |

---

**Next**: [05 — Graph Algorithms](05_graph_algorithms.md)
