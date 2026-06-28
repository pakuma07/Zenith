// ============================================================
// COMPETITIVE PROGRAMMING TEMPLATE -- C++11
// World-class: Lazy SegTree, Sparse Table, LCA, Dinic, NTT,
// Miller-Rabin, safe hash, coordinate compression, geometry
// Compile: g++ -std=c++11 -O2 -Wall solution.cpp -o solution
// ============================================================
#pragma GCC optimize("O2,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt")

#ifdef __GNUC__
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <utility>
#include <cmath>
#include <climits>
#include <cassert>
#include <numeric>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <sstream>
#endif
using namespace std;

// ============================================================
// TYPES
// ============================================================
typedef long long           ll;
typedef unsigned long long  ull;
typedef __int128            lll;
typedef pair<int,int>       pii;
typedef pair<ll,ll>         pll;
typedef vector<int>         vi;
typedef vector<ll>          vl;
typedef vector<pii>         vpii;
typedef vector<vi>          vvi;

// ============================================================
// CONSTANTS
// ============================================================
const int    INF  = 0x3f3f3f3f;
const ll     LINF = 0x3f3f3f3f3f3f3f3fLL;
const int    MOD  = 1e9 + 7;
const int    MOD2 = 998244353;
const double PI   = acos(-1.0);
const double EPS  = 1e-9;

// ============================================================
// MACROS
// ============================================================
#define FOR(i,a,b)   for(int i=(a);i<(b);++i)
#define REP(i,n)     for(int i=0;i<(n);++i)
#define RFOR(i,b,a)  for(int i=(b)-1;i>=(a);--i)
#define all(v)       (v).begin(),(v).end()
#define rall(v)      (v).rbegin(),(v).rend()
#define sz(v)        (int)(v).size()
#define pb           push_back
#define eb           emplace_back
#define mp           make_pair
#define fi           first
#define se           second
#define YES          cout<<"YES\n"
#define NO           cout<<"NO\n"

// ============================================================
// RANDOM + SAFE HASH (anti-hack for unordered_map)
// ============================================================
mt19937    rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

struct SafeHash {
    size_t operator()(ull x) const {
        x += 0x9e3779b97f4a7c15ULL;
        x  = (x^(x>>30))*0xbf58476d1ce4e5b9ULL;
        x  = (x^(x>>27))*0x94d049bb133111ebULL;
        return x^(x>>31);
    }
};
template<typename K,typename V> using HashMap = unordered_map<K,V,SafeHash>;
template<typename K>            using HashSet  = unordered_set<K,SafeHash>;

// ============================================================
// MATH
// ============================================================
ll gcd(ll a,ll b)    { return b?gcd(b,a%b):a; }
ll lcm(ll a,ll b)    { return a/gcd(a,b)*b; }
ll power(ll b,ll e,ll m) { ll r=1;b%=m;for(;e;e>>=1){if(e&1)r=r*b%m;b=b*b%m;}return r; }
ll modInv(ll a,ll m) { return power(a,m-2,m); }

ll extgcd(ll a,ll b,ll &x,ll &y) {
    if(!b){x=1;y=0;return a;}
    ll x1,y1; ll g=extgcd(b,a%b,x1,y1); x=y1; y=x1-(a/b)*y1; return g;
}

const int FACT_MAX = 500005;
ll fact_[FACT_MAX], inv_fact_[FACT_MAX];
void precompute(int n=FACT_MAX-1) {
    fact_[0]=1; for(int i=1;i<=n;++i) fact_[i]=fact_[i-1]*i%MOD;
    inv_fact_[n]=modInv(fact_[n],MOD);
    for(int i=n-1;i>=0;--i) inv_fact_[i]=inv_fact_[i+1]*(i+1)%MOD;
}
ll C(int n,int k) { return(k<0||k>n)?0:fact_[n]*inv_fact_[k]%MOD*inv_fact_[n-k]%MOD; }

// ============================================================
// DSU / UNION-FIND (path compression + union by size)
// ============================================================
struct DSU {
    vi p,sz; int comps;
    DSU(int n):p(n),sz(n,1),comps(n){ iota(all(p),0); }
    int  find(int x)          { return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b)   {
        a=find(a);b=find(b);if(a==b)return false;
        if(sz[a]<sz[b])swap(a,b); p[b]=a;sz[a]+=sz[b];--comps;return true;
    }
    bool same(int a,int b)    { return find(a)==find(b); }
    int  size(int a)          { return sz[find(a)]; }
};

// ============================================================
// SPARSE TABLE -- O(1) RMQ, 0-indexed, static
// ============================================================
struct SparseTable {
    int n; vvi tb; vi lg;
    void build(const vi &a) {
        n=a.size(); int LOG=1; while((1<<LOG)<=n)LOG++;
        tb.assign(LOG,vi(n)); lg.resize(n+1);
        lg[1]=0; for(int i=2;i<=n;++i) lg[i]=lg[i/2]+1;
        tb[0]=a;
        for(int j=1;j<LOG;++j)
            for(int i=0;i+(1<<j)<=n;++i)
                tb[j][i]=min(tb[j-1][i],tb[j-1][i+(1<<(j-1))]);
    }
    int query(int l,int r){ int k=lg[r-l+1]; return min(tb[k][l],tb[k][r-(1<<k)+1]); }
};

// ============================================================
// SEGMENT TREE -- point update, range sum, 1-indexed
// ============================================================
struct SegTree {
    int n; vl tree;
    SegTree(int n):n(n),tree(4*n,0){}
    void upd(int v,int l,int r,int i,ll val){
        if(l==r){tree[v]=val;return;} int m=(l+r)/2;
        if(i<=m)upd(2*v,l,m,i,val);else upd(2*v+1,m+1,r,i,val);
        tree[v]=tree[2*v]+tree[2*v+1];
    }
    ll qry(int v,int l,int r,int ql,int qr){
        if(qr<l||r<ql)return 0;
        if(ql<=l&&r<=qr)return tree[v]; int m=(l+r)/2;
        return qry(2*v,l,m,ql,qr)+qry(2*v+1,m+1,r,ql,qr);
    }
    void update(int i,ll val){ upd(1,1,n,i,val); }
    ll   query(int l,int r)  { return qry(1,1,n,l,r); }
};

// ============================================================
// LAZY SEGMENT TREE -- range add update, range sum, 1-indexed
// ============================================================
struct LazySegTree {
    int n; vl tree,lazy;
    LazySegTree(int n):n(n),tree(4*n,0),lazy(4*n,0){}
    void push(int v,int l,int r){
        if(!lazy[v])return; int m=(l+r)/2;
        for(int c:{2*v,2*v+1}){
            int len=(c==2*v)?(m-l+1):(r-m);
            tree[c]+=lazy[v]*len; lazy[c]+=lazy[v];
        }
        lazy[v]=0;
    }
    void upd(int v,int l,int r,int ql,int qr,ll val){
        if(qr<l||r<ql)return;
        if(ql<=l&&r<=qr){tree[v]+=val*(r-l+1);lazy[v]+=val;return;}
        push(v,l,r); int m=(l+r)/2;
        upd(2*v,l,m,ql,qr,val); upd(2*v+1,m+1,r,ql,qr,val);
        tree[v]=tree[2*v]+tree[2*v+1];
    }
    ll qry(int v,int l,int r,int ql,int qr){
        if(qr<l||r<ql)return 0;
        if(ql<=l&&r<=qr)return tree[v];
        push(v,l,r); int m=(l+r)/2;
        return qry(2*v,l,m,ql,qr)+qry(2*v+1,m+1,r,ql,qr);
    }
    void update(int l,int r,ll v){ upd(1,1,n,l,r,v); }
    ll   query(int l,int r)      { return qry(1,1,n,l,r); }
};

// ============================================================
// BINARY INDEXED TREE / FENWICK TREE (1-indexed)
// ============================================================
struct BIT {
    int n; vl bit;
    BIT(int n):n(n),bit(n+1,0){}
    void upd(int i,ll d){ for(;i<=n;i+=i&-i)bit[i]+=d; }
    ll   qry(int i)     { ll s=0;for(;i>0;i-=i&-i)s+=bit[i];return s; }
    ll   qry(int l,int r){ return qry(r)-qry(l-1); }
};

// ============================================================
// LCA -- BINARY LIFTING, 1-indexed trees
// ============================================================
const int LCA_LOG=18, LCA_MAX=200005;
int lca_up[LCA_MAX][LCA_LOG], lca_dep[LCA_MAX];

void lca_dfs(int u,int p,int d,const vvi &adj){
    lca_up[u][0]=p; lca_dep[u]=d;
    for(int j=1;j<LCA_LOG;++j) lca_up[u][j]=lca_up[lca_up[u][j-1]][j-1];
    for(int v:adj[u]) if(v!=p) lca_dfs(v,u,d+1,adj);
}
void buildLCA(int root,const vvi &adj){ lca_dfs(root,root,0,adj); }
int lca(int u,int v){
    if(lca_dep[u]<lca_dep[v])swap(u,v);
    int diff=lca_dep[u]-lca_dep[v];
    for(int j=0;j<LCA_LOG;++j)if((diff>>j)&1)u=lca_up[u][j];
    if(u==v)return u;
    for(int j=LCA_LOG-1;j>=0;--j)
        if(lca_up[u][j]!=lca_up[v][j]){u=lca_up[u][j];v=lca_up[v][j];}
    return lca_up[u][0];
}
int treeDist(int u,int v){ return lca_dep[u]+lca_dep[v]-2*lca_dep[lca(u,v)]; }

// ============================================================
// GRAPH: DIJKSTRA, BFS, 0-1 BFS
// ============================================================
vl dijkstra(int src,const vector<vpii> &adj,int N){
    vl dist(N+1,LINF);
    priority_queue<pll,vector<pll>,greater<pll>> pq;
    dist[src]=0;pq.push({0,src});
    while(!pq.empty()){
        auto top=pq.top();pq.pop();
        ll d=top.fi;int u=top.se;
        if(d>dist[u])continue;
        for(int i=0;i<sz(adj[u]);++i){
            int v=adj[u][i].fi;ll w=adj[u][i].se;
            if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pq.push({dist[v],v});}
        }
    }
    return dist;
}
vi bfs_graph(int src,const vvi &adj,int N){
    vi dist(N+1,-1); queue<int>q;
    dist[src]=0;q.push(src);
    while(!q.empty()){
        int u=q.front();q.pop();
        for(int v:adj[u])if(dist[v]==-1){dist[v]=dist[u]+1;q.push(v);}
    }
    return dist;
}
// 0-1 BFS: edge weights 0 or 1
vl bfs01(int src,const vector<vpii> &adj,int N){
    vl dist(N+1,LINF);deque<int>dq;
    dist[src]=0;dq.push_back(src);
    while(!dq.empty()){
        int u=dq.front();dq.pop_front();
        for(int i=0;i<sz(adj[u]);++i){
            int v=adj[u][i].fi;ll w=adj[u][i].se;
            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                if(w==0)dq.push_front(v);else dq.push_back(v);
            }
        }
    }
    return dist;
}

// ============================================================
// MAX FLOW -- DINIC'S ALGORITHM
// ============================================================
struct Dinic {
    struct Edge { int to,rev; ll cap; };
    vector<vector<Edge>> g; vi level,iter_;
    Dinic(int n):g(n),level(n),iter_(n){}
    void addEdge(int u,int v,ll c){
        g[u].push_back({v,(int)g[v].size(),c});
        g[v].push_back({u,(int)g[u].size()-1,0});
    }
    bool bfs(int s,int t){
        fill(all(level),-1);queue<int>q;level[s]=0;q.push(s);
        while(!q.empty()){
            int v=q.front();q.pop();
            for(int i=0;i<sz(g[v]);++i){
                Edge &e=g[v][i];
                if(e.cap>0&&level[e.to]<0){level[e.to]=level[v]+1;q.push(e.to);}
            }
        }
        return level[t]>=0;
    }
    ll dfs(int v,int t,ll f){
        if(v==t)return f;
        for(int &i=iter_[v];i<sz(g[v]);++i){
            Edge &e=g[v][i];
            if(e.cap>0&&level[v]<level[e.to]){
                ll d=dfs(e.to,t,min(f,e.cap));
                if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}
            }
        }
        return 0;
    }
    ll maxflow(int s,int t){
        ll flow=0;
        while(bfs(s,t)){fill(all(iter_),0);ll d;while((d=dfs(s,t,LINF))>0)flow+=d;}
        return flow;
    }
};

// ============================================================
// NTT -- polynomial multiplication mod 998244353
// ============================================================
namespace NTT {
    const ll MOD=998244353, G=3;
    ll pw(ll a,ll b,ll m=MOD){ll r=1;a%=m;for(;b;b>>=1){if(b&1)r=r*a%m;a=a*a%m;}return r;}
    void ntt(vl &a,bool inv){
        int n=a.size();
        for(int i=1,j=0;i<n;++i){int b=n>>1;for(;j&b;b>>=1)j^=b;j^=b;if(i<j)swap(a[i],a[j]);}
        for(int len=2;len<=n;len<<=1){
            ll w=inv?pw(G,MOD-1-(MOD-1)/len):pw(G,(MOD-1)/len);
            for(int i=0;i<n;i+=len){ll wn=1;
                for(int j=0;j<len/2;++j){
                    ll u=a[i+j],v=a[i+j+len/2]*wn%MOD;
                    a[i+j]=(u+v)%MOD;a[i+j+len/2]=(u-v+MOD)%MOD;wn=wn*w%MOD;
                }
            }
        }
        if(inv){ll ni=pw(n,MOD-2);for(ll &x:a)x=x*ni%MOD;}
    }
    vl multiply(vl a,vl b){
        int sz2=a.size()+b.size()-1,n=1;while(n<sz2)n<<=1;
        a.resize(n);b.resize(n);ntt(a,false);ntt(b,false);
        for(int i=0;i<n;++i)a[i]=a[i]*b[i]%MOD;ntt(a,true);a.resize(sz2);return a;
    }
}

// ============================================================
// STRING: KMP + Z-FUNCTION
// ============================================================
vi kmpFail(const string &p){
    int m=p.size();vi f(m,0);
    for(int i=1;i<m;++i){int j=f[i-1];while(j>0&&p[i]!=p[j])j=f[j-1];if(p[i]==p[j])j++;f[i]=j;}
    return f;
}
vi zFunc(const string &s){
    int n=s.size();vi z(n,0);z[0]=n;
    for(int i=1,l=0,r=0;i<n;++i){
        if(i<r)z[i]=min(r-i,z[i-l]);
        while(i+z[i]<n&&s[z[i]]==s[i+z[i]])z[i]++;
        if(i+z[i]>r){l=i;r=i+z[i];}
    }
    return z;
}

// ============================================================
// GEOMETRY -- integer coordinates
// ============================================================
struct P {
    ll x,y; P(ll x=0,ll y=0):x(x),y(y){}
    P operator-(const P &o)const{return{x-o.x,y-o.y};}
    P operator+(const P &o)const{return{x+o.x,y+o.y};}
    ll cross(const P &o)const{return x*o.y-y*o.x;}
    ll dot  (const P &o)const{return x*o.x+y*o.y;}
    ll norm2()          const{return x*x+y*y;}
    bool operator<(const P &o)const{return x<o.x||(x==o.x&&y<o.y);}
    bool operator==(const P &o)const{return x==o.x&&y==o.y;}
};
ll cross(P O,P A,P B){return (A-O).cross(B-O);}
vector<P> convexHull(vector<P> pts){
    sort(all(pts));pts.erase(unique(all(pts)),pts.end());
    int n=pts.size();if(n<3)return pts;
    vector<P>h;int lo;
    for(int i=0;i<n;++i){while(h.size()>=2&&cross(h[h.size()-2],h.back(),pts[i])<=0)h.pop_back();h.push_back(pts[i]);}
    lo=h.size();
    for(int i=n-2;i>=0;--i){while((int)h.size()>lo&&cross(h[h.size()-2],h.back(),pts[i])<=0)h.pop_back();h.push_back(pts[i]);}
    h.pop_back();return h;
}

// ============================================================
// UTILITIES
// ============================================================
// Coordinate compression (0-indexed ranks)
vi compress(vi &a){
    vi s=a;sort(all(s));s.erase(unique(all(s)),s.end());
    vi r(a.size());
    for(int i=0;i<sz(a);++i)r[i]=(int)(lower_bound(all(s),a[i])-s.begin());
    return r;
}

// Miller-Rabin (deterministic for n < 3.3e24)
ull mulmod(ull a,ull b,ull m){return(lll)a*b%m;}
ull powmod(ull a,ull b,ull m){ull r=1;a%=m;for(;b;b>>=1){if(b&1)r=mulmod(r,a,m);a=mulmod(a,a,m);}return r;}
bool isPrime(ull n){
    if(n<2)return false;
    for(ull a:{2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL}){
        if(n==a)return true;if(n%a==0)return false;
        ull d=n-1;int r=0;while(d%2==0){d/=2;r++;}
        ull x=powmod(a,d,n);if(x==1||x==n-1)continue;
        bool ok=false;for(int i=0;i<r-1&&!ok;++i){x=mulmod(x,x,n);if(x==n-1)ok=true;}
        if(!ok)return false;
    }
    return true;
}

// ============================================================
// DEBUG (compile with -DLOCAL to enable; remove before submit)
// ============================================================
#ifdef LOCAL
#define dbg(x)   cerr<<#x<<" = "<<(x)<<"\n"
#define dbgv(v)  cerr<<#v<<" =";for(auto& _x:(v))cerr<<" "<<_x;cerr<<"\n"
#define dbg2(x,y) cerr<<#x<<"="<<(x)<<" "<<#y<<"="<<(y)<<"\n"
#else
#define dbg(x)
#define dbgv(v)
#define dbg2(x,y)
#endif

// ============================================================
// SOLUTION
// ============================================================
void solve() {
    int n;
    cin >> n;

    // --- write your solution here ---

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T = 1;
    // cin >> T;    // uncomment for multiple test cases
    while (T--) solve();

    return 0;
}