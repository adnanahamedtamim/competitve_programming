#include <iostream>
#include <vector>

using namespace std;

struct Node {
    long long sum;
    long long val_and;
    long long val_or;
    long long lazy_add;
    int len;
};

class SegmentTreeBitwise {
private:
    int n;
    vector<Node> tree;

    void push_up(int node) {
        int lc = 2 * node;
        int rc = 2 * node + 1;
        tree[node].sum = tree[lc].sum + tree[rc].sum;
        tree[node].val_and = (tree[lc].val_and & tree[rc].val_and);
        tree[node].val_or = (tree[lc].val_or | tree[rc].val_or);
    }

    // Applies a uniform shift to the segment.
    // Because this is ONLY called when the bits being changed are identical 
    // across all elements, standard integer addition perfectly simulates the bit flip!
    void apply_add(int node, long long v) {
        tree[node].sum += v * tree[node].len;
        tree[node].val_and += v;
        tree[node].val_or += v;
        tree[node].lazy_add += v;
    }

    void push_down(int node) {
        if (tree[node].lazy_add != 0) {
            apply_add(2 * node, tree[node].lazy_add);
            apply_add(2 * node + 1, tree[node].lazy_add);
            tree[node].lazy_add = 0;
        }
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        tree[node].len = r - l + 1;
        tree[node].lazy_add = 0;
        
        if (l == r) {
            tree[node].sum = tree[node].val_and = tree[node].val_or = a[l];
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_and(int node, int l, int r, int ql, int qr, long long x) {
        if (l > qr || r < ql) return;
        
        // Break Condition: The bits we are trying to clear (~x) are ALREADY 0 
        // for every element in this segment.
        if ((tree[node].val_or & ~x) == 0) return;
        
        if (ql <= l && r <= qr) {
            long long bits_to_clear_or = tree[node].val_or & ~x;
            long long bits_to_clear_and = tree[node].val_and & ~x;
            
            // Tag Condition: The bits being cleared are identical for all elements
            if (bits_to_clear_or == bits_to_clear_and) {
                apply_add(node, -bits_to_clear_or);
                return;
            }
        }
        
        push_down(node);
        int mid = l + (r - l) / 2;
        update_and(2 * node, l, mid, ql, qr, x);
        update_and(2 * node + 1, mid + 1, r, ql, qr, x);
        push_up(node);
    }

    void update_or(int node, int l, int r, int ql, int qr, long long x) {
        if (l > qr || r < ql) return;
        
        // Break Condition: The bits we are trying to set (x) are ALREADY 1 
        // for every element in this segment.
        if ((~tree[node].val_and & x) == 0) return;
        
        if (ql <= l && r <= qr) {
            // Bits that are 0 in at least one element, which we want to set to 1
            long long bits_to_set_or = ~tree[node].val_and & x;
            // Bits that are 0 in ALL elements, which we want to set to 1
            long long bits_to_set_and = ~tree[node].val_or & x;
            
            // Tag Condition: The bits being set are identical for all elements
            if (bits_to_set_or == bits_to_set_and) {
                apply_add(node, bits_to_set_or);
                return;
            }
        }
        
        push_down(node);
        int mid = l + (r - l) / 2;
        update_or(2 * node, l, mid, ql, qr, x);
        update_or(2 * node + 1, mid + 1, r, ql, qr, x);
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
    SegmentTreeBitwise(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a);
    }

    void range_and(int l, int r, long long x) { update_and(1, 1, n - 1, l, r, x); }
    void range_or(int l, int r, long long x) { update_or(1, 1, n - 1, l, r, x); }
    long long get_sum(int l, int r) { return query_sum(1, 1, n - 1, l, r); }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // 1-based indexing
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    SegmentTreeBitwise st(a);

    // Process Queries
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        
        if (type == 1) {
            // 1 l r x: Range AND
            long long x;
            cin >> x;
            st.range_and(l, r, x);
        } 
        else if (type == 2) {
            // 2 l r x: Range OR
            long long x;
            cin >> x;
            st.range_or(l, r, x);
        } 
        else if (type == 3) {
            // 3 l r: Query Sum
            cout << st.get_sum(l, r) << "\n";
        }
    }

    return 0;
}
