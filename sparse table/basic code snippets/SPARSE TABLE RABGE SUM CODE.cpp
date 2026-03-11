#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Build Sparse Table for Range Minimum Query
vector<vector<ll>> buildSparseTable(vector<ll> &arr) {
    ll n = arr.size();

    vector<vector<ll>> dp(n + 1, vector<ll>(log2(n) + 1, 0));

    for (ll i = 0; i < n; i++) {
        dp[i][0] = arr[i];
    }

    for (ll j = 1; (1LL << j) <= n; j++) {
        for (ll i = 0; (i + (1LL << j) - 1) < n; i++) {
            dp[i][j] = dp[i][j - 1]+dp[i + (1LL << (j - 1))][j - 1];
        }
    }

    return dp;
}

// Returns minimum of arr[L..R]
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
   ll ans=0;
  ll len=(R-L+1);
  ll ind=L;
  for(ll i=0;i<30;i++){
    if((len & 1<<i)){
      ans+=lookup[ind][i];
      ind+=(1<<i);
    }
  }
  
   return ans;
}

vector<ll> solveQueries(vector<ll>& arr, vector<vector<ll>>& queries) {
    ll n = arr.size();
    ll m = queries.size();
    vector<ll> result(m);

    // Build the sparse table
    vector<vector<ll>> lookup = buildSparseTable(arr);

    // Process each query
    for (ll i = 0; i < m; i++) {
        ll L = queries[i][0];
        ll R = queries[i][1];
        result[i] = query(L, R, lookup);
    }

    return result;
}

int main() {
    vector<ll> arr = { 7, 2, 3, 0, 5, 10, 3, 12, 18 };
    vector<vector<ll>> queries = { {0, 4}, {4, 7}, {7, 8} };

    vector<ll> res = solveQueries(arr, queries);
    for (ll i = 0; i < (ll)res.size(); i++) {
        cout << res[i] << " ";
    }
    return 0;
}