#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vll vector<ll>

struct Node {
    long long sum,maxi;
    ll pref,suff;

    Node(long long val = 0) {
         sum=pref=suff=maxi=val;
    }
};

class SegmentTree {
    int n;
    vector<Node> tree;

    Node merge(const Node& left, const Node& right) {
        // return Node(left.sum + right.sum);
        Node x(0);
        x.pref=max(left.pref,left.sum+right.pref);
        x.suff=max(right.suff,right.sum+left.suff);
        x.sum=left.sum+right.sum;
        x.maxi=max({left.maxi,right.maxi,left.suff+right.pref});
        return x;
    }

    void build(int node, int l, int r, const vector<long long>& a) {
        if (l == r) {
            tree[node] = Node(a[l]);
            return;
        }

        int mid = (l + r) / 2;

        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);

        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    Node query(int node, int l, int r, int ql, int qr) {
        // Completely outside
        if (r < ql || qr < l) {
            return Node(-1e12);
        }

        // Completely inside
        if (ql <= l && r <= qr) {
            return tree[node];
        }

        int mid = (l + r) / 2;

        Node left = query(node * 2, l, mid, ql, qr);
        Node right = query(node * 2 + 1, mid + 1, r, ql, qr);

        return merge(left, right);
    }

public:
    SegmentTree(const vector<long long>& a) {
        n = a.size();
        tree.resize(4 * n);

        build(1, 0, n - 1, a);
    }

    long long query(int l, int r) {
        return query(1, 0, n - 1, l, r).maxi;
    }
};

int main() {
    // vector<long long> a = {1, 2, 3, 4, 5};

    // SegmentTree st(a);

    // cout << st.query(1, 3) << '\n'; // 9
    // cout << st.query(0, 4) << '\n'; // 15
    // cout << st.query(2, 4) << '\n'; // 12

      ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n;
    cin >> n;

    vll ara(n);
    for(ll i=0;i<n;i++){
          cin >> ara[i];
    }

    SegmentTree st(ara);
   
   ll q;
   cin >> q;


   while(q--){
      ll l,r;
      cin >> l >> r;
      
      l--;
      r--;

      cout << st.query(l,r) << endl;
   }



}
