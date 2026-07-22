#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent a 2D coordinate
struct Point {
    long long x, y;
};

// Global anchor point P0 used by the sorting comparator
Point P0;

// Calculates the squared distance between two points
long long distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Calculates the cross product of the vectors (p->q) and (q->r)
// < 0 : Clockwise (Right Turn)
// > 0 : Anti-Clockwise (Left Turn / Dent)
// = 0 : Collinear (Straight Line)
long long orientation(Point p, Point q, Point r) {
    return (q.x - p.x) * (r.y - q.y) - (q.y - p.y) * (r.x - q.x);
}

// Comparator to sort points in clockwise order relative to P0
bool compare(Point p1, Point p2) {
    long long o = orientation(P0, p1, p2);
    
    // If collinear, sort by distance (closest first)
    if (o == 0) {
        return distSq(P0, p1) < distSq(P0, p2);
    }
    
    // Sort in clockwise order (cross product < 0)
    return (o < 0);
}

void solve() {
    int n;
    cin >> n;
    
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }

    // Step 1: Find the bottom-most, left-most point
    int min_idx = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[min_idx].y || 
           (points[i].y == points[min_idx].y && points[i].x < points[min_idx].x)) {
            min_idx = i;
        }
    }

    // Place the anchor point at the 0th index
    swap(points[0], points[min_idx]);
    P0 = points[0];

    // Step 2: Sort the remaining points relative to P0
    sort(points.begin() + 1, points.end(), compare);

    // Step 3: Use a vector as a stack to build the hull
    vector<Point> hull;
    hull.push_back(points[0]);
    
    if (n > 1) {
        hull.push_back(points[1]);
    }

    for (int i = 2; i < n; i++) {
        // While the stack has at least 2 elements and the sequence 
        // does NOT make a strict clockwise turn (meaning it's a dent or collinear)
        while (hull.size() > 1 && 
               orientation(hull[hull.size() - 2], hull.back(), points[i]) >= 0) {
            hull.pop_back(); // Discard the point creating the dent/straight line
        }
        hull.push_back(points[i]); // Accept the new point
    }

    // Print the size of the hull and its vertices
    cout << hull.size() << "\n";
    for (int i = 0; i < hull.size(); i++) {
        cout << hull[i].x << " " << hull[i].y << "\n";
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}
