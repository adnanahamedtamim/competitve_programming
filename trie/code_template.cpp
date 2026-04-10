#include <bits/stdc++.h>
#include <unordered_map>
#include <chrono>
using namespace std;

typedef long long ll;
typedef long double ld;

#define pb push_back
#define all(x) x.begin(), x.end()
#define endl '\n'
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl

#define isSet(x, i) ((x >> i) & 1)
#define setbit(x, i) (x | (1LL << i))
#define resetbit(x, i) (x & (~(1LL << i)))
#define toggleBit(x, i) ((x) ^ (1LL << (i)))
#define clz(x) __builtin_clzll(x)
#define ctz(x) __builtin_ctzll(x)
#define csb(x) __builtin_popcountll(x)
#define msb(x) (ll)((x) ? (63 - __builtin_clzll((ll)(x))) : -1)
#define lsb(x) (ll)((x) ? (__builtin_ctzll((ll)(x))) : -1)
typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<int>> vvi;

// TRIE//
// assuming all the input strings are in small letters

struct trie_node
{
    int pc; // prefix count
    trie_node *child[26];
    bool isend; // word ends here or not

    trie_node()
    {
        pc = 0;
        isend = false;
        for (ll i = 0; i < 26; i++)
        {
            child[i] = NULL;
        }
    }
};

trie_node *root = new trie_node();

void insert(string s)
{

    trie_node *curr = root;
    for (auto c : s)
    {
        ll index = c - 'a';
        if (curr->child[index] == NULL)
        {
            curr->child[index] = new trie_node();
        }
        curr = curr->child[index];
        curr->pc++;
    }
    curr->isend = true;
}

bool search(string s)
{
    trie_node *curr = root;

    for (auto c : s)
    {
        ll index = c - 'a';

        if (curr->child[index] == NULL || curr->child[index]->pc == 0)
        {
            return false;
        }
        curr = curr->child[index];
    }
    return curr->isend;
}

void removeword(string s)
{
    if (!search(s))
        return;

    trie_node *curr = root;
    for (auto c : s)
    {
        ll index = c - 'a';
        curr = curr->child[index];
        curr->pc--;
    }
    curr->isend = false;
    return;
}

// count the number string of which s is a prefix

ll count_prefix(string s)
{
    trie_node *curr = root;

    for (auto c : s)
    {
        ll index = c - 'a';

        if (curr->child[index] == NULL || curr->child[index]->pc == 0)
        {
            return 0;
        }
        curr = curr->child[index];
    }

    return curr->pc;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    insert("adnan");
    insert("tamim");
    insert("adu");

    if (search("adnan"))
    {
        cout << "paisi" << endl;
    }

    removeword("adnan");

    if (!search("adnan"))
    {
        cout << "pai nai" << endl;
    }

    insert("adnan");

    cout << count_prefix("addd") << endl;
    cout << count_prefix("tam") << endl;
    cout << count_prefix("ad") << endl;

    return 0;
}
