/*
problem link : https://www.spoj.com/problems/ADAMATCH/
 */ 

#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
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

vector<long long> multiply(vector<long long> &a, vector<long long> &b) {
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

    vector<long long> res(n);

    for (int i = 0; i < n; i++)
        res[i] = llround(fa[i].real());

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, r;
    cin >> s >> r;

    int n = (int)s.size();
    int m = (int)r.size();

    vector<long long> matches(n + m + 5, 0);

    string dna = "ACGT";

    for (char ch : dna) {
        vector<long long> A(n, 0);
        vector<long long> B(m, 0);

        for (int i = 0; i < n; i++)
            if (s[i] == ch)
                A[i] = 1;

        for (int i = 0; i < m; i++)
            if (r[i] == ch)
                B[m - 1 - i] = 1; // reverse pattern

        vector<long long> conv = multiply(A, B);

        for (int i = 0; i < (int)conv.size(); i++)
            matches[i] += conv[i];
    }

    int answer = m;

    for (int start = 0; start <= n - m; start++) {
        int pos = start + m - 1;

        long long matched = matches[pos];

        answer = min(answer, m - (int)matched);
    }

    cout << answer << '\n';

    return 0;
}
