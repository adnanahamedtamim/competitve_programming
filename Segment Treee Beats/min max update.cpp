#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Node {
    long long sum;
    long long max1, max2;
    long long min1, min2;
    int max_cnt, min_cnt;
    // No lazy_add or len needed anymore!
};

class SegmentTreeBeats {
private:
    int n;
    vector<Node> tree;

    void push_up(int node) {
        int lc = 2 * node, rc = 2 * node + 1;
        
        tree[node].sum = tree[lc].sum + tree[rc].sum;

        // --- Maintain Maximums ---
        tree[node].max1 = max(tree[lc].max1, tree[rc].max1);
        tree[node].max2 = -INF;
        tree[node].max_cnt = 0;
        
        if (tree[lc].max1 == tree[node].max1) tree[node].max_cnt += tree[lc].max_cnt;
        else tree[node].max2 = max(tree[node].max2, tree[lc].max1);
        
        if (tree[rc].max1 == tree[node].max1) tree[node].max_cnt += tree[rc].max_cnt;
        else tree[node].max2 = max(tree[node].max2, tree[rc].max1);
        
        tree[node].max2 = max({tree[node].max2, tree[lc].max2, tree[rc].max2});

        // --- Maintain Minimums ---
        tree[node].min1 = min(tree[lc].min1, tree[rc].min1);
        tree[node].min2 = INF;
        tree[node].min_cnt = 0;
        
        if (tree[lc].min1 == tree[node].min1) tree[node].min_cnt += tree[lc].min_cnt;
        else tree[node].min2 = min(tree[node].min2, tree[lc].min1);
        
        if (tree[rc].min1 == tree[node].min1) tree[node].min_cnt += tree[rc].min_cnt;
        else tree[node].min2 = min(tree[node].min2, tree[rc].min1);
        
        tree[node].min2 = min({tree[node].min2, tree[lc].min2, tree[rc].min2});
    }

    void apply_chmin(int node, long long v) {
        if (tree[node].max1 <= v) return;
        
        tree[node].sum -= (tree[node].max1 - v) * tree[node].max_cnt;
        
        // Variable Crosstalk: if the max we are squashing is also the min
        if (tree[node].min1 == tree[node].max1) tree[node].min1 = v;
        if (tree[node].min2 == tree[node].max1) tree[node].min2 = v;
        
        tree[node].max1 = v;
    }

    void apply_chmax(int node, long long v) {
        if (tree[node].min1 >= v) return;
        
        tree[node].sum += (v - tree[node].min1) * tree[node].min_cnt;
        
        // Variable Crosstalk: if the min we are boosting is also the max
        if (tree[node].max1 == tree[node].min1) tree[node].max1 = v;
        if (tree[node].max2 == tree[node].min1) tree[node].max2 = v;
        
        tree[node].min1 = v;
    }

    void push_down(int node) {
        int lc = 2 * node, rc = 2 * node + 1;
        
        // Enforce the parent's ceiling (chmin)
        apply_chmin(lc, tree[node].max1);
        apply_chmin(rc, tree[node].max1);
        
        // Enforce the parent's floor (chmax)
        apply_chmax(lc, tree[node].min1);
        apply_chmax(rc, tree[node].min1);
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        if (l == r) {
            tree[node].sum = tree[node].max1 = tree[node].min1 = a[l];
            tree[node].max2 = -INF;
            tree[node].min2 = INF;
            tree[node].max_cnt = tree[node].min_cnt = 1;
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_chmin(int node, int l, int r, int ql, int qr, long long v) {
        if (l > qr || r < ql || tree[node].max1 <= v) return;
        if (ql <= l && r <= qr && tree[node].max2 < v) {
            apply_chmin(node, v);
            return;
        }
        push_down(node);
        int mid = l + (r - l) / 2;
        update_chmin(2 * node, l, mid, ql, qr, v);
        update_chmin(2 * node + 1, mid + 1, r, ql, qr, v);
        push_up(node);
    }

    void update_chmax(int node, int l, int r, int ql, int qr, long long v) {
        if (l > qr || r < ql || tree[node].min1 >= v) return;
        if (ql <= l && r <= qr && tree[node].min2 > v) {
            apply_chmax(node, v);
            return;
        }
        push_down(node);
        int mid = l + (r - l) / 2;
        update_chmax(2 * node, l, mid, ql, qr, v);
        update_chmax(2 * node + 1, mid + 1, r, ql, qr, v);
        push_up(node);
    }

    long long query_sum(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node].sum;
        push_down(node);
        int mid = l + (r - l) / 2;
        return query_sum(2 * node, l, mid, ql, qr) + query_sum(2 * node + 1, mid + 1, r, ql, qr);
    }

public:
    SegmentTreeBeats(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a); 
    }

    void chmin(int l, int r, long long v) { update_chmin(1, 1, n - 1, l, r, v); }
    void chmax(int l, int r, long long v) { update_chmax(1, 1, n - 1, l, r, v); }
    long long get_sum(int l, int r) { return query_sum(1, 1, n - 1, l, r); }
};


int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // 1-based indexing array
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    // Initialize the Segment Tree
    SegmentTreeBeats st(a);

    // Process Q queries
    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            // Range Chmin: 1 l r v
            // Applies a[i] = min(a[i], v) for i in [l, r]
            int l, r;
            long long v;
            cin >> l >> r >> v;
            st.chmin(l, r, v);
        } 
        else if (type == 2) {
            // Range Chmax: 2 l r v
            // Applies a[i] = max(a[i], v) for i in [l, r]
            int l, r;
            long long v;
            cin >> l >> r >> v;
            st.chmax(l, r, v);
        } 
        else if (type == 3) {
            // Range Sum: 3 l r
            // Prints the sum of a[i] for i in [l, r]
            int l, r;
            cin >> l >> r;
            cout << st.get_sum(l, r) << "\n";
        }
    }

    return 0;
}
