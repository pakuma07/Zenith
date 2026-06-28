// advanced_algorithms.cpp
// Advanced graph algorithm: all-pairs shortest paths via Floyd-Warshall.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// All-pairs shortest paths in place on an adjacency matrix.
// DP over intermediate vertices (Floyd-Warshall). Time O(n^3), space O(1) extra.
void floydWarshall(vector<vector<int>>& dist) {
    int n = (int)dist.size();
    // k = highest-numbered intermediate vertex allowed on the path so far.
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Skip if either leg is unreachable (avoids INF+INF overflow).
                if (dist[i][k] < 1000000000 && dist[k][j] < 1000000000) {
                    // Relax: best of current path vs. routing i->...->k->...->j.
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main() {
    const int INF = 1000000000;
    vector<vector<int>> dist = {
        {0, 3, INF, 7},
        {8, 0, 2, INF},
        {5, INF, 0, 1},
        {2, INF, INF, 0}
    };
    floydWarshall(dist);
    for (const auto& row : dist) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }
    return 0;
}
