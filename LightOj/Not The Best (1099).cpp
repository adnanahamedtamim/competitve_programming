#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define pb push_back
#define vll vector<ll>
#define vvll vector<vector<ll>>

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll t;
    cin >> t;

    for (ll i = 0; i < t; i++) {
        ll n, r;
        cin >> n >> r;

        vector<vector<pair<ll, ll>>> adj(n);

       
        for (ll j = 0; j < r; j++) {
            ll u, v, w;
            cin >> u >> v >> w;

            u--;
            v--;

            adj[u].pb({v, w});
            adj[v].pb({u, w});
            adj[u].pb({u, 2 * w});  //go to neighbour and come back to the same node again
            adj[v].pb({v, 2 * w});
        }

        vll sp(n, 1e9), ssp(n, 1e9);

        sp[0] = 0;
        priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;

        pq.push({0, 0});

        while (!pq.empty()) {
            auto it = pq.top();
            pq.pop();
            ll w = it.first;
            ll u = it.second;

            if (w > ssp[u]) continue;

            
            for (auto edge : adj[u]) {
                ll v = edge.first;
                ll weight = edge.second;

                if (sp[v] > weight + w) { // shortest path check
                    ssp[v] = sp[v];
                    sp[v] = w + weight;
                    pq.push({ssp[v], v});
                    pq.push({w + weight, v});
                } 
                else if (sp[v] < weight + w && ssp[v] > weight + w) {   
                   
                    //****"(sp[v] < weight + w)"  we must have to check it  to ensure that the second shortest
                    // path is strictly smaller than the shortest one.
                    
                    ssp[v] = w + weight;
                    pq.push({w + weight, v});
                }
            }
        }

     
        cout << "Case " << i + 1 << ": " << ssp[n - 1] << "\n";
    }

    return 0;
}
