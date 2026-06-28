// dynamic_programming.cpp
// Classic DP examples: Fibonacci (1-D table) and 0/1 knapsack (2-D table).

#include <iostream>
#include <vector>
using namespace std;

// n-th Fibonacci number via bottom-up tabulation.
// Recurrence dp[i] = dp[i-1] + dp[i-2]. Time O(n), space O(n).
long long fib(int n) {
    if (n <= 1) return n;             // base cases F(0)=0, F(1)=1
    vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; ++i) dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

// Max value fitting capacity W choosing each item at most once (0/1 knapsack).
// 2-D DP over (items considered, capacity). Time O(n*W), space O(n*W).
int knapsack01(const vector<int>& wt, const vector<int>& val, int W) {
    int n = (int)wt.size();
    // dp[i][w] = best value using first i items within capacity w.
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = dp[i - 1][w];          // skip item i (default choice)
            if (wt[i - 1] <= w) {             // item fits -> consider taking it
                // Take item i: its value plus best for remaining capacity.
                dp[i][w] = max(dp[i][w], val[i - 1] + dp[i - 1][w - wt[i - 1]]);
            }
        }
    }
    return dp[n][W];
}

int main() {
    cout << "fib(10) = " << fib(10) << endl;
    vector<int> wt = {1, 3, 4, 5};
    vector<int> val = {1, 4, 5, 7};
    cout << "knapsack = " << knapsack01(wt, val, 7) << endl;
    return 0;
}
