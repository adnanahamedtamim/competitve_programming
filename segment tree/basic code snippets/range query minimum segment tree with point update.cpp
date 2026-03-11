#include <bits/stdc++.h>
using namespace std;

typedef long long ll;


void buildsegtree(ll i,ll l,ll r,vector<ll>& st,vector<ll>& ara){
  if(l==r){
 st[i]=ara[l];  
 return ;
  }
  
  ll mid=l+(r-l)/2;
  
  buildsegtree(2*i+1,l,mid,st,ara);
  buildsegtree(2*i+2,mid+1,r,st,ara);
  st[i]=min(st[2*i+1],st[2*i+2]);
  
  return;

}

ll query(ll start,ll end,ll i,ll l,ll r,vector<ll>& st){
  if(l>end  || r<start){
    return INT_MAX;
  }
   if(start<=l && end>=r){
     return st[i];
   }
   else{
      ll mid=l+(r-l)/2;
      
      return min(query(start,end,2*i+1,l,mid,st),query(start,end,2*i+2,mid+1,r,st));
   }
   
   return 0;
}

void  update(ll i,ll l,ll r,ll ind,ll val,vector<ll>& st,vector<ll>& ara){
  if(l==r){
    st[i]=val;
    return;
  }
  
  ll mid=l+(r-l)/2;
  
  if(ind<=mid){
     update(2*i+1,l,mid,ind,val,st,ara);
  }
  else{
    update(2*i+2,mid+1,r,ind,val,st,ara);
  }
  st[i]=min(st[2*i+1],st[2*i+2]);
  
  return ;
}


int main() {
    ll n,q;
 cin >> n >> q;


vector<ll> ara(n);
for(ll i=0;i<n;i++){
  cin >> ara[i];
}

vector<ll> segtree(4*n,INT_MAX);
buildsegtree(0,0,n-1,segtree,ara);

while(q--){
  ll a,l,r;
  cin >> a >> l >> r;
  
  
  if(a==1){
     l--;
    
    update(0,0,n-1,l,r,segtree,ara);
  }
  else{
      cout << query(l-1,r-1,0,0,n-1,segtree) << endl;
  }
  
}
  
    return 0;
}


