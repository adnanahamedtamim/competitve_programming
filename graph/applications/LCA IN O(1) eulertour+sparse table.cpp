#include <bits/stdc++.h>
using namespace std;

#define pb push_back
typedef long long ll;

vector<ll> depth; // global depth array

// Build Sparse Table for RMQ on node indices
vector<vector<ll>> buildSparseTable(vector<ll> &val) {
    ll n = val.size();
    ll LOG = log2(n) + 1;
    vector<vector<ll>> dp(n, vector<ll>(LOG, 0));

    for (ll i = 0; i < n; i++)
        dp[i][0] = val[i];  // store node index

    for (ll j = 1; (1LL << j) <= n; j++) {
        for (ll i = 0; (i + (1LL << j) - 1) < n; i++) {
            if (depth[dp[i][j - 1]] < depth[dp[i + (1LL << (j - 1))][j - 1]])
                dp[i][j] = dp[i][j - 1];
            else
                dp[i][j] = dp[i + (1LL << (j - 1))][j - 1];
        }
    }
    return dp;
}

// Returns minimum depth node in range [L..R]
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
    ll j = log2(R - L + 1);
    if (depth[lookup[L][j]] < depth[lookup[R - (1LL << j) + 1][j]])
        return lookup[L][j];
    else
        return lookup[R - (1LL << j) + 1][j];
}

// Solve LCA queries
vector<ll> solveQueries(vector<ll>& arr, vector<vector<ll>>& queries, vector<ll>& start) {
    ll n = arr.size();
    ll m = queries.size();

    vector<ll> result(m);
    vector<vector<ll>> lookup = buildSparseTable(arr);

    for (ll i = 0; i < m; i++) {
        ll u = queries[i][0];
        ll v = queries[i][1];
        ll L = start[u];
        ll R = start[v];
        if (L > R) swap(L, R);
        result[i] = query(L, R, lookup);
    }

    return result;
}

// Euler Tour DFS
void dfs(ll i, ll par, vector<vector<ll>>& adj, vector<ll>& val, ll& time, vector<ll>& start) {
    start[i] = time;
    val[time++] = i;

    for (auto it : adj[i]) {
        if (it == par) continue;
        depth[it] = depth[i] + 1;
        dfs(it, i, adj, val, time, start);
        val[time++] = i;
    }
}

int main() {
    ll n, q;
    cin >> n >> q;

    depth.resize(n, 0);  // global depth
    vector<ll> start(n, 0);
    vector<vector<ll>> adj(n);

    for (ll i = 0; i < n - 1; i++) {
        ll x, y;
        cin >> x >> y;
        x--; y--; // 0-based
        adj[x].pb(y);
        adj[y].pb(x);
    }

    ll time = 0;
    vector<ll> val(2 * n - 1, 0); // Euler tour

    dfs(0, -1, adj, val, time, start);

    vector<vector<ll>> queries;
    
    for (ll i = 0; i < q; i++) {
        ll x, y;
        cin >> x >> y;
        x--; y--; // 0-based
        queries.pb({x, y});
    }

    vector<ll> ans = solveQueries(val, queries, start);

    for (auto it : ans)
        cout << it + 1 << endl; 

    return 0;
}

