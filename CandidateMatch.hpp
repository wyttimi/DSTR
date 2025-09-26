#ifndef CANDIDATEMATCH_HPP
#define CANDIDATEMATCH_HPP

#include <string>
#include "Resume.hpp"
using namespace std;

struct CandidateMatch {
    Resume resume;
    int score;

    // Add comparison helper for sorting
    bool operator<(const CandidateMatch& other) const {
        return score < other.score;
    }
    bool operator>(const CandidateMatch& other) const {
        return score > other.score;
    }
};

#endif
