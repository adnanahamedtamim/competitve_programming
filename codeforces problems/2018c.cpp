#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define pb push_back

const ll N=600000;
vector<pair<ll,ll>> dep(N);
vector<vector<ll>> adj(N);

void dfs(ll i,ll par){
  ll maxi=dep[i].first;
  
  for(auto it : adj[i]){
      if(it==par) continue;
      
      dep[it].first=dep[i].first+1;
      dfs(it,i);
      maxi=max(maxi,dep[it].second);
   }
   dep[i].second=maxi;
}


vector<ll> bit(N, 0); 

void update(ll idx, ll val, ll n) {
    idx++; // BIT is usually 1-indexed
    while (idx <= n) {
        bit[idx] += val;
        idx += idx & (-idx);
    }
}

ll query(ll idx) {
    idx++; // BIT is 1-indexed
    ll sum = 0;
    while (idx > 0) {
        sum += bit[idx];
        idx -= idx & (-idx);
    }
    return sum;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
ll t;
cin >> t;

while(t--){
    
ll n;
cin >> n;

fill(bit.begin(), bit.begin() + n + 1, 0);

for(ll i=0;i<n-1;i++){
    ll x,y;
    cin >> x >> y;
    x--;
    y--;
    adj[x].pb(y);
    adj[y].pb(x);
}

dep[0].first=0;
dfs(0,-1); 


sort(dep.begin(),dep.begin()+n);
// for(auto it : dep){
//     cout << it.first << " " << it.second << endl;
// }

ll ans=1;
for(ll i=0;i<n;i++){
    ll x=dep[i].first;
    update(dep[i].second,1,n);
    ll cand=query(n-1);
    if(x>0){
       cand-=query(x-1);
    }
    ans=max(ans,cand);
}
 
 cout << n-ans << endl;
 
 for(ll i=0;i<n;i++){
     adj[i].clear();
     dep[i]={0,0};
 }
}

    return 0;
}
