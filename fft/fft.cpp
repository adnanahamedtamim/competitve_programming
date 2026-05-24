/*
 * Problem Name : POLYMUL - Polynomial Multiplication
 * Problem Link : https://www.spoj.com/problems/POLYMUL/
 */

#include <bits/stdc++.h>
using namespace std;

#define ll long long

using cd = complex<double>;
const double PI = acos(-1);

vector<cd> fft(vector<cd> p, ll n) {
    if (n == 1) return p;

    vector<cd> Pe, Po;

    for (ll i = 0; 2 * i < n; i++) {
        Pe.push_back(p[2 * i]);
        Po.push_back(p[2 * i + 1]);
    }

    Pe = fft(Pe, n / 2);
    Po = fft(Po, n / 2);

    double ang = (2 * PI) / n;

    cd w(1);
    cd w_delta(cos(ang), sin(ang));

    for (ll i = 0; i < n / 2; i++) {
        p[i] = Pe[i] + w * Po[i];
        p[i + n / 2] = Pe[i] - w * Po[i];

        w *= w_delta;
    }

    return p;
}

vector<cd> inversefft(vector<cd> p, ll n) {
    if (n == 1) return p;

    vector<cd> Pe, Po;

    for (ll i = 0; 2 * i < n; i++) {
        Pe.push_back(p[2 * i]);
        Po.push_back(p[2 * i + 1]);
    }

    Pe = inversefft(Pe, n / 2);
    Po = inversefft(Po, n / 2);

    double ang = (-2 * PI) / n;

    cd w(1);
    cd w_delta(cos(ang), sin(ang));

    for (ll i = 0; i < n / 2; i++) {
        p[i] = Pe[i] + w * Po[i];
        p[i + n / 2] = Pe[i] - w * Po[i];

        w *= w_delta;
    }

    return p;
}

vector<ll> multiply(vector<ll>& a, vector<ll>& b) {
    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());

    ll n = 1;
    while (n < (ll)a.size() + (ll)b.size())
        n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fa = fft(fa, n);
    fb = fft(fb, n);

    for (ll i = 0; i < n; i++)
        fa[i] *= fb[i];

    fa = inversefft(fa, n);

    vector<ll> res(n);

    for (ll i = 0; i < n; i++)
        res[i] = llround(fa[i].real() / n);

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll t;
    cin >> t;

    while (t--) {
        ll n;
        cin >> n;

        vector<ll> a(n + 1), b(n + 1);

        for (ll i = 0; i <= n; i++)
            cin >> a[i];

        for (ll i = 0; i <= n; i++)
            cin >> b[i];

        vector<ll> res = multiply(a, b);

        for (ll i = 0; i <= 2 * n; i++)
            cout << res[i] << " ";

        cout << '\n';
    }

    return 0;
}
