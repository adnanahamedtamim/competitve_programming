#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll,ll> pll;
typedef vector<pll> vpair;

ll orientation(pll a, pll b, pll c)
{
    ll val = (b.first - a.first) * (c.second - a.second)
           - (b.second - a.second) * (c.first - a.first);

    if(val == 0) return 0;
    if(val > 0) return 1;      // Counter-clockwise
    return -1;                 // Clockwise
}

ll distSq(pll a, pll b)
{
    ll dx = a.first - b.first;
    ll dy = a.second - b.second;
    return dx * dx + dy * dy;
}

vpair convexHull(vpair &p)
{
    ll n = p.size();

    if(n < 3) return p;

    ll left = 0;
    for(ll i = 1; i < n; i++)
    {
        if(p[i].first < p[left].first)
            left = i;
        else if(p[i].first == p[left].first &&
                p[i].second < p[left].second)
            left = i;
    }

    vpair hull;

    ll cur = left;

    do
    {
        hull.push_back(p[cur]);

        ll nxt = (cur + 1) % n;

        for(ll i = 0; i < n; i++)
        {
            ll o = orientation(p[cur], p[nxt], p[i]);

            if(o == 1)
            {
                nxt = i;
            }
            else if(o == 0)
            {
                if(distSq(p[cur], p[i]) > distSq(p[cur], p[nxt]))
                    nxt = i;
            }
        }

        cur = nxt;

    } while(cur != left);

    return hull;
}

int main()
{
    ll n;
    cin >> n;

    vpair points(n);

    for(ll i = 0; i < n; i++)
        cin >> points[i].first >> points[i].second;

    vpair hull = convexHull(points);

    cout << hull.size() << endl;

    for(auto pt : hull)
        cout << pt.first << " " << pt.second << endl;

    return 0;
}
