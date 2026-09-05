#include <bits/stdc++.h>
using namespace std;
 
#define int long long
 
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
 
struct Node {
    int value;        // the actual array element
    int priority;      // random number, keeps the tree balanced
    int size;          // how many nodes are in this subtree
    Node *left, *right;
    bool rev;
    int sum;
 
    Node(int v) {
        value = v;
        priority = rand();
        size = 1;
       left = right = nullptr;
       rev=false;
       sum=v;
    }
};
 
void push(Node* t) {
    if (t && t->rev) {
       swap(t->left,t->right);
       if(t->left) t->left->rev^=1;
       if(t->right) t->right->rev^=1;
       t->rev=false;          
    }
}
 
// treat a missing node (nullptr) as having size 0 and sum 0
int getSize(Node* t) { return t ? t->size : 0; }
int getsum(Node* t) { return t ? t->sum : 0; }
 
// after changing a node's children, recompute its size and sum
void update(Node* t) {
    if (!t) return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->sum = t->value + getsum(t->left) + getsum(t->right);
}
 
// cut t into (first k elements, the rest)
pair<Node*, Node*> split(Node* t, int k) {
    if (!t) return {nullptr, nullptr};
    push(t);
 
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
 
    push(a);
    push(b);
 
    if (a->priority > b->priority) {
        a->right = merge(a->right, b);  update(a);
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
 
signed main() {
    int n; cin >> n;
    int q; cin >>  q;
    vector<int> ara(n);
 
    for(int i=0;i<n;i++){
         cin >> ara[i];
    }
 
    Node* root = build(ara);
 
    while (q--) { 
        int t;
        cin >> t;
        int l, r; cin >> l >> r;
        pair<Node*,Node*> p1=split(root,l-1);
        pair<Node*,Node*> p2=split(p1.second,r-l+1);
        
 
        if(t==1){
            p2.first->rev^=1;
        }
        else{
             cout << getsum(p2.first) << endl;   
       }
        root=merge(p1.first,merge(p2.first,p2.second));
            
    }
}
