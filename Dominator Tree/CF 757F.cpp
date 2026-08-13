#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define int long long
typedef vector<int> vi;
#define vll vector<ll>
typedef vector<vector<int>> vvi;
#define pb push_back
#define vvll vector<vector<ll>>

struct DominatorTree {
    int n, timer;
    vvi adj, rev_adj, bucket, dom_tree;
    vi dfn, rev_dfn, sdom, idom, par, dsu, label;

    // 0-based indexing for N nodes
    DominatorTree(int n) : n(n), timer(-1), 
        adj(n), rev_adj(n), bucket(n), dom_tree(n),
        dfn(n, -1), rev_dfn(n, -1), sdom(n, -1), 
        idom(n, -1), par(n, -1), dsu(n, -1), label(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    void dfs0(int u) {
        dfn[u] = ++timer;
        rev_dfn[timer] = u;
        sdom[u] = label[u] = dsu[u] = u;
        
        for (int v : adj[u]) {
            if (dfn[v] == -1) {
                dfs0(v);
                par[v] = u;
            }
        }
    }

    // DSU with path compression
    int find(int u, bool is_recursive = false) {
        if (u == dsu[u]) return is_recursive ? -1 : u;
        
        int p = find(dsu[u], true);
        if (p < 0) return u;
        
        if (dfn[sdom[label[dsu[u]]]] < dfn[sdom[label[u]]]) {
            label[u] = label[dsu[u]];
        }
        dsu[u] = p;
        return is_recursive ? p : label[u];
    }

    // Builds the idom array and the dom_tree adjacency list
    void build(int root) {
        dfs0(root);
        
        // Process in reverse DFS order (Bottom-Up)
        for (int i = timer; i >= 1; i--) {
            int u = rev_dfn[i];
            for (int v : rev_adj[u]) {
                if (dfn[v] != -1) {
                    find(v);
                    if (dfn[sdom[label[v]]] < dfn[sdom[u]]) {
                        sdom[u] = sdom[label[v]];
                    }
                }
            }
            bucket[sdom[u]].push_back(u);
            int p = par[u];
            dsu[u] = p;

            for (int v : bucket[p]) {
                find(v);
                if (sdom[label[v]] == sdom[v]) idom[v] = sdom[v];
                else idom[v] = label[v];
            }
            bucket[p].clear();
        }

        // Process in forward DFS order (Top-Down)
        for (int i = 1; i <= timer; i++) {
            int u = rev_dfn[i];
            if (idom[u] != sdom[u]) idom[u] = idom[idom[u]];
            
            // Construct the final Dominator Tree
            dom_tree[idom[u]].push_back(u);
        }
    }
};

void solve() {
   ll n,m,s;
   while((cin >> n >> m >> s)){
   s--;

   vector<vector<pair<ll,ll>>> adj(n);
  
   for(ll i=0;i<m;i++){
        ll u,v,x;
        cin >> u >> v >> x;
        u--;
        v--;
        adj[u].pb({v,x});
        adj[v].pb({u,x});
   }

   vll dist(n,LLONG_MAX);
   priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
   dist[s]=0;
   pq.push({0,s});

   while(!pq.empty()){
      auto it = pq.top();
      pq.pop();
      ll dis=it.first;
      ll ind=it.second;

      if(dis>dist[ind]) continue;

      for(auto it : adj[ind]){
          ll ind2=it.first;
          ll c=it.second;
          if((dist[ind2])>(dis+c))
          {
             dist[ind2]=dis+c;
             pq.push({dist[ind2],ind2});
          }
      }
   }




    DominatorTree dt(n);

    for(ll i=0;i<n;i++){
          for(auto it : adj[i]){
              if(dist[it.first]==dist[i]+it.second){
                  dt.add_edge(i,it.first);
              }
          }
    }


    dt.build(s);
    vi subtree_size(n + 1, 0);

  
    auto dfs_tree = [&](auto& self, int u) -> void {
        subtree_size[u] = 1;
        for (int v : dt.dom_tree[u]) {
            self(self, v);
            subtree_size[u] += subtree_size[v];
        }
    };

   
    dfs_tree(dfs_tree, s);

    
    ll maxi=0;
    for(auto it : dt.dom_tree[s]){
          maxi=max(maxi,subtree_size[it]);
    }
    cout << maxi << endl;

   }

   return;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    ll t;
    // cin >> t;

    // while(true){
          solve();
    // }
    
    return 0;
}
