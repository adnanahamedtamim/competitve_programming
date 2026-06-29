#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;
const ll BASE = 911382323;

struct Hash {
    int n;
    vector<ll> h, pw;

    Hash(string s) {
        n = s.size();
        h.assign(n + 1, 0);
        pw.assign(n + 1, 1);

        for (int i = 0; i < n; i++) {
            h[i + 1] = (h[i] * BASE + s[i]) % MOD;
            pw[i + 1] = (pw[i] * BASE) % MOD;
        }
    }

    ll get(int l, int r) { // inclusive
        ll res = h[r + 1] - h[l] * pw[r - l + 1] % MOD;
        if (res < 0) res += MOD;
        return res;
    }
};

string s, revs;
Hash *H1, *H2;

bool check(int len, int &pos) {
    int n = s.size();

    for (int i = 0; i + len - 1 < n; i++) {
        int j = i + len - 1;

        ll h1 = H1->get(i, j);

        int rl = n - 1 - j;
        int rr = n - 1 - i;

        ll h2 = H2->get(rl, rr);

        if (h1 == h2) {
            pos = i;
            return true;
        }
    }
    return false;
}

int main() {
    cin >> s;

    revs = s;
    reverse(revs.begin(), revs.end());

    H1 = new Hash(s);
    H2 = new Hash(revs);

    int n = s.size();

    int bestLen = 1;
    int bestPos = 0;

    // ---------- odd ----------
    int lo = 1, hi = (n % 2 ? n : n - 1);

    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (mid % 2 == 0) mid--;

        if (mid < lo) {
            lo++;
            continue;
        }

        int pos;

        if (check(mid, pos)) {
            if (mid > bestLen) {
                bestLen = mid;
                bestPos = pos;
            }
            lo = mid + 2;
        } else
            hi = mid - 2;
    }

    // ---------- even ----------
    lo = 2;
    hi = (n % 2 == 0 ? n : n - 1);

    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (mid & 1) mid--;

        if (mid < lo) {
            lo++;
            continue;
        }

        int pos;

        if (check(mid, pos)) {
            if (mid > bestLen) {
                bestLen = mid;
                bestPos = pos;
            }
            lo = mid + 2;
        } else
            hi = mid - 2;
    }

    cout << s.substr(bestPos, bestLen);
}
