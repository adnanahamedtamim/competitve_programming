#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;

vll parent, head, heavy, depth, sze;

void dfs(ll u, vvll &adj) {
    sze[u] = 1;

    for (auto v : adj[u]) {
        if (v == parent[u]) continue;

        parent[v] = u;
        depth[v] = depth[u] + 1;

        dfs(v, adj);

        sze[u] += sze[v];

        if (heavy[u] == -1 || sze[v] > sze[heavy[u]]) {
            heavy[u] = v;
        }
    }
}

void dfs_hld(ll u, ll chain_head, vvll &adj) {
    head[u] = chain_head;

    if (heavy[u] != -1) {
        dfs_hld(heavy[u], chain_head, adj);
    }

    for (auto v : adj[u]) {
        if (v == parent[u] || v == heavy[u]) continue;
        dfs_hld(v, v, adj);
    }
}

ll lca(ll a, ll b) {
    while (head[a] != head[b]) {
        if (depth[head[a]] < depth[head[b]]) {
            swap(a, b);
        }

        a = parent[head[a]];
    }

    return (depth[a] < depth[b] ? a : b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    vvll adj(n);

    parent.assign(n, -1);
    head.assign(n, -1);
    heavy.assign(n, -1);
    depth.assign(n, 0);
    sze.assign(n, 0);

    for (ll i = 0; i < n - 1; i++) {
        ll a, b;
        cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    dfs(0, adj);
    dfs_hld(0, 0, adj);

    ll a, b;
    cin >> a >> b;

    --a;
    --b;

    cout << lca(a, b) + 1 << '\n';

    return 0;
}
