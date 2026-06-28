// graph_algorithms.cpp
// Single-source shortest paths on a weighted graph via Dijkstra's algorithm.

#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

// Shortest distance from src to every vertex (non-negative weights).
// Greedy + min-heap; adj holds (neighbor, weight). Time O((V+E) log V), space O(V).
vector<int> dijkstra(int n, const vector<vector<pair<int,int>>>& adj, int src) {
    const int INF = 1e9;
    vector<int> dist(n, INF);
    // Min-heap of (distance, vertex); greater<> makes smallest distance pop first.
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    dist[src] = 0;
    pq.push(make_pair(0, src));

    while (!pq.empty()) {
        pair<int,int> cur = pq.top(); pq.pop();
        int d = cur.first, u = cur.second;
        if (d != dist[u]) continue;            // stale entry (already improved): skip
        for (const auto& edge : adj[u]) {
            int v = edge.first, w = edge.second;
            if (dist[u] + w < dist[v]) {        // relax edge u->v
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v)); // push improved distance (lazy deletion)
            }
        }
    }
    return dist;
}

int main() {
    int n = 4;
    vector<vector<pair<int,int>>> adj(n);
    adj[0].push_back(make_pair(1, 4));
    adj[0].push_back(make_pair(2, 1));
    adj[2].push_back(make_pair(1, 2));
    adj[1].push_back(make_pair(3, 1));
    adj[2].push_back(make_pair(3, 5));

    vector<int> dist = dijkstra(n, adj, 0);
    for (int d : dist) cout << d << " ";
    cout << endl;
    return 0;
}
