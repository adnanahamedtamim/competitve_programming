#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Optimized Complex struct
struct cd {
    double r, i;
    cd(double r = 0, double i = 0) : r(r), i(i) {}
    inline cd operator+(const cd& o) const { return {r + o.r, i + o.i}; }
    inline cd operator-(const cd& o) const { return {r - o.r, i - o.i}; }
    inline cd operator*(const cd& o) const { return {r * o.r - i * o.i, r * o.i + i * o.r}; }
    inline void operator/=(double d) { r /= d; i /= d; }
};

const double PI = acos(-1.0);
vector<cd> roots;
vector<int> rev;

// Call this ONCE with the target power of 2 (e.g., 2^18 for N=10^5)
void prepare_fft(int n) {
    rev.assign(n, 0);
    for (int i = 0; i < n; i++) {
        int r = 0;
        for (int j = 0; (1 << j) < n; j++) {
            if (i & (1 << j)) r |= (n >> (j + 1));
        }
        rev[i] = r;
    }
    roots.assign(n, {0, 0});
    for (int i = 0; i < n; i++) {
        double ang = 2.0 * PI * i / n;
        roots[i] = cd(cos(ang), sin(ang));
    }
}

// Internal FFT logic
void fft(vector<cd>& a, bool invert) {
    int n = a.size();
    for (int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);

    for (int len = 2; len <= n; len <<= 1) {
        int step = n / len;
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < len / 2; j++) {
                cd w = roots[step * j];
                if (invert) w.i = -w.i;
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
        }
    }
    if (invert) for (auto& x : a) x /= n;
}

// The clean interface you requested
vector<ll> multiply(const vector<ll>& a, const vector<ll>& b) {
    int n = 1;
    while (n < a.size() + b.size()) n <<= 1;
    
    // Resize or prepare if n changed unexpectedly
    if (rev.size() != n) prepare_fft(n);

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < a.size(); i++) fa[i] = cd(a[i], 0);
    for (int i = 0; i < b.size(); i++) fb[i] = cd(b[i], 0);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i];
    fft(fa, true);

    vector<ll> result(n);
    for (int i = 0; i < n; i++) result[i] = llround(fa[i].r);
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.size();

    // 1. Calculate required size and prepare FFT once
    int m = 1;
    while (m < 2 * n) m <<= 1;
    prepare_fft(m);

    // 2. Use the multiply function easily
    vector<ll> overall(n, 0);
    char alphabet[] = {'a', 'b', 'c'};

    for (char ch : alphabet) {
        vector<ll> a(n, 0), b(n, 0);
        for (int i = 0; i < n; i++) {
            if (s[i] == ch) {
                a[i] = 1;
                b[n - 1 - i] = 1; // Reversed for matching
            }
        }

        vector<ll> res = multiply(a, b);

        for (int k = 1; k < n; k++) {
            overall[k] += res[n - 1 - k]; 
        }
    }

    // 3. Find max and output all indices
    ll max_val = 0;
    for (int k = 1; k < n; k++) max_val = max(max_val, overall[k]);

    cout << max_val << "\n";
   

    return 0;
}
