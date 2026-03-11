#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5+5;
vector<int> adj[MAXN];
int tin[MAXN], low[MAXN], timer;
bool visited[MAXN];
bool isArticulation[MAXN];

void dfs(int node, int parent) {
    visited[node] = true;
    tin[node] = low[node] = ++timer;
    int children = 0;

    for (int v : adj[node]) {
        if (v == parent) continue;
        if (visited[v]) {
            low[node] = min(low[node], tin[v]);
        } else {
            dfs(v, node);
            low[node] = min(low[node], low[v]);
            if (low[v] >= tin[node] && parent != -1) {
                isArticulation[node] = true;
            }
            children++;
        }
    }

    if (parent == -1 && children > 1) {
        isArticulation[node] = true;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m; // n = number of nodes, m = number of edges
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;  // 0-based indexing
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    fill(visited, visited+n, false);
    fill(isArticulation, isArticulation+n, false);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(i, -1);
        }
    }

    vector<int> ans;
    for (int i = 0; i < n; i++) {
        if (isArticulation[i]) ans.push_back(i);
    }

    if (ans.empty()) cout << -1 << "\n";
    else {
        for (int x : ans) cout << x << " ";
        cout << "\n";
    }

    return 0;
}

