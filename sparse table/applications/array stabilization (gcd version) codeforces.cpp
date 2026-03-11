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
            dp[i][j] =gcdn(dp[i][j - 1], dp[i + (1LL << (j - 1))][j - 1]);
        }
    }

    return dp;
}

// Returns minimum of arr[L..R]
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
    ll j = (ll)log2(R - L + 1);
    return gcdn(lookup[L][j], lookup[R - (1LL << j) + 1][j]);
}



int main() {
    
     ll t;
     cin >> t;
     while(t--){
     ll n;
      cin >>  n;
      vector<ll> ara(2*n);
      
      for(ll i=0;i<n;i++)
      {
        cin >> ara[i];
      }
      for(ll i=0;i<n;i++){
        ara[i+n]=ara[i];
      }
      
   vector<vector<ll>> su=buildSparseTable(ara);
    ll ans=-1;
    ll l=0;
   ll h=n-1;
   
   while(l<=h){
     ll mid=(l+h)/2;
    ll tr=0;
    ll x=-1;
    for(ll i=0;i<n;i++){
      ll gc=query(i,i+mid,su);
        if(x!=-1 && x!=gc){
        tr=1;
        break;
        }
        else {
          x=gc;
        }
    }
   if(tr==0){
     ans=mid;
    h=mid-1;
   }  
   else{
     l=mid+1;
   }
}


cout << ans << endl;
     }
    
    return 0;
}
