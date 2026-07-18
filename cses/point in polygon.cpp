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
 
int cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
} 
 
 
bool on_segment(Point a, Point b, Point p) {
    if (cross_product(a, b, p) == 0) {
        if (p.x >= min(a.x, b.x) && p.x <= max(a.x, b.x) &&
            p.y >= min(a.y, b.y) && p.y <= max(a.y, b.y)) {
            return true;
        }
    }
    return false;
}
 
void solve() {
    
    int n,m;
    cin >> n >> m;
    vector<Point> ara(n);
 
    for(ll i=0;i<n;i++){
          cin >> ara[i].x >> ara[i].y;
    }
 
    while(m--){
 
         Point p;
         cin >> p.x >> p.y;
 
         
         ll cnt=0;
         ll tr=0;
 
         for(ll i=0;i<n;i++){
            Point a=ara[i];
            Point b=ara[(i+1)%n];
            
            if(a.y>b.y){
                  swap(a,b);
            }
 
            if(on_segment(a,b,p)){
                    tr=1;
                    break;
                }
            if(cross_product(a,b,p)>0 && a.y<=p.y && b.y>p.y){
                cnt++;
            }
         }
         
         if(tr){
              cout << "BOUNDARY" << endl;
 
         }
         else if(cnt%2==1){
              cout << "INSIDE" << endl;
         }
         else{
              cout << "OUTSIDE" << endl;
         }
         
 
    }
 
 
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
}
