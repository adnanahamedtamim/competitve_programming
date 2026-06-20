#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll sum;
    Node *l, *r;

    Node(ll s = 0) : sum(s), l(nullptr), r(nullptr) {}

    Node(Node* L, Node* R) {
        l = L;
        r = R;
        sum = 0;

        if (L) sum += L->sum;
        if (R) sum += R->sum;
    }
};

Node* build(int tl, int tr) {

    if (tl == tr)
        return new Node();

    int tm = (tl + tr) >> 1;

    Node* left = build(tl, tm);
    Node* right = build(tm + 1, tr);

    return new Node(left, right);
}

Node* update(Node* node, int tl, int tr, int pos, ll val) {

    if (tl == tr) {
        Node* cur = new Node();
        cur->sum = node->sum + val;
        return cur;
    }

    int tm = (tl + tr) >> 1;

    if (pos <= tm) {
        return new Node(
            update(node->l, tl, tm, pos, val),
            node->r
        );
    }

    return new Node(
        node->l,
        update(node->r, tm + 1, tr, pos, val)
    );
}

ll query(Node* node, int tl, int tr, int l, int r) {

    if (!node || l > tr || r < tl)
        return 0;

    if (l <= tl && tr <= r)
        return node->sum;

    int tm = (tl + tr) >> 1;

    return query(node->l, tl, tm, l, r)
         + query(node->r, tm + 1, tr, l, r);
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<ll> a(n + 1);

    vector<ll> vals;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        vals.push_back(a[i]);
    }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    int m = vals.size();

    vector<Node*> root(n + 1);

    root[0] = build(1, m);

    for (int i = 1; i <= n; i++) {

        int pos =
            lower_bound(
                vals.begin(),
                vals.end(),
                a[i]
            ) - vals.begin() + 1;

        root[i] = update(
            root[i - 1],
            1,
            m,
            pos,
            1
        );
    }

    while (q--) {

        int l, r, k;
        cin >> l >> r >> k;

        int ans = -1;

        int lo = 1;
        int hi = m;

        while (lo <= hi) {

            int mid = (lo + hi) >> 1;

            ll total =
                query(root[r], 1, m, 1, mid)
                - query(root[l - 1], 1, m, 1, mid);

            if (total >= k) {
                ans = mid;
                hi = mid - 1;
            }
            else {
                lo = mid + 1;
            }
        }

        cout << vals[ans - 1] << '\n';
    }

    return 0;
}
