#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

vector<ll> tree;  // Segment tree
vector<ll> lazy;  // Lazy array

// Build the segment tree
void constructSTUtil(vector<ll>& arr, ll ss, ll se, ll si) {
    if (ss > se) return;

    if (ss == se) { // Leaf node
        tree[si] = arr[ss];
        return;
    }

    ll mid = (ss + se) / 2;
    constructSTUtil(arr, ss, mid, si * 2 + 1);
    constructSTUtil(arr, mid + 1, se, si * 2 + 2);

    tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
}

void constructST(vector<ll>& arr, ll n) {
    tree.assign(4 * n, 0);
    lazy.assign(4 * n, 0);
    constructSTUtil(arr, 0, n - 1, 0);
}

// Push lazy values down
void pushDown(ll si, ll ss, ll se) {
    if (lazy[si] != 0) {
        tree[si] += (se - ss + 1) * lazy[si];

        if (ss != se) { // Not a leaf
            lazy[si * 2 + 1] += lazy[si];
            lazy[si * 2 + 2] += lazy[si];
        }

        lazy[si] = 0;
    }
}

// Range update
void updateRangeUtil(ll si, ll ss, ll se, ll us, ll ue, ll diff) {
    pushDown(si, ss, se);

    // Out of range
    if (ss > se || ss > ue || se < us)
        return;

    // Fully in range
    if (ss >= us && se <= ue) {
        tree[si] += (se - ss + 1) * diff;

        if (ss != se) {
            lazy[si * 2 + 1] += diff;
            lazy[si * 2 + 2] += diff;
        }
        return;
    }

    ll mid = (ss + se) / 2;
    updateRangeUtil(si * 2 + 1, ss, mid, us, ue, diff);
    updateRangeUtil(si * 2 + 2, mid + 1, se, us, ue, diff);

    tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
}

void updateRange(ll n, ll us, ll ue, ll diff) {
    updateRangeUtil(0, 0, n - 1, us, ue, diff);
}

// Query range sum
ll getSumUtil(ll ss, ll se, ll qs, ll qe, ll si) {
    pushDown(si, ss, se);

    if (ss > se || ss > qe || se < qs)
        return 0;

    if (ss >= qs && se <= qe)
        return tree[si];

    ll mid = (ss + se) / 2;
    return getSumUtil(ss, mid, qs, qe, 2 * si + 1) +
           getSumUtil(mid + 1, se, qs, qe, 2 * si + 2);
}

ll getSum(ll n, ll qs, ll qe) {
    if (qs < 0 || qe > n - 1 || qs > qe) {
        cout << "Invalid Input\n";
        return -1;
    }
    return getSumUtil(0, n - 1, qs, qe, 0);
}

// ------------------- DRIVER -------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    vector<ll> arr(n);
    for (ll i = 0; i < n; i++) cin >> arr[i];

    constructST(arr, n);

    ll q;
    cin >> q;
    while (q--) {
        ll type;
        cin >> type;
        if (type == 1) { 
            ll l, r, val;
            cin >> l >> r >> val;
            updateRange(n, l, r, val);
        } else {
            ll l, r;
            cin >> l >> r;
            cout << getSum(n, l, r) << "\n";
        }
    }

    return 0;
}



