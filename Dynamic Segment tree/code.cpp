#include <iostream>
#include <vector>

using namespace std;

// The maximum possible salary limit defined by the problem
const int MAX_VAL = 1000000000;

// 1. Define the Node Blueprint
struct Node {
    int count = 0;        // How many employees have a salary in this range
    int left_child = -1;  // Index of the left child (-1 means "not built yet")
    int right_child = -1; // Index of the right child (-1 means "not built yet")
};

// 2. The Vector that holds all our nodes
// This replaces the massive tree[4 * N] array used in normal segment trees
vector<Node> tree;

// 3. The Update Function (Adding or Removing a salary)
void update(int node, int L, int R, int target_salary, int val) {
    // val is +1 (adding an employee) or -1 (removing an employee)
    tree[node].count += val;
    
    // Base Case: We found the exact room for this specific salary
    if (L == R) return;
    
    int mid = L + (R - L) / 2;
    
    if (target_salary <= mid) {
        // We need to go LEFT. If the left child doesn't exist, build it!
        if (tree[node].left_child == -1) {
            tree[node].left_child = tree.size(); // The new node's index will be the current size of the vector
            tree.push_back(Node());              // Actually create the blank node
        }
        // Travel down to the left child
        update(tree[node].left_child, L, mid, target_salary, val);
    } else {
        // We need to go RIGHT. If the right child doesn't exist, build it!
        if (tree[node].right_child == -1) {
            tree[node].right_child = tree.size(); // The new node's index will be the current size of the vector
            tree.push_back(Node());               // Actually create the blank node
        }
        // Travel down to the right child
        update(tree[node].right_child, mid + 1, R, target_salary, val);
    }
}

// 4. The Query Function (Counting salaries in a range [a, b])
int query(int node, int L, int R, int a, int b) {
    // If this node was never built, there are exactly 0 employees here
    if (node == -1) return 0;
    
    // If the current range [L, R] is completely inside our query [a, b]
    if (a <= L && R <= b) {
        return tree[node].count;
    }
    
    // If the current range [L, R] is completely outside our query [a, b]
    if (R < a || L > b) {
        return 0;
    }
    
    // Otherwise, the range partially overlaps. Split and check both children!
    int mid = L + (R - L) / 2;
    int left_sum = query(tree[node].left_child, L, mid, a, b);
    int right_sum = query(tree[node].right_child, mid + 1, R, a, b);
    
    return left_sum + right_sum;
}

int main() {
    // Fast I/O is mandatory for strict time limits
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);


    return 0;
}
