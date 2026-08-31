#include <bits/stdc++.h>
using namespace std;

#define pb push_back
typedef long long ll;
#define vll vector<ll>

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
         vector<ll> &start,vll& end) {

    start[i] = timer;
    val[timer++] = i;

    for (auto it : adj[i]) {

        if (it == par)
            continue;

        depth[it] = depth[i] + 1;

        dfs(it, i, adj, val, timer, start, end);

        val[timer++] = i;
    }
   
    end[i]=timer;
}

int main() {


    ll t;
    cin >> t;

    while(t--){ 

        ll n, q;
        cin >> n;

        depth.resize(n, 0);

        vector<ll> start(n),end(n);
        vector<vector<ll>> adj(n);
        vll col(n);
        vector<vector<ll>> colnodes(n);
        
        for(ll i=0;i<n;i++)
        {
             cin >> col[i];
             col[i]--;
             colnodes[col[i]].pb(i);
        }

        for (ll i = 2; i <= n; i++) {
            ll x,y;
            cin >> x >> y;
            x--;
            y--;
            adj[x].pb(y);
            adj[y].pb(x);
        }

        vector<ll> val(2 * n - 1);

        ll timer = 0;

        dfs(0, -1, adj, val, timer, start, end);

        vector<vector<ll>> lookup = buildSparseTable(val);


        auto func=[&](ll x,ll y){
          return start[x]<=start[y] && end[x]>=end[y];
        };
        
        vector<vector<ll>> vadj(n);
        ll ans=0;

        for(ll c=0;c<n;c++){

              if(colnodes[c].empty()) continue;

              vll nodes=colnodes[c];
              

              sort(nodes.begin(),nodes.end(),[&](ll a,ll b){
                  return start[a]<start[b];
              });

              ll sz=nodes.size();

              for(ll i=0;i<sz-1;i++){
                  ll lc=getLCA(nodes[i],nodes[i+1],start,lookup);
                  nodes.pb(lc);
              }

              sort(nodes.begin(),nodes.end(),[&](ll a,ll b){
                  return start[a]<start[b];
              });

              nodes.erase(unique(nodes.begin(),nodes.end()),nodes.end());

              stack<ll> st;

              for(auto it : nodes)
              {
                while(!st.empty() && !func(st.top(),it)){
                      st.pop();
                }
                if(!st.empty())
                {
                    vadj[st.top()].pb(it);
                }
                st.push(it);
              }

              auto dfs=[&](auto& self,ll i)->ll{
                    ll cur=0;

                    for(auto it : vadj[i]){
                         ll x=self(self,it);
                         if(c==col[i]){
                              ans+=x;
                         }
                         else{
                              ans+=(x*cur);
                              cur+=x;
                         }
                    }
                vadj[i].clear();    
                if(c==col[i]) return 1;
                return cur;    
              };


              dfs(dfs,nodes[0]);

        }
        cout << ans << endl;
    }

    return 0;
}
