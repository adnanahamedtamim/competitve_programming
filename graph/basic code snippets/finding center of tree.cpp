#include <bits/stdc++.h>

using namespace std;

vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
    if (n == 1) {
        return {0};
    }

    vector<vector<int>> adj(n);
    vector<int> degree(n, 0);

    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    queue<int> leaves;
    for (int i = 0; i < n; i++) {
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }

    int remaining_nodes = n;

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

    vector<int> result;
    while (!leaves.empty()) {
        result.push_back(leaves.front());
        leaves.pop();
    }

    return result;
}

int main() {
    int n = 6;
    vector<vector<int>> edges = {{3, 0}, {3, 1}, {3, 2}, {3, 4}, {5, 4}};

    vector<int> mht_roots = findMinHeightTrees(n, edges);

    cout << "Roots for Minimum Height Trees are: ";
    for (int root : mht_roots) {
        cout << root << " ";
    }
    cout << "\n";

    return 0;
}
