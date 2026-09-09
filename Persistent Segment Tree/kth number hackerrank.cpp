#include <bits/stdc++.h>
using namespace std;
 
using ll = long long;
 
struct Node {
    ll sum;
    Node *l, *r;
 
    Node(ll s = 0) : sum(s), l(nullptr), r(nullptr) {}
 
    Node(Node* L, Node* R) {
        l = L;
        r = R;
        sum = 0;
        if (L) sum += L->sum;
        if (R) sum += R->sum;
    }
};
 
Node* build(int tl, int tr) {
    if (tl == tr) return new Node();
 
    int tm = (tl + tr) >> 1;
 
    Node* left = build(tl, tm);
    Node* right = build(tm + 1, tr);
 
    return new Node(left, right);
}
 
Node* update(Node* node, int tl, int tr, int pos, ll val) {
    if (tl == tr) {
        Node* cur = new Node();
        cur->sum = node->sum + val;
        return cur;
    }
 
    int tm = (tl + tr) >> 1;
 
    if (pos <= tm) {
        return new Node(
            update(node->l, tl, tm, pos, val),
            node->r
        );
    }
 
    return new Node(
        node->l,
        update(node->r, tm + 1, tr, pos, val)
    );
}
 
ll query(Node* node, int tl, int tr, int l, int r) {
    if (!node || l > tr || r < tl) return 0;
 
    if (l <= tl && tr <= r)
        return node->sum;
 
    int tm = (tl + tr) >> 1;
 
    return query(node->l, tl, tm, l, r)
         + query(node->r, tm + 1, tr, l, r);
}

int kth(Node* node,int tl,int tr,int k){

    if(tl==tr) return tl;

    int tm=(tl+tr)>>1;

    int leftCnt=node->l->sum;

    if(leftCnt>=k)
        return kth(node->l,tl,tm,k);

    return kth(node->r,tm+1,tr,k-leftCnt);
}

#define pb push_back
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, q;
    cin >> n >> q;
 
    vector<ll> a(n + 1,0);
 
    vector<pair<ll,ll>> vals;
    vals.pb({1e12,1e12});
    

    for(ll i=1;i<=n;i++){
          cin >> a[i];
          vals.push_back({a[i],i});
    }
    sort(vals.rbegin(),vals.rend());

    vector<Node*> root(n + 1);
 
    root[0] = build(1, n);
  
    // for (int i = 1; i <= n; i++) {
    //     root[i] = update(root[i - 1], 1, n, vals[i].second, 1);
    // }
    
    vector<vector<ll>> queries;
    
    for(ll i=0;i<q;i++){
         ll a,b;
         cin >> a >> b;
         queries.pb({a,b});
    }

    vector<ll> anss(q);

    ll j=0;

    for(ll i=1;i<=n;i++){
        root[i] = update(root[i - 1], 1, n, vals[i].second, 1);
    }

   for(auto it : queries){
     ll v=it[0];
     ll k=it[1];
     
     ll l=1,r=n;
     ll ind=1;

     while(l<=r){
         ll mid=(l+r)/2;
         if(vals[mid].first>=v){
             ind=mid;
             l=mid+1;  
         }
         else{
              r=mid-1;
         }
     }
    ll ans=kth(root[ind],1,n,k);
   cout << a[ans] << endl;
   }
    
 
    return 0;
}
