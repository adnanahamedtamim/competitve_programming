#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
using u128 = __uint128_t;

// (a * b) % mod without overflow
ull mod_mul(ull a, ull b, ull mod) {
    return (u128)a * b % mod;
}

// (a^e) % mod
ull mod_pow(ull a, ull e, ull mod) {
    ull res = 1;

    while (e) {
        if (e & 1)
            res = mod_mul(res, a, mod);

        a = mod_mul(a, a, mod);
        e >>= 1;
    }

    return res;
}

bool isPrime(ull n) {

    if (n < 2)
        return false;

    if (n % 2 == 0)
        return n == 2;

    ull d = n - 1;
    int r = 0;

    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    const ull bases[] = {
        2,
        325,
        9375,
        28178,
        450775,
        9780504,
        1795265022
    };

    for (ull a : bases) {

        if (a % n == 0)
            continue;

        ull x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;

        for (int i = 1; i < r; i++) {

            x = mod_mul(x, x, n);

            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;
    }

    return true;
}
