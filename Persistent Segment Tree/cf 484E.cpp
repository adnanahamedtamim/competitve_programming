
#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <vector>

using namespace std;
#define ll long long
#define vll vector<ll> 
#define pb push_back


// Maximum nodes needed: roughly N (for build) + Q * log2(N) (for updates)
const int MAX_NODES = 2e6; 

struct Node {
    long long sum;
    int lc, rc; // Integer indices instead of pointers
    ll cnt; // max consecutive number of ones in this segment
    ll prefcnt; // max consecutive number of ones in prefix
    ll suffcnt; // max consecutive number of ones in suffix
    ll len;
} tree[MAX_NODES];

int node_cnt = 0; // Tracks the next available node index

// Function to create a new node and return its index
int new_node(long long sum = 0, int lc = 0, int rc = 0) {
    int id = ++node_cnt;
    tree[id].sum = sum;
    tree[id].lc = lc;
    tree[id].rc = rc;
    
    if(lc==0 && rc==0)  {
        tree[id].cnt=tree[id].suffcnt=tree[id].prefcnt=tree[id].sum;
        tree[id].len=1;
    }
    else{
          tree[id].cnt=max({tree[lc].cnt,tree[rc].cnt,tree[lc].suffcnt+tree[rc].prefcnt});
          tree[id].len=tree[lc].len+tree[rc].len;
          if(tree[lc].sum==tree[lc].len){
              tree[id].prefcnt=tree[lc].sum+tree[rc].prefcnt;
          }
          else{
              tree[id].prefcnt=tree[lc].prefcnt;
          }
          if(tree[rc].sum==tree[rc].len){
              tree[id].suffcnt=tree[rc].sum+tree[lc].suffcnt;
          }
          else{
              tree[id].suffcnt=tree[rc].suffcnt;
          }

    }
    return id;
}

// Build the initial tree
int build(int l, int r, const vector<long long>& a) {
    if (l == r) {
        return new_node(a[l], 0, 0);
    }
    int mid = l + (r - l) / 2;
    int left_child = build(l, mid, a);
    int right_child = build(mid + 1, r, a);
    
    return new_node(tree[left_child].sum + tree[right_child].sum, left_child, right_child);
}

// Point update: creates new nodes along the updated path
int update(int prev_root, int l, int r, int pos, long long val) {
    if (l == r) {
        return new_node(val, 0, 0); // Create a new leaf
    }
    int mid = l + (r - l) / 2;
    
    int left_child = 0;
    int right_child = 0;

    if(prev_root != 0){
        left_child = tree[prev_root].lc;
        right_child = tree[prev_root].rc;
    }
        
    if (pos <= mid) {
        left_child = update(left_child, l, mid, pos, val);
    } else {
        right_child = update(right_child, mid + 1, r, pos, val);
    }
    
    return new_node(tree[left_child].sum + tree[right_child].sum, left_child, right_child);
}

Node merge(Node a, Node b){
    if(a.len==0) return b;
    if(b.len==0) return a;

    Node res;

    res.sum=a.sum+b.sum;
    res.len=a.len+b.len;

    res.cnt=max({a.cnt,b.cnt,a.suffcnt+b.prefcnt});

    if(a.sum==a.len)
        res.prefcnt=a.len+b.prefcnt;
    else
        res.prefcnt=a.prefcnt;

    if(b.sum==b.len)
        res.suffcnt=b.len+a.suffcnt;
    else
        res.suffcnt=b.suffcnt;

    res.lc=res.rc=0;

    return res;
}

Node query(int root,int l,int r,int ql,int qr){
    if(root==0 || r<ql || l>qr){
        Node res{};
        res.len=0;
        return res;
    }

    if(ql<=l && r<=qr){
        return tree[root];
    }

    int mid=(l+r)/2;

    Node a=query(tree[root].lc,l,mid,ql,qr);
    Node b=query(tree[root].rc,mid+1,r,ql,qr);

    return merge(a,b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

   
   ll n;
   cin >> n;

    vector<pair<ll,ll>>ara;
    vll su(n,0);

   for(ll i=0;i<n;i++){
       ll x;
       cin >> x;
       ara.pb({x,i});
   }

    sort(ara.rbegin(),ara.rend());

    vll roots;
    roots.pb(build(0,n-1,su));
    for(ll i=0;i<n;i++){
        roots.pb(update(roots.back(),0,n-1,ara[i].second,1));
    }

    ll q;
    cin >> q;


    while(q--){
          ll l,r,w;
          cin >> l >> r >> w;
          l--;
          r--;

          ll lo=1,h=n;
          ll ans=0;
          while(lo<=h){
             ll mid=(lo+h)/2;
           
             if(query(roots[mid],0,n-1,l,r).cnt>=w){
                   ans=ara[mid-1].first;
                   h=mid-1;
             }
             else{ 
                  lo=mid+1;
             }

          }

          cout << ans << endl;
    }
    





    return 0;
}
