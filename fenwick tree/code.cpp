#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct FenwickTree {
    ll n;
    vector<ll> tree;

    FenwickTree(ll n) : n(n), tree(n + 1, 0) {}

    void add(ll i, ll delta) {
        for (; i <= n; i += i & -i) {
            tree[i] += delta;
        }
    }

    ll query(ll i) {
        ll sum = 0;
        for (; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }

    ll query(ll l, ll r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll n;
    cin >> n;

    FenwickTree ft(n);

    for (ll i = 1; i <= n; i++) {
        ll val;
        cin >> val;
        ft.add(val, 1);
    }

    ll q;
    cin >> q;
    while (q--) {
        ll type, val;
        cin >> type >> val;
        if (type == 1) {
            ft.add(val, 1);
        } else {
            ft.add(val, -1);
        }
    }

    cout << ft.query(n) << endl;

    return 0;
}
