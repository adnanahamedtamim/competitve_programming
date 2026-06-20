#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll cnt, sum;
    Node *l, *r;

    Node() : cnt(0), sum(0), l(nullptr), r(nullptr) {}
};

Node* update(Node* prev, int tl, int tr, int pos, ll val) {

    Node* cur = new Node();

    if (prev) *cur = *prev;

    if (tl == tr) {
        cur->cnt += 1;
        cur->sum += val;
        return cur;
    }

    int tm = (tl + tr) >> 1;

    if (pos <= tm)
        cur->l = update(prev ? prev->l : nullptr, tl, tm, pos, val);
    else
        cur->r = update(prev ? prev->r : nullptr, tm + 1, tr, pos, val);

    cur->cnt = (cur->l ? cur->l->cnt : 0) + (cur->r ? cur->r->cnt : 0);
    cur->sum = (cur->l ? cur->l->sum : 0) + (cur->r ? cur->r->sum : 0);

    return cur;
}

pair<ll,ll> query(Node* R, Node* L, int tl, int tr, int ql, int qr) {

    if (!R) R = new Node();
    if (!L) L = new Node();

    if (qr < tl || tr < ql)
        return {0, 0};

    if (ql <= tl && tr <= qr)
        return {R->cnt - L->cnt, R->sum - L->sum};

    int tm = (tl + tr) >> 1;

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

    int N, Q;
    cin >> N >> Q;

    vector<ll> a(N + 1);
    vector<ll> comp;

    for (int i = 1; i <= N; i++) {
        cin >> a[i];
        comp.push_back(a[i]);
    }

    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());

    int M = comp.size();

    vector<Node*> root(N + 1);
    root[0] = new Node();

    for (int i = 1; i <= N; i++) {

        int pos = lower_bound(comp.begin(), comp.end(), a[i]) - comp.begin() + 1;

        root[i] = update(root[i - 1], 1, M, pos, a[i]);
    }

    while (Q--) {

        int l, r;
        ll x;
        cin >> l >> r >> x;

        int idx = upper_bound(comp.begin(), comp.end(), x) - comp.begin();

        auto ans = query(root[r], root[l], 1, M, 1, idx);

        cout << ans.first << " " << ans.second << "\n";
    }

    return 0;
}
