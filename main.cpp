#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <gumbo.h>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include "crawler_index.h"
#include "words.h"

using namespace std;


int main() {
    make_it();
    init();
    cout << "Welcome to our search engine, <3.\n";


    cout << "To close program type (-1)\n";
    cout << "you can write what you want to search about here: ";
    // taking search query from the user
    string user_text;
    getline(cin, user_text);

    while (user_text != "-1") {

        for (int j = 0; j < user_text.size(); ++j) {
            if ((user_text[j] < 'a' || user_text[j] > 'z') && (user_text[j] < 'A' || user_text[j] > 'Z'))
                user_text[j] = ' ';
        }

        stringstream words(user_text);
        string word;
        vector<string> user_req;
        while (words >> word) {
            int ans = 1e9;
            for (int j = 0; j < (int) word.size(); ++j) {

                word[j] = tolower(word[j]);
            }
            while (word.back() < 'a' || word.back() > 'z')word.pop_back();
            if (stopWords.count(word) || word.size() == 0)continue;

            //correct
            string temp;
            for (auto &it: englishWords) {
                int dif = correct(word, it);
                if (dif < ans) {
                    ans = dif;
                    temp = it;
                }
            }
            word = temp;



            // change word to its root
            word = root(word);
            user_req.push_back(word);
        }


        map<int, int> freq_words;
        for (auto search_index: user_req) {

            auto _setOfPages = invertedIndex.whereWordExist(search_index);
            for (auto [page,freq]: _setOfPages) {
                freq_words[page]+=freq;
            }
        }
        vector<pair<int, int>> to_sort;
        for (auto [page, freq]: freq_words) {
            to_sort.push_back({freq, page});
        }
        sort(to_sort.rbegin(), to_sort.rend());
        int num_of_page = 1;

        for (int i = 0; i < to_sort.size(); ++i) {
            string link = id[to_sort[i].second];
            cout << num_of_page++ << "- ";
            cout << link << "\n\n";
        }

        cout << "To close program type (-1)\n";
        cout << "you can write what you want to search about here: ";
        getline(cin, user_text);
    }

    cout << "Tank U for using our search engine.\n";


    return 0;
}
