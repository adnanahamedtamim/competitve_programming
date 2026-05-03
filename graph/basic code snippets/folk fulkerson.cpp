#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Define 'll' for long long to make the code cleaner
typedef long long ll;

const ll INF = 1e18; // A very large value for long long
ll capacity[100][100];
bool visited[100];
int n; // Number of nodes

// STEP 1: Find an Augmenting Path using DFS
// This function looks for any path from 'u' to 't' where capacity > 0
ll findPath(int u, int t, ll pushed_flow) {
    if (u == t) return pushed_flow; // Base case: reached the sink!
    
    visited[u] = true;
    
    for (int v = 0; v < n; v++) {
        // If 'v' is not visited and there is room in the pipe (capacity > 0)
        if (!visited[v] && capacity[u][v] > 0) {
            
            // Try to find the bottleneck along this path
            ll bottleneck = findPath(v, t, min(pushed_flow, capacity[u][v]));
            
            // If we found a valid path to the sink
            if (bottleneck > 0) {
                // STEP 2: UPDATE THE RESIDUAL GRAPH
                // We use the bottleneck to update the pipes
                capacity[u][v] -= bottleneck; // Decrease forward capacity
                capacity[v][u] += bottleneck; // Increase reverse capacity (The "Undo" credit)
                
                return bottleneck; // Return the flow to the main loop
            }
        }
    }
    return 0; // No path found from this node
}

ll fordFulkerson(int s, int t) {
    ll max_flow = 0;
    
    // STEP 3: Keep searching as long as there is an augmenting path
    while (true) {
        // Clear 'visited' array for each new search
        for (int i = 0; i < n; i++) visited[i] = false;
        
        // Try to find a path and get its bottleneck value
        ll flow_added = findPath(s, t, INF);
        
        // If findPath returns 0, no more flow can reach the sink
        if (flow_added == 0) break;
        
        // Add this path's flow to our total running answer
        max_flow += flow_added;
    }
    
    return max_flow;
}

int main() {
    // Example Setup: 4 nodes, find flow from 0 to 3
    n = 4;
    capacity[0][1] = 10;
    capacity[0][2] = 5;
    capacity[1][2] = 15;
    capacity[1][3] = 7;
    capacity[2][3] = 10;
    
    cout << "Maximum Flow: " << fordFulkerson(0, 3) << endl;
    
    return 0;
}
