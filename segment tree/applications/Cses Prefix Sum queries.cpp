#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void buildsegtree(ll i, ll l, ll r,
                  vector<pair<ll, ll>>& st,
                  vector<ll>& ara) {

    if (l == r) {
        st[i].first = ara[l];
        st[i].second = ara[l];
        return;
    }

    ll mid = l + (r - l) / 2;

    buildsegtree(2 * i + 1, l, mid, st, ara);
    buildsegtree(2 * i + 2, mid + 1, r, st, ara);

    st[i].first = st[2 * i + 1].first + st[2 * i + 2].first;

    st[i].second = max(
        st[2 * i + 1].second,
        st[2 * i + 1].first + st[2 * i + 2].second
    );

    st[i].second = max(st[i].second, 0LL);
}

pair<ll, ll> query(ll start, ll end,
                   ll i, ll l, ll r,
                   vector<pair<ll, ll>>& st) {

    // No overlap
    if (l > end || r < start) {
        return {0, 0};
    }

    // Complete overlap
    if (start <= l && r <= end) {
        return st[i];
    }

    // Partial overlap
    ll mid = l + (r - l) / 2;

    pair<ll, ll> p1 = query(start, end, 2 * i + 1, l, mid, st);
    pair<ll, ll> p2 = query(start, end, 2 * i + 2, mid + 1, r, st);

    return {
        p1.first + p2.first,
        max(0LL, max(p1.second, p2.second + p1.first))
    };
}

void update(ll i, ll l, ll r,
            ll ind, ll val,
            vector<pair<ll, ll>>& st,
            vector<ll>& ara) {

    if (l == r) {
        st[i] = {val, val};
        return;
    }

    ll mid = l + (r - l) / 2;

    if (ind <= mid) {
        update(2 * i + 1, l, mid, ind, val, st, ara);
    }
    else {
        update(2 * i + 2, mid + 1, r, ind, val, st, ara);
    }

    st[i].first = st[2 * i + 1].first + st[2 * i + 2].first;

    st[i].second = max(
        st[2 * i + 1].second,
        st[2 * i + 1].first + st[2 * i + 2].second
    );

    st[i].second = max(st[i].second, 0LL);
}

// ll findkth(ll i, ll l, ll r, ll val, vector<ll>& segtree) {
//     if (l == r) {
//         return l;
//     }
//
//     ll mid = (l + r) / 2;
//
//     if (segtree[2 * i + 1] >= val) {
//         return findkth(2 * i + 1, l, mid, val, segtree);
//     }
//     else {
//         return findkth(
//             2 * i + 2,
//             mid + 1,
//             r,
//             val - segtree[2 * i + 1],
//             segtree
//         );
//     }
// }

int main() {
    ll n, q;
    cin >> n >> q;

    vector<ll> ara(n);

    for (ll i = 0; i < n; i++) {
        cin >> ara[i];
    }

    vector<pair<ll, ll>> segtree(4 * n);

    buildsegtree(0, 0, n - 1, segtree, ara);

    while (q--) {
        ll x;
        cin >> x;

        if (x == 1) {
            ll k, u;
            cin >> k >> u;

            k--;

            update(0, 0, n - 1, k, u, segtree, ara);
        }
        else {
            ll a, b;
            cin >> a >> b;

            a--;
            b--;

            cout << max(
                0LL,
                query(a, b, 0, 0, n - 1, segtree).second
            ) << '\n';
        }
    }

    return 0;
}
