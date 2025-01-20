#include <iostream>
#include <vector>
#include <string>
#include <fstream>


std::vector<std::string> englishWords;


void init() {
    std::ifstream inputFile("words.txt");
    if (inputFile.is_open()) {
        std::string word;
        while (inputFile >> word) {
            englishWords.push_back(word);
        }
        inputFile.close();

    }
}

int solve(string &s1, string &s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    // Base cases
    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }

    // Fill DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({
                                           dp[i - 1][j],    // deletion
                                           dp[i - 1][j - 1],  // replacement
                                           dp[i][j - 1]     // insertion
                                   });
            }
        }
    }

    return dp[n][m];
}


int correct(string &s1, string &s2) {
    string ret;
    while (!s2.empty() && s2.back() == ' ')s2.pop_back();
    return solve(s1, s2);

}


