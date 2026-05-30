#include <bits/stdc++.h>
#include <unordered_map>
#include <chrono>

using namespace std;

typedef long long ll;
typedef long double ld;

#define pb push_back
#define all(x) x.begin(), x.end()
#define endl '\n'
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl

#define isSet(x, i) ((x >> i) & 1)
#define setbit(x, i) (x | (1LL << i))
#define resetbit(x, i) (x & (~(1LL << i)))
#define toggleBit(x, i) ((x) ^ (1LL << (i)))
#define clz(x) __builtin_clzll(x)
#define ctz(x) __builtin_ctzll(x)
#define csb(x) __builtin_popcountll(x)
#define msb(x) (ll)((x) ? (63 - __builtin_clzll((ll)(x))) : -1)
#define lsb(x) (ll)((x) ? (__builtin_ctzll((ll)(x))) : -1)

typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<int>> vvi;
typedef pair<ll, ll> pll;
typedef vector<bool> vbool;

/*======================
        CONSTANTS
=======================*/

const ll MOD = 1000000007;
const ld PI = acos(-1.0L);
const ld EPS = 1e-12;

const int MAXN = 2e5 + 5;
const int MAXSIEVE = 2e5 + 5;

/*======================
        FAST HASH
=======================*/

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }

    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& p) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash<T1>()(p.first)) ^
               splitmix64(hash<T2>()(p.second) + FIXED_RANDOM);
    }

    size_t operator()(const vector<ll>& v) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h = v.size();
        for (auto &x : v)
            h ^= splitmix64(x + FIXED_RANDOM);
        return h;
    }

    size_t operator()(const string &s) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash<string>()(s) + FIXED_RANDOM);
    }
};

template <class K, class V>
using umap = unordered_map<K, V, custom_hash>;

/*======================
    MODULAR ARITHMETIC
=======================*/

ll binexp(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = (res * a);
        a = (a * a);
        b >>= 1;
    }
    return res;
}

ll modinv(ll a) {
    return binexp(a, MOD - 2);
}

/*======================
    COMBINATORICS
=======================*/

ll fact[MAXN];
ll invfact[MAXN];

void build_factorial() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++)
        fact[i] = (fact[i - 1] * i) % MOD;
    invfact[MAXN - 1] = modinv(fact[MAXN - 1]);
    for (int i = MAXN - 2; i >= 0; i--)
        invfact[i] = (invfact[i + 1] * (i + 1)) % MOD;
}

ll ncr(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
}

/*======================
    NUMBER THEORY
=======================*/

ll gcd_ll(ll a, ll b) {
    if (b == 0) return a;
    return gcd_ll(b, a % b);
}

ll lcm_ll(ll a, ll b) {
    return (a / gcd_ll(a, b)) * b;
}

/*======================
    SIEVE + FACTORS
=======================*/

ll spf[MAXSIEVE];

void build_spf() {
    for (int i = 1; i < MAXSIEVE; i++) spf[i] = i;
    for (int i = 2; i * i < MAXSIEVE; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j < MAXSIEVE; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

vector<pair<ll, ll>> prime_factorization(ll x) {
    vector<pair<ll, ll>> factors;
    while (x > 1) {
        ll prime = spf[x];
        ll cnt = 0;
        while (x % prime == 0) {
            x /= prime;
            cnt++;
        }
        factors.pb({prime, cnt});
    }
    return factors;
}

vector<ll> getalldivisors_sorted(ll n) {
    vector<ll> dib;
    for (ll i = 1; i * i <= n; i++) {
        if ((n % i == 0)) {
            dib.pb(i);
            if (i * i != n) {
                dib.pb(n / i);
            }
        }
    }
    sort(dib.begin(), dib.end());
    return dib;
}

/*======================
    BIT TRICK UTILITY
=======================*/

long long sum_floor_log2(long long n) {
    long long ans = 0;
    long long power = 1;
    long long k = 0;
    while ((power << 1LL) <= n) {
        ans = (ans + k % MOD * power % MOD) % MOD;
        power <<= 1LL;
        k++;
    }
    long long cnt = (n - power + 1) % MOD;
    ans = (ans + k % MOD * cnt % MOD) % MOD;
    return ans;
}

/*======================
    3D GEOMETRY
=======================*/

struct vec {
    ld x, y, z;
};

vec operator+(vec a, vec b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
vec operator-(vec a, vec b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
vec operator/(vec a, ld d) { return {a.x / d, a.y / d, a.z / d}; }
ld dot(vec a, vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
ld norm(vec a) { return sqrt(dot(a, a)); }

bool small(string a, string b) {
    ll n = a.size();
    ll m = b.size();
    ll i = 0;
    ll j = 0;
    while (i < n && j < m) {
        if (a[i] < b[j]) return true;
        if (a[i] > b[j]) return false;
        i++;
        j++;
    }
    if (i == n && j == m) return true;
    if (j < m) return true;
    return false;
}

ll lcm(ll a, ll b, ll m) {
    if (a == 0 || b == 0) return m + 1;
    ll g = gcd_ll(a, b);
    ll res = a / g;
    if (res > m / b) return m + 1;
    return res * b;
}

ll lcmwithoutmod(ll a, ll b) {
    if (a == 0) return b;
    if (b == 0) return a;
    ll g = gcd_ll(a, b);
    ll res = a / g;
    return res * b;
}

bool func(ll i, ll a, ll b, ll c, ll d, vector<pair<ll, ll>> &ara, ll &x, ll &y) {
    if (i == ara.size()) {
        ll ac = ((a / x) + 1) * x;
        ll bd = ((b / y) + 1) * y;

        if ((ac > a && ac <= c) && (bd > b && bd <= d)) {
            x = ac;
            y = bd;
            return true;
        }
        return false;
    }

    auto it = ara[i];
    ll val = it.first;
    ll cnt = it.second;

    bool tr = false;
    ll p = binexp(val, cnt);
    ll aa = 1;

    for (ll j = 0; j <= cnt; j++) {
        x = x * aa;
        y = y * (p / aa);
        tr |= func(i + 1, a, b, c, d, ara, x, y);
        if (tr) {
            break;
        }
        x /= aa;
        y /= (p / aa);
        aa *= val;
    }

    return tr;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll t;
    cin >> t;

    while (t--) {
        ll a, b, c, d;
        cin >> a >> b >> c >> d;

        umap<ll, ll> mp;
        ll aa = a;
        ll bb = b;

        for (ll i = 2; i * i <= a; i++) {
            while (a % i == 0) {
                mp[i]++;
                a /= i;
            }
        }
        if (a > 1) {
            mp[a]++;
        }
        for (ll i = 2; i * i <= b; i++) {
            while (b % i == 0) {
                mp[i]++;
                b /= i;
            }
        }
        if (b > 1) {
            mp[b]++;
        }

        vector<pair<ll, ll>> ara;
        for (auto it : mp) {
            ara.pb({it.first, it.second});
        }

        ll x = 1, y = 1;
        bool ans = func(0, aa, bb, c, d, ara, x, y);

        if (ans) {
            cout << x << " " << y << endl;
        } else {
            cout << "-1 -1" << endl;
        }
    }
    return 0;
}
