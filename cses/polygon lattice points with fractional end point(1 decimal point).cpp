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
        double x, y;
        cin >> x >> y;
        p[i].first = round(x * 10);
        p[i].second = round(y * 10);
    }
    
    int b = 0;
    double area = 0;
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int X1 = p[i].first, Y1 = p[i].second;
        int X2 = p[j].first, Y2 = p[j].second;
        
        area += (double)X1 * Y2 - (double)Y1 * X2;
        
        int dX = X2 - X1;
        int dY = Y2 - Y1;
        int g = gcd(abs(dX), abs(dY));
        
        if (g > 0) {
            int u = dX / g;
            int v = dY / g;
            int P = 10 / gcd(abs(u), gcd(abs(v), (int)10));
            
            int k_start = -1;
            for (int k = 0; k < P; k++) {
                if ((X1 + k * u) % 10 == 0 && (Y1 + k * v) % 10 == 0) {
                    k_start = k;
                    break;
                }
            }
            
            if (k_start != -1 && k_start < g) {
                b += (g - k_start + P - 1) / P;
            }
        }
    }
    
    area = abs(area) / 200.0;
    
    int in = 0;
    int minY = p[0].second;
    int maxY = p[0].second;
    for (int i = 1; i < n; i++) {
        minY = min(minY, p[i].second);
        maxY = max(maxY, p[i].second);
    }
    
    int L_y = ceil(minY / 10.0);
    int R_y = floor(maxY / 10.0);
    
    for (int y = L_y * 10; y <= R_y * 10; y += 10) {
        vector<double> inter;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            int Y1 = p[i].second, Y2 = p[j].second;
            int X1 = p[i].first,  X2 = p[j].first;
            
            if (Y1 > Y2) {
                swap(Y1, Y2);
                swap(X1, X2);
            }
            
            if (y >= Y1 && y < Y2) {
                double x = X1 + (double)(y - Y1) * (X2 - X1) / (Y2 - Y1);
                inter.push_back(x);
            }
        }
        
        sort(inter.begin(), inter.end());
        
        for (int i = 0; i + 1 < (int)inter.size(); i += 2) {
            double x1 = inter[i];
            double x2 = inter[i+1];
            
            int L = floor(x1 / 10.0 + 1e-9) + 1;
            int leftX = L * 10;
            
            int R = ceil(x2 / 10.0 - 1e-9) - 1;
            int rightX = R * 10;
            
            if (leftX <= rightX) {
                in += (rightX - leftX) / 10 + 1;
            }
        }
    }
    
    cout << fixed << setprecision(2) << "Area: " << area << "\n";
    cout << "Inside Points: " << in << "\n";
    cout << "Boundary Points: " << b << "\n";
    
    return 0;
}
