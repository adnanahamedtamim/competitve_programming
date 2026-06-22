#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll sum, mx, mn;

    Node(ll v = 0) {
        sum = mx = mn = v;
    }
};

struct SegTree {
    ll n;

    vector<Node> seg;
    vector<ll> lazyAdd;
    vector<ll> lazySet;
    vector<bool> hasSet;

    SegTree(ll n) {
        this->n = n;

        seg.assign(4 * n + 5, Node());

        lazyAdd.assign(4 * n + 5, 0);
        lazySet.assign(4 * n + 5, 0);
        hasSet.assign(4 * n + 5, false);
    }

    Node merge(Node a, Node b) {
        Node res;

        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);

        return res;
    }

    void applySet(ll node, ll l, ll r, ll val) {

        seg[node].sum = (r - l + 1) * val;
        seg[node].mx = val;
        seg[node].mn = val;

        hasSet[node] = true;
        lazySet[node] = val;

        lazyAdd[node] = 0;
    }

    void applyAdd(ll node, ll l, ll r, ll val) {

        seg[node].sum += (r - l + 1) * val;
        seg[node].mx += val;
        seg[node].mn += val;

        if (hasSet[node])
            lazySet[node] += val;
        else
            lazyAdd[node] += val;
    }

    void push(ll node, ll l, ll r) {

        if (l == r)
            return;

        ll mid = (l + r) >> 1;

        if (hasSet[node]) {

            applySet(node * 2,
                     l,
                     mid,
                     lazySet[node]);

            applySet(node * 2 + 1,
                     mid + 1,
                     r,
                     lazySet[node]);

            hasSet[node] = false;
        }

        if (lazyAdd[node]) {

            applyAdd(node * 2,
                     l,
                     mid,
                     lazyAdd[node]);

            applyAdd(node * 2 + 1,
                     mid + 1,
                     r,
                     lazyAdd[node]);

            lazyAdd[node] = 0;
        }
    }

    void build(ll node,
               ll l,
               ll r,
               vector<ll>& arr) {

        if (l == r) {
            seg[node] = Node(arr[l]);
            return;
        }

        ll mid = (l + r) >> 1;

        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    void build(vector<ll>& arr) {
        build(1, 0, n - 1, arr);
    }

    void updateAdd(ll node,
                   ll l,
                   ll r,
                   ll ql,
                   ll qr,
                   ll val) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            applyAdd(node, l, r, val);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) >> 1;

        updateAdd(node * 2,
                  l,
                  mid,
                  ql,
                  qr,
                  val);

        updateAdd(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr,
                  val);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    void updateSet(ll node,
                   ll l,
                   ll r,
                   ll ql,
                   ll qr,
                   ll val) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            applySet(node, l, r, val);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) >> 1;

        updateSet(node * 2,
                  l,
                  mid,
                  ql,
                  qr,
                  val);

        updateSet(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr,
                  val);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    Node query(ll node,
               ll l,
               ll r,
               ll ql,
               ll qr) {

        if (qr < l || r < ql) {

            Node res;

            res.sum = 0;
            res.mx = LLONG_MIN;
            res.mn = LLONG_MAX;

            return res;
        }

        if (ql <= l && r <= qr)
            return seg[node];

        push(node, l, r);

        ll mid = (l + r) >> 1;

        return merge(
            query(node * 2,
                  l,
                  mid,
                  ql,
                  qr),

            query(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr)
        );
    }

    void updateAdd(ll l,
                   ll r,
                   ll val) {

        updateAdd(1,
                  0,
                  n - 1,
                  l,
                  r,
                  val);
    }

    void updateSet(ll l,
                   ll r,
                   ll val) {

        updateSet(1,
                  0,
                  n - 1,
                  l,
                  r,
                  val);
    }

    Node query(ll l,
               ll r) {

        return query(1,
                     0,
                     n - 1,
                     l,
                     r);
    }

    ll rangeSum(ll l,
                ll r) {

        return query(l, r).sum;
    }

    ll rangeMax(ll l,
                ll r) {

        return query(l, r).mx;
    }

    ll rangeMin(ll l,
                ll r) {

        return query(l, r).mn;
    }

    void pointAssign(ll pos,
                     ll val) {

        updateSet(pos,
                  pos,
                  val);
    }

    void pointAdd(ll pos,
                  ll val) {

        updateAdd(pos,
                  pos,
                  val);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    vector<ll> a(n);

    for (ll i = 0; i < n; i++)
        cin >> a[i];

    SegTree seg(n);
    seg.build(a);

    cout << seg.rangeSum(1, 4) << '\n';
    cout << seg.rangeMax(1, 4) << '\n';
    cout << seg.rangeMin(1, 4) << '\n';

    seg.updateAdd(2, 5, 10);
    seg.updateSet(3, 6, 7);

    seg.pointAdd(4, 100);
    seg.pointAssign(2, 50);

    Node res = seg.query(0, n - 1);

    cout << "SUM = " << res.sum << '\n';
    cout << "MAX = " << res.mx << '\n';
    cout << "MIN = " << res.mn << '\n';

    return 0;
}
