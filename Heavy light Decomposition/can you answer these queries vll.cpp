#include <bits/stdc++.h>
using namespace std;

using ll = long long;

/* ---------------- NODE ---------------- */

struct Node {
    ll sum, pref, suff, best;

    Node(ll v = 0) {
        sum = v;
        pref = suff = best = max(0LL, v);
    }
};

Node mergeNode(Node A, Node B) {
    Node res;

    res.sum = A.sum + B.sum;
    res.pref = max(A.pref, A.sum + B.pref);
    res.suff = max(B.suff, B.sum + A.suff);
    res.best = max({A.best, B.best, A.suff + B.pref});

    return res;
}

Node reverseNode(Node x) {
    swap(x.pref, x.suff);
    return x;
}

/* ---------------- SEGMENT TREE (LAZY ASSIGN) ---------------- */

struct SegTree {
    int n;
    vector<Node> seg;
    vector<ll> lazy;
    vector<bool> has;

    SegTree(int n) {
        this->n = n;
        seg.assign(4 * n, Node(0));
        lazy.assign(4 * n, 0);
        has.assign(4 * n, false);
    }

    void apply(int node, int l, int r, ll val) {
        seg[node].sum = val * (r - l + 1);
        seg[node].pref = seg[node].suff = seg[node].best =
            max(0LL, seg[node].sum);

        lazy[node] = val;
        has[node] = true;
    }

    void push(int node, int l, int r) {
        if (!has[node]) return;

        int mid = (l + r) >> 1;

        apply(node * 2, l, mid, lazy[node]);
        apply(node * 2 + 1, mid + 1, r, lazy[node]);

        has[node] = false;
    }

    void build(int node, int l, int r, vector<ll> &a) {
        if (l == r) {
            seg[node] = Node(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);

        seg[node] = mergeNode(seg[node * 2], seg[node * 2 + 1]);
    }

    void update(int node, int l, int r, int ql, int qr, ll val) {
        if (qr < l || r < ql) return;

        if (ql <= l && r <= qr) {
            apply(node, l, r, val);
            return;
        }

        push(node, l, r);

        int mid = (l + r) >> 1;

        update(node * 2, l, mid, ql, qr, val);
        update(node * 2 + 1, mid + 1, r, ql, qr, val);

        seg[node] = mergeNode(seg[node * 2], seg[node * 2 + 1]);
    }

    Node query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql)
            return Node(0);

        if (ql <= l && r <= qr)
            return seg[node];

        push(node, l, r);

        int mid = (l + r) >> 1;

        return mergeNode(
            query(node * 2, l, mid, ql, qr),
            query(node * 2 + 1, mid + 1, r, ql, qr)
        );
    }

    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    void update(int l, int r, ll val) {
        update(1, 0, n - 1, l, r, val);
    }
};

/* ---------------- HLD ---------------- */

const int N = 100005;

vector<int> adj[N];

int parent_[N], depth_[N], heavy[N];
int head[N], pos[N], sz[N];

ll val[N];

int curPos = 0;

/* DFS for subtree size + heavy child */
void dfs(int u, int p) {
    parent_[u] = p;
    sz[u] = 1;
    heavy[u] = -1;

    int mx = 0;

    for (int v : adj[u]) {
        if (v == p) continue;

        depth_[v] = depth_[u] + 1;
        dfs(v, u);

        sz[u] += sz[v];

        if (sz[v] > mx) {
            mx = sz[v];
            heavy[u] = v;
        }
    }
}

/* decompose tree */
void decompose(int u, int h) {
    head[u] = h;
    pos[u] = curPos++;

    if (heavy[u] != -1)
        decompose(heavy[u], h);

    for (int v : adj[u]) {
        if (v == parent_[u] || v == heavy[u]) continue;
        decompose(v, v);
    }
}

/* ---------------- PATH QUERY ---------------- */

Node queryPath(int a, int b, SegTree &seg) {
    Node left, right;

    while (head[a] != head[b]) {

        if (depth_[head[a]] > depth_[head[b]]) {
            Node cur = seg.query(pos[head[a]], pos[a]);
            cur = reverseNode(cur);

            left = mergeNode(left, cur);

            a = parent_[head[a]];
        }
        else {
            Node cur = seg.query(pos[head[b]], pos[b]);

            right = mergeNode(cur, right);

            b = parent_[head[b]];
        }
    }

    if (depth_[a] > depth_[b]) {
        Node cur = seg.query(pos[b], pos[a]);
        cur = reverseNode(cur);
        left = mergeNode(left, cur);
    }
    else {
        Node cur = seg.query(pos[a], pos[b]);
        right = mergeNode(cur, right);
    }

    return mergeNode(left, right);
}

/* ---------------- PATH UPDATE ---------------- */

void updatePath(int a, int b, ll c, SegTree &seg) {

    while (head[a] != head[b]) {

        if (depth_[head[a]] > depth_[head[b]]) {
            seg.update(pos[head[a]], pos[a], c);
            a = parent_[head[a]];
        }
        else {
            seg.update(pos[head[b]], pos[b], c);
            b = parent_[head[b]];
        }
    }

    if (depth_[a] > depth_[b]) {
        seg.update(pos[b], pos[a], c);
    }
    else {
        seg.update(pos[a], pos[b], c);
    }
}

/* ---------------- MAIN ---------------- */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> val[i];

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    depth_[1] = 0;
    dfs(1, 0);
    decompose(1, 1);

    vector<ll> base(n);

    for (int i = 1; i <= n; i++)
        base[pos[i]] = val[i];

    SegTree seg(n);
    seg.build(1, 0, n - 1, base);

    int q;
    cin >> q;

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int a, b;
            cin >> a >> b;

            cout << queryPath(a, b, seg).best << "\n";
        }
        else {
            int a, b;
            ll c;
            cin >> a >> b >> c;

            updatePath(a, b, c, seg);
        }
    }

    return 0;
}
