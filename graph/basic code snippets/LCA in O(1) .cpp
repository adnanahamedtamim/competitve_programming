#include <bits/stdc++.h>
using namespace std;

#define pb push_back
typedef long long ll;

vector<ll> depth;

// Build Sparse Table
vector<vector<ll>> buildSparseTable(vector<ll> &val) {
    ll n = val.size();
    ll LOG = log2(n) + 1;

    vector<vector<ll>> dp(n, vector<ll>(LOG));

    for (ll i = 0; i < n; i++)
        dp[i][0] = val[i];

    for (ll j = 1; (1LL << j) <= n; j++) {
        for (ll i = 0; i + (1LL << j) - 1 < n; i++) {

            ll left = dp[i][j - 1];
            ll right = dp[i + (1LL << (j - 1))][j - 1];

            if (depth[left] < depth[right])
                dp[i][j] = left;
            else
                dp[i][j] = right;
        }
    }

    return dp;
}

// RMQ Query
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
    ll j = log2(R - L + 1);

    ll left = lookup[L][j];
    ll right = lookup[R - (1LL << j) + 1][j];

    if (depth[left] < depth[right])
        return left;

    return right;
}

// LCA query for one pair
ll getLCA(ll u, ll v, vector<ll> &start,
          vector<vector<ll>> &lookup) {

    ll L = start[u];
    ll R = start[v];

    if (L > R)
        swap(L, R);

    return query(L, R, lookup);
}

// Euler Tour DFS
void dfs(ll i, ll par,
         vector<vector<ll>> &adj,
         vector<ll> &val,
         ll &timer,
         vector<ll> &start) {

    start[i] = timer;
    val[timer++] = i;

    for (auto it : adj[i]) {

        if (it == par)
            continue;

        depth[it] = depth[i] + 1;

        dfs(it, i, adj, val, timer, start);

        val[timer++] = i;
    }
}

int main() {

    ll n, q;
    cin >> n >> q;

    depth.resize(n, 0);

    vector<ll> start(n);
    vector<vector<ll>> adj(n);

    for (ll i = 2; i <= n; i++) {
        ll x;
        cin >> x;

        x--;
        ll y = i - 1;

        adj[x].pb(y);
        adj[y].pb(x);
    }

    vector<ll> val(2 * n - 1);

    ll timer = 0;

    dfs(0, -1, adj, val, timer, start);

    vector<vector<ll>> lookup = buildSparseTable(val);

    while (q--) {

        ll x, y;
        cin >> x >> y;

        x--;
        y--;

        cout << getLCA(x, y, start, lookup) + 1 << '\n';
    }

    return 0;
}
