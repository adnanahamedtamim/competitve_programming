```cpp
/*
    Problem: Minimum Height Trees / Center of a Tree

    Given an undirected tree with n nodes, find all possible roots that
    produce a tree with minimum height.

    The answer consists of the center node(s) of the tree. We can find
    them by repeatedly removing all leaf nodes (nodes with degree 1)
    until at most two nodes remain.

    Time Complexity: O(n)
    Space Complexity: O(n)
*/

#include <bits/stdc++.h>
using namespace std;

vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
    if (n == 1) {
        return {0};
    }

    vector<vector<int>> adj(n);
    vector<int> degree(n, 0);

    // Build adjacency list and calculate degrees.
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];

        adj[u].push_back(v);
        adj[v].push_back(u);

        degree[u]++;
        degree[v]++;
    }

    // Initially, all leaves are added to the queue.
    queue<int> leaves;

    for (int i = 0; i < n; i++) {
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }

    int remaining_nodes = n;

    // Remove leaves layer by layer until only the center(s) remain.
    while (remaining_nodes > 2) {
        int leaves_count = leaves.size();
        remaining_nodes -= leaves_count;

        for (int i = 0; i < leaves_count; i++) {
            int leaf = leaves.front();
            leaves.pop();

            for (int neighbor : adj[leaf]) {
                degree[neighbor]--;

                if (degree[neighbor] == 1) {
                    leaves.push(neighbor);
                }
            }
        }
    }

    // The remaining node(s) are the center(s).
    vector<int> result;

    while (!leaves.empty()) {
        result.push_back(leaves.front());
        leaves.pop();
    }

    return result;
}

int main() {
    int n = 6;

    vector<vector<int>> edges = {
        {3, 0},
        {3, 1},
        {3, 2},
        {3, 4},
        {5, 4}
    };

    vector<int> mht_roots = findMinHeightTrees(n, edges);

    cout << "Roots for Minimum Height Trees are: ";

    for (int root : mht_roots) {
        cout << root << " ";
    }

    cout << "\n";

    return 0;
}
```
