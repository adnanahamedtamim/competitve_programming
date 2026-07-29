#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MOD = 1e9 + 7;
const int MAXB = 1000000;

int phi[MAXB + 1];
int count_multiple[MAXB + 1];
long long pow2[100005];

void precompute() {
    // Precompute Euler's Totient Function
    for (int i = 0; i <= MAXB; i++) phi[i] = i;
    for (int i = 2; i <= MAXB; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= MAXB; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    // Precompute powers of 2 for subsequence counts
    pow2[0] = 1;
    for (int i = 1; i <= 100000; i++) {
        pow2[i] = (pow2[i - 1] * 2) % MOD;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    precompute();

    int n;
    if (!(cin >> n)) return 0;

    vector<int> b(n);
    int max_b = 0;

    for (int i = 0; i < n; i++) {
        cin >> b[i];
        max_b = max(max_b, b[i]);
        // Count frequencies of each number in array
        count_multiple[b[i]]++;
    }

    // For each divisor, count how many elements in the array are multiples of that divisor
    for (int g = 1; g <= max_b; g++) {
        int multiples_count = 0;
        for (int j = g; j <= max_b; j += g) {
            multiples_count += count_multiple[j];
        }
        count_multiple[g] = multiples_count; // reuse array to store total multiples count for divisor g
    }

    long long total_answer = 0;

    // We use the identity: gcd(S) = sum_{d | gcd(S)} phi(d)
    // Swapping summation: sum_{g=1}^{max_b} phi(g) * (number of non-empty subsequences whose elements are all multiples of g)
    for (int g = 1; g <= max_b; g++) {
        int cnt = count_multiple[g];
        if (cnt == 0) continue;

        // Number of non-empty subsequences formed by elements that are multiples of g is (2^cnt - 1)
        long long subseq_count = (pow2[cnt] - 1 + MOD) % MOD;

        long long term = (1LL * phi[g] * subseq_count) % MOD;
        total_answer = (total_answer + term) % MOD;
    }

    cout << total_answer << "\n";

    return 0;
}
