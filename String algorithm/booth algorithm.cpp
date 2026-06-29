#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll booth(string s) {  // booth's algorithm 0(n)
// returns the starting index p of the lexicographically smallest substring of a string s 
    s += s;
    ll n = s.size() / 2;

    ll i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        if (s[i + k] == s[j + k]) {
            k++;
        } else if (s[i + k] < s[j + k]) {
            j += k + 1;
            if (i == j) j++;
            k = 0;
        } else {
            i += k + 1;
            if (i == j) i++;
            k = 0;
        }
    }

    return min(i, j);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    ll pos = booth(s);

    cout << (s + s).substr(pos, s.size());
}
