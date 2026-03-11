//problem : https://codeforces.com/problemset/problem/1324/F

#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
#define pb push_back
const long long MOD = 1000000007LL;

#define noo cout << "NO" << endl
#define yess cout << "YES" << endl





void dfs(ll i,ll par,vector<vector<ll>>& adj,vector<ll>& dp,vector<ll>& ara){

 dp[i]=ara[i];   
  for(auto it : adj[i]){
       if(it==par){
           continue;
       }
       dfs(it,i,adj,dp,ara);
      dp[i]+=max(0LL,dp[it]);
  }
  
}

void dfs2(ll i,ll par,vector<vector<ll>>& adj,vector<ll>& ans,vector<ll>& dp){

  ans[i]=dp[i];
  
  for(auto it : adj[i]){
       if(it==par){
           continue;
       }
       ll x=dp[i];
       ll y=dp[it];
       dp[i]=dp[i]-max(0LL,dp[it]);
       dp[it]+=max(dp[i],0LL);
       
       dfs2(it,i,adj,ans,dp);   //re-root
  
       dp[i]=x;
       dp[it]=y;
  }
    
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // precompute_fact();
    
    ll n;
    cin >> n;
    vector<ll> ara(n);
    for(ll i=0;i<n;i++){
       cin >> ara[i];
       if(ara[i]==0)
       {
           ara[i]=-1;
       }
    }
     vector<vector<ll>> adj(n);
     
    for(ll i=0;i<n-1;i++){
        ll x,y;
        cin >> x >> y;
        adj[x-1].pb(y-1);
        adj[y-1].pb(x-1);
    }   
   vector<ll> dp(n,0);
   
   dfs(0,-1,adj,dp,ara);
   
  vector<ll> ans(n,0);
  dfs2(0,-1,adj,ans,dp);
  
for(auto it : ans){
    cout << it << " ";
}


    return 0;
}
