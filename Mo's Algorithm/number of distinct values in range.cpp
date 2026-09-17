#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int block_size;

struct Query {
    int l, r, id;
    
    // The Even-Odd Sorting Optimization
    bool operator<(const Query& other) const {
        int block_own = l / block_size;
        int block_other = other.l / block_size;
        
        if (block_own != block_other) {
            return block_own < block_other;
        }
        
        // If they are in the same block, sort by R.
        // Snake pattern: ascending for even blocks, descending for odd blocks
        if (block_own % 2 == 1) {
            return r < other.r;
        }
        return r > other.r;
    }
};

// Global state variables
int distinct_count = 0;
vector<int> freq;

// Add element at index 'idx' to the active window
void add(int idx, const vector<int>& a) {
    int val = a[idx];
    freq[val]++;
    if (freq[val] == 1) {
        distinct_count++;
    }
}

// Remove element at index 'idx' from the active window
void remove(int idx, const vector<int>& a) {
    int val = a[idx];
    freq[val]--;
    if (freq[val] == 0) {
        distinct_count--;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> a(n);
    // Note: If values are up to 10^9, coordinate compress them first!
    int max_val = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        max_val = max(max_val, a[i]);
    }

    freq.assign(max_val + 1, 0);
    
    // Standard block size is sqrt(N)
    block_size = max(1, (int)sqrt(n));

    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].l--; // Convert to 0-based indexing
        queries[i].r--;
        queries[i].id = i;
    }

    sort(queries.begin(), queries.end());

    vector<int> ans(q);
    
    // Initialize pointers to an empty window
    int curr_L = 0;
    int curr_R = -1;

    for (const auto& qry : queries) {
        int L = qry.l;
        int R = qry.r;

        // Expand the window
        while (curr_L > L) {
            curr_L--;
            add(curr_L, a);
        }
        while (curr_R < R) {
            curr_R++;
            add(curr_R, a);
        }

        // Shrink the window
        while (curr_L < L) {
            remove(curr_L, a);
            curr_L++;
        }
        while (curr_R > R) {
            remove(curr_R, a);
            curr_R--;
        }

        // Store the answer for the current query
        ans[qry.id] = distinct_count;
    }

    for (int i = 0; i < q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
