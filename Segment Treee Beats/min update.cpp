#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Node {
    long long sum;
    long long max1;
    long long max2;
    int max_cnt;
};

class SegmentTreeBeats {
private:
    int n;
    vector<Node> tree;

    // Helper to merge two children's data into the parent
    void push_up(int node) {
        int lc = 2 * node;
        int rc = 2 * node + 1;
        
        tree[node].sum = tree[lc].sum + tree[rc].sum;

        if (tree[lc].max1 == tree[rc].max1) {
            tree[node].max1 = tree[lc].max1;
            tree[node].max2 = max(tree[lc].max2, tree[rc].max2);
            tree[node].max_cnt = tree[lc].max_cnt + tree[rc].max_cnt;
        } else if (tree[lc].max1 > tree[rc].max1) {
            tree[node].max1 = tree[lc].max1;
            tree[node].max2 = max(tree[lc].max2, tree[rc].max1);
            tree[node].max_cnt = tree[lc].max_cnt;
        } else {
            tree[node].max1 = tree[rc].max1;
            tree[node].max2 = max(tree[lc].max1, tree[rc].max2);
            tree[node].max_cnt = tree[rc].max_cnt;
        }
    }

    // Helper to apply the chmin to a specific node
    void apply_chmin(int node, long long v) {
        // If the max is already smaller than or equal to v, do nothing
        if (tree[node].max1 <= v) return;
        
        // Update the sum based on how much the maximums are shrinking
        tree[node].sum -= (tree[node].max1 - v) * tree[node].max_cnt;
        // Cap the maximum
        tree[node].max1 = v;
    }

    // Propagate the parent's max1 down to its children
    void push_down(int node) {
        apply_chmin(2 * node, tree[node].max1);
        apply_chmin(2 * node + 1, tree[node].max1);
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        if (l == r) {
            tree[node].sum = a[l];
            tree[node].max1 = a[l];
            tree[node].max2 = -INF;
            tree[node].max_cnt = 1;
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_chmin(int node, int l, int r, int ql, int qr, long long v) {
        // Case 1: Out of bounds or Break condition
        if (l > qr || r < ql || tree[node].max1 <= v) {
            return;
        }
        
        // Case 2: Tag condition (fully covered and only max1 is affected)
        if (ql <= l && r <= qr && tree[node].max2 < v) {
            apply_chmin(node, v);
            return;
        }
        
        // Case 3: Recurse condition
        push_down(node);
        int mid = l + (r - l) / 2;
        update_chmin(2 * node, l, mid, ql, qr, v);
        update_chmin(2 * node + 1, mid + 1, r, ql, qr, v);
        push_up(node);
    }

    long long query_sum(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node].sum;
        
        push_down(node);
        int mid = l + (r - l) / 2;
        long long left_sum = query_sum(2 * node, l, mid, ql, qr);
        long long right_sum = query_sum(2 * node + 1, mid + 1, r, ql, qr);
        return left_sum + right_sum;
    }

public:
    SegmentTreeBeats(const vector<long long>& a) {
        n = a.size();
        // 1-based indexing for the tree array
        tree.resize(4 * n + 1);
        // Assuming a is 1-indexed: a[1] to a[n-1]
        build(1, 1, n - 1, a);
    }

    // Update a[i] = min(a[i], v) for i in [l, r]
    void chmin(int l, int r, long long v) {
        update_chmin(1, 1, n - 1, l, r, v);
    }

    // Query sum of a[i] for i in [l, r]
    long long get_sum(int l, int r) {
        return query_sum(1, 1, n - 1, l, r);
    }
};

int main() {
    // Fast I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    // Read number of elements and number of queries
    if (!(cin >> n >> q)) return 0;

    // 1-based indexing to match the template
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    // Initialize the Segment Tree
    SegmentTreeBeats st(a);

    // Process queries
    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            // Type 1: Range chmin(l, r, x)
            // Example input format: 1 l r x
            int l, r;
            long long x;
            cin >> l >> r >> x;
            st.chmin(l, r, x);
        } 
        else if (type == 2) {
            // Type 2: Range sum query(l, r)
            // Example input format: 2 l r
            int l, r;
            cin >> l >> r;
            cout << st.get_sum(l, r) << "\n";
        }
    }

    return 0;
}
