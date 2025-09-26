#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
using namespace std;

inline string toLowerStr(const string &s) {
    string t;
    for (char c : s) t.push_back(tolower(c));
    return t;
}

#endif