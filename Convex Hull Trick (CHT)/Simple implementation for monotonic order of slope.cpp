#include <bits/stdc++.h>
using namespace std;

struct LiChaoTree {
    static const long long INF = 1LL << 60; // Represents Infinity

    struct Line {
        long long m, c;
        long long eval(long long x) const { return m * x + c; }
    };

    struct Node {
        Line line;
        int left_child = -1;
        int right_child = -1;
    };

    vector<Node> tree;
    long long MIN_X, MAX_X;

    // Initialize with the minimum and maximum possible x-coordinates in your problem
    LiChaoTree(long long min_x = -2e9, long long max_x = 2e9) {
        MIN_X = min_x;
        MAX_X = max_x;
        // Create root node with a "dummy" line that gives -INFINITY everywhere
        tree.push_back({{0, -INF}, -1, -1});
    }

    // Public function to add any random line y = mx + c
    void add(long long m, long long c) {
        insert({m, c}, 0, MIN_X, MAX_X);
    }

    // Public function to query any random point x
    long long query(long long x) {
        return get_max(x, 0, MIN_X, MAX_X);
    }

private:
    void insert(Line new_line, int node, long long l, long long r) {
        long long mid = l + (r - l) / 2;

        // Step 1: If the new line is better at the midpoint, swap it!
        // The better line stays here, and we push the old line down.
        bool better_at_mid = new_line.eval(mid) > tree[node].line.eval(mid);
        if (better_at_mid) {
            swap(tree[node].line, new_line);
        }

        // Base case: If we reached a single point, we are done
        if (l == r) return;

        // Step 2: Where could the "loser" (new_line) still be better?
        // Check if it's better at the left boundary 'l'
        bool better_at_left = new_line.eval(l) > tree[node].line.eval(l);

        if (better_at_left) {
            // Push to left child
            if (tree[node].left_child == -1) {
                tree[node].left_child = tree.size();
                tree.push_back({{0, -INF}, -1, -1});
            }
            insert(new_line, tree[node].left_child, l, mid);
        } else {
            // Otherwise, if it has any chance, it must be on the right half
            if (tree[node].right_child == -1) {
                tree[node].right_child = tree.size();
                tree.push_back({{0, -INF}, -1, -1});
            }
            insert(new_line, tree[node].right_child, mid + 1, r);
        }
    }

    long long get_max(long long x, int node, long long l, long long r) {
        if (node == -1) return -INF; // Reached an empty branch

        // The answer could be the line stored in THIS node...
        long long res = tree[node].line.eval(x);
        if (l == r) return res;

        long long mid = l + (r - l) / 2;

        // ...OR a better line stored deeper down the tree in the child node!
        if (x <= mid) {
            return max(res, get_max(x, tree[node].left_child, l, mid));
        } else {
            return max(res, get_max(x, tree[node].right_child, mid + 1, r));
        }
    }
};

int main(){

    LiChaoTree cht;
    cht.add(-m, -c);               // Insert with minus signs
    long long ans = -cht.query(x); // Put a minus sign on the result

}
