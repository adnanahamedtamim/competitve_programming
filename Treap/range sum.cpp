#include <bits/stdc++.h>
using namespace std;

struct Node {
    int value;        // the actual array element
    int priority;      // random number, keeps the tree balanced
    int size;          // how many nodes are in this subtree
    long long sum;     // sum of all values in this subtree
    Node *left, *right;

    Node(int v) {
        value = v;
        priority = rand();
        size = 1;
        sum = v;
        left = right = nullptr;
    }
};

// treat a missing node (nullptr) as having size 0 and sum 0
int getSize(Node* t) { return t ? t->size : 0; }
long long getSum(Node* t) { return t ? t->sum : 0; }

// after changing a node's children, recompute its size and sum
void update(Node* t) {
    if (!t) return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->sum  = t->value + getSum(t->left) + getSum(t->right);
}

// cut t into (first k elements, the rest)
pair<Node*, Node*> split(Node* t, int k) {
    if (!t) return {nullptr, nullptr};

    if (getSize(t->left) < k) {
        // t itself is among the first k, so recurse into its right side
        auto piece = split(t->right, k - getSize(t->left) - 1);
        t->right = piece.first;
        update(t);
        return {t, piece.second};
    } else {
        // t belongs after the cut, so recurse into its left side
        auto piece = split(t->left, k);
        t->left = piece.second;
        update(t);
        return {piece.first, t};
    }
}

// glue two trees together (every element of a comes before every element of b)
Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->priority > b->priority) {
        a->right = merge(a->right, b);
        update(a);
        return a;
    } else {
        b->left = merge(a, b->left);
        update(b);
        return b;
    }
}

// build the tree by adding array elements one at a time, left to right
Node* build(vector<int>& arr) {
    Node* root = nullptr;
    for (int x : arr) root = merge(root, new Node(x));
    return root;
}

// sum of arr[l..r], 1-indexed, inclusive
long long querySum(Node*& root, int l, int r) {
    auto part1 = split(root, l - 1);        // part1.first = arr[1..l-1]
    auto part2 = split(part1.second, r - l + 1); // part2.first = arr[l..r]

    long long answer = getSum(part2.first);

    root = merge(merge(part1.first, part2.first), part2.second); // stitch it back

    return answer;
}

int main() {
    int n; cin >> n;
    vector<int> arr(n);
    for (int& x : arr) cin >> x;

    Node* root = build(arr);

    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        cout << querySum(root, l, r) << "\n";
    }
}
