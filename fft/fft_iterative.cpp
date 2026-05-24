#include <bits/stdc++.h>
using namespace std;

#define ll long long

using cd = complex<double>;
const double PI = acos(-1);
#define vll vector<ll>

void fft(vector<cd> &a, bool invert) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1)
            j ^= bit;

        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);

        cd wlen(cos(ang), sin(ang));

        for (int i = 0; i < n; i += len) {
            cd w(1);

            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;

                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;

                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd &x : a)
            x /= n;
    }
}
vector<ll> multiply(vector<ll>& a, vector<ll>& b) {
    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());

    int n = 1;

    while (n < (int)a.size() + (int)b.size())
        n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    vector<ll> res(n);

    for (int i = 0; i < n; i++)
        res[i] = (llround(fa[i].real()) > 0);

    while (!res.empty() && res.back() == 0)
        res.pop_back();

    if ((int)res.size() > 1000001)
        res.resize(1000001);

    return res;
}

vector<ll> power(vector<ll> base, ll k) {
    vector<ll> result(1, 1);

    while (k) {
        if (k & 1){
            result = multiply(result, base);
         }
        k >>= 1;

        if (k){
            base = multiply(base, base);
          
        }
    }

    return result;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

 ll n,k;
 cin >> n >> k;
 vll ara(n);
 ll maxi=0;
 for(ll i=0;i<n;i++){
   cin >> ara[i];
   maxi=max(maxi,ara[i]);
 }
 
 vll poly(maxi+1,0);
 
 for(auto it : ara){
     poly[it]=1;
 }
   
  vll ans=power(poly,k); 
  
  
  for(ll i=0;i<ans.size();i++){
      if(ans[i]!=0){
          cout << i << " ";
      }
  }
  
  cout << endl;

    return 0;
}

