#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // Required for std::gcd

using namespace std;

const long long INF = 1e18;

struct Node {
    long long sum;
    long long max1;
    long long max2;
    int max_cnt;
    
    long long gcd_all;
    long long gcd_other; 
};

class SegmentTreeGcdBeats {
private:
    int n;
    vector<Node> tree;

    void push_up(int node) {
        int lc = 2 * node, rc = 2 * node + 1;
        tree[node].sum = tree[lc].sum + tree[rc].sum;
        
        if (tree[lc].max1 == tree[rc].max1) {
            tree[node].max1 = tree[lc].max1;
            tree[node].max_cnt = tree[lc].max_cnt + tree[rc].max_cnt;
            tree[node].max2 = max(tree[lc].max2, tree[rc].max2);
            // max1 matches, so gcd_other is combined from both children's gcd_other
            tree[node].gcd_other = std::gcd(tree[lc].gcd_other, tree[rc].gcd_other);
            
        } else if (tree[lc].max1 > tree[rc].max1) {
            tree[node].max1 = tree[lc].max1;
            tree[node].max_cnt = tree[lc].max_cnt;
            tree[node].max2 = max(tree[lc].max2, tree[rc].max1);
            // Right child's max is smaller, so its ENTIRE segment goes into parent's gcd_other
            tree[node].gcd_other = std::gcd(tree[lc].gcd_other, tree[rc].gcd_all);
            
        } else {
            tree[node].max1 = tree[rc].max1;
            tree[node].max_cnt = tree[rc].max_cnt;
            tree[node].max2 = max(tree[lc].max1, tree[rc].max2);
            // Left child's max is smaller, so its ENTIRE segment goes into parent's gcd_other
            tree[node].gcd_other = std::gcd(tree[lc].gcd_all, tree[rc].gcd_other);
        }
        
        // Total GCD is just the GCD of the max element and the rest of the elements
        tree[node].gcd_all = std::gcd(tree[node].gcd_other, tree[node].max1);
    }

    void apply_chmin(int node, long long v) {
        // If the max is already less than or equal to v, doing chmin(v) changes nothing
        if (tree[node].max1 <= v) return;
        
        tree[node].sum -= (tree[node].max1 - v) * tree[node].max_cnt;
        tree[node].max1 = v;
        
        // O(1) recalculation of GCD: combine the unchanged `gcd_other` with the new `max1`
        tree[node].gcd_all = std::gcd(tree[node].gcd_other, tree[node].max1);
    }

    void push_down(int node) {
        int lc = 2 * node, rc = 2 * node + 1;
        // The parent's max1 acts as the lazy tag for the children
        if (tree[lc].max1 > tree[node].max1) {
            apply_chmin(lc, tree[node].max1);
        }
        if (tree[rc].max1 > tree[node].max1) {
            apply_chmin(rc, tree[node].max1);
        }
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        if (l == r) {
            tree[node].sum = a[l];
            tree[node].max1 = a[l];
            tree[node].max2 = -INF;
            tree[node].max_cnt = 1;
            
            tree[node].gcd_all = a[l];
            tree[node].gcd_other = 0; // 0 is the identity element for GCD
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_chmin(int node, int l, int r, int ql, int qr, long long v) {
        if (l > qr || r < ql || tree[node].max1 <= v) return;
        
        // If v strictly falls between max2 and max1, we can apply the tag safely
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

    long long query_sum(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node].sum;
        
        push_down(node);
        int mid = l + (r - l) / 2;
        return query_sum(2 * node, l, mid, ql, qr) + query_sum(2 * node + 1, mid + 1, r, ql, qr);
    }

    long long query_gcd(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node].gcd_all;
        
        push_down(node);
        int mid = l + (r - l) / 2;
        return std::gcd(query_gcd(2 * node, l, mid, ql, qr), 
                        query_gcd(2 * node + 1, mid + 1, r, ql, qr));
    }

public:
    SegmentTreeGcdBeats(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a);
    }

    void chmin(int l, int r, long long v) { update_chmin(1, 1, n - 1, l, r, v); }
    long long get_sum(int l, int r) { return query_sum(1, 1, n - 1, l, r); }
    long long get_gcd(int l, int r) { return query_gcd(1, 1, n - 1, l, r); }
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

    SegmentTreeGcdBeats st(a);

    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        
        if (type == 1) {
            // Type 1: Range Chmin (1 l r x)
            long long x;
            cin >> x;
            st.chmin(l, r, x);
        } else if (type == 2) {
            // Type 2: Range Sum (2 l r)
            cout << st.get_sum(l, r) << "\n";
        } else if (type == 3) {
            // Type 3: Range GCD (3 l r)
            cout << st.get_gcd(l, r) << "\n";
        }
    }

    return 0;
}
