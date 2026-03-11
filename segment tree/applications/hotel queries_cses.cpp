#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void buildsegtree(ll i, ll l, ll r, vector<ll>& st, vector<ll>& ara) {
    if (l == r) {
        st[i] = l;  // store index
        return;
    }
    ll mid = l + (r - l) / 2;
    buildsegtree(2*i+1, l, mid, st, ara);
    buildsegtree(2*i+2, mid+1, r, st, ara);

    // pick index with max value
    ll leftIdx = st[2*i+1];
    ll rightIdx = st[2*i+2];
    if (ara[leftIdx] >= ara[rightIdx]) st[i] = leftIdx;
    else st[i] = rightIdx;
}

void update(ll i, ll l, ll r, ll pos, ll val, vector<ll>& st, vector<ll>& ara) {
    if (l == r) {
        ara[pos] = val;
        st[i] = l;  // store index
        return;
    }
    ll mid = l + (r - l) / 2;
    if (pos <= mid) update(2*i+1, l, mid, pos, val, st, ara);
    else update(2*i+2, mid+1, r, pos, val, st, ara);

    // merge again
    ll leftIdx = st[2*i+1];
    ll rightIdx = st[2*i+2];
    if (ara[leftIdx] >= ara[rightIdx]) st[i] = leftIdx;
    else st[i] = rightIdx;
}

ll query(ll i, ll l, ll r, ll ql, ll qr, vector<ll>& st, vector<ll>& ara) {
    if (l > qr || r < ql) return -1;  // out of range
    if (ql <= l && r <= qr) return st[i]; // fully inside

    ll mid = l + (r - l) / 2;
    ll leftIdx = query(2*i+1, l, mid, ql, qr, st, ara);
    ll rightIdx = query(2*i+2, mid+1, r, ql, qr, st, ara);

    if (leftIdx == -1) return rightIdx;
    if (rightIdx == -1) return leftIdx;
    return (ara[leftIdx] >= ara[rightIdx] ? leftIdx : rightIdx);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;
    vector<ll> ara(n);
    for (ll i = 0; i < n; i++) cin >> ara[i];

    vector<ll> st(4*n);
    buildsegtree(0, 0, n-1, st, ara);

    while (q--) {
        ll x;
        cin >> x;
        ll l=0;
        ll r=n-1;
        ll tr=-1;
        
        while(l<=r){
          ll mid=l+(r-l)/2;
          ll ind=query(0,0,n-1,l,mid,st,ara);
       if(ara[ind]>=x){
         tr=ind;
         r=mid-1;
       }
       else{
         l=mid+1;
       }
   
        }
        cout << tr+1 << " ";
        if(tr!=-1){
          update(0,0,n-1,tr,ara[tr]-x,st,ara);
        }
    }
    
    return 0;
}



