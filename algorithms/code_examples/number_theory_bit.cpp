// number_theory_bit.cpp
// Number theory with bit tricks: modular exponentiation and the Sieve of Eratosthenes.

#include <iostream>
#include <vector>
using namespace std;

// Computes (a^e) mod m by exponentiation-by-squaring (binary exponentiation).
// Each bit of e either multiplies in a or not. Time O(log e), space O(1).
long long modPow(long long a, long long e, long long mod) {
    long long r = 1 % mod;                 // 1 % mod handles mod == 1 correctly
    while (e > 0) {
        if (e & 1) r = (r * a) % mod;      // current low bit set -> fold a into result
        a = (a * a) % mod;                 // square base for the next bit's weight
        e >>= 1;                           // advance to next bit of the exponent
    }
    return r;
}

// Returns all primes <= n using the Sieve of Eratosthenes.
// Marks composites by crossing off multiples. Time O(n log log n), space O(n).
vector<int> sieve(int n) {
    vector<bool> isPrime(n + 1, true);
    vector<int> primes;
    if (n >= 0) isPrime[0] = false;        // 0 and 1 are not prime
    if (n >= 1) isPrime[1] = false;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
            // Start at i*i; smaller multiples already crossed by smaller primes.
            if (1LL * i * i <= n) {        // 1LL guard avoids int overflow in i*i
                for (int j = i * i; j <= n; j += i) isPrime[j] = false;
            }
        }
    }
    return primes;
}

int main() {
    cout << modPow(2, 10, 1000000007) << endl;
    vector<int> primes = sieve(20);
    for (int p : primes) cout << p << " ";
    cout << endl;
    return 0;
}
