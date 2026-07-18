#include <bits/stdc++.h>
using namespace std;

#define int long long

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<pair<int, int>> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i].first >> p[i].second;
    }
    
    int b = 0;
    int area2 = 0;
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        
        b += gcd(abs(p[i].first - p[j].first), abs(p[i].second - p[j].second));
        
        area2 += p[i].first * p[j].second - p[i].second * p[j].first;
    }
    
    area2 = abs(area2);
    int in = (area2 - b + 2) / 2;
    
    cout << in << " " << b << "\n";
    
    return 0;
}
