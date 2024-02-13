#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

const int MAX_CHAR = 26;
struct trie {
    trie* child[MAX_CHAR];
    bool isLeaf;
    set<int>st , temp;


    trie() {
        for (int i = 0; i < MAX_CHAR; ++i) {
            child[i] = 0;
        }
        isLeaf = 0;
    }

    void insert(string &str , int idxOfStr , int indexOfPage) {
        if(idxOfStr == str.size())
        {
            isLeaf = 1;
            st.insert(indexOfPage);
        }
        else {
            int cur = str[idxOfStr] - 'a';
            if(child[cur] == 0 )
                child[cur] = new trie();
            child[cur]->insert(str ,idxOfStr + 1, indexOfPage);
        }
    }

    bool wordExist(string &str , int idxOfStr) {
        if(idxOfStr == str.size())
            return isLeaf;

        int cur = str[idxOfStr] - 'a';
        if(child[cur] == 0 )
            return false;	// such path don't exist

        return child[cur]->wordExist(str ,idxOfStr + 1);
    }
    set<int> whereWordExist(string &str , int idxOfStr) {
        if(idxOfStr == str.size())
            return st;

        int cur = str[idxOfStr] - 'a';
        if(child[cur] == 0 )
            return temp;	// such path don't exist

        return child[cur]->whereWordExist(str ,idxOfStr + 1);
    }

    bool prefixExist(char* str) {
        if(*str == '\0')
            return true;

        int cur = *str - 'a';
        if(child[cur] == 0 )
            return false;	// such path don't exist

        return child[cur]->prefixExist(str+1);
    }
};

