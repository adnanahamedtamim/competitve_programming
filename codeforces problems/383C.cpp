#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;
#define pb push_back

struct SegmentTree {
    vll tree;
    vll lazy;

    void constructSTUtil(vll& arr, ll ss, ll se, ll si) {
        if (ss > se) return;

        if (ss == se) {
            tree[si] = arr[ss];
            return;
        }

        ll mid = (ss + se) / 2;

        constructSTUtil(arr, ss, mid, si * 2 + 1);
        constructSTUtil(arr, mid + 1, se, si * 2 + 2);

        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    void constructST(vll& arr, ll n) {
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);

        constructSTUtil(arr, 0, n - 1, 0);
    }

    void pushDown(ll si, ll ss, ll se) {
        if (lazy[si] != 0) {
            tree[si] += (se - ss + 1) * lazy[si];

            if (ss != se) {
                lazy[si * 2 + 1] += lazy[si];
                lazy[si * 2 + 2] += lazy[si];
            }

            lazy[si] = 0;
        }
    }

    void updateRangeUtil(ll si, ll ss, ll se,
                         ll us, ll ue, ll diff) {

        pushDown(si, ss, se);

        if (ss > se || ss > ue || se < us)
            return;

        if (ss >= us && se <= ue) {
            tree[si] += (se - ss + 1) * diff;

            if (ss != se) {
                lazy[si * 2 + 1] += diff;
                lazy[si * 2 + 2] += diff;
            }

            return;
        }

        ll mid = (ss + se) / 2;

        updateRangeUtil(si * 2 + 1, ss, mid, us, ue, diff);
        updateRangeUtil(si * 2 + 2, mid + 1, se, us, ue, diff);

        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    void updateRange(ll n, ll us, ll ue, ll diff) {
        updateRangeUtil(0, 0, n - 1, us, ue, diff);
    }

    ll getSumUtil(ll ss, ll se,
                  ll qs, ll qe, ll si) {

        pushDown(si, ss, se);

        if (ss > se || ss > qe || se < qs)
            return 0;

        if (ss >= qs && se <= qe)
            return tree[si];

        ll mid = (ss + se) / 2;

        return getSumUtil(ss, mid, qs, qe, 2 * si + 1)
             + getSumUtil(mid + 1, se, qs, qe, 2 * si + 2);
    }

    ll getSum(ll n, ll qs, ll qe) {
        qs=max(qs,0LL);
        qe=min(qe,n-1);
        if (qs > qe) {
            // cout << "Invalid Input\n";
            return 0;
        }

        return getSumUtil(0, n - 1, qs, qe, 0);
    }
};

void dfs(ll i,ll par,vvll& adj,vll& start,vll& end,ll& time,vll& levels){
     start[i]=time++;
     for(auto it : adj[i]){
          if(it==par) continue;
          levels[it]=1-levels[i];
          dfs(it,i,adj,start,end,time,levels);
     }
     end[i]=time++;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n,m;
    cin >> n >> m;

    vvll adj(n);
    vll ara(n);
    for(ll i=0;i<n;i++){
         cin >> ara[i];
    }
    

    for(ll i=0;i<n-1;i++){
         ll x,y;
         cin >> x >> y;
         x--;
         y--;
         adj[x].pb(y);
         adj[y].pb(x);
   }
   if(n==1){
     ll cur=ara[0];
     while(m--){
          ll t;
          cin >>  t;
          if(t==1){
              ll x,v;
              cin >> x >> v;
              cur+=v;
          }
          else{
            ll x;
            cin >> x;
             cout << cur << endl;
          }
     }
     return 0;
   }
   

   ll time=0;
   vll levels(n,0),start(n,0),end(n,0);
   levels[0]=0;

   dfs(0,-1,adj,start,end,time,levels);

   vector<vector<pair<ll,ll>>> oe(2);

   for(ll i=0;i<n;i++){
       oe[levels[i]].pb({start[i],i});
   }
   for(ll i=0;i<2;i++){
     sort(oe[i].begin(),oe[i].end());
   }
   vll oind(n,-1),eind(n,-1);
   for(ll i=0;i<oe[0].size();i++){
       eind[oe[0][i].second]=i;
   }
   for(ll i=0;i<oe[1].size();i++){
       oind[oe[1][i].second]=i;
   }

   vll evenara,oddara;
   for(ll i=0;i<oe[0].size();i++){
     evenara.pb(ara[oe[0][i].second]);
   }
   for(ll i=0;i<oe[1].size();i++){
     oddara.pb(ara[oe[1][i].second]);
   }

  SegmentTree ste,sto;

   ste.constructST(evenara,evenara.size());
   sto.constructST(oddara,oddara.size());

auto func = [&](ll s, ll e, ll val) {

    ll ind1 = lower_bound(
        oe[0].begin(),
        oe[0].end(),
        make_pair(s, LLONG_MIN)
    ) - oe[0].begin();

    ll ind2 = upper_bound(
        oe[0].begin(),
        oe[0].end(),
        make_pair(e, LLONG_MIN)
    ) - oe[0].begin() - 1;

    ste.updateRange(evenara.size(), ind1, ind2, val);


    ind1 = lower_bound(
        oe[1].begin(),
        oe[1].end(),
        make_pair(s, LLONG_MIN)
    ) - oe[1].begin();

    ind2 = upper_bound(
        oe[1].begin(),
        oe[1].end(),
        make_pair(e, LLONG_MIN)
    ) - oe[1].begin() - 1;

    sto.updateRange(oddara.size(), ind1, ind2, -val);
};

   while(m--){
     ll t;
     cin >> t;

     if(t==1){
          ll x,y;
          cin >> x >> y;
          x--;

          if(levels[x]==0){
            func(start[x],end[x],y);
          }
          else{
            func(start[x],end[x],-1*y);
          }
     }
     else{
          ll x;
          cin >> x;
          x--;

          if(levels[x]==0){
              cout << ste.getSum(evenara.size(),eind[x],eind[x]) << endl;
          }
          else{
              cout << sto.getSum(oddara.size(),oind[x],oind[x]) << endl;
          }
     }


   }


   



    return 0;
}


