#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    long long value;       
    int priority;      
    int size;          
    Node *left, *right;
    
    // Lazy tags
    bool rev;
    long long lazy;    // Stores pending additions for the subtree
    long long sum;

    Node(long long v) {
        value = v;
        priority = rng();
        size = 1;
        left = right = nullptr;
        
        rev = false;
        lazy = 0;      // Initialize lazy tag to 0
        sum = v;
    }
};

int getSize(Node* t) { return t ? t->size : 0; }
long long getsum(Node* t) { return t ? t->sum : 0LL; }

// Push pending lazy updates DOWN to children
void push(Node* t) {
    if (!t) return;
    
    // 1. Push Reverse
    if (t->rev) {
        swap(t->left, t->right);
        if (t->left) t->left->rev ^= 1;
        if (t->right) t->right->rev ^= 1;
        t->rev = false;          
    }
    
    // 2. Push Range Add
    if (t->lazy != 0) {
        if (t->left) {
            t->left->lazy += t->lazy;
            t->left->value += t->lazy;
            // A node's sum increases by (value added * number of elements)
            t->left->sum += t->lazy * t->left->size; 
        }
        if (t->right) {
            t->right->lazy += t->lazy;
            t->right->value += t->lazy;
            t->right->sum += t->lazy * t->right->size;
        }
        t->lazy = 0; // Clear the tag after pushing
    }
}

// Update current node's structural values from children
void update(Node* t) {
    if (!t) return;
    // Note: update() is only called after push(), meaning t->lazy is 0 here.
    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->sum = t->value + getsum(t->left) + getsum(t->right);
}

pair<Node*, Node*> split(Node* t, int k) {
    if (!t) return {nullptr, nullptr};
    push(t); // ALWAYS push before going down

    if (getSize(t->left) < k) {
        auto piece = split(t->right, k - getSize(t->left) - 1);
        t->right = piece.first;
        update(t);
        return {t, piece.second};
    } else {
        auto piece = split(t->left, k);
        t->left = piece.second;
        update(t);
        return {piece.first, t};
    }
}

Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;

    push(a); // ALWAYS push before merging
    push(b);

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

Node* build(vector<long long>& arr) {
    Node* root = nullptr;
    for (long long x : arr) root = merge(root, new Node(x));
    return root;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q; 
    cin >> n >> q;
    vector<long long> ara(n);

    for(int i = 0; i < n; i++) cin >> ara[i];

    Node* root = build(ara);

    while (q--) { 
        int type, l, r;
        cin >> type >> l >> r;
        
        // Isolate the sub-segment [l, r]
        auto p1 = split(root, l - 1);
        auto p2 = split(p1.second, r - l + 1);
        
        if (type == 1) {
            // REVERSE RANGE
            if (p2.first) p2.first->rev ^= 1;
        }
        else if (type == 2) {
            // RANGE ADD: Add 'val' to all elements in [l, r]
            long long val; 
            cin >> val;
            
            if (p2.first) {
                p2.first->lazy += val;
                p2.first->value += val;
                p2.first->sum += val * p2.first->size; // Update sum immediately
            }
        }
        else if (type == 3) {
            // RANGE SUM
            cout << getsum(p2.first) << "\n";   
        }
        
        // Stitch the tree back together
        root = merge(p1.first, merge(p2.first, p2.second));
    }
    
    return 0;
}
