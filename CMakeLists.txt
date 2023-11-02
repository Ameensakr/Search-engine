cmake_minimum_required(VERSION 3.26)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(search_engine main.cpp
        main.cpp
        stopwords.h
        trie.h
        crawler_index.h
        words.h)
