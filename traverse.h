//#include "danish_stem.h"
//#include "dutch_stem.h"
#include "english_stem.h"
//#include "finnish_stem.h"
//#include "french_stem.h"
//#include "german_stem.h"
//#include "italian_stem.h"
//#include "norwegian_stem.h"
//#include "portuguese_stem.h"
//#include "russian_stem.h"
//#include "spanish_stem.h"
//#include "swedish_stem.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <locale>

using namespace std;


string root(string &original_word)
{

    // the word to be stemmed
    //input
    string x= original_word;


    //store in wstring
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring word = converter.from_bytes(x);


    //change it to its root
    stemming::english_stem<> StemEnglish;
    StemEnglish(word);


    //change it to string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter2;
    std::string narrowString = converter.to_bytes(word);

    return narrowString;
}





