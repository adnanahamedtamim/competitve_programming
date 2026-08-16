#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Safe integer square root
long long floor_sqrt(long long x) {
    if (x <= 0) return 0;
    long long res = sqrt(x);
    // Floating point precision guard
    while ((res + 1) * (res + 1) <= x) res++;
    while (res * res > x) res--;
    return res;
}

struct Node {
    long long sum;
    long long max1;
    long long min1;
    long long lazy_add;
    int len;
};

class SegmentTreeSqrt {
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

    void update_sqrt(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return;
        
        // Break condition: if the max is already 0 or 1, sqrt doesn't change anything
        if (tree[node].max1 <= 1) return;
        
        if (ql <= l && r <= qr) {
            // Calculate how much the max and min will drop
            long long diff_max = tree[node].max1 - floor_sqrt(tree[node].max1);
            long long diff_min = tree[node].min1 - floor_sqrt(tree[node].min1);
            
            // If they drop by the exact same amount, apply as Range Subtraction
            if (diff_max == diff_min) {
                apply_add(node, -diff_max);
                return;
            }
        }
        
        // Recurse condition
        push_down(node);
        int mid = l + (r - l) / 2;
        update_sqrt(2 * node, l, mid, ql, qr);
        update_sqrt(2 * node + 1, mid + 1, r, ql, qr);
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
    SegmentTreeSqrt(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n + 1);
        build(1, 1, n - 1, a);
    }

    void add(int l, int r, long long v) { update_add(1, 1, n - 1, l, r, v); }
    void range_sqrt(int l, int r) { update_sqrt(1, 1, n - 1, l, r); }
    long long get_sum(int l, int r) { return query_sum(1, 1, n - 1, l, r); }
};
