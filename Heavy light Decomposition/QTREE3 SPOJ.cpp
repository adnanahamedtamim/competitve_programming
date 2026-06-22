#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> parent_, depth_, heavy, head, pos, sze,nodeatpos;
vector<ll> val;
ll curPos = 0;

struct Fenwick {
    ll n;
    vector<ll> bit;

    Fenwick(ll n) : n(n), bit(n + 1, 0) {}

    void add(ll idx, ll delta) {
        idx++;
        while (idx <= n) {
            bit[idx] += delta;
            idx += idx & -idx;
        }
    }

    ll sum(ll idx) {
        idx++;
        ll res = 0;

        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }

        return res;
    }

    ll rangeSum(ll l, ll r) {
        if (l > r) return 0;
        return sum(r) - (l ? sum(l - 1) : 0);
    }
};

void dfs(ll u, vector<vector<ll>> &adj) {
    sze[u] = 1;

    for (ll v : adj[u]) {
        if (v == parent_[u]) continue;

        parent_[v] = u;
        depth_[v] = depth_[u] + 1;

        dfs(v, adj);

        sze[u] += sze[v];

        if (heavy[u] == -1 || sze[v] > sze[heavy[u]])
            heavy[u] = v;
    }
}

void dfs_hld(ll u, ll h, vector<vector<ll>> &adj) {
    head[u] = h;
    pos[u] = curPos++;
    nodeatpos[curPos-1]=u;

    if (heavy[u] != -1)
        dfs_hld(heavy[u], h, adj);

    for (ll v : adj[u]) {
        if (v == parent_[u] || v == heavy[u]) continue;
        dfs_hld(v, v, adj);
    }
}

ll queryPath(ll a, ll b, Fenwick &fw) {

    ll ansnode=-1;

    while (true) {
        // if (depth_[head[a]] < depth_[head[b]])
        //     swap(a, b);

        ll ans = fw.rangeSum(pos[head[b]], pos[b]);
        if(ans>0){
              ansnode=b;
        }
        // cout << ans << b << endl;

        if(head[0]==head[b]) break;

        b = parent_[head[b]];
    }
    
    if(ansnode==-1) return -1;
    
    ll l=pos[head[ansnode]],r=pos[ansnode];
    
    ll ans=0;
    while(l<=r){
          ll mid=(l+r)/2;
          ll sum=fw.rangeSum(l,mid);
          if(sum>0){
               ans=nodeatpos[mid];
               r=mid-1;
          }
          else{
              l=mid+1;
          }
    }

    return ans+1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

    val.resize(n);

    for (ll i = 0; i < n; i++)
          val[i]=0;

    vector<vector<ll>> adj(n);

    for (ll i = 0; i < n - 1; i++) {
        ll a, b;
        cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    parent_.assign(n, -1);
    depth_.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, -1);
    pos.assign(n, 0);
    sze.assign(n, 0);
    nodeatpos.assign(n,0);

    dfs(0, adj);
    dfs_hld(0, 0, adj);

    Fenwick fw(n);

    for (ll i = 0; i < n; i++) {
        fw.add(pos[i], val[i]);
    }

    // for(ll i=0;i<n;i++){
    //       cout << i+1 << " " << head[i] << endl;
    // }

    while (q--) {
        ll type;
        cin >> type;

        if (type == 0) {
            ll s, x;

            cin >> s;
            --s;

            x = 1 - val[s];
            ll delta=x-val[s];
            val[s] = x;

            fw.add(pos[s], delta);
        }
        else {
            ll a, b;
            cin >> b;
            
            --b;

            cout << queryPath(0, b, fw) << '\n';
        }
    }

    return 0;
}
