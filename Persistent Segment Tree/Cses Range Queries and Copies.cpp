#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Node {
    ll sum;
    Node *left, *right;

    Node(ll val = 0) {
        sum = val;
        left = right = nullptr;
    }

    Node(Node* l, Node* r) {
        left = l;
        right = r;
        sum = 0;
        if (l) sum += l->sum;
        if (r) sum += r->sum;
    }
};

Node* build(int l, int r, vector<ll>& a) {
    if (l == r)
        return new Node(a[l]);

    int mid = (l + r) >> 1;

    Node* left = build(l, mid, a);
    Node* right = build(mid + 1, r, a);

    return new Node(left, right);
}

Node* update(Node* node, int l, int r, int pos, ll val) {
    if (l == r)
        return new Node(val);

    int mid = (l + r) >> 1;

    if (pos <= mid) {
        return new Node(
            update(node->left, l, mid, pos, val),
            node->right
        );
    }

    return new Node(
        node->left,
        update(node->right, mid + 1, r, pos, val)
    );
}

ll query(Node* node, int l, int r, int ql, int qr) {
    if (r < ql || l > qr)
        return 0;

    if (ql <= l && r <= qr)
        return node->sum;

    int mid = (l + r) >> 1;

    return query(node->left, l, mid, ql, qr) +
           query(node->right, mid + 1, r, ql, qr);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<ll> a(n);

    for (int i = 0; i < n; i++)
        cin >> a[i];

    vector<Node*> roots;

    roots.push_back(build(0, n - 1, a));

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int k, pos;
            ll x;

            cin >> k >> pos >> x;

            k--;
            pos--;

            roots[k] = update(roots[k], 0, n - 1, pos, x);
        }
        else if (type == 2) {
            int k, l, r;

            cin >> k >> l >> r;

            k--;
            l--;
            r--;

            cout << query(roots[k], 0, n - 1, l, r) << '\n';
        }
        else {
            int k;
            cin >> k;

            k--;

            roots.push_back(roots[k]);
        }
    }

    return 0;
}
