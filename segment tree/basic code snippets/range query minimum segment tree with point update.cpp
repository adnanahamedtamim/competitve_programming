class SegmentTree {
private:
    ll n;
    vector<ll> seg;

    void build(ll node, ll l, ll r, vector<ll>& ara) {
        if (l == r) {
            seg[node] = ara[l];
            return;
        }

        ll mid = l + (r - l) / 2;

        build(2 * node + 1, l, mid, ara);
        build(2 * node + 2, mid + 1, r, ara);

        seg[node] = min(seg[2 * node + 1], seg[2 * node + 2]);
    }

    ll query(ll start, ll end, ll node, ll l, ll r) {
        if (r < start || l > end)
            return LLONG_MAX;

        if (start <= l && r <= end)
            return seg[node];

        ll mid = l + (r - l) / 2;

        return min(
            query(start, end, 2 * node + 1, l, mid),
            query(start, end, 2 * node + 2, mid + 1, r)
        );
    }

    void update(ll node, ll l, ll r, ll idx, ll val) {
        if (l == r) {
            seg[node] = val;
            return;
        }

        ll mid = l + (r - l) / 2;

        if (idx <= mid)
            update(2 * node + 1, l, mid, idx, val);
        else
            update(2 * node + 2, mid + 1, r, idx, val);

        seg[node] = min(seg[2 * node + 1], seg[2 * node + 2]);
    }

public:
    // Constructor with initial array
    SegmentTree(vector<ll>& ara) {
        n = ara.size();
        seg.assign(4 * n, LLONG_MAX);
        build(0, 0, n - 1, ara);
    }

    // Constructor with size n (all values = 0)
    SegmentTree(ll size) {
        n = size;
        seg.assign(4 * n, 0);
    }

    ll query(ll l, ll r) {
        return query(l, r, 0, 0, n - 1);
    }

    void update(ll idx, ll val) {
        update(0, 0, n - 1, idx, val);
    }
};
