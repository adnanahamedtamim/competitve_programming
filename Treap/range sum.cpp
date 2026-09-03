#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    int val, prio, sz;
    long long sum;
    Node *l, *r;
    Node(int v) : val(v), prio((int)rng()), sz(1), sum(v), l(nullptr), r(nullptr) {}
};

int sz(Node* t)        { return t ? t->sz : 0; }
long long sum(Node* t) { return t ? t->sum : 0; }

void pull(Node* t) {
    if (!t) return;
    t->sz  = 1 + sz(t->l) + sz(t->r);
    t->sum = t->val + sum(t->l) + sum(t->r);
}

// splits t into L (first k elements) and R (the rest), in-place via reference
void split(Node* t, int k, Node*& L, Node*& R) {
    if (!t) { L = R = nullptr; return; }
    if (sz(t->l) < k) {
        split(t->r, k - sz(t->l) - 1, t->r, R);
        L = t;
    } else {
        split(t->l, k, L, t->l);
        R = t;
    }
    pull(t);
}

Node* merge(Node* L, Node* R) {
    if (!L) return R;
    if (!R) return L;
    if (L->prio > R->prio) { L->r = merge(L->r, R); pull(L); return L; }
    else                   { R->l = merge(L, R->l); pull(R); return R; }
}

Node* build(const vector<int>& a) {
    Node* root = nullptr;
    for (int x : a) root = merge(root, new Node(x));   // O(N log N) expected
    return root;
}

// 1-indexed, inclusive: sum of a[l..r]
long long rangeSum(Node*& root, int l, int r) {
    Node *A, *temp, *B, *C;
    split(root, l - 1, A, temp);
    split(temp, r - l + 1, B, C);
    long long ans = sum(B);
    root = merge(merge(A, B), C);
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<int> a(n);
    for (auto& x : a) cin >> x;

    Node* root = build(a);

    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        cout << rangeSum(root, l, r) << "\n";
    }
}
