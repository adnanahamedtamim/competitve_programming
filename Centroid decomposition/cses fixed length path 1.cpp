#include <bits/stdc++.h>
using namespace std;
 
using ll = long long;
#define pb push_back
typedef vector<ll> vll;
 
vector<vector<ll>> adj;
vector<ll> sub;
vector<ll> centroidParent;
vector<bool> removed;
vector<ll> cnt(200001,0);
ll ans=0,k;
 
 
void dfs_size(ll u, ll p) {
 
    sub[u] = 1;
 
    for (ll v : adj[u]) {
 
        if (v == p || removed[v]) continue;
 
        dfs_size(v, u);
 
        sub[u] += sub[v];
    }
}
 
ll find_centroid(ll u, ll p, ll totalSize) {
 
    for (ll v : adj[u]) {
 
        if (v == p || removed[v]) continue;
 
        if (sub[v] > totalSize / 2)
            return find_centroid(v, u, totalSize);
    }
 
    return u;
}
 
void get_dist(ll i,ll par,ll d,vll & dist){
     if(d>k) return;
 
     dist.pb(d);
 
     for(auto v : adj[i]){
          if(removed[v] || v==par) continue;
          get_dist(v,i,d+1,dist);
     }
  
}
 
void build(ll entry, ll par) {
 
    dfs_size(entry, -1);
 
    ll centroid = find_centroid(entry, -1, sub[entry]);
 
    centroidParent[centroid] = par;
 
    removed[centroid] = true;
    
    vll distance_used;
    cnt[0]=1;
    distance_used.pb(0);
    
    for(auto v : adj[centroid]){
          if(removed[v]) continue;
 
          vll dist;
 
          get_dist(v,centroid,1,dist);
 
          for(auto d : dist){
              if(d>k) continue;
              ans+=cnt[k-d];
          }
         
          for(auto d : dist){
             if(d<=k) {
                cnt[d]++;
                distance_used.pb(d);
             }
 
          }
 
    }
    
    for(auto it :distance_used){
          cnt[it]=0;
    }
 
    for (ll v : adj[centroid]) {
 
        if (removed[v]) continue;
 
        build(v, centroid);
    }
 
 
}
 
int main() {
 
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    ll n;
    cin >> n >> k;
 
    adj.resize(n + 1);
    sub.resize(n + 1);
    centroidParent.resize(n + 1);
    removed.assign(n + 1, false);
 
    for (ll i = 0; i < n - 1; i++) {
 
        ll u, v;
        cin >> u >> v;
 
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    build(1, -1);
 
   
   cout << ans << endl;
 
    return 0;
