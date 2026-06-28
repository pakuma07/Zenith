# Graphs -- NeetCode 150

---

## Problem 80: Number of Islands
**LeetCode #200** | Count islands (groups of '1') in a 2D grid.

### Brute Force -- O((M*N)^2) for each '1', do a BFS and mark visited separately each time
```cpp
// Naive: re-scan grid for each unvisited '1' without union-find
```

### Optimal -- O(M*N) DFS/BFS flood fill
```cpp
int numIslands(vector<vector<char>>& grid) {
    int m=grid.size(),n=grid[0].size(),cnt=0;
    function<void(int,int)> dfs=[&](int r,int c){
        if(r<0||r>=m||c<0||c>=n||grid[r][c]!='1') return;
        grid[r][c]='0';
        dfs(r+1,c);dfs(r-1,c);dfs(r,c+1);dfs(r,c-1);
    };
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) if(grid[r][c]=='1'){dfs(r,c);cnt++;}
    return cnt;
}
```

---

## Problem 81: Clone Graph
**LeetCode #133**

### Brute Force -- O(V+E) BFS, same as optimal
### Optimal -- O(V+E) DFS with hash map
```cpp
struct Node{int val;vector<Node*>neighbors;Node(int v=0):val(v){}};
Node* cloneGraph(Node* node) {
    if(!node) return nullptr;
    unordered_map<Node*,Node*> mp;
    function<Node*(Node*)> dfs=[&](Node* n)->Node*{
        if(mp.count(n)) return mp[n];
        mp[n]=new Node(n->val);
        for(Node* nb:n->neighbors) mp[n]->neighbors.push_back(dfs(nb));
        return mp[n];
    };
    return dfs(node);
}
```

---

## Problem 82: Max Area of Island
**LeetCode #695**

### Brute Force -- O((M*N)^2) scan all cells repeatedly
### Optimal -- O(M*N) DFS returning area
```cpp
int maxAreaOfIsland(vector<vector<int>>& grid) {
    int m=grid.size(),n=grid[0].size(),best=0;
    function<int(int,int)> dfs=[&](int r,int c)->int{
        if(r<0||r>=m||c<0||c>=n||grid[r][c]==0) return 0;
        grid[r][c]=0;
        return 1+dfs(r+1,c)+dfs(r-1,c)+dfs(r,c+1)+dfs(r,c-1);
    };
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) best=max(best,dfs(r,c));
    return best;
}
```

---

## Problem 83: Pacific Atlantic Water Flow
**LeetCode #417**

### Brute Force -- O(M^2*N^2) BFS/DFS from each cell to check both oceans
### Optimal -- O(M*N) reverse BFS from both oceans
```cpp
vector<vector<int>> pacificAtlantic(vector<vector<int>>& h) {
    int m=h.size(),n=h[0].size();
    vector<vector<bool>> pac(m,vector<bool>(n,false)),atl(m,vector<bool>(n,false));
    int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
    auto bfs=[&](queue<pair<int,int>>& q,vector<vector<bool>>& vis){
        while(!q.empty()){
            auto[r,c]=q.front();q.pop();
            for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d];
                if(nr>=0&&nr<m&&nc>=0&&nc<n&&!vis[nr][nc]&&h[nr][nc]>=h[r][c]){vis[nr][nc]=true;q.push({nr,nc});}}
        }
    };
    queue<pair<int,int>> pq,aq;
    for(int r=0;r<m;r++){pac[r][0]=true;pq.push({r,0});atl[r][n-1]=true;aq.push({r,n-1});}
    for(int c=0;c<n;c++){pac[0][c]=true;pq.push({0,c});atl[m-1][c]=true;aq.push({m-1,c});}
    bfs(pq,pac);bfs(aq,atl);
    vector<vector<int>> res;
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) if(pac[r][c]&&atl[r][c]) res.push_back({r,c});
    return res;
}
```

---

## Problem 84: Surrounded Regions
**LeetCode #130** | Capture all 'O' regions not connected to border.

### Brute Force -- O(M*N) BFS for each 'O' region, check if connected to border
### Optimal -- O(M*N) mark border-connected 'O's, then flip rest
```cpp
void solve(vector<vector<char>>& board) {
    int m=board.size(),n=board[0].size();
    function<void(int,int)> dfs=[&](int r,int c){
        if(r<0||r>=m||c<0||c>=n||board[r][c]!='O') return;
        board[r][c]='S'; dfs(r+1,c);dfs(r-1,c);dfs(r,c+1);dfs(r,c-1);
    };
    for(int r=0;r<m;r++){dfs(r,0);dfs(r,n-1);}
    for(int c=0;c<n;c++){dfs(0,c);dfs(m-1,c);}
    for(int r=0;r<m;r++) for(int c=0;c<n;c++){
        if(board[r][c]=='O') board[r][c]='X';
        else if(board[r][c]=='S') board[r][c]='O';
    }
}
```

---

## Problem 85: Rotting Oranges
**LeetCode #994** | Min minutes to rot all oranges; -1 if impossible.

### Brute Force -- O((M*N)^2) simulate minute by minute by scanning full grid
### Optimal -- O(M*N) multi-source BFS
```cpp
int orangesRotting(vector<vector<int>>& grid) {
    int m=grid.size(),n=grid[0].size(),fresh=0,time=0;
    queue<pair<int,int>> q;
    for(int r=0;r<m;r++) for(int c=0;c<n;c++){
        if(grid[r][c]==2) q.push({r,c});
        else if(grid[r][c]==1) fresh++;
    }
    int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
    while(!q.empty()&&fresh>0){
        time++;
        for(int i=q.size();i>0;i--){
            auto[r,c]=q.front();q.pop();
            for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d];
                if(nr>=0&&nr<m&&nc>=0&&nc<n&&grid[nr][nc]==1){grid[nr][nc]=2;fresh--;q.push({nr,nc});}}
        }
    }
    return fresh==0?time:-1;
}
```

---

## Problem 86: Walls and Gates
**LeetCode #286** | Fill each empty room with distance to nearest gate.

### Brute Force -- O(M^2*N^2) BFS from each empty room
### Optimal -- O(M*N) multi-source BFS from all gates simultaneously
```cpp
void wallsAndGates(vector<vector<int>>& rooms) {
    int m=rooms.size(),n=rooms[0].size(),INF=2147483647;
    queue<pair<int,int>> q;
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) if(rooms[r][c]==0) q.push({r,c});
    int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
    while(!q.empty()){
        auto[r,c]=q.front();q.pop();
        for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d];
            if(nr>=0&&nr<m&&nc>=0&&nc<n&&rooms[nr][nc]==INF){rooms[nr][nc]=rooms[r][c]+1;q.push({nr,nc});}}
    }
}
```

---

## Problem 87: Course Schedule
**LeetCode #207** | Detect cycle in directed graph (can finish all courses?).

### Brute Force -- O(V*(V+E)) DFS from each node checking for cycle
### Optimal -- O(V+E) DFS with 3-color marking (0=unvisited, 1=visiting, 2=done)
```cpp
bool canFinish(int n, vector<vector<int>>& prereqs) {
    vector<vector<int>> adj(n);
    for(auto& e:prereqs) adj[e[1]].push_back(e[0]);
    vector<int> state(n,0);
    function<bool(int)> dfs=[&](int u)->bool{
        if(state[u]==1) return false; // cycle
        if(state[u]==2) return true;
        state[u]=1;
        for(int v:adj[u]) if(!dfs(v)) return false;
        state[u]=2; return true;
    };
    for(int i=0;i<n;i++) if(!dfs(i)) return false;
    return true;
}
```

---

## Problem 88: Course Schedule II
**LeetCode #210** | Return topological order, or [] if cycle.

### Brute Force -- O(V^2+E) repeated removal of nodes with in-degree 0
### Optimal -- O(V+E) Kahn's BFS topological sort
```cpp
vector<int> findOrder(int n, vector<vector<int>>& prereqs) {
    vector<vector<int>> adj(n); vector<int> indeg(n,0);
    for(auto& e:prereqs){adj[e[1]].push_back(e[0]);indeg[e[0]]++;}
    queue<int> q; for(int i=0;i<n;i++) if(!indeg[i]) q.push(i);
    vector<int> order;
    while(!q.empty()){int u=q.front();q.pop();order.push_back(u);for(int v:adj[u])if(--indeg[v]==0)q.push(v);}
    return (int)order.size()==n?order:vector<int>();
}
```

---

## Problem 89: Redundant Connection
**LeetCode #684** | Find the edge that creates a cycle (return last such edge).

### Brute Force -- O(N^2) add edges one by one, BFS/DFS to check for cycle
### Optimal -- O(N * alpha(N)) Union-Find
```cpp
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n=edges.size(); vector<int> par(n+1),rank_(n+1,0);
    iota(par.begin(),par.end(),0);
    function<int(int)> find=[&](int x)->int{return par[x]==x?x:par[x]=find(par[x]);};
    for(auto& e:edges){
        int a=find(e[0]),b=find(e[1]);
        if(a==b) return e;
        if(rank_[a]<rank_[b]) swap(a,b); par[b]=a; if(rank_[a]==rank_[b]) rank_[a]++;
    }
    return {};
}
```

---

## Problem 90: Number of Connected Components
**LeetCode #323** | Count connected components in undirected graph.

### Brute Force -- O(V+E) DFS from each unvisited node
```cpp
int countComponents(int n, vector<vector<int>>& edges) {
    vector<vector<int>> adj(n); for(auto& e:edges){adj[e[0]].push_back(e[1]);adj[e[1]].push_back(e[0]);}
    vector<bool> vis(n,false); int cnt=0;
    function<void(int)> dfs=[&](int u){vis[u]=true;for(int v:adj[u])if(!vis[v])dfs(v);};
    for(int i=0;i<n;i++) if(!vis[i]){dfs(i);cnt++;}
    return cnt;
}
```

### Optimal -- O(N * alpha(N)) Union-Find
```cpp
int countComponents(int n, vector<vector<int>>& edges) {
    vector<int> par(n); iota(par.begin(),par.end(),0);
    function<int(int)> find=[&](int x)->int{return par[x]==x?x:par[x]=find(par[x]);};
    int cnt=n;
    for(auto& e:edges){int a=find(e[0]),b=find(e[1]);if(a!=b){par[a]=b;cnt--;}}
    return cnt;
}
```

---

## Problem 91: Graph Valid Tree
**LeetCode #261** | Check if undirected graph is a valid tree (connected + no cycles).

### Brute Force -- O(V+E) DFS cycle check + connectivity check
```cpp
bool validTree(int n, vector<vector<int>>& edges) {
    if((int)edges.size()!=n-1) return false; // necessary condition
    vector<vector<int>> adj(n); for(auto& e:edges){adj[e[0]].push_back(e[1]);adj[e[1]].push_back(e[0]);}
    vector<bool> vis(n,false); int cnt=0;
    function<void(int,int)> dfs=[&](int u,int p){vis[u]=true;cnt++;for(int v:adj[u])if(!vis[v])dfs(v,u);};
    dfs(0,-1); return cnt==n;
}
```

### Optimal -- O(N * alpha(N)) Union-Find
```cpp
bool validTree(int n, vector<vector<int>>& edges) {
    vector<int> par(n); iota(par.begin(),par.end(),0);
    function<int(int)> find=[&](int x)->int{return par[x]==x?x:par[x]=find(par[x]);};
    for(auto& e:edges){int a=find(e[0]),b=find(e[1]);if(a==b)return false;par[a]=b;n--;}
    return n==1;
}
```

---

## Problem 92: Word Ladder
**LeetCode #127** | Minimum transformations to change beginWord to endWord changing one letter at a time.

### Brute Force -- O(N^2 * L) BFS comparing every pair
```cpp
// Naive: for each word in queue, compare with every word in wordList
```

### Optimal -- O(N * L^2) BFS with character substitution
```cpp
int ladderLength(string begin, string end, vector<string>& wordList) {
    unordered_set<string> ws(wordList.begin(),wordList.end());
    if(!ws.count(end)) return 0;
    queue<string> q; q.push(begin); int steps=1;
    while(!q.empty()){
        for(int i=q.size();i>0;i--){
            string w=q.front();q.pop();
            if(w==end) return steps;
            for(int j=0;j<(int)w.size();j++){
                char orig=w[j];
                for(char c='a';c<='z';c++){
                    w[j]=c;
                    if(ws.count(w)){q.push(w);ws.erase(w);}
                }
                w[j]=orig;
            }
        }
        steps++;
    }
    return 0;
}
```