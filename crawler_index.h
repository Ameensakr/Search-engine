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

using namespace std;
int cnt = 1;
map<int, string> id;
map<string, set<int>> inverted_index;
const string http = "http";


const int LIMIT = 100 , LIMIT_WORDS = 500;
vector<string> links;
string tii ;

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
                inverted_index[word].insert(cnt);

                if(inverted_index.size() > LIMIT_WORDS)break;
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
        if (href && links.size() < LIMIT) {
            string link = href->value;
            if(link.substr(0 , 4) == http)
                links.push_back(link);
        } else if (links.size() >= LIMIT)return;
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
                tii = text->v.text.text;
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
    q.push("https://en.wikipedia.org/wiki/Football");
    set<string> s;
    s.insert("https://en.wikipedia.org/wiki/Football");
    while (!q.empty()) {

        string currLink = q.front();



        q.pop();
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


            id[cnt] = (string)(tii + ": " + currLink);
            cnt++;

            extractLinks(output->root);


            string temp = extractPlainText(output->root);

            gumbo_destroy_output(&kGumboDefaultOptions, output);
        }

        // Cleanup libcurl.
        curl_easy_cleanup(curl);

        for (auto it: links) {
            if (!s.count(it) && s.size() < LIMIT)
                q.push(it), s.insert(it);
        }
        links.clear();
        links.resize(0);
    }


}

