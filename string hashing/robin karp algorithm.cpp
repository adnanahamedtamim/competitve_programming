#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;        // Large prime modulus
const ll P = 31;               // Base for polynomial hash (small prime)

// Precompute powers of P modulo MOD
vector<ll> p_pow;

// Precompute prefix hashes of string s
vector<ll> prefix_hash(const string &s) {
    int n = (int)s.size();
    vector<ll> h(n + 1, 0);  // h[0] = 0
    for (int i = 0; i < n; i++) {
        h[i + 1] = (h[i] + (s[i] - 'a' + 1) * p_pow[i]) % MOD;
    }
    return h;
}

// Compute hash of substring s[l..r] using prefix hashes
// Note: returns hash multiplied by p^l (not normalized)
ll substring_hash(const vector<ll> &h, int l, int r) {
    ll res = h[r + 1] - h[l];
    if (res < 0) res += MOD;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text, pattern;
    cin >> text >> pattern;

    int n = (int)text.size();
    int m = (int)pattern.size();

    // Precompute powers up to max length
    int max_len = max(n, m);
    p_pow.resize(max_len + 1, 1);
    for (int i = 1; i <= max_len; i++) {
        p_pow[i] = (p_pow[i - 1] * P) % MOD;
    }

    // Precompute prefix hashes
    vector<ll> h_text = prefix_hash(text);
    vector<ll> h_pattern = prefix_hash(pattern);

    // Hash of full pattern (already multiplied by p^0 = 1)
    ll pattern_hash = substring_hash(h_pattern, 0, m - 1);

    vector<int> occurrences;

    for (int i = 0; i + m - 1 < n; i++) {
        // Get hash of text substring [i, i+m-1]
        ll cur_hash = substring_hash(h_text, i, i + m - 1);

        // Align powers: multiply smaller by p^(difference)
        if (i > 0) {
            // multiply pattern_hash by p^i to match cur_hash's power shift
            ll aligned_pattern_hash = (pattern_hash * p_pow[i]) % MOD;
            if (cur_hash == aligned_pattern_hash) {
                occurrences.push_back(i);
            }
        } else {
            // i == 0, powers already aligned
            if (cur_hash == pattern_hash) {
                occurrences.push_back(i);
            }
        }
    }

    // Output all occurrence positions
    for (int pos : occurrences) {
        cout << pos << " ";
    }
    cout << "\n";

    return 0;
}
