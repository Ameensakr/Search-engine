#include <iostream>
#include <map>
#include <string>

using namespace std;

class trieNode
{
public:
    map<char , trieNode*>child;
    bool isEndOfWord;
    trieNode() : isEndOfWord(false){}
};

class trie
{
private:
    trieNode* root;
public:
    trie(){
        root = new trieNode();
    }

    void insert(const string &str)
    {
        trieNode* current = root;
        for(auto &c : str)
        {
            if(!current->child.count(c))
            {
                current->child[c] = new trieNode();
            }
            current = current->child[c];
        }
        current->isEndOfWord = true;
    }

    bool search(const string &str)
    {
        trieNode* current = root;
        for(auto &c : str)
        {
            if(!current->child.count(c))return false;
            current = current->child[c];
        }
        return current->isEndOfWord;
    }
    bool isPrefix(const string &str)
    {
        trieNode* current = root;
        for(auto &c : str)
        {
            if(!current->child.count(c))return false;
            current = current->child[c];
        }
        return true;
    }
};

