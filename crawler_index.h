#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <gumbo.h>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include "stopwords.h"
#include "traverse.h"
#include "trie.h"

using namespace std;
int cnt = 1;
map<int, string> id;

const string http = "http";


const int LIMIT_LINKS = 100 , LIMIT_WORDS = 100000;
const int LIMIT_OF_WORDS_IN_PAGE = 1000;
int number_of_taken_words_in_page = 0;
int numberOfTakenLinks = 0;
vector<string> links;
string title ;
trie invertedIndex; // for every word stored the number of pages appers in it
int numberOfCurrentWords = 0;
string extractPlainText(GumboNode *node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return node->v.text.text;
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {

        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            string text = extractPlainText(static_cast<GumboNode *>(children->data[i]));
            for (int j = 0; j < text.size(); ++j) {
                if ((text[j] < 'a' || text[j] > 'z') && (text[j] < 'A' || text[j] > 'Z'))text[j] = ' ';
            }
            stringstream words(text);
            string word;
            while (words >> word) {
                for (int j = 0; j < (int) word.size(); ++j) {

                    word[j] = tolower(word[j]);
                }
                while (word.back() < 'a' || word.back() > 'z')word.pop_back();
                if (stopWords.count(word) || word.size() == 0)continue;
                // change word to its root
                word = root(word);
                // store it
                if(numberOfCurrentWords > LIMIT_WORDS||number_of_taken_words_in_page>LIMIT_OF_WORDS_IN_PAGE)break;
                    invertedIndex.insert(word , 0 , cnt);
                    number_of_taken_words_in_page++;
                    numberOfCurrentWords++;



            }
        }
        return "";
    } else {
        return "";
    }
}

void extractLinks(GumboNode *node) {

    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    if (node->v.element.tag == GUMBO_TAG_A) {
        GumboAttribute *href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href && links.size() < LIMIT_LINKS) {
            string link = href->value;
            if(link.substr(0 , 4) == http)
                links.push_back(link);
        } else if (links.size() >= LIMIT_LINKS||numberOfCurrentWords>=LIMIT_WORDS)return;
    }
    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        extractLinks(static_cast<GumboNode *>(children->data[i]));
    }
}


void ExtractTitle(const GumboNode* node) {
    if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag == GUMBO_TAG_TITLE) {
        if (node->v.element.children.length > 0) {
            const GumboNode* text = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if (text->type == GUMBO_NODE_TEXT) {
                title = text->v.text.text;
            }
        }
    }

    if (node->type == GUMBO_NODE_ELEMENT) {
        const GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            ExtractTitle(static_cast<GumboNode*>(children->data[i]));
        }
    }
}

// Callback function for libcurl to write response data into a string.
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}


void make_it() {
    queue<string> q;
    q.push("https://en.wikipedia.org/wiki/Food");
    string start = q.front();
    set<string>s;
    s.insert(start);
    while (!q.empty()) {

        string currLink = q.front();
        q.pop();
        numberOfTakenLinks++;
        CURL *curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize libcurl." << std::endl;
            return;
        }

        std::string webpageUrl = currLink;  // Replace with the URL of the webpage you want to fetch.
        // Set the URL for the HTTP request.
        curl_easy_setopt(curl, CURLOPT_URL, webpageUrl.c_str());

        // Set the callback function for response data.
        std::string responseBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the HTTP request.
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            continue;
        }
        // Parse HTML using Gumbo.
        GumboOutput *output = gumbo_parse(responseBuffer.c_str());
        if (output) {
            // Traverse the GumboOutput to extract links.
            ExtractTitle(output->root);

            id[cnt] = (string)(title + ": " + currLink);

            extractLinks(output->root);
            number_of_taken_words_in_page=0;
            string temp = extractPlainText(output->root);
            cnt++;
            gumbo_destroy_output(&kGumboDefaultOptions, output);
        }

        // Cleanup libcurl.
        curl_easy_cleanup(curl);

        for (auto it: links) {
            if (!s.count(it ) && numberOfTakenLinks < LIMIT_LINKS||numberOfCurrentWords<LIMIT_WORDS)
                q.push(it), s.insert(it );
        }
        if(numberOfTakenLinks > LIMIT_LINKS||numberOfCurrentWords>LIMIT_WORDS)break;

        links.clear();
        links.resize(0);
    }


}

