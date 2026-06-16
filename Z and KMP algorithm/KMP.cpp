#include <bits/stdc++.h>
using namespace std;

vector<int> build_lps(const string& pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);

    int j = 0;

    for (int i = 1; i < m; i++) {

        while (j > 0 && pattern[i] != pattern[j]) {
            j = lps[j - 1];
        }

        if (pattern[i] == pattern[j]) {
            j++;
        }

        lps[i] = j;
    }

    return lps;
}

vector<int> kmp_search(const string& text, const string& pattern) {

    vector<int> occurrences;

    if (pattern.empty()) return occurrences;

    vector<int> lps = build_lps(pattern);

    int n = text.size();
    int m = pattern.size();

    int j = 0;

    for (int i = 0; i < n; i++) {

        while (j > 0 && text[i] != pattern[j]) {
            j = lps[j - 1];
        }

        if (text[i] == pattern[j]) {
            j++;
        }

        if (j == m) {

            // Pattern found starting at:
            occurrences.push_back(i - m + 1);

            // Continue searching for overlapping matches
            j = lps[j - 1];
        }
    }

    return occurrences;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text, pattern;

    cin >> text >> pattern;

    vector<int> matches = kmp_search(text, pattern);

    cout << matches.size() << '\n';

    for (int pos : matches) {
        cout << pos << ' ';
    }

    cout << '\n';

    return 0;
}
