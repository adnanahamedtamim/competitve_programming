#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 2e7; // Increased slightly just to be safe for range updates

struct Node {
    long long sum;
    long long lazy; // Added lazy tag
    int lc, rc;
} tree[MAX_NODES];

int node_cnt = 0;

// Create a new node and return its index
int new_node(long long sum = 0, long long lazy = 0, int lc = 0, int rc = 0) {
    int id = ++node_cnt;
    tree[id].sum = sum;
    tree[id].lazy = lazy;
    tree[id].lc = lc;
    tree[id].rc = rc;
    return id;
}

// Build the initial tree
int build(int l, int r, const vector<long long>& a) {
    if (l == r) {
        return new_node(a[l], 0, 0, 0);
    }
    int mid = l + (r - l) / 2;
    int left_child = build(l, mid, a);
    int right_child = build(mid + 1, r, a);
    
    return new_node(tree[left_child].sum + tree[right_child].sum, 0, left_child, right_child);
}

// Range Update: O(log N) time and space
int update(int prev_root, int l, int r, int ql, int qr, long long val) {
    // Out of bounds: return the old node (no changes, no new memory used)
    if (l > qr || r < ql) {
        return prev_root;
    }
    
    // Copy the current node so we don't mutate old versions
    int curr = new_node(tree[prev_root].sum, tree[prev_root].lazy, tree[prev_root].lc, tree[prev_root].rc);
    
    // Fully covered: update this node's lazy tag and sum, stop recursing
    if (ql <= l && r <= qr) {
        tree[curr].lazy += val;
        tree[curr].sum += val * (r - l + 1);
        return curr;
    }
    
    // Partially covered: recurse to children
    int mid = l + (r - l) / 2;
    tree[curr].lc = update(tree[curr].lc, l, mid, ql, qr, val);
    tree[curr].rc = update(tree[curr].rc, mid + 1, r, ql, qr, val);
    
    // Recalculate sum from children + this node's OWN lazy tag
    tree[curr].sum = tree[tree[curr].lc].sum + tree[tree[curr].rc].sum + tree[curr].lazy * (r - l + 1);
    
    return curr;
}

// Range sum query: O(log N) time
long long query(int root, int l, int r, int ql, int qr) {
    if (r < ql || l > qr || root == 0) {
        return 0;
    }
    if (ql <= l && r <= qr) {
        return tree[root].sum; // Fully covered
    }
    
    int mid = l + (r - l) / 2;
    long long res = 0;
    
    // Add the contribution of THIS node's lazy tag to the overlapping query area
    long long overlap_l = max((long long)l, (long long)ql);
    long long overlap_r = min((long long)r, (long long)qr);
    res += tree[root].lazy * (overlap_r - overlap_l + 1);
    
    // Recurse
    res += query(tree[root].lc, l, mid, ql, qr);
    res += query(tree[root].rc, mid + 1, r, ql, qr);
    
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    vector<int> roots;
    roots.push_back(build(0, n - 1, a)); // Version 0

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) { // Range Update (modified from point update)
            int k, l, r;
            long long x;
            cin >> k >> l >> r >> x;
            k--; l--; r--; // Assuming 1-based indexing for inputs
            
            // Overwrite the specific version's root with the newly generated root
            roots[k] = update(roots[k], 0, n - 1, l, r, x);
        }
        else if (type == 2) { // Query
            int k, l, r;
            cin >> k >> l >> r;
            k--; l--; r--;
            cout << query(roots[k], 0, n - 1, l, r) << '\n';
        }
        else if (type == 3) { // Copy version
            int k;
            cin >> k;
            k--;
            roots.push_back(roots[k]);
        }
    }
    return 0;
}
