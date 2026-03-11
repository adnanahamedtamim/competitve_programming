#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
vector<int> tree[4*MAXN];
int arr[MAXN];
int n, q;

// find all the elements in range a to b  between c<=x<=d

void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = {arr[l]};
        return;
    }
    int mid = (l + r) / 2;
    build(2*node, l, mid);
    build(2*node+1, mid+1, r);
    merge(tree[2*node].begin(), tree[2*node].end(),
          tree[2*node+1].begin(), tree[2*node+1].end(),
          back_inserter(tree[node]));
}

int query(int node, int l, int r, int ql, int qr, int c, int d) {
    if (qr < l || ql > r) return 0;  
    if (ql <= l && r <= qr) {
  
        return upper_bound(tree[node].begin(), tree[node].end(), d)
             - lower_bound(tree[node].begin(), tree[node].end(), c);
    }
    int mid = (l + r) / 2;
    return query(2*node, l, mid, ql, qr, c, d)
         + query(2*node+1, mid+1, r, ql, qr, c, d);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;
    for (int i = 1; i <= n; i++) cin >> arr[i];
    build(1, 1, n);

    while (q--) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        cout << query(1, 1, n, a, b, c, d) << "\n";
    }
}




