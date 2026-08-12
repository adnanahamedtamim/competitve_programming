/*QUESTION :: 

You are in City 1 (the capital) of a country with N cities and M one-way roads. If an enemy destroys City U,
how many other cities become completely unreachable from the capital ? You need to answer this for every city U

How Dominator Tree solves this:
By definition, if City U dominates City V, it means destroying U breaks all paths to V.
Therefore, if you build the Dominator Tree,
the number of cities isolated by destroying U is exactly the size of U's subtree in the Dominator Tree (minus 1, for U itself)
*/
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

struct DominatorTree {
    int n, timer;
    vvi adj, rev_adj, bucket, dom_tree;
    vi dfn, rev_dfn, sdom, idom, par, dsu, label;

    // 1-based indexing for N nodes
    DominatorTree(int n) : n(n), timer(0), 
        adj(n + 1), rev_adj(n + 1), bucket(n + 1), dom_tree(n + 1),
        dfn(n + 1, 0), rev_dfn(n + 1, 0), sdom(n + 1, 0), 
        idom(n + 1, 0), par(n + 1, 0), dsu(n + 1, 0), label(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    void dfs0(int u) {
        dfn[u] = ++timer;
        rev_dfn[timer] = u;
        sdom[u] = label[u] = dsu[u] = u;
        
        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs0(v);
                par[v] = u;
            }
        }
    }

    // DSU with path compression to find the node with the minimum sdom
    int find(int u, int x = 0) {
        if (u == dsu[u]) return x ? -1 : u;
        int p = find(dsu[u], u + 1);
        if (p < 0) return u;
        
        if (dfn[sdom[label[dsu[u]]]] < dfn[sdom[label[u]]]) {
            label[u] = label[dsu[u]];
        }
        dsu[u] = p;
        return x ? p : label[u];
    }

    // Builds the idom array and the dom_tree adjacency list
    void build(int root) {
        dfs0(root);
        
        // Process in reverse DFS order (Bottom-Up)
        for (int i = timer; i >= 2; i--) {
            int u = rev_dfn[i];
            for (int v : rev_adj[u]) {
                if (dfn[v]) {
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
        for (int i = 2; i <= timer; i++) {
            int u = rev_dfn[i];
            if (idom[u] != sdom[u]) idom[u] = idom[idom[u]];
            
            // Construct the final Dominator Tree
            dom_tree[idom[u]].push_back(u);
        }
    }
};

void solve() {
    int n, m;
    // Read N cities and M directed roads
    if (!(cin >> n >> m)) return;

    DominatorTree dt(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        dt.add_edge(u, v);
    }

    // Capital is city 1. Build the tree!
    dt.build(1);

    // Now dt.dom_tree is a directed tree where idom(V) points to V.
    // Let's find the subtree sizes to answer the "Critical Checkpoints" problem.
    vi subtree_size(n + 1, 0);

    // Simple DFS on the Dominator Tree to calculate subtree sizes
    auto dfs_tree = [&](auto& self, int u) -> void {
        subtree_size[u] = 1;
        for (int v : dt.dom_tree[u]) {
            self(self, v);
            subtree_size[u] += subtree_size[v];
        }
    };

    // Calculate sizes starting from the capital (Root = 1)
    dfs_tree(dfs_tree, 1);

    // Output how many cities become isolated if city i is destroyed
    for (int i = 1; i <= n; i++) {
        if (dt.dfn[i] == 0) {
            cout << "0 "; // City was already unreachable from the start
        } else {
            cout << subtree_size[i] - 1 << " "; 
        }
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // For single testcase
    solve();
    
    return 0;
}
