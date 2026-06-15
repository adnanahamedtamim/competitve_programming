#include <iostream>
#include <vector>
 
using namespace std;
 
// The maximum possible salary
const int MAX_VAL = 1e9;
 
// Node structure for our Dynamic Segment Tree
struct Node {
    int count = 0;      // Number of employees in this salary range
    int left_child = -1;  // Index of left child in our vector
    int right_child = -1; // Index of right child in our vector
};
 
// We store all nodes in a flat vector. This is much faster than pointers!
vector<Node> tree;
 
// Function to update the tree (add or remove an employee)
void update(int node, int L, int R, int idx, int val) {
    // Add the value (+1 for adding, -1 for removing)
    tree[node].count += val;
    
    // If we reached a leaf node, we are done
    if (L == R) return;
    
    int mid = L + (R - L) / 2;
    
    if (idx <= mid) {
        // If left child doesn't exist, create it
        if (tree[node].left_child == -1) {
            tree[node].left_child = tree.size();
            tree.push_back(Node());
        }
        update(tree[node].left_child, L, mid, idx, val);
    } else {
        // If right child doesn't exist, create it
        if (tree[node].right_child == -1) {
            tree[node].right_child = tree.size();
            tree.push_back(Node());
        }
        update(tree[node].right_child, mid + 1, R, idx, val);
    }
}
 
// Function to get the number of employees in a salary range
int query(int node, int L, int R, int qL, int qR) {
    // If we hit a dead end (no node), there are 0 employees here
    if (node == -1) return 0;
    
    // If our query completely covers this range, return the whole count
    if (qL <= L && R <= qR) {
        return tree[node].count;
    }
    
    // If our query is completely outside this range, return 0
    if (R < qL || L > qR) {
        return 0;
    }
    
    int mid = L + (R - L) / 2;
    int left_sum = query(tree[node].left_child, L, mid, qL, qR);
    int right_sum = query(tree[node].right_child, mid + 1, R, qL, qR);
    
    return left_sum + right_sum;
}
 
int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    int n, q;
    if (!(cin >> n >> q)) return 0;
 

    tree.push_back(Node());
 
    vector<int> salary(n + 1);
    

    for (int i = 1; i <= n; i++) {
        cin >> salary[i];
        update(0, 1, MAX_VAL, salary[i], 1);
    }
 
  
    for (int i = 0; i < q; i++) {
        char type;
        cin >> type;
        
        if (type == '!') {
            int k, x;
            cin >> k >> x;
            
            // Remove the old salary
            update(0, 1, MAX_VAL, salary[k], -1);
            
            // Update array
            salary[k] = x;
            
            // Add the new salary
            update(0, 1, MAX_VAL, salary[k], 1);
            
        } else if (type == '?') {
            int a, b;
            cin >> a >> b;
            
           
            cout << query(0, 1, MAX_VAL, a, b) << "\n";
        }
    }
 
    return 0;
}
