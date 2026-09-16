#pragma GCC optimize("O3")
#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;
const int MAX_VAL = 500005; // The maximum possible value in the array a[i]

// The segment tree stores pairs of {min_prev_pos, array_index}
pair<int, int> tree[4 * MAX_VAL];

// Point update: Sets the 'prev_val' for a specific array 'pos'
void update(int node, int l, int r, int pos, int prev_val) {
    if (l == r) {
        tree[node] = {prev_val, pos};
        return;
    }
    int mid = l + (r - l) / 2;
    if (pos <= mid) {
        update(2 * node, l, mid, pos, prev_val);
    } else {
        update(2 * node + 1, mid + 1, r, pos, prev_val);
    }
    
    // Pull up the minimum from the children
    if (tree[2 * node].first < tree[2 * node + 1].first) {
        tree[node] = tree[2 * node];
    } else {
        tree[node] = tree[2 * node + 1];
    }
}

// Range minimum query: Finds the minimum 'prev_pos' in the range [ql, qr]
pair<int, int> query(int node, int l, int r, int ql, int qr) {
    if (ql > r || qr < l) return {INF, 0}; // Out of bounds
    if (ql <= l && r <= qr) return tree[node]; // Fully covered
    
    int mid = l + (r - l) / 2;
    pair<int, int> left_ans = query(2 * node, l, mid, ql, qr);
    pair<int, int> right_ans = query(2 * node + 1, mid + 1, r, ql, qr);
    
    if (left_ans.first < right_ans.first) {
        return left_ans;
    }
    return right_ans;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    int q;
    cin >> q;

    // Group queries by their right endpoint (R)
    // queries[i] will store a list of pairs: {L, query_index}
    vector<vector<pair<int, int>>> queries(n + 1);
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        queries[r].push_back({l, i});
    }

    vector<int> last_pos(MAX_VAL, 0);
    vector<int> ans(q, 0);

    // Initialize segment tree with INF
    for (int i = 0; i < 4 * MAX_VAL; i++) {
        tree[i] = {INF, 0};
    }

    // Process the array from left to right (The Sweep-Line)
    for (int i = 1; i <= n; i++) {
        int x = a[i];

        // 1. If 'x' appeared before, deactivate its old position
        if (last_pos[x] != 0) {
            update(1, 1, n, last_pos[x], INF);
        }

        // 2. Activate the new position, storing where it appeared previously
        update(1, 1, n, i, last_pos[x]);
        
        // 3. Update the last seen position of 'x' to the current index
        last_pos[x] = i;

        // 4. Answer all queries that end exactly at the current sweep-line (R = i)
        for (auto& qry : queries[i]) {
            int L = qry.first;
            int q_idx = qry.second;

            // Query the segment tree for the minimum prev_pos in the range [L, R]
            pair<int, int> res = query(1, 1, n, L, i);
            
            // If the minimum previous position is strictly less than L,
            // it means the element at res.second has NO other copies inside [L, R]
            if (res.first < L) {
                ans[q_idx] = a[res.second];
            } else {
                ans[q_idx] = 0;
            }
        }
    }

    // Output all answers in their original order
    for (int i = 0; i < q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
