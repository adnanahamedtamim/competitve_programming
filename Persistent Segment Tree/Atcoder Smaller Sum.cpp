#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll cnt, sum;
    Node *l, *r;

    Node() : cnt(0), sum(0), l(nullptr), r(nullptr) {}
};

Node* update(Node* prev, ll tl, ll tr, ll pos, ll val) {

    Node* cur = new Node();

    if (prev) {
        cur->cnt = prev->cnt;
        cur->sum = prev->sum;
        cur->l = prev->l;
        cur->r = prev->r;
    }

    if (tl == tr) {
        cur->cnt += 1;
        cur->sum += val;
        return cur;
    }

    ll tm = (tl + tr) >> 1;

    if (pos <= tm)
        cur->l = update(prev ? prev->l : nullptr, tl, tm, pos, val);
    else
        cur->r = update(prev ? prev->r : nullptr, tm + 1, tr, pos, val);

    cur->cnt = (cur->l ? cur->l->cnt : 0) + (cur->r ? cur->r->cnt : 0);
    cur->sum = (cur->l ? cur->l->sum : 0) + (cur->r ? cur->r->sum : 0);

    return cur;
}

pair<ll,ll> query(Node* R, Node* L, ll tl, ll tr, ll ql, ll qr) {

    if (!R) R = new Node();
    if (!L) L = new Node();

    if (qr < tl || tr < ql)
        return {0, 0};

    if (ql <= tl && tr <= qr)
        return {
            R->cnt - L->cnt,
            R->sum - L->sum
        };

    ll tm = (tl + tr) >> 1;

    auto left = query(R->l, L->l, tl, tm, ql, qr);
    auto right = query(R->r, L->r, tm + 1, tr, ql, qr);

    return {
        left.first + right.first,
        left.second + right.second
    };
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N;
    cin >> N;

    vector<ll> a(N + 1), comp;

    for (ll i = 1; i <= N; i++) {
        cin >> a[i];
        comp.push_back(a[i]);
    }

    ll Q;
    cin >> Q;

    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());

    ll M = comp.size();

    vector<Node*> root(N + 1);
    root[0] = new Node();

    for (ll i = 1; i <= N; i++) {

        ll pos = lower_bound(comp.begin(), comp.end(), a[i]) - comp.begin() + 1;

        root[i] = update(root[i - 1], 1, M, pos, a[i]);
    }

    ll prev = 0;

    while (Q--) {

        ll l, r;
        ll x;

        cin >> l >> r >> x;

        l ^= prev;
        r ^= prev;
        x ^= prev;

        if (l < 1) l = 1;
        if (r < 1) r = 1;

        if (l > r) swap(l, r);

        ll idx = upper_bound(comp.begin(), comp.end(), x) - comp.begin();

        auto ans = query(root[r], root[l - 1], 1, M, 1, idx);

        prev = ans.second;

        cout << prev << "\n";
    }

    return 0;
}
