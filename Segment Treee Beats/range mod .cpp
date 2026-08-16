#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    long long sum;
    long long max1;
    // Notice: No lazy tags are needed for Range Modulo!
};

class SegmentTreeModulo {
private:
    int n;
    vector<Node> tree;

    void push_up(int node) {
        int lc = 2 * node;
        int rc = 2 * node + 1;
        tree[node].sum = tree[lc].sum + tree[rc].sum;
        tree[node].max1 = max(tree[lc].max1, tree[rc].max1);
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        if (l == r) {
            tree[node].sum = a[l];
            tree[node].max1 = a[l];
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_mod(int node, int l, int r, int ql, int qr, long long mod_val) {
        // Case 1: Out of bounds OR the break condition (modulo has no effect)
        if (l > qr || r < ql || tree[node].max1 < mod_val) {
            return;
        }
        
        // Case 2: We reached a leaf node! Apply the modulo directly.
        if (l == r) {
            tree[node].sum %= mod_val;
            tree[node].max1 %= mod_val;
            return;
        }
        
        // Case 3: We are covering the segment, but elements inside need to be modulo'd.
        // We recurse deeper until we hit the leaves or the break condition.
        int mid = l + (r - l) / 2;
        update_mod(2 * node, l, mid, ql, qr, mod_val);
        update_mod(2 * node + 1, mid + 1, r, ql, qr, mod_val);
        push_up(node);
    }

    // Standard Point Update: a[idx] = val
    void update_point(int node, int l, int r, int idx, long long val) {
        if (l == r) {
            tree[node].sum = val;
            tree[node].max1 = val;
            return;
        }
        int mid = l + (r - l) / 2;
        if (idx <= mid) {
            update_point(2 * node, l, mid, idx, val);
        } else {
            update_point(2 * node + 1, mid + 1, r, idx, val);
        }
        push_up(node);
    }

    long long query_sum(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node].sum;
        
        int mid = l + (r - l) / 2;
        return query_sum(2 * node, l, mid, ql, qr) + query_sum(2 * node + 1, mid + 1, r, ql, qr);
    }

public:
    SegmentTreeModulo(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a); // Assuming 1-based indexing for 'a'
    }

    void range_mod(int l, int r, long long mod_val) {
        update_mod(1, 1, n - 1, l, r, mod_val);
    }

    void point_update(int idx, long long val) {
        update_point(1, 1, n - 1, idx, val);
    }

    long long get_sum(int l, int r) {
        return query_sum(1, 1, n - 1, l, r);
    }
};
