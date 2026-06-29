#include <bits/stdc++.h>
using namespace std;

using ll = long long;


class Manacher
{
private:

    string s;              // original string
    string t;              // transformed string

    vector<ll> p;          // palindrome radius


    // Convert:
    // abc -> #a#b#c#
    void transform()
    {
        t = "#";

        for(char c : s)
        {
            t += c;
            t += '#';
        }
    }



    void build()
    {
        ll n = t.size();

        p.assign(n, 0);


        ll center = 0;
        ll right = 0;


        for(ll i = 0; i < n; i++)
        {
            ll mirror = 2 * center - i;


            // use previously calculated palindrome
            if(i < right)
            {
                p[i] = min(right - i, p[mirror]);
            }


            // expand around center i
            while(
                i + p[i] + 1 < n &&
                i - p[i] - 1 >= 0 &&
                t[i + p[i] + 1] == t[i - p[i] - 1]
            )
            {
                p[i]++;
            }


            // update current rightmost palindrome
            if(i + p[i] > right)
            {
                center = i;
                right = i + p[i];
            }
        }
    }



public:


    Manacher(string str)
    {
        s = str;

        transform();

        build();
    }



    // O(1)
    bool isPalindrome(ll l, ll r)
    {
        /*
            Original index mapping:

            original index x
                    |
                    v
            transformed index = 2*x + 1
        */


        ll left = 2 * l + 1;
        ll right = 2 * r + 1;


        ll center = (left + right) / 2;


        ll need = (right - left) / 2;


        return p[center] >= need;
    }



    // Return longest palindromic substring
    string getLPS()
    {
        ll bestCenter = 0;
        ll bestLen = 0;


        ll n = t.size();

        for(ll i = 0; i < n; i++)
        {
            if(p[i] > bestLen)
            {
                bestLen = p[i];
                bestCenter = i;
            }
        }


        ll start = (bestCenter - bestLen) / 2;


        return s.substr(start, bestLen);
    }

};



int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    string s = "babad";


    Manacher M(s);



    cout << M.getLPS() << "\n";


    cout << boolalpha;


    cout << M.isPalindrome(0,2) << "\n"; // bab

    cout << M.isPalindrome(1,3) << "\n"; // aba

    cout << M.isPalindrome(0,3) << "\n"; // baba
}
