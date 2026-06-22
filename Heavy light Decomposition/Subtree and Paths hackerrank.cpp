#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> parent_, depth_, heavy, head, pos, sze,nodeatpos;
vector<ll> val;
ll curPos = 0;


struct Node {
    ll sum, mx, mn;

    Node(ll v = 0) {
        sum = mx = mn = v;
    }
};

struct SegTree {
    ll n;

    vector<Node> seg;
    vector<ll> lazyAdd;
    vector<ll> lazySet;
    vector<bool> hasSet;

    SegTree(ll n) {
        this->n = n;

        seg.assign(4 * n + 5, Node());

        lazyAdd.assign(4 * n + 5, 0);
        lazySet.assign(4 * n + 5, 0);
        hasSet.assign(4 * n + 5, false);
    }

    Node merge(Node a, Node b) {
        Node res;

        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);

        return res;
    }

    void applySet(ll node, ll l, ll r, ll val) {

        seg[node].sum = (r - l + 1) * val;
        seg[node].mx = val;
        seg[node].mn = val;

        hasSet[node] = true;
        lazySet[node] = val;

        lazyAdd[node] = 0;
    }

    void applyAdd(ll node, ll l, ll r, ll val) {

        seg[node].sum += (r - l + 1) * val;
        seg[node].mx += val;
        seg[node].mn += val;

        if (hasSet[node])
            lazySet[node] += val;
        else
            lazyAdd[node] += val;
    }

    void push(ll node, ll l, ll r) {

        if (l == r)
            return;

        ll mid = (l + r) >> 1;

        if (hasSet[node]) {

            applySet(node * 2,
                     l,
                     mid,
                     lazySet[node]);

            applySet(node * 2 + 1,
                     mid + 1,
                     r,
                     lazySet[node]);

            hasSet[node] = false;
        }

        if (lazyAdd[node]) {

            applyAdd(node * 2,
                     l,
                     mid,
                     lazyAdd[node]);

            applyAdd(node * 2 + 1,
                     mid + 1,
                     r,
                     lazyAdd[node]);

            lazyAdd[node] = 0;
        }
    }

    void build(ll node,
               ll l,
               ll r,
               vector<ll>& arr) {

        if (l == r) {
            seg[node] = Node(arr[l]);
            return;
        }

        ll mid = (l + r) >> 1;

        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    void build(vector<ll>& arr) {
        build(1, 0, n - 1, arr);
    }

    void updateAdd(ll node,
                   ll l,
                   ll r,
                   ll ql,
                   ll qr,
                   ll val) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            applyAdd(node, l, r, val);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) >> 1;

        updateAdd(node * 2,
                  l,
                  mid,
                  ql,
                  qr,
                  val);

        updateAdd(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr,
                  val);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    void updateSet(ll node,
                   ll l,
                   ll r,
                   ll ql,
                   ll qr,
                   ll val) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            applySet(node, l, r, val);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) >> 1;

        updateSet(node * 2,
                  l,
                  mid,
                  ql,
                  qr,
                  val);

        updateSet(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr,
                  val);

        seg[node] =
            merge(seg[node * 2],
                  seg[node * 2 + 1]);
    }

    Node query(ll node,
               ll l,
               ll r,
               ll ql,
               ll qr) {

        if (qr < l || r < ql) {

            Node res;

            res.sum = 0;
            res.mx = LLONG_MIN;
            res.mn = LLONG_MAX;

            return res;
        }

        if (ql <= l && r <= qr)
            return seg[node];

        push(node, l, r);

        ll mid = (l + r) >> 1;

        return merge(
            query(node * 2,
                  l,
                  mid,
                  ql,
                  qr),

            query(node * 2 + 1,
                  mid + 1,
                  r,
                  ql,
                  qr)
        );
    }

    void updateAdd(ll l,
                   ll r,
                   ll val) {

        updateAdd(1,
                  0,
                  n - 1,
                  l,
                  r,
                  val);
    }

    void updateSet(ll l,
                   ll r,
                   ll val) {

        updateSet(1,
                  0,
                  n - 1,
                  l,
                  r,
                  val);
    }

    Node query(ll l,
               ll r) {

        return query(1,
                     0,
                     n - 1,
                     l,
                     r);
    }

    ll rangeSum(ll l,
                ll r) {

        return query(l, r).sum;
    }

    ll rangeMax(ll l,
                ll r) {

        return query(l, r).mx;
    }

    ll rangeMin(ll l,
                ll r) {

        return query(l, r).mn;
    }

    void pointAssign(ll pos,
                     ll val) {

        updateSet(pos,
                  pos,
                  val);
    }

    void pointAdd(ll pos,
                  ll val) {

        updateAdd(pos,
                  pos,
                  val);
    }
};


void dfs(ll u, vector<vector<ll>> &adj) {
    sze[u] = 1;

    for (ll v : adj[u]) {
        if (v == parent_[u]) continue;

        parent_[v] = u;
        depth_[v] = depth_[u] + 1;

        dfs(v, adj);

        sze[u] += sze[v];

        if (heavy[u] == -1 || sze[v] > sze[heavy[u]])
            heavy[u] = v;
    }
}

void dfs_hld(ll u, ll h, vector<vector<ll>> &adj) {
    head[u] = h;
    pos[u] = curPos++;
    nodeatpos[curPos-1]=u;

    if (heavy[u] != -1)
        dfs_hld(heavy[u], h, adj);

    for (ll v : adj[u]) {
        if (v == parent_[u] || v == heavy[u]) continue;
        dfs_hld(v, v, adj);
    }
}


ll queryPath(ll a, ll b, SegTree& st) {

    ll ans = LLONG_MIN;

    while (head[a] != head[b]) {

        if (depth_[head[a]] < depth_[head[b]])
            swap(a, b);

        ans = max(
            ans,
            st.query(pos[head[a]], pos[a]).mx
        );

        a = parent_[head[a]];
    }

    if (depth_[a] > depth_[b])
        swap(a, b);

    ans = max(
        ans,
        st.query(pos[a], pos[b]).mx
    );

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n;

    val.resize(n);

    for (ll i = 0; i < n; i++)
          val[i]=0;

    vector<vector<ll>> adj(n);

    for (ll i = 0; i < n - 1; i++) {
        ll a, b;
        cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    parent_.assign(n, -1);
    depth_.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, -1);
    pos.assign(n, 0);
    sze.assign(n, 0);
    nodeatpos.assign(n,0);

    dfs(0, adj);
    dfs_hld(0, 0, adj);

    SegTree st(n);
    st.build(val);
    cin >> q;

    while (q--) {
        string type;
        cin >> type;

        if (type == "add") {
            ll s, x;

            cin >> s >> x;
            s--;

            st.updateAdd(pos[s],pos[s]+sze[s]-1,x);
        }
        else {
           ll a,b;
           cin >> a >> b;
           
           a--;
           b--;
           
            cout << queryPath(a,b,st) << endl;
            
        }
    }

    return 0;
}
