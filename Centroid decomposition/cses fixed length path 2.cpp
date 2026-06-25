#include <bits/stdc++.h>
using namespace std;
 
using ll = long long;
 
#define pb push_back
 
typedef vector<ll> vll;
 
vector<ll> centroidParent;
vector<bool> removed;
vector<vector<int>> adj;
vector<int> sub;
vector<long long> depth;
 
// cnt[d] = number of nodes already processed
// at distance d from the current centroid
ll ans=0;
ll k1,k2;
 
struct Fenwick {
    vector<int> bit;
    int n;
 
    Fenwick(int n = 0) {
        init(n);
    }
 
    void init(int N) {
        n = N;
        bit.assign(n + 1, 0);
    }
 
    void add(int idx, int val) {
        idx++;
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }
 
    int sum(int idx) {
        idx++;
 
        int res = 0;
 
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
 
        return res;
    }
     void set(int i)
    {
        for (++i; i <= n; i += i & -i)
             bit[i] = 0;
    }
 
    int range_sum(int l, int r) {
        if (r < l || r<0) return 0;
 
        l = max(l, 0);
 
        return sum(r) - (l ? sum(l - 1) : 0);
    }
};
 
Fenwick bit(200005);
 
/*------------------------------------------------------------*
 | Compute subtree sizes ignoring removed centroid nodes
 *------------------------------------------------------------*/
void dfs_size(ll u, ll p) {
    sub[u] = 1;
 
    for (ll v : adj[u]) {
        if (v == p || removed[v])
            continue;
 
        dfs_size(v, u);
        sub[u] += sub[v];
    }
}
 
/*------------------------------------------------------------*
 | Find centroid of current component
 *------------------------------------------------------------*/
ll find_centroid(ll u, ll p, ll totalSize) {
    for (ll v : adj[u]) {
        if (v == p || removed[v])
            continue;
 
        if (sub[v] > totalSize / 2)
            return find_centroid(v, u, totalSize);
    }
 
    return u;
}
 
/*------------------------------------------------------------*
 | Collect distances from centroid to nodes in one subtree
 *------------------------------------------------------------*/
 
int get_dist(int u, int p, int d) {
    depth[d]++;
    int ret = d;
 
    for (int v : adj[u]) {
        if (v == p || removed[v] || d + 1 > k2)
            continue;
 
        ret = max(ret, get_dist(v, u, d + 1));
    }
 
    return ret;
}
 
 
void build(ll entry, ll par) {   
    dfs_size(entry, -1);
 
    ll centroid = find_centroid(entry, -1, sub[entry]);
 
    centroidParent[centroid] = par;
    removed[centroid] = true;
 
    // Distances whose frequencies were modified.
    // We'll reset only these positions afterwards.
 
    bit.add(0,1);              // centroid itself
   
    for (ll v : adj[centroid]) {
        if (removed[v])
            continue;
 
        
        ll maxd = get_dist(v, centroid, 1);
 
        for(ll i=maxd;i>=1;i--){
              if(depth[i]==0) continue;
              ll left=max(0LL,k1-i);
              ll right=k2-i;
              ans+=depth[i]*bit.range_sum(left,right);
        }
 
        for(ll i=maxd;i>=1;i--){
                if(depth[i]==0) continue;
                bit.add(i, depth[i]);
                depth[i] = 0;
        }
                     
    }
 
    for(ll i=0;i<=sub[entry];i++){
          bit.set(i);
          depth[i]=0;
    } 
   
    // Recurse on remaining components
    for (ll v : adj[centroid]) {
        if (!removed[v])
            build(v, centroid);
    }
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    ll n;
    cin >> n >> k1 >> k2;
 
    adj.resize(n + 1);
    sub.resize(n + 1);
    centroidParent.resize(n + 1);
    removed.assign(n + 1, false);
    depth.assign(n+1,0);
 
    for (ll i = 0; i < n - 1; i++) {
        ll u, v;
        cin >> u >> v;
 
        adj[u].pb(v);
        adj[v].pb(u);
    }
 
    build(1, -1);
 
    cout << ans << '\n';
 
    return 0;
