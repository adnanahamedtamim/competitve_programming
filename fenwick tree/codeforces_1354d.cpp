#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void update(ll i, ll delta, ll n, vector<ll>& bit) {
    for (; i <= n; i += i & -i)
        bit[i] += delta;
}

ll query(ll i, vector<ll>& bit) {
    ll sum = 0;
    for (; i > 0; i -= i & -i)
        sum += bit[i];
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, q;
    cin >> n >> q;

    vector<ll> bit(n + 1, 0);
    vector<ll> freq(n + 1, 0);

    for (ll i = 0; i < n; i++) {
        ll x;
        cin >> x;
        freq[x]++;
        update(x, 1, n, bit);
    }

    while (q--) {
        ll x;
        cin >> x;
        if (x > 0) {
            freq[x]++;
            update(x, 1, n, bit);
        } else {
            x = abs(x);
            ll l = 1, r = n, ans = 1;
            while (l <= r) {
                ll mid = l + (r - l) / 2;
                if (query(mid, bit) >= x) {
                    ans = mid;
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            }
            freq[ans]--;
            update(ans, -1, n, bit);
        }
    }

    ll ans = 0;
    for (ll i = 1; i <= n; i++) {
        if (freq[i] > 0) {
            ans = i;
            break;
        }
    }

    cout << ans << endl;

    return 0;
}
