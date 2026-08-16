#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Safely performs mathematical floor division, even for negative numbers
long long floor_div(long long a, long long d) {
    return a / d - ((a ^ d) < 0 && a % d != 0);
}

struct Node {
    long long sum;
    long long max1;
    long long min1;
    long long lazy_add;
    int len;
};

class SegmentTreeDivide {
private:
    int n;
    vector<Node> tree;

    void push_up(int node) {
        int lc = 2 * node, rc = 2 * node + 1;
        tree[node].sum = tree[lc].sum + tree[rc].sum;
        tree[node].max1 = max(tree[lc].max1, tree[rc].max1);
        tree[node].min1 = min(tree[lc].min1, tree[rc].min1);
    }

    void apply_add(int node, long long v) {
        tree[node].sum += v * tree[node].len;
        tree[node].max1 += v;
        tree[node].min1 += v;
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
            tree[node].sum = tree[node].max1 = tree[node].min1 = a[l];
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        push_up(node);
    }

    void update_add(int node, int l, int r, int ql, int qr, long long v) {
        if (l > qr || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(node, v);
            return;
        }
        push_down(node);
        int mid = l + (r - l) / 2;
        update_add(2 * node, l, mid, ql, qr, v);
        update_add(2 * node + 1, mid + 1, r, ql, qr, v);
        push_up(node);
    }

    void update_div(int node, int l, int r, int ql, int qr, long long d) {
        if (l > qr || r < ql) return;
        
        if (ql <= l && r <= qr) {
            // Calculate how much the max and min will drop
            long long diff_max = tree[node].max1 - floor_div(tree[node].max1, d);
            long long diff_min = tree[node].min1 - floor_div(tree[node].min1, d);
            
            // If they drop by the exact same amount, it's just a Range Subtraction!
            if (diff_max == diff_min) {
                apply_add(node, -diff_max);
                return;
            }
        }
        
        // Otherwise, recurse to children
        push_down(node);
        int mid = l + (r - l) / 2;
        update_div(2 * node, l, mid, ql, qr, d);
        update_div(2 * node + 1, mid + 1, r, ql, qr, d);
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
    SegmentTreeDivide(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a);
    }

    void add(int l, int r, long long v) { update_add(1, 1, n - 1, l, r, v); }
    void divide(int l, int r, long long d) { 
        if (d == 1) return; // Edge case: dividing by 1 does nothing
        update_div(1, 1, n - 1, l, r, d); 
    }
    long long get_sum(int l, int r) { return query_sum(1, 1, n - 1, l, r); }
};
