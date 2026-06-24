#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<vector<ll>> adj;
vector<ll> sub;
vector<ll> centroidParent;
vector<bool> removed;

void dfs_size(ll u, ll p) {

    sub[u] = 1;

    for (ll v : adj[u]) {

        if (v == p || removed[v]) continue;

        dfs_size(v, u);

        sub[u] += sub[v];
    }
}

ll find_centroid(ll u, ll p, ll totalSize) {

    for (ll v : adj[u]) {

        if (v == p || removed[v]) continue;

        if (sub[v] > totalSize / 2)
            return find_centroid(v, u, totalSize);
    }

    return u;
}

void build(ll entry, ll par) {

    dfs_size(entry, -1);

    ll centroid = find_centroid(entry, -1, sub[entry]);

    centroidParent[centroid] = par;

    removed[centroid] = true;

    for (ll v : adj[centroid]) {

        if (removed[v]) continue;

        build(v, centroid);
    }
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    adj.resize(n + 1);
    sub.resize(n + 1);
    centroidParent.resize(n + 1);
    removed.assign(n + 1, false);

    for (ll i = 0; i < n - 1; i++) {

        ll u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    build(1, -1);

    cout << "Centroid Parent Array:\n";

    for (ll i = 1; i <= n; i++) {

        cout << i << " -> ";

        if (centroidParent[i] == -1)
            cout << "ROOT";
        else
            cout << centroidParent[i];

        cout << '\n';
    }

    return 0;
}
