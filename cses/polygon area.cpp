#include <bits/stdc++.h>
using namespace std;
 
#define int long long
#define ll long long
 
struct Point {
    int x, y;
};
 
int sign(int val) {
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}
 
int cross(Point a, Point b) {
    return a.x*b.y-a.y*b.x;
}
 
bool onSegment(Point p, Point a, Point b) {
    return p.x >= min(a.x, b.x) && p.x <= max(a.x, b.x) &&
           p.y >= min(a.y, b.y) && p.y <= max(a.y, b.y);
}
 
void solve() {
    
    int n;
    cin >> n;
    vector<Point> ara(n);
 
    for(ll i=0;i<n;i++){
          cin >> ara[i].x >> ara[i].y;
    }
 
    ll ans=0;
 
    for(ll i=0;i<n;i++){
 
        ans+=cross(ara[i],ara[(i+1)%n]);
    }
    cout << abs(ans) << endl;
    return;
 
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t=1;
    // cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
