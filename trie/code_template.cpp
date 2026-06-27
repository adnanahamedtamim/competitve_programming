#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode *child[26];

    int prefixCount;   // words passing through this node
    int endCount;      // words ending here
    bool isLeaf;

    TrieNode() {
        prefixCount = 0;
        endCount = 0;
        isLeaf = false;

        for(int i = 0; i < 26; i++)
            child[i] = nullptr;
    }
};

class Trie {

    TrieNode *root;

public:

    Trie() {
        root = new TrieNode();
    }

    void insert(const string &s) {

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            if(cur->child[id] == nullptr)
                cur->child[id] = new TrieNode();

            cur = cur->child[id];
            cur->prefixCount++;
        }

        cur->endCount++;
        cur->isLeaf = true;
    }

    bool search(const string &s) {

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            if(cur->child[id] == nullptr)
                return false;

            cur = cur->child[id];
        }

        return cur->endCount > 0;
    }

    bool startsWith(const string &s) {

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            if(cur->child[id] == nullptr)
                return false;

            cur = cur->child[id];
        }

        return true;
    }

    int countWordsEqualTo(const string &s) {

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            if(cur->child[id] == nullptr)
                return 0;

            cur = cur->child[id];
        }

        return cur->endCount;
    }

    int countWordsStartingWith(const string &s) {

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            if(cur->child[id] == nullptr)
                return 0;

            cur = cur->child[id];
        }

        return cur->prefixCount;
    }

    void erase(const string &s) {

        if(!search(s))
            return;

        TrieNode *cur = root;

        for(char c : s) {

            int id = c - 'a';

            cur = cur->child[id];
            cur->prefixCount--;
        }

        cur->endCount--;

        if(cur->endCount == 0)
            cur->isLeaf = false;
    }
};

int main() {

    Trie trie;

    // Insert words
    trie.insert("apple");
    trie.insert("app");
    trie.insert("apple");
    trie.insert("ape");
    trie.insert("bat");
    trie.insert("ball");

    cout << boolalpha; //a C++ stream manipulator that tells cout to print boolean values as true and false instead of 1 and 0.

    cout << "Search examples\n";
    cout << "apple : " << trie.search("apple") << "\n";
    cout << "app   : " << trie.search("app") << "\n";
    cout << "ap    : " << trie.search("ap") << "\n";
    cout << "cat   : " << trie.search("cat") << "\n\n";

    cout << "Prefix examples\n";
    cout << "ap : " << trie.startsWith("ap") << "\n";
    cout << "ba : " << trie.startsWith("ba") << "\n";
    cout << "ca : " << trie.startsWith("ca") << "\n\n";

    cout << "Word Counts\n";
    cout << "apple : " << trie.countWordsEqualTo("apple") << "\n";
    cout << "app   : " << trie.countWordsEqualTo("app") << "\n";
    cout << "ape   : " << trie.countWordsEqualTo("ape") << "\n";
    cout << "cat   : " << trie.countWordsEqualTo("cat") << "\n\n";

    cout << "Prefix Counts\n";
    cout << "ap : " << trie.countWordsStartingWith("ap") << "\n";
    cout << "app: " << trie.countWordsStartingWith("app") << "\n";
    cout << "ba : " << trie.countWordsStartingWith("ba") << "\n";
    cout << "b  : " << trie.countWordsStartingWith("b") << "\n\n";

    cout << "Erase one occurrence of apple\n";
    trie.erase("apple");

    cout << "apple count = "
         << trie.countWordsEqualTo("apple") << "\n";

    cout << "prefix app count = "
         << trie.countWordsStartingWith("app") << "\n";

    return 0;
}
