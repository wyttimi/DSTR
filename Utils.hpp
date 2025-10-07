#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// Convert string to lowercase
inline string toLowerStr(const string &s) {
    string t;
    for (char c : s) t.push_back(tolower(c));
    return t;
}

// Trim spaces and tabs from both ends
inline string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end   = s.find_last_not_of(" \t");
    if (start == string::npos || end == string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

// Check if word is a stopword (common word to ignore)
inline bool isStopword(const string& word) {
    const string stopwords[] = {
        "and", "or", "with", "in", "on", "for", "to", "a", "an", "the"
    };
    const int stopCount = 10;
    for (int i = 0; i < stopCount; i++) {
        if (word == stopwords[i]) return true;
    }
    return false;
}

#endif
