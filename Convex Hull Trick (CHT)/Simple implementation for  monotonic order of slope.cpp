#include <bits/stdc++.h>
using namespace std;

struct SimpleCHT {
    struct Line {
        long long m, c;
        // Plug x into y = m*x + c
        long long eval(long long x) const { 
            return m * x + c; 
        }
    };

    vector<Line> hull;

    // Helper: Checks if line l2 is "buried" between l1 and l3
    // Using simple algebra (cross-multiplication) to avoid decimals
    bool is_bad(const Line& l1, const Line& l2, const Line& l3) {
        return (__int128)(l3.c - l1.c) * (l1.m - l2.m) <= 
               (__int128)(l2.c - l1.c) * (l1.m - l3.m);
    }

    // Add a new line y = m*x + c
    // RULE: Slopes 'm' must be added in INCREASING order!
    void add(long long m, long long c) {
        Line new_line = {m, c};

        // While the line at the very back is useless, throw it away
        while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull.back(), new_line)) {
            hull.pop_back();
        }

        // Put the new line at the back
        hull.push_back(new_line);
    }

    // Find the MAXIMUM value at point x in O(log N) time
    long long query(long long x) {
        int low = 0;
        int high = hull.size() - 1;

        // Normal Binary Search to find the highest peak
        while (low < high) {
            int mid = low + (high - low) / 2;

            // If moving to the right gives a bigger value, the peak is to the right!
            if (hull[mid].eval(x) <= hull[mid + 1].eval(x)) {
                low = mid + 1;
            } 
            // Otherwise, the peak is at 'mid' or to the left!
            else {
                high = mid;
            }
        }

        // 'low' now points directly to the best line
        return hull[low].eval(x);
    }
};

int main(){

        SimpleCHT cht;
      int m=1,c=1,x=1;
        
        // CASE 4: MINIMUM with INCREASING SLOPES
        cht.add(m, -c);               // 1. Keep 'm' positive so slopes stay INCREASING!
        long long ans = -cht.query(-x); // 2. Negate 'x' inside, and negate the result outside!

}

//DISCALIMER 
/*

Rule 1: FIX THE SLOPES
Your template strictly requires INCREASING slopes.
-> If slopes are already Increasing: Pass 'm'
-> If slopes are Decreasing:         You MUST pass '-m' to make them increasing!

Rule 2: BALANCE THE ALGEBRA
Look at what you did to 'm' and balance the equation:
-> For MAXIMUM: Whatever sign you gave 'm', give the exact same sign to 'x' in query().
(e.g., if you passed '-m', you must query '-x'). Intercept is always '+c'.
-> For MINIMUM: You must ALWAYS negate the final result (-query) and the intercept (-c).
Then, just give 'x' the OPPOSITE sign of whatever you passed for 'm' so they multiply to positive m*x!

*/
