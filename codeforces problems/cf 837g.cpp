#pragma GCC optimize("O3")
#include <iostream>
#include <vector>

using namespace std;

// Maximum nodes needed: roughly N (for build) + Q * log2(N) (for updates)
const int MAX_NODES = 1e7; 
#define ll long long
#define pll pair<ll,ll>
#define pb push_back

struct Node {
    long long suma,sumb;
    int lc, rc; // Integer indices instead of pointers
} tree[MAX_NODES];

int node_cnt = 0; // Tracks the next available node index

// Function to create a new node and return its index
int new_node(long long suma = 0 , long long sumb=0, int lc = 0, int rc = 0) {
    int id = ++node_cnt;
    tree[id].suma = suma;
    tree[id].sumb = sumb;
    tree[id].lc = lc;
    tree[id].rc = rc;
    return id;
}

// Build the initial tree
int build(int l, int r, const vector<pll>& a) {
    if (l == r) {
        return new_node(a[l].first,a[l].second, 0, 0);
    }
    int mid = l + (r - l) / 2;
    int left_child = build(l, mid, a);
    int right_child = build(mid + 1, r, a);
    
    return new_node(tree[left_child].suma + tree[right_child].suma,tree[left_child].sumb + tree[right_child].sumb, left_child, right_child);
}

// Point update: creates new nodes along the updated path
int update(int prev_root, int l, int r, int pos, pll val) {
    if (l == r) {
        return new_node(tree[prev_root].suma + val.first, tree[prev_root].sumb + val.second, 0, 0); 
    }
    int mid = l + (r - l) / 2;
    
    int left_child = tree[prev_root].lc;
    int right_child = tree[prev_root].rc;
    
    if (pos <= mid) {
        left_child = update(tree[prev_root].lc, l, mid, pos, val);
    } else {
        right_child = update(tree[prev_root].rc, mid + 1, r, pos, val);
    }
    
    return new_node(tree[left_child].suma + tree[right_child].suma,tree[left_child].sumb + tree[right_child].sumb, left_child, right_child);
}

// Range sum query
pll query(int root, int l, int r, int ql, int qr) {
    if (r < ql || l > qr || root == 0) {
        return {0,0}; // Out of bounds or empty node
    }
    if (ql <= l && r <= qr) {
        return {tree[root].suma,tree[root].sumb}; // Fully covered
    }
    int mid = l + (r - l) / 2;
    pll x= query(tree[root].lc, l, mid, ql, qr);
    pll y=query(tree[root].rc, mid + 1, r, ql, qr);
    return {x.first+y.first,x.second+y.second};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    
    ll maxn=200005;

    vector<pll> ara(maxn,make_pair(0,0));

    vector<ll> roots;
    roots.pb(build(0,maxn-1,ara));

    ll n;
    cin >> n;
    
    
    ll ind = roots[0];
   
    for(ll i=0;i<n;i++){
          ll x1,x2,y1,a,b,y2;
          cin >> x1 >> x2 >> y1 >> a >> b >> y2;      
          ind=update(ind,0,maxn-1,0,make_pair(0,y1));
          ind=update(ind,0,maxn-1,x1+1,make_pair(0,-y1));
          ind=update(ind,0,maxn-1,x1+1,make_pair(a,b));
          ind=update(ind,0,maxn-1,x2+1,make_pair(-a,-b));
          ind=update(ind,0,maxn-1,x2+1,make_pair(0,y2));
          roots.pb(ind);
    }

    ll q;
    cin >> q;

    
    ll last = 0;

    while(q--){
         ll l,r,xraw;
         cin >> l >> r >> xraw;

       
         ll x = (xraw + last) % 1000000000LL;
         
    
         ll queryx = min(x, maxn - 1);

         pll aa=query(roots[r],0,maxn-1,0,queryx);
         pll bb=query(roots[l-1],0,maxn-1,0,queryx);
         aa.first-=bb.first;
         aa.second-=bb.second;
         
        
         ll ans = aa.first*x + aa.second;
  
         cout << ans << "\n";

         last = ans;
    }

    return 0;
}
