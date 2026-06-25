#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
const int INF = 1e9;

vector<int> adj[N];

int n, q;

int sub[N];
bool removed[N];

int best[N];

vector<pair<int,int>> pathCentroid[N];

void dfs_size(int u, int p)
{
    sub[u] = 1;

    for(int v : adj[u])
    {
        if(v == p || removed[v]) continue;

        dfs_size(v,u);
        sub[u] += sub[v];
    }
}

int find_centroid(int u,int p,int total)
{
    for(int v : adj[u])
    {
        if(v == p || removed[v]) continue;

        if(sub[v] > total/2)
            return find_centroid(v,u,total);
    }

    return u;
}

void dfs_dist(int u,int p,int d,int centroid)
{
    pathCentroid[u].push_back({centroid,d});

    for(int v : adj[u])
    {
        if(v == p || removed[v]) continue;

        dfs_dist(v,u,d+1,centroid);
    }
}

void build(int entry)
{
    dfs_size(entry,-1);

    int centroid =
        find_centroid(entry,-1,sub[entry]);

    removed[centroid] = true;

    dfs_dist(centroid,-1,0,centroid);

    for(int v : adj[centroid])
    {
        if(!removed[v])
            build(v);
    }
}

void update(int u)
{
    for(auto [c,d] : pathCentroid[u])
    {
        best[c] = min(best[c], d);
    }
}

int query(int u)
{
    int ans = INF;

    for(auto [c,d] : pathCentroid[u])
    {
        ans = min(ans,
                  best[c] + d);
    }

    return ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i=0;i<n-1;i++)
    {
        int u,v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    build(1);

    for(int i=1;i<=n;i++)
        best[i] = INF;

    
    update(1);

    while(q--)
    {
        int type,v;
        cin >> type >> v;

        if(type == 1)
        {
            update(v);
        }
        else
        {
            cout << query(v) << '\n';
        }
    }
}
