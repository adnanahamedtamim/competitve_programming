#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
const double PI = acos(-1);

void fft(vector<cd> &a, bool invert) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
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

vector<long long> multiply(vector<int> &a, vector<int> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());

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

    vector<long long> res(n);
    for (int i = 0; i < n; i++)
        res[i] = (long long)round(fa[i].real());

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, r;
    cin >> s >> r;

    int n = s.size(), m = r.size();

    string dna = "ACGT";
    vector<int> best(n, 0);

    for (char c : dna) {
        vector<int> A(n, 0), B(m, 0);

        for (int i = 0; i < n; i++)
            if (s[i] == c) A[i] = 1;

        for (int i = 0; i < m; i++)
            if (r[i] == c) B[m - i - 1] = 1; // reverse pattern

        vector<long long> conv = multiply(A, B);

        for (int i = m - 1; i < n; i++) {
            best[i] += conv[i];
        }
    }

    int ans = m;
    for (int i = m - 1; i < n; i++) {
        ans = min(ans, m - best[i]);
    }

    cout << ans << "\n";
}
