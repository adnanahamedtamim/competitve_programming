#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long cap;
    int rev;
};

struct Dinic {
    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n) : n(n), g(n), level(n), it(n) {}

    void add_edge(int u, int v, long long cap) {
        Edge a = {v, cap, (int)g[v].size()};
        Edge b = {u, 0, (int)g[u].size()};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                if (level[e.to] < 0 && e.cap > 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    long long dfs(int u, int t, long long f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                long long pushed = dfs(e.to, t, min(f, e.cap));
                if (pushed > 0) {
                    e.cap -= pushed;
                    g[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    long long maxflow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (long long pushed = dfs(s, t, LLONG_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<long long> p(N), q(M);
    for (int i = 0; i < N; i++) cin >> p[i];
    for (int j = 0; j < M; j++) cin >> q[j];

    // Node indexing:
    // 0 ............ source
    // 1..N ......... projects
    // N+1..N+M ..... machines
    // N+M+1 ........ sink
    int S = 0;
    int T = N + M + 1;

    Dinic dinic(N + M + 2);

    long long totalRevenue = 0;

    // S -> Project edges
    for (int i = 0; i < N; i++) {
        dinic.add_edge(S, 1 + i, p[i]);
        totalRevenue += p[i];
    }

    // Machine -> T edges
    for (int j = 0; j < M; j++) {
        dinic.add_edge(1 + N + j, T, q[j]);
    }

    // Project -> Machine edges (dependencies)
    // Input: for each project, number of required machines followed by indices
    for (int i = 0; i < N; i++) {
        int k;
        cin >> k;
        while (k--) {
            int m;
            cin >> m;  // assume 1-based
            m--;
            dinic.add_edge(1 + i, 1 + N + m, LLONG_MAX);
        }
    }

    long long minCut = dinic.maxflow(S, T);
    long long maxProfit = totalRevenue - minCut;

    cout << maxProfit << "\n";
    return 0;
}
