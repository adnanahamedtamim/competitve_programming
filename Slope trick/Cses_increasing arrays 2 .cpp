#include <iostream>
#include <vector>
#include <queue>
 
using namespace std;
 
/**
 * Problem: Increasing Array II (CSES)
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    int n;
    cin >> n;
 
    priority_queue<long long> pq;
    long long total_moves = 0;
 
    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
 
        // Push current element into the max-priority queue
        pq.push(x);
 
        // If the current element is smaller than the largest element 
        // seen so far, it violates the non-decreasing property.
        if (pq.top() > x) {
            // The cost to fix this violation is the difference
            total_moves += (pq.top() - x);
            
            // Remove the old maximum and replace it with the current x
            // to maintain the slope-trick invariant.
            pq.pop();
            pq.push(x);
        }
    }
 
    cout << total_moves << endl;
 
    return 0;
}
