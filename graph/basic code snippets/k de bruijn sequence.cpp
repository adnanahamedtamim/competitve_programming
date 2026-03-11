#include <bits/stdc++.h>
using namespace std;

string de_bruijn(int k, int n) {
    const string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (k <= 0 || n <= 0) return "";
    if (k > (int)alphabet.size()) return "";

    if (n == 1) {
        string s;
        for (int d = 0; d < k; ++d) s.push_back(alphabet[d]);
        return s;
    }

    long long numNodes = 1;
    for (int i = 0; i < n - 1; ++i) numNodes *= k;

    vector<int> nextEdge(numNodes, 0);
    vector<pair<int, int>> stack;
    stack.reserve(numNodes * k + 5);
    stack.push_back({0, -1});

    vector<int> circuitDigits;

    while (!stack.empty()) {
        int v = stack.back().first;
        if (nextEdge[v] < k) {
            int d = nextEdge[v]++;
            long long u = ((long long)v * k + d) % numNodes;
            stack.push_back({(int)u, d});
        } else {
            auto p = stack.back();
            stack.pop_back();
            if (p.second != -1) {
                circuitDigits.push_back(p.second);
            }
        }
    }

    reverse(circuitDigits.begin(), circuitDigits.end());

    string result;
    result.reserve(circuitDigits.size() + (n - 1));
    for (int i = 0; i < n - 1; ++i) result.push_back(alphabet[0]);
    for (int d : circuitDigits) result.push_back(alphabet[d]);

    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n;
    if (!(cin >> n)) return 0;
    k = 2;

    string s = de_bruijn(k, n);
    cout << s << "\n";
    
    return 0;
}
