#include <bits/stdc++.h>
using namespace std;

int timer = 1;

void dfs(int node, int parent, vector<int> &vis, vector<int> adj[],
         vector<int> &tin, vector<int> &low, vector<pair<int,int>> &bridges) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    
    for (int v : adj[node]) {
        if (v == parent) continue;
        if (!vis[v]) {
            dfs(v, node, vis, adj, tin, low, bridges);
            low[node] = min(low[node], low[v]);
            if (low[v] > tin[node]) {
                bridges.push_back({node, v});
            }
        } else {
            low[node] = min(low[node], tin[v]);
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;  // number of nodes and edges

    vector<int> adj[n];
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // 0-based indexing assumed
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> vis(n, 0);
    vector<int> tin(n, -1), low(n, -1);
    vector<pair<int,int>> bridges;

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(i, -1, vis, adj, tin, low, bridges);
        }
    }

    for (auto &b : bridges) {
        cout << b.first << " " << b.second << "\n";
    }

    return 0;
}
