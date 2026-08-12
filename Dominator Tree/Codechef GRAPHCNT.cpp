#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

struct DominatorTree {
    int n, timer;
    vvi adj, rev_adj, bucket, dom_tree;
    vi dfn, rev_dfn, sdom, idom, par, dsu, label;

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

    void build(int root) {
        dfs0(root);
        
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

        for (int i = 1; i <= timer; i++) {
            int u = rev_dfn[i];
            if (idom[u] != sdom[u]) idom[u] = idom[idom[u]];
            
            dom_tree[idom[u]].push_back(u);
        }
    }
};

void solve() {
    ll n, m;
    if (!(cin >> n >> m)) return;

    DominatorTree dt(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        dt.add_edge(u, v);
    }

    dt.build(0);

    vector<ll> subtree_size(n, 0);
    ll reachable_nodes = 1;

    auto dfs_tree = [&](auto& self, int u) -> void {
        subtree_size[u] = 1;
        reachable_nodes++;
        for (int v : dt.dom_tree[u]) {
            self(self, v);
            subtree_size[u] += subtree_size[v];
        }
    };

    for (int child : dt.dom_tree[0]) {
        dfs_tree(dfs_tree, child);
    }

    ll total_pairs = (reachable_nodes * (reachable_nodes - 1)) / 2;
    ll invalid_pairs = 0;

    for (int child : dt.dom_tree[0]) {
        ll S = subtree_size[child];
        invalid_pairs += (S * (S - 1)) / 2;
    }

    cout << total_pairs - invalid_pairs << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}
