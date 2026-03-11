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

vector<ll> su(n);
vector<ll> ara(n);
for(ll i=0;i<n;i++){
  cin >> ara[i];
  su[i]=ara[i]-i;
ara[i]+=i;
}

vector<ll> segtree(4*n,INT_MAX);
buildsegtree(0,0,n-1,segtree,ara);

vector<ll> seg2(4*n,INT_MAX);
buildsegtree(0,0,n-1,seg2,su);

while(q--){

 ll x;
cin >> x;
ll a,b;
if(x==1){
  cin >> a;
 cin >> b;
 a--;
  update(0,0,n-1,a,b+a,segtree,ara);
   update(0,0,n-1,a,b-a,seg2,su);
}

else{
 cin >> a;
 a--;
  ll ans1=INT_MAX,ans2=INT_MAX;

   ans1=query(a,n-1,0,0,n-1,segtree)-a;
 
    ans2=query(0,a,0,0,n-1,seg2)+a;
    
 cout << min(ans1,ans2) << endl;
  
}
  
}
  
    return 0;
}




