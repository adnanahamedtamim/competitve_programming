#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int MAX_NODES = 3000005; // N * log2(N) is enough for COT

struct Node {
    int count; // Changed from sum to count for the frequency array
    int lc, rc; // Integer indices instead of pointers
} tree[MAX_NODES];

int node_cnt = 0; // Tracks the next available node index

// Function to create a new node and return its index
int new_node(int count = 0, int lc = 0, int rc = 0) {
    int id = ++node_cnt;
    tree[id].count = count;
    tree[id].lc = lc;
    tree[id].rc = rc;
    return id;
}

// Build the initial empty tree (Version 0)
int build(int l, int r) {
    if (l == r) {
        return new_node(0, 0, 0);
    }
    int mid = l + (r - l) / 2;
    int left_child = build(l, mid);
    int right_child = build(mid + 1, r);
    
    return new_node(tree[left_child].count + tree[right_child].count, left_child, right_child);
}

// Point update: creates new nodes along the updated path
int update(int prev_root, int l, int r, int pos) {
    if (l == r) {
        return new_node(tree[prev_root].count + 1, 0, 0); // Increment frequency
    }
    int mid = l + (r - l) / 2;
    
    int left_child = tree[prev_root].lc;
    int right_child = tree[prev_root].rc;
    
    if (pos <= mid) {
        left_child = update(tree[prev_root].lc, l, mid, pos);
    } else {
        right_child = update(tree[prev_root].rc, mid + 1, r, pos);
    }
    
    return new_node(tree[left_child].count + tree[right_child].count, left_child, right_child);
}

// Query the 4 tree versions simultaneously to find the k-th smallest element
int query(int u, int v, int lca, int p_lca, int l, int r, int k) {
    if (l == r) return l;

    // Inclusion-exclusion formula applied to the left child
    int count_left = tree[tree[u].lc].count + tree[tree[v].lc].count 
                   - tree[tree[lca].lc].count - tree[tree[p_lca].lc].count;

    int mid = l + (r - l) / 2;
    
    if (count_left >= k) {
        return query(tree[u].lc, tree[v].lc, tree[lca].lc, tree[p_lca].lc, l, mid, k);
    } else {
        return query(tree[u].rc, tree[v].rc, tree[lca].rc, tree[p_lca].rc, mid + 1, r, k - count_left);
    }
}

int n, m;
int w[MAXN];
vector<int> sorted_w;
vector<int> adj[MAXN];
int root[MAXN];

// Binary Lifting tables for LCA
int up[MAXN][20];
int depth[MAXN];

void dfs(int u, int p) {
    depth[u] = depth[p] + 1;
    up[u][0] = p;
    
    // Build binary lifting table for node u
    for (int i = 1; i < 20; i++) {
        up[u][i] = up[up[u][i-1]][i-1];
    }

    // Coordinate compress the node's weight
    int mapped_val = lower_bound(sorted_w.begin(), sorted_w.end(), w[u]) - sorted_w.begin() + 1;
    
    // Build version `u` strictly upon version `p`
    root[u] = update(root[p], 1, sorted_w.size(), mapped_val);

    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
        }
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    int diff = depth[u] - depth[v];
    for (int i = 0; i < 20; i++) {
        if ((diff >> i) & 1) u = up[u][i];
    }
    
    if (u == v) return u;
    
    for (int i = 19; i >= 0; i--) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    
    return up[u][0];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> m)) return 0;

    for (int i = 1; i <= n; i++) {
        cin >> w[i];
        sorted_w.push_back(w[i]);
    }

    // Sort and remove duplicates for coordinate compression
    sort(sorted_w.begin(), sorted_w.end());
    sorted_w.erase(unique(sorted_w.begin(), sorted_w.end()), sorted_w.end());

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Initialize the dummy global root with the build function
    root[0] = build(1, sorted_w.size());

    // Build the LCA tables and the Persistent Segment Tree via DFS
    dfs(1, 0);

    for (int i = 0; i < m; i++) {
        int u, v, k;
        cin >> u >> v >> k;

        int lca = get_lca(u, v);
        int p_lca = up[lca][0];

        // The query returns the 1-based index of the compressed weight
        int ans_idx = query(root[u], root[v], root[lca], root[p_lca], 1, sorted_w.size(), k);
        
        // Map the compressed index back to the real weight
        cout << sorted_w[ans_idx - 1] << "\n";
    }

    return 0;
}
