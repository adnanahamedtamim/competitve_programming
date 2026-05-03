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

        // Changed loop variable from 'i' to 'j' to avoid shadowing the outer loop's 'i'
        for (ll j = 0; j < r; j++) {
            ll u, v, w;
            cin >> u >> v >> w;

            u--;
            v--;

            adj[u].pb({v, w});
            adj[v].pb({u, w});
            adj[u].pb({u, 2 * w});
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

            // Renamed 'it' to 'edge' to avoid confusion with the 'it' from pq.top()
            for (auto edge : adj[u]) {
                ll v = edge.first;
                ll weight = edge.second;

                if (sp[v] > weight + w) {
                    ssp[v] = sp[v];
                    sp[v] = w + weight;
                    pq.push({ssp[v], v});
                    pq.push({w + weight, v});
                } 
                else if (sp[v] < weight + w && ssp[v] > weight + w) {
                    ssp[v] = w + weight;
                    pq.push({w + weight, v});
                }
            }
        }

        // Used "\n" instead of endl for faster output
        cout << "Case " << i + 1 << ": " << ssp[n - 1] << "\n";
    }

    return 0;
}
