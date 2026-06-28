# Advanced Graphs -- NeetCode 150

---

## Problem 93: Reconstruct Itinerary
**LeetCode #332** | Find Eulerian path using DFS + Hierholzer's algorithm. Start from "JFK".

### Brute Force -- O(E! * E) try all orderings via backtracking
```cpp
// Impractical for large inputs; Hierholzer's is the right approach
```

### Optimal -- O(E log E) Hierholzer's algorithm (post-order DFS)
```cpp
vector<string> findItinerary(vector<vector<string>>& tickets) {
    unordered_map<string,multiset<string>> adj;
    for(auto& t:tickets) adj[t[0]].insert(t[1]);
    vector<string> res;
    function<void(string)> dfs=[&](string u){
        while(!adj[u].empty()){string v=*adj[u].begin();adj[u].erase(adj[u].begin());dfs(v);}
        res.push_back(u);
    };
    dfs("JFK");
    reverse(res.begin(),res.end());
    return res;
}
```

---

## Problem 94: Min Cost to Connect All Points
**LeetCode #1584** | Manhattan distance MST.

### Brute Force -- O(N^2 log N) Kruskal's with all edges
```cpp
int minCostConnectPoints(vector<vector<int>>& pts) {
    int n=pts.size();
    vector<tuple<int,int,int>> edges;
    for(int i=0;i<n;i++) for(int j=i+1;j<n;j++)
        edges.push_back({abs(pts[i][0]-pts[j][0])+abs(pts[i][1]-pts[j][1]),i,j});
    sort(edges.begin(),edges.end());
    vector<int> par(n); iota(par.begin(),par.end(),0);
    function<int(int)> find=[&](int x)->int{return par[x]==x?x:par[x]=find(par[x]);};
    int cost=0,cnt=0;
    for(auto&[w,u,v]:edges){int a=find(u),b=find(v);if(a!=b){par[a]=b;cost+=w;if(++cnt==n-1)break;}}
    return cost;
}
```

### Optimal -- O(N^2) Prim's algorithm without building all edges
```cpp
int minCostConnectPoints(vector<vector<int>>& pts) {
    int n=pts.size(),cost=0,added=0;
    vector<int> minDist(n,INT_MAX); vector<bool> inMST(n,false); minDist[0]=0;
    for(int i=0;i<n;i++){
        int u=-1;
        for(int j=0;j<n;j++) if(!inMST[j]&&(u==-1||minDist[j]<minDist[u])) u=j;
        inMST[u]=true; cost+=minDist[u]; added++;
        for(int v=0;v<n;v++) if(!inMST[v]){
            int d=abs(pts[u][0]-pts[v][0])+abs(pts[u][1]-pts[v][1]);
            minDist[v]=min(minDist[v],d);
        }
    }
    return cost;
}
```

---

## Problem 95: Network Delay Time
**LeetCode #743** | Dijkstra SSSP; return max dist to all nodes, -1 if unreachable.

### Brute Force -- O(V^2 + E) Dijkstra with linear scan instead of heap
### Optimal -- O((V+E) log V) Dijkstra with min-heap
```cpp
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int,int>>> adj(n+1);
    for(auto& t:times) adj[t[0]].push_back({t[1],t[2]});
    vector<int> dist(n+1,INT_MAX); dist[k]=0;
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq; pq.push({0,k});
    while(!pq.empty()){
        auto[d,u]=pq.top();pq.pop();
        if(d>dist[u]) continue;
        for(auto&[v,w]:adj[u]) if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pq.push({dist[v],v});}
    }
    int mx=0;
    for(int i=1;i<=n;i++){if(dist[i]==INT_MAX)return -1;mx=max(mx,dist[i]);}
    return mx;
}
```

---

## Problem 96: Swim in Rising Water
**LeetCode #778** | Find path from (0,0) to (n-1,n-1) minimizing maximum elevation traversed.

### Brute Force -- O(N^2 log N) binary search on answer + BFS for each
```cpp
bool canReach(vector<vector<int>>& g,int t){
    int n=g.size(); if(g[0][0]>t||g[n-1][n-1]>t)return false;
    vector<vector<bool>> vis(n,vector<bool>(n,false)); queue<pair<int,int>> q; q.push({0,0}); vis[0][0]=true;
    int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
    while(!q.empty()){auto[r,c]=q.front();q.pop();if(r==n-1&&c==n-1)return true;
        for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d];if(nr>=0&&nr<n&&nc>=0&&nc<n&&!vis[nr][nc]&&g[nr][nc]<=t){vis[nr][nc]=true;q.push({nr,nc});}}}
    return false;
}
int swimInWater(vector<vector<int>>& grid){int n=grid.size(),lo=grid[0][0],hi=n*n-1;while(lo<hi){int mid=(lo+hi)/2;if(canReach(grid,mid))hi=mid;else lo=mid+1;}return lo;}
```

### Optimal -- O(N^2 log N) Dijkstra (treat elevation as edge weight)
```cpp
int swimInWater(vector<vector<int>>& grid) {
    int n=grid.size();
    priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<>> pq; pq.push({grid[0][0],0,0});
    vector<vector<int>> dist(n,vector<int>(n,INT_MAX)); dist[0][0]=grid[0][0];
    int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
    while(!pq.empty()){
        auto[t,r,c]=pq.top();pq.pop();
        if(r==n-1&&c==n-1) return t;
        for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d];
            if(nr>=0&&nr<n&&nc>=0&&nc<n){int nt=max(t,grid[nr][nc]);if(nt<dist[nr][nc]){dist[nr][nc]=nt;pq.push({nt,nr,nc});}}}
    }
    return -1;
}
```

---

## Problem 97: Alien Dictionary
**LeetCode #269** | Determine character order from sorted alien dictionary words.

### Brute Force -- O(C^2) compare all pairs of characters
### Optimal -- O(V+E) topological sort from derived character order
```cpp
string alienOrder(vector<string>& words) {
    unordered_map<char,unordered_set<char>> adj;
    unordered_map<char,int> indeg;
    for(auto& w:words) for(char c:w) indeg[c];
    for(int i=0;i<(int)words.size()-1;i++){
        auto& a=words[i]; auto& b=words[i+1];
        int mn=min(a.size(),b.size()); bool found=false;
        for(int j=0;j<(int)mn;j++) if(a[j]!=b[j]){if(!adj[a[j]].count(b[j])){adj[a[j]].insert(b[j]);indeg[b[j]]++;}found=true;break;}
        if(!found&&a.size()>b.size()) return ""; // invalid
    }
    queue<char> q; for(auto&[c,d]:indeg) if(!d) q.push(c);
    string res;
    while(!q.empty()){char u=q.front();q.pop();res+=u;for(char v:adj[u])if(--indeg[v]==0)q.push(v);}
    return (int)res.size()==(int)indeg.size()?res:"";
}
```

---

## Problem 98: Cheapest Flights Within K Stops
**LeetCode #787** | Bellman-Ford with at most K+1 relaxation rounds.

### Brute Force -- O(K * E^K) DFS exploring all paths up to K stops
### Optimal -- O(K * E) Bellman-Ford, K+1 rounds
```cpp
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<int> price(n,INT_MAX); price[src]=0;
    for(int i=0;i<=k;i++){
        vector<int> tmp=price;
        for(auto& f:flights){
            if(price[f[0]]==INT_MAX) continue;
            tmp[f[1]]=min(tmp[f[1]],price[f[0]]+f[2]);
        }
        price=tmp;
    }
    return price[dst]==INT_MAX?-1:price[dst];
}
```