class SegmentTree {
private:
    ll n;
    vector<pair<ll, ll>> seg;   // {max, min}

    pair<ll, ll> merge(pair<ll, ll> a, pair<ll, ll> b) {
        return {max(a.first, b.first), min(a.second, b.second)};
    }

    void build(ll node, ll l, ll r, vector<ll>& ara) {
        if (l == r) {
            seg[node] = {ara[l], ara[l]};
            return;
        }

        ll mid = l + (r - l) / 2;

        build(2 * node + 1, l, mid, ara);
        build(2 * node + 2, mid + 1, r, ara);

        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    pair<ll, ll> query(ll start, ll end, ll node, ll l, ll r) {
        // No overlap
        if (r < start || l > end)
            return {LLONG_MIN, LLONG_MAX};

        // Complete overlap
        if (start <= l && r <= end)
            return seg[node];

        ll mid = l + (r - l) / 2;

        return merge(
            query(start, end, 2 * node + 1, l, mid),
            query(start, end, 2 * node + 2, mid + 1, r)
        );
    }

    void update(ll node, ll l, ll r, ll idx, ll val) {
        if (l == r) {
            seg[node] = {val, val};
            return;
        }

        ll mid = l + (r - l) / 2;

        if (idx <= mid)
            update(2 * node + 1, l, mid, idx, val);
        else
            update(2 * node + 2, mid + 1, r, idx, val);

        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

public:
    // Constructor with initial array
    SegmentTree(vector<ll>& ara) {
        n = ara.size();
        seg.assign(4 * n, {LLONG_MIN, LLONG_MAX});
        build(0, 0, n - 1, ara);
    }

    // Constructor with size n
    SegmentTree(ll size) {
        n = size;
        seg.assign(4 * n, {LLONG_MIN, LLONG_MAX});
    }

    // Returns {max, min} in range [l, r]
    pair<ll, ll> query(ll l, ll r) {
        return query(l, r, 0, 0, n - 1);
    }

    // Point assignment: ara[idx] = val
    void update(ll idx, ll val) {
        update(0, 0, n - 1, idx, val);
    }
};
