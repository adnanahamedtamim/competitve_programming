#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;

#define pb push_back
#define endl '\n'

const ll MOD = 998244353;
const int MAXN = 2e5 + 5;
const int MAXSIEVE = 2e5 + 5;

/* FAST HASH */

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }

    size_t operator()(const vector<ll>& v) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();

        uint64_t h = v.size();
        for (auto &x : v)
            h ^= splitmix64(x + FIXED_RANDOM);

        return h;
    }
};

template <class K, class V>
using umap = unordered_map<K, V, custom_hash>;

/* UTILITIES */

ll gcd_ll(ll a, ll b) {
    return b == 0 ? a : gcd_ll(b, a % b);
}

void get_vector0based(vll &v) {
    for (ll i = 0; i < v.size(); i++)
        cin >> v[i];
}

/* MAIN */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll t;
    cin >> t;

    ll maxi = 1e6 + 2;
    vector<vector<ll>> cnt(maxi);
    vll sieve(maxi + 1, 0);

    for (ll i = 1; i <= maxi; i++) {
        cnt[i].pb(-1);
    }

    for (ll i = 2; i <= maxi; i++) {
        if (sieve[i] == 1) continue;

        for (ll j = i; j <= maxi; j += i) {
            sieve[j] = 1;

            ll x = j, cntt = 0;
            while (x % i == 0) {
                cntt++;
                x /= i;
            }

            if (cntt % 2 == 1) {
                cnt[j].pb(i);
            }
        }
    }

    while (t--) {
        ll n;
        cin >> n;

        vll ara(n);
        get_vector0based(ara);

        umap<vll, ll> mp;

        for (auto it : ara) {
            mp[cnt[it]]++;
        }

        ll ans1 = 0;
        for (auto it : mp) {
            ans1 = max(ans1, it.second);
        }

        ll ans2 = 0, even = 0;

        for (auto it : mp) {
            if (it.second % 2 == 0 || it.first.size() == 1) {
                even += it.second;
            } else {
                ans2 = max(ans2, it.second);
            }
        }

        ans2 = max(ans2, even);

        ll q;
        cin >> q;

        while (q--) {
            ll w;
            cin >> w;

            if (w == 0) {
                cout << ans1 << endl;
            } else {
                cout << ans2 << endl;
            }
        }
    }

    return 0;
}
