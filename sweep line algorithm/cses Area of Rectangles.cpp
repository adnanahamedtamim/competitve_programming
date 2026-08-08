#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
typedef long long ll;
 
// 1. The Sweep-Line Event
// We split every rectangle into a left edge (+1) and a right edge (-1).
struct Event {
    ll x, y1, y2;
    int val; 
    
    // Sort events from left to right on the x-axis
    bool operator<(const Event& other) const {
        return x < other.x;
    }
};
 
const int MAXN = 200005; // Max 10^5 rectangles = 2*10^5 y-coordinates
ll cnt[4 * MAXN];        // Tracks if a node is completely covered
ll active_len[4 * MAXN]; // Tracks the actual physical length covered
 
// 2. The Segment Tree Update (Editing the Y-axis coverage)
void update(int node, int l, int r, int ql, int qr, int val, const vector<ll>& Y) {
    // Step A: Update the coverage counter
    if (ql <= l && r <= qr) {
        cnt[node] += val; 
    } else {
        int mid = l + (r - l) / 2;
        if (ql <= mid) update(2 * node, l, mid, ql, qr, val, Y);
        if (qr > mid)  update(2 * node + 1, mid + 1, r, ql, qr, val, Y);
    }
    
    // Step B: Recalculate the active physical length
    if (cnt[node] > 0) {
        // Entirely covered by at least one rectangle
        active_len[node] = Y[r + 1] - Y[l];
    } else {
        // Partially covered (or empty), ask the children
        if (l == r) active_len[node] = 0;
        else active_len[node] = active_len[2 * node] + active_len[2 * node + 1];
    }
}
 
int main() {
    // Fast I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    int n;
    if (!(cin >> n)) return 0;
 
    vector<Event> events;
    vector<ll> Y;
 
    for (int i = 0; i < n; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        events.push_back({x1, y1, y2, 1});   // Left edge starts covering
        events.push_back({x2, y1, y2, -1});  // Right edge stops covering
        
        Y.push_back(y1);
        Y.push_back(y2);
    }
 
    // 3. Coordinate Compression
    // Put all unique y-coordinates into a sorted list so the Segment Tree 
    // can map large coordinates (like 10^9) to small indices (0, 1, 2...)
    sort(Y.begin(), Y.end());
    Y.erase(unique(Y.begin(), Y.end()), Y.end());
    
    // Sort events to move left-to-right
    sort(events.begin(), events.end());
 
    ll total_area = 0;
    ll last_x = events[0].x;
    // for(auto it : Y){
    //       cout << it << " ";
    // }
    // cout << endl;
    // 4. The Calculate -> Edit -> Move Cycle
    for (const auto& ev : events) {
        
        // CALCULATE: Multiply the distance moved on X by the current height on Y
        ll dx = ev.x - last_x;
        total_area += dx * active_len[1]; // active_len[1] is the Root Node
        
        // Find where our y1 and y2 live in the compressed Segment Tree indices
        int ql = lower_bound(Y.begin(), Y.end(), ev.y1) - Y.begin();
        int qr = lower_bound(Y.begin(), Y.end(), ev.y2) - Y.begin() - 1; 
        // cout << ql << " " << qr << endl;
 
        // EDIT: Tell the segment tree to add (+1) or remove (-1) this rectangle
        if (ql <= qr) {
            update(1, 0, Y.size() - 2, ql, qr, ev.val, Y);
        }
        
        // MOVE: Step forward
        last_x = ev.x;
    }
 
    cout << total_area << "\n";
    return 0;
}
