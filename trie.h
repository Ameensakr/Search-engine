#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

const int MAX_CHAR = 26;

struct trie {
    trie *child[MAX_CHAR];
    bool isLeaf = false;
    map<int,int> mp;

    void insert(string &str, int idxOfStr, int indexOfPage) {
        if (idxOfStr == str.size()) {
            isLeaf = 1;
            mp[indexOfPage]++;
            return;
        }

        int cur = str[idxOfStr] - 'a';
        if (child[cur] == nullptr)
            child[cur] = new trie();
        child[cur]->insert(str, idxOfStr + 1, indexOfPage);

    }

    map<int,int> whereWordExist(string &str, int idxOfStr = 0) {
        if (idxOfStr == str.size()) {
            return mp;
        }

        int cur = str[idxOfStr] - 'a';
        if (child[cur] == nullptr) {
            return {};
        }// such path don't exist

        return child[cur]->whereWordExist(str, idxOfStr + 1);
    }

};

