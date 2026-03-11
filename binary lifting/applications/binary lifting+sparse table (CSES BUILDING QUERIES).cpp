#include <bits/stdc++.h>
using namespace std;

typedef long long ll;


 ll gcdn(ll a,ll b){
   if(b==0){
     return a;
   }
   return gcdn(b,a%b);
 }

    // Build Sparse Table for Range Minimum Query
    
    vector<vector<ll>> buildSparseTable(vector<ll> &arr) {
    
    ll n = arr.size();

    vector<vector<ll>> dp(n + 1, vector<ll>(log2(n) + 1, 0));

  for (ll i = 0; i < n; i++) {
        dp[i][0] = arr[i];
    }

    for (ll j = 1; (1LL << j) <= n; j++) {
        for (ll i = 0; (i + (1LL << j) - 1) < n; i++) {
            dp[i][j] =min(dp[i][j - 1], dp[i + (1LL << (j - 1))][j - 1]);
        }
    }

    return dp;
}

// Returns minimum of arr[L..R]
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
    ll j = (ll)log2(R - L + 1);
    return min(lookup[L][j], lookup[R - (1LL << j) + 1][j]);
}



int main() {
    
  ll n,q;
cin >> n >> q;

vector<ll> ara(n);

for(ll i=0;i<n;i++){
  cin >> ara[i];
}
 vector<ll> right(n,n);
 
 stack<ll> st;
 st.push(n-1);
 
 for(ll i=n-2;i>=0;i--){
   while(!st.empty() && ara[st.top()]<=ara[i]){
     st.pop();
   }
   if(!st.empty())
   {
     right[i]=st.top();
   }
   st.push(i);
 }
 
 vector<vector<ll>> dp(n+2,vector<ll>(30+1,n));
 
 for(ll i=0;i<n;i++){
   dp[i][0]=right[i];
 }
// for(ll i=0;i<n;i++){
//   dp[i][1]=right[i];
// }
 
 
 for(ll j=1;j<=(log2(n)+1);j++){
      for(ll i=0;i<n;i++){
      if(dp[i][j-1]==n) continue;
       dp[i][j]=dp[dp[i][j-1]][j-1];
  
    }
 }
 
while(q--){
  
  ll a,b;
  cin >> a >> b;
  b--;
  a--;
 
  ll ans = 1; // minimum length is 1
    ll pos = a;
  for(ll i=20;i>=0;i--){
    if(dp[pos][i]<=b)
    {
    ans+=(1LL<<i);
    pos=dp[pos][i];
      
    }
  }
 
  cout << ans << endl;
}
 
    return 0;
}



