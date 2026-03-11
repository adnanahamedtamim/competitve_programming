#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define pb push_back


struct Edge {
    ll to;
    ll capacity;
    ll rev;
};

class Dinic {
public:
    ll V;
    vector<vector<Edge>> graph;
    vector<ll> level;
    vector<ll> ptr;

    Dinic(ll n) {
        V = n;
        graph.assign(V, {});
        level.resize(V);
        ptr.resize(V);
    }

    void addEdge(ll u, ll v, ll cap) {
        graph[u].push_back({v, cap, (ll)graph[v].size()});
        graph[v].push_back({u, 0, (ll)graph[u].size() - 1});
    }

    bool bfs(ll s, ll t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<ll> q;
        q.push(s);
        while (!q.empty()) {
            ll v = q.front();
            q.pop();
            for (auto& edge : graph[v]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(ll v, ll t, ll pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (ll& cid = ptr[v]; cid < (ll)graph[v].size(); ++cid) {
            auto& edge = graph[v][cid];
            ll tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.capacity == 0) continue;
            ll tr_pushed = dfs(tr, t, min(pushed, edge.capacity));
            if (tr_pushed == 0) continue;
            edge.capacity -= tr_pushed;
            graph[tr][edge.rev].capacity += tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    ll maxFlow(ll s, ll t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(s, t, LLONG_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, m;
   cin >> n >> m;
   vector<vector<ll>> edges;
    Dinic dinic(n);
    for (ll i = 0; i < m; i++) {
        ll u, v, c;
        cin >> u >> v;
        edges.pb({u-1,v-1});
        dinic.addEdge(u - 1, v - 1, 1);
        edges.pb({v-1,u-1});
        dinic.addEdge(v - 1, u - 1, 1);
    }

    cout << dinic.maxFlow(0, n - 1) << endl;
    
    vector<ll> reach(n,0);
    reach[0]=1;
    queue<ll> q;
    q.push(0);
    
    while(!q.empty()){
      ll i=q.front();
      q.pop();
      
      for(auto &it : dinic.graph[i]){
          if(it.capacity>0 && !reach[it.to]){
             reach[it.to]=1;
             q.push(it.to);
          }
      }
      
    }
    
 for(auto it : edges){
     if(reach[it[0]] && !reach[it[1]]){
         cout << it[0]+1 << " " << it[1]+1  << endl;
     }
 }   
    

    return 0;
}