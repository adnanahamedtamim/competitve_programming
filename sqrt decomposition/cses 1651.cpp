#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define pb push_back
#define MOD 998244353

vector<ll> cnt(100010, 0);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

    vector<ll> ara(n);

    ll block = (ll)sqrt(n) + 1;

    vector<ll> b(block + 1, 0);

    for (ll i = 0; i < n; i++) {
        cin >> ara[i];
        // b[i / block] += ara[i];
    }

    while (q--) {
        ll t;
        cin >> t;

        if (t == 1) {
            ll l, r, u;
            cin >> l >> r >> u;

            l--;
            r--;

            ll lb = l / block;
            ll rb = r / block;

            if (lb == rb) {
                for (ll i = l; i <= r; i++) {
                    ara[i] += u;
                }
            }
            else {
                for (ll i = l; i < (lb + 1) * block; i++) {
                    ara[i] += u;
                }

                for (ll i = lb + 1; i < rb; i++) {
                    b[i] += u;
                }

                for (ll i = rb * block; i <= r; i++) {
                    ara[i] += u;
                }
            }
        }
        else {
            ll k;
            cin >> k;

            k--;

            cout << ara[k] + b[k / block] << '\n';
        }
    }

    return 0;
}
