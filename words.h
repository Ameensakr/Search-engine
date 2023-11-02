#include <iostream>
#include <vector>
#include <string>
#include <fstream>


std::vector<std::string> englishWords;


void init()
{
    std::ifstream inputFile("words.txt");
    if (inputFile.is_open()) {
        std::string word;
        while (inputFile >> word) {
            englishWords.push_back(word);
        }
        inputFile.close();

    }
}

int solve(int i , int j , string &s1 , string &s2 , vector<vector<int>>&dp)
{
    if(i < 0)return j + 1;
    if(j < 0)return i + 1;
    int &ret = dp[i][j] ;
    if(ret != -1)return ret;
    if(s1[i] == s2[j])return ret = solve(i - 1 , j - 1 , s1 , s2 , dp);
    return ret = min({1 + solve(i - 1 , j , s1 , s2 , dp), 1 + solve(i - 1 , j - 1, s1 , s2 , dp) , 1 + solve(i , j - 1 , s1 , s2 , dp)});
}


int correct(string &s1 , string &s2)
{
    string ret;
    while( !s2.empty() && s2.back() == ' ')s2.pop_back();

    int n = (int)s1.size() , m = (int)s2.size();
    vector<vector<int>>dp(n , vector<int>(m , -1));
    int num = solve(n-1,m-1,s1,s2,dp);
    return num;
}


