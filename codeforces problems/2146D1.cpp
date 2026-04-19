#include<bits/stdc++.h>

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
#define isSet(x, i)((x >> i) & 1)
#define setbit(x, i)(x | (1 LL << i))
#define resetbit(x, i)(x & (~(1 LL << i)))
#define toggleBit(x, i)((x) ^ (1 LL << (i)))
#define clz(x) __builtin_clzll(x)
#define ctz(x) __builtin_ctzll(x)
#define csb(x) __builtin_popcountll(x)
#define msb(x)(ll)((x) ? (63 - __builtin_clzll((ll)(x))) : -1)
#define lsb(x)(ll)((x) ? (__builtin_ctzll((ll)(x))) : -1)
typedef long long ll;
typedef vector < ll > vll;
typedef vector < int > vi;
typedef vector < vector < ll >> vvll;
typedef vector < vector < int >> vvi;
/*======================
        CONSTANTS
=======================*/
const ll MOD = 998244353;
const ld PI = acos(-1.0 L);
const ld EPS = 1e-12;
const int MAXN = 2e5 + 5;
const int MAXSIEVE = 2e5 + 5;
/*======================
        FAST HASH
=======================*/
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15 ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9 ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb ULL;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static
        const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
    template < typename T1, typename T2 > size_t operator()(const pair < T1, T2 > & p) const {
        static
        const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash < T1 > ()(p.first)) ^ splitmix64(hash < T2 > ()(p.second) + FIXED_RANDOM);
    }
    size_t operator()(const vector < ll > & v) const {
        static
        const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h = v.size();
        for (auto & x: v) h ^= splitmix64(x + FIXED_RANDOM);
        return h;
    }
    size_t operator()(const string & s) const {
        static
        const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash < string > ()(s) + FIXED_RANDOM);
    }
};
template < class K, class V > using umap = unordered_map < K, V, custom_hash > ;
/*======================
    MODULAR ARITHMETIC
=======================*/
ll binexp(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while (b) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
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
    for (int i = 1; i < MAXN; i++) fact[i] = (fact[i - 1] * i) % MOD;
    invfact[MAXN - 1] = modinv(fact[MAXN - 1]);
    for (int i = MAXN - 2; i >= 0; i--) invfact[i] = (invfact[i + 1] * (i + 1)) % MOD;
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
vector < pair < ll, ll >> prime_factorization(ll x) {
    vector < pair < ll, ll >> factors;
    while (x > 1) {
        ll prime = spf[x];
        ll cnt = 0;
        while (x % prime == 0) {
            x /= prime;
            cnt++;
        }
        factors.pb({
            prime,
            cnt
        });
    }
    return factors;
}
vector < ll > getalldivisors_sorted(ll n) {
    vector < ll > dib;
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
        INPUT
=======================*/
void get_vector0based(vector < ll > & v) {
    for (ll i = 0; i < v.size(); i++) cin >> v[i];
}
void get_vector1based(vector < ll > & v) {
    for (ll i = 1; i < v.size(); i++) cin >> v[i];
}
/*======================
    BIT TRICK UTILITY
=======================*/
long long sum_floor_log2(long long n) {
    long long ans = 0;
    long long power = 1;
    long long k = 0;
    while ((power << 1 LL) <= n) {
        ans = (ans + k % MOD * power % MOD) % MOD;
        power <<= 1 LL;
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
vec operator + (vec a, vec b) {
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}
vec operator - (vec a, vec b) {
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}
vec operator / (vec a, ld d) {
    return {
        a.x / d,
        a.y / d,
        a.z / d
    };
}
ld dot(vec a, vec b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
ld norm(vec a) {
    return sqrt(dot(a, a));
}
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
vector < ll > primes;
void sieve() {
    const ll N = 31623;
    vector < bool > is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (ll i = 2; i * i <= N; i++) {
        if (is_prime[i]) {
            for (ll j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }
    for (ll i = 2; i <= N; i++) {
        if (is_prime[i]) primes.push_back(i);
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t;
    cin >> t;
    while (t--) {
        ll l, r;
        cin >> l >> r;
        vll ans(r + 1, -1);
        ll ansval = 0;
        for (ll i = r; i >= 0; i--) {
            if (ans[i] != -1) continue;
            ll on = 0;
            ll num = 0;
            for (ll j = 40; j >= 0; j--) {
                if ((i & (1 LL << j)) > 0) {
                    on = 1;
                }
                if ((i & (1 LL << j)) == 0 && on) {
                    num |= (1 LL << j);
                }
            }
            if (ans[num] == -1) {
                ans[i] = num;
                ans[num] = i;
            }
        }
        for (ll i = 0; i <= r; i++) {
            ansval += (ans[i] | i);
        }
        cout << ansval << endl;
        for (auto it: ans) {
            cout << it << " ";
        }
        cout << endl;
    }
    return 0;
}
