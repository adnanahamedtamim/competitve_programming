#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
using ll = long long;
using vll = vector<ll>;

const double PI = acos(-1);

void fft(vector<cd> &a, bool invert) {
    int n = (int)a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;

        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }

        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));

        for (int i = 0; i < n; i += len) {
            cd w(1);

            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;

                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;

                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd &x : a)
            x /= n;
    }
}

vll multiply(vll &a, vll &b) {
    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());

    int n = 1;

    while (n < (int)a.size() + (int)b.size())
        n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    vll res(n);

    for (int i = 0; i < n; i++)
        res[i] = llround(fa[i].real());

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    ll n = s.size();

    string su = "abc";

    vector<ll> overall(n, 0);

    for (auto ch : su) {

        vector<ll> a(n, 0), b(n, 0);

        for (ll i = 0; i < n; i++) {
            if (s[i] == ch) {
                a[i] = 1;
                b[n - 1 - i] = 1;
            }
        }

        vector<ll> ans = multiply(a, b);

        for (ll j = 1; j < n; j++) {
            overall[j] += ans[n - 1 + j];
        }
    }

    ll maxi = -1;

    for (ll i = 1; i < n; i++) {
        if (maxi == -1 || overall[i] > overall[maxi]) {
            maxi = i;
        }
    }

    cout << overall[maxi] << endl;

    for (ll i = 1; i < n; i++) {
        if (overall[i] == overall[maxi]) {
            cout << i << " ";
        }
    }

    cout << endl;

    return 0;
}
