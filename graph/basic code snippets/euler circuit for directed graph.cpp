#include <bits/stdc++.h>
using namespace std;
 
#define pb push_back
typedef long long ll;
 
int main() {
  
  ll n,m;
  cin >> n >> m;
  vector<vector<pair<ll,ll>>> adj(n);
  vector<ll> indeg(n,0),outdeg(n,0);
  
  for(ll i=0;i<m;i++){
      ll x,y;
      cin >> x >> y;
      adj[x-1].pb({y-1,i});
      // adj[y-1].pb({x-1,i});
      outdeg[x-1]++;
      indeg[y-1]++;
  }
  
  ll tr=0;
  
  for(ll i=0;i<n;i++){
    if(i==0 && !((-indeg[i]+outdeg[i])==1)){
      tr=1;
      break;
    }
    else if(i==n-1 && !((indeg[i]-outdeg[i])==1)){
      tr=1;
      break;
    }
    else if(i>0 && i<n-1 && (indeg[i]!=outdeg[i])){
        tr=1;
        break;
    }
  }
  
  if(tr==1){
      cout << "IMPOSSIBLE" << endl;
      return 0;
  }
  vector<ll> vis(m+1,0);
  // cout << "erg";
  vector<ll> path;
  stack<ll> st;
  st.push(0);
  
  while(!st.empty()){
     auto it=st.top();
  
    
    ll tr=0;
  while(!adj[it].empty()){
    
    auto it2=adj[it].back();
    adj[it].pop_back();
    
    if(!vis[it2.second]){
      st.push(it2.first);
      vis[it2.second]=1;
      tr=1;
      break;
    }
  }
  if(!tr){
      path.pb(it);
      st.pop();
  }
  }
 
      // cout << "fdv";
  
  if(path.size()!=m+1){
      cout << "IMPOSSIBLE" << endl;
  }
  else{
      reverse(path.begin(),path.end());
       for(auto it : path){
         cout << it+1 << " ";
       }
       cout << endl;
  }
  
 
    return 0;
}