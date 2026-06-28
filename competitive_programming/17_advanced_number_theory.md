# 17 — Advanced Number Theory

These topics appear in Codeforces Div 1 D/E and require mathematical depth.

---

## 17.1 Miller-Rabin Primality Test — O(k log² N)

Deterministic for N < 3.3 × 10^24 using specific witnesses.

```cpp
typedef unsigned long long ull;
typedef __int128 lll;

ull mulmod(ull a, ull b, ull m) { return (lll)a * b % m; }

ull powmod(ull a, ull b, ull m) {
    ull res = 1; a %= m;
    for (; b > 0; b >>= 1) { if (b & 1) res = mulmod(res, a, m); a = mulmod(a, a, m); }
    return res;
}

bool millerTest(ull n, ull a) {
    if (n % a == 0) return n == a;
    ull d = n - 1; int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ull x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; ++i) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool isPrime(ull n) {
    if (n < 2) return false;
    // Deterministic witnesses for n < 3,317,044,064,679,887,385,961,981
    for (ull a : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL})
        if (!millerTest(n, a)) return false;
    return true;
}
```

---

## 17.2 Pollard's Rho — O(N^(1/4)) Factorisation

```cpp
ull pollardRho(ull n) {
    if (n % 2 == 0) return 2;
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true) {
        ull x = rng() % (n - 2) + 2;
        ull y = x;
        ull c = rng() % (n - 1) + 1;
        ull d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            d = __gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

map<ull,int> factorize(ull n) {
    if (n == 1) return {};
    if (isPrime(n)) return {{n, 1}};
    ull d = n;
    while (d == n) d = pollardRho(n);
    auto f1 = factorize(d), f2 = factorize(n / d);
    for (auto& kv : f2) f1[kv.first] += kv.second;
    return f1;
}

// Usage: auto factors = factorize(N);  — works up to N ~ 10^18
```

---

## 17.3 Number Theoretic Transform (NTT) — O(N log N) Polynomial Multiplication

**Use for**: polynomial multiplication modulo a prime, convolutions.

```cpp
const int NTT_MOD = 998244353;  // = 119 × 2^23 + 1, primitive root g = 3
const int NTT_G   = 3;

long long power(long long a, long long b, long long mod) {
    long long res = 1; a %= mod;
    for (; b > 0; b >>= 1) { if (b&1) res=res*a%mod; a=a*a%mod; }
    return res;
}

void ntt(vector<long long>& a, bool inv) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit; if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long w = inv ? power(NTT_G, NTT_MOD-1-(NTT_MOD-1)/len, NTT_MOD)
                          : power(NTT_G, (NTT_MOD-1)/len, NTT_MOD);
        for (int i = 0; i < n; i += len) {
            long long wn = 1;
            for (int j = 0; j < len/2; ++j) {
                long long u = a[i+j], v = a[i+j+len/2] * wn % NTT_MOD;
                a[i+j] = (u+v) % NTT_MOD; a[i+j+len/2] = (u-v+NTT_MOD) % NTT_MOD;
                wn = wn * w % NTT_MOD;
            }
        }
    }
    if (inv) {
        long long n_inv = power(n, NTT_MOD-2, NTT_MOD);
        for (long long& x : a) x = x * n_inv % NTT_MOD;
    }
}

vector<long long> multiply(vector<long long> a, vector<long long> b) {
    int result_size = a.size() + b.size() - 1;
    int n = 1; while (n < result_size) n <<= 1;
    a.resize(n); b.resize(n);
    ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true);
    a.resize(result_size);
    return a;
}
```

---

## 17.4 Linear Recurrence — Berlekamp-Massey + Kitamasa

**Use for**: given the first few terms of a linear recurrence, find the recurrence relation, then compute the Nth term in O(K² log N) or O(K log K log N) with NTT.

```cpp
// Berlekamp-Massey: find the shortest linear recurrence for a sequence
vector<long long> berlekampMassey(vector<long long> s, long long mod) {
    int n = s.size();
    vector<long long> cur, last;
    int lf = 0, ld = 0;
    for (int i = 0; i < n; ++i) {
        long long t = 0;
        for (int j = 0; j < (int)cur.size(); ++j)
            t = (t + cur[j] * s[i-1-j]) % mod;
        if ((s[i] - t) % mod == 0) continue;
        if (cur.empty()) { cur.resize(i+1); lf = i; ld = (s[i]-t)%mod; continue; }
        long long k = -(s[i]-t) % mod * power(ld, mod-2, mod) % mod;
        vector<long long> c(i-lf-1); c.push_back(-k);
        for (long long x : last) c.push_back(x*k%mod);
        if (c.size() < cur.size()) c.resize(cur.size());
        for (int j = 0; j < (int)cur.size(); ++j) c[j] = (c[j]+cur[j]) % mod;
        if (i-lf+(int)last.size() >= (int)cur.size()) { last=cur; lf=i; ld=(s[i]-t)%mod; }
        cur = c;
    }
    for (long long& x : cur) x = (x % mod + mod) % mod;
    return cur;
}

// Compute Nth term of linear recurrence f[] with initial values s[]
// Using matrix exponentiation O(K² log N) or Kitamasa's O(K log K log N)
long long linearRecurrence(vector<long long>& f, vector<long long>& s, long long n, long long mod) {
    int k = f.size();
    if (n < k) return s[n];
    // Build companion matrix, then use matrix exponentiation (see section 4.11)
    // For simplicity: iterative O(K²) per step is fine for small K
    vector<long long> cur(s.begin(), s.begin()+k);
    for (long long i = k; i <= n; ++i) {
        long long next = 0;
        for (int j = 0; j < k; ++j) next = (next + f[j]*cur[k-1-j]) % mod;
        cur.erase(cur.begin()); cur.push_back(next);
    }
    return cur[k-1];
}
```

---

## 17.5 Primitive Root and Discrete Logarithm

```cpp
// Find primitive root modulo p (p is prime)
long long primitiveRoot(long long p) {
    auto phi_factors = factorize(p - 1);
    for (long long g = 2; ; ++g) {
        bool ok = true;
        for (auto& kv : phi_factors)
            if (power(g, (p-1)/kv.first, p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}

// Baby-step Giant-step discrete log: find x such that g^x ≡ a (mod p)
// O(√p) time and space
long long babyGiantStep(long long g, long long a, long long p) {
    long long m = (long long)ceil(sqrt((double)p));
    unordered_map<long long,long long> table;
    long long gm = power(g, m, p);
    long long val = a;
    for (long long j = 0; j <= m; ++j) { table[val] = j; val = val * g % p; }
    val = 1;
    for (long long i = 1; i <= m; ++i) {
        val = val * gm % p;
        if (table.count(val)) {
            long long ans = i*m - table[val];
            return (ans % (p-1) + (p-1)) % (p-1);
        }
    }
    return -1;  // no solution
}
```

---

## 17.6 Integer Partitions and Generating Functions

```cpp
// Number of ways to partition n using parts from set S (generating function approach)
// dp[i] = number of ways to form sum i
vector<long long> countPartitions(int n, const vector<int>& parts, long long mod) {
    vector<long long> dp(n+1, 0);
    dp[0] = 1;
    for (int p : parts)
        for (int i = p; i <= n; ++i)
            dp[i] = (dp[i] + dp[i-p]) % mod;
    return dp;
}

// Euler's partition function p(n) — number of ways to partition n
// p(0)=1, p(1)=1, p(2)=2, p(3)=3, p(4)=5, p(5)=7, ...
// Computed via Euler's pentagonal theorem in O(N√N)
```

---

## 17.7 Möbius Function and Inclusion-Exclusion over Divisors

```cpp
// Compute Möbius function mu[1..N]
// mu[n] = 0 if n has squared prime factor
//       = (-1)^k if n is product of k distinct primes
vector<int> mobiusSieve(int N) {
    vector<int> mu(N+1, 0), primes;
    vector<bool> notPrime(N+1, false);
    mu[1] = 1;
    for (int i = 2; i <= N; ++i) {
        if (!notPrime[i]) { primes.push_back(i); mu[i] = -1; }
        for (int j = 0; j < (int)primes.size() && (long long)i*primes[j] <= N; ++j) {
            notPrime[i*primes[j]] = true;
            if (i % primes[j] == 0) { mu[i*primes[j]] = 0; break; }
            mu[i*primes[j]] = -mu[i];
        }
    }
    return mu;
}

// Count pairs (a,b) with 1<=a,b<=N and gcd(a,b)=1 using Möbius
// Answer = sum_{d=1}^{N} mu[d] * floor(N/d)^2
long long coprimePairs(int N, const vector<int>& mu) {
    long long ans = 0;
    for (int d = 1; d <= N; ++d)
        if (mu[d]) ans += (long long)mu[d] * (N/d) * (N/d);
    return ans;
}
```

---

## 17.8 Summary

| Problem | Algorithm | Complexity |
|---------|-----------|------------|
| Is N prime? (N up to 10^18) | Miller-Rabin | O(12 log²N) |
| Factorise N (N up to 10^18) | Pollard's Rho + Miller-Rabin | O(N^(1/4)) |
| Polynomial multiplication mod p | NTT | O(N log N) |
| Nth term of linear recurrence | BM + matrix exp | O(K² log N) |
| Discrete log g^x = a (mod p) | Baby-step Giant-step | O(√p) |
| Count coprime pairs | Möbius sieve + summation | O(N log log N) |
| Find primitive root mod p | Trial with Euler's criterion | O(p^(1/4) log p) |

---

**Back to**: [README](README.md)
