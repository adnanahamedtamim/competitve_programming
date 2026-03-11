#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

//CSES FOREST QUERIES 2, but it will tle

class SegmentTree2D {
private:
    int N, M;
    vector<vector<int>> rect;
    vector<vector<int>> ini_seg;
    vector<vector<int>> fin_seg;

    // Build 1D segment tree for one row
    void buildRow(int low, int high, int pos, int strip) {
        if (low == high) {
            ini_seg[strip][pos] = rect[strip][low];
        } else {
            int mid = (low + high) / 2;
            buildRow(low, mid, 2 * pos, strip);
            buildRow(mid + 1, high, 2 * pos + 1, strip);
            ini_seg[strip][pos] =
                ini_seg[strip][2 * pos] +
                ini_seg[strip][2 * pos + 1];
        }
    }

    // Build Y tree
    void buildY(int low, int high, int pos) {
        if (low == high) {
            for (int i = 1; i < 4 * M; i++)
                fin_seg[pos][i] = ini_seg[low][i];
        } else {
            int mid = (low + high) / 2;
            buildY(low, mid, 2 * pos);
            buildY(mid + 1, high, 2 * pos + 1);

            for (int i = 1; i < 4 * M; i++)
                fin_seg[pos][i] =
                    fin_seg[2 * pos][i] +
                    fin_seg[2 * pos + 1][i];
        }
    }

    // Query in X dimension
    int queryX(int pos, int start, int end,
               int x1, int x2, int node) {
        if (x2 < start || end < x1) return 0;
        if (x1 <= start && end <= x2)
            return fin_seg[node][pos];

        int mid = (start + end) / 2;
        return queryX(2 * pos, start, mid, x1, x2, node) +
               queryX(2 * pos + 1, mid + 1, end, x1, x2, node);
    }

    // Query in Y dimension
    int queryY(int pos, int start, int end,
               int y1, int y2, int x1, int x2) {
        if (y2 < start || end < y1) return 0;

        if (y1 <= start && end <= y2)
            return queryX(1, 0, M - 1, x1, x2, pos);

        int mid = (start + end) / 2;
        return queryY(2 * pos, start, mid, y1, y2, x1, x2) +
               queryY(2 * pos + 1, mid + 1, end, y1, y2, x1, x2);
    }

    // Update in X dimension
    void updateX(int pos, int low, int high,
                 int x, int val, int node) {
        if (low == high) {
            fin_seg[node][pos] = val;
        } else {
            int mid = (low + high) / 2;
            if (x <= mid)
                updateX(2 * pos, low, mid, x, val, node);
            else
                updateX(2 * pos + 1, mid + 1, high, x, val, node);

            fin_seg[node][pos] =
                fin_seg[node][2 * pos] +
                fin_seg[node][2 * pos + 1];
        }
    }

    // Update in Y dimension
    void updateY(int pos, int low, int high,
                 int y, int x, int val) {
        if (low == high) {
            updateX(1, 0, M - 1, x, val, pos);
        } else {
            int mid = (low + high) / 2;
            if (y <= mid)
                updateY(2 * pos, low, mid, y, x, val);
            else
                updateY(2 * pos + 1, mid + 1, high, y, x, val);

            // merge children X trees
            for (int i = 1; i < 4 * M; i++)
                fin_seg[pos][i] =
                    fin_seg[2 * pos][i] +
                    fin_seg[2 * pos + 1][i];
        }
    }

public:
    SegmentTree2D(vector<vector<int>> matrix) {
        rect = matrix;
        N = rect.size();
        M = rect[0].size();

        ini_seg.assign(N, vector<int>(4 * M, 0));
        fin_seg.assign(4 * N, vector<int>(4 * M, 0));

        for (int i = 0; i < N; i++)
            buildRow(0, M - 1, 1, i);

        buildY(0, N - 1, 1);
    }

    int query(int y1, int x1, int y2, int x2) {
        return queryY(1, 0, N - 1, y1, y2, x1, x2);
    }

    void update(int y, int x, int val) {
        updateY(1, 0, N - 1, y, x, val);
    }
};



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, q;
    cin >> N >> q;

    vector<vector<int>> matrix(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char c;
            cin >> c;
            matrix[i][j] = (c == '*');
        }
    }

  SegmentTree2D seg(matrix);

// cout << seg.query(1,1,3,3) << "\n";

// seg.update(2,2,5);   // change cell (2,2) to 5

// cout << seg.query(1,1,3,3) << "\n";


while(q--){
  
  ll t;
  cin >> t;
   if(t==1){
      ll x,y;
      cin >> y >> x;
      x--;
      y--;
      
      ll s=seg.query(y,x,y,x);
      if(s==1){
           seg.update(y,x,0);
      }
      else{
          seg.update(y,x,1);
      }
   }
else{
   ll x,y,x2,y2;
   cin >> y >> x >> y2 >> x2;
   y--;
   x--;
   y2--;
   x2--;
 cout << seg.query(y,x,y2,x2) << endl;  
} 

}

    return 0;
}