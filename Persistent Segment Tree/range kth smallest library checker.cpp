
#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <vector>

using namespace std;
#define ll long long
#define  vll vector<ll>
#define pb push_back

// Maximum nodes needed: roughly N (for build) + Q * log2(N) (for updates)
const int MAX_NODES = 1e7; 

struct Node {
    long long sum;
    int lc, rc; // Integer indices instead of pointers
} tree[MAX_NODES];

int node_cnt = 0; // Tracks the next available node index

// Function to create a new node and return its index
int new_node(long long sum = 0, int lc = 0, int rc = 0) {
    int id = ++node_cnt;
    tree[id].sum = sum;
    tree[id].lc = lc;
    tree[id].rc = rc;
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
    
    int left_child = tree[prev_root].lc;
    int right_child = tree[prev_root].rc;
    
    if (pos <= mid) {
        left_child = update(tree[prev_root].lc, l, mid, pos, val);
    } else {
        right_child = update(tree[prev_root].rc, mid + 1, r, pos, val);
    }
    
    return new_node(tree[left_child].sum + tree[right_child].sum, left_child, right_child);
}

// Range sum query
long long query(int root, int l, int r, int ql, int qr) {
    if (r < ql || l > qr || root == 0) {
        return 0; // Out of bounds or empty node
    }
    if (ql <= l && r <= qr) {
        return tree[root].sum; // Fully covered
    }
    int mid = l + (r - l) / 2;
    return query(tree[root].lc, l, mid, ql, qr) + 
           query(tree[root].rc, mid + 1, r, ql, qr);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<pair<ll,ll>> ara(n);
    
    for(ll i=0;i<n;i++){
          cin >> ara[i].first;
          ara[i].second=i;
    }

    sort(ara.begin(),ara.end());

    vector<ll> su(n,0);
    su[ara[0].second]=1;

    vll roots;
    roots.pb(build(0,n-1,su));

    for(ll i=1;i<n;i++){
          roots.pb(update(roots[i-1],0,n-1,ara[i].second,1));
        //   cout << query(roots[roots.size()-1],0,n-1,1,1) << endl;
    }

    // for(auto it : ara){
    //       cout << it.first << " " << it.second << endl;
    // }

   
   while(q--){
      
      ll l,r,k;
      cin >> l >> r >> k;
      r--;
      k++;

      ll lo=0,h=n-1;
      ll ans=0;

      while(lo<=h){
         ll mid=(lo+h)/2;

         ll cnt=query(roots[mid],0,n-1,l,r);

         if(cnt>=k){
              ans=mid;
              h=mid-1;
         }
         else{
              lo=mid+1;
         }
      }


      cout   << ara[ans].first << endl;

     
   }

    
    



    return 0;
}
