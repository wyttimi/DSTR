#ifndef CANDIDATEMATCH_HPP
#define CANDIDATEMATCH_HPP

#include <string>
#include "Job.hpp"
#include "Resume.hpp"
using namespace std;

struct CandidateMatch {
    Job job;         // used when matching Resume → Job
    Resume resume;   // used when matching Job → Resume
    int score;

    // Constructors for both modes
    CandidateMatch() : score(0) {}
    CandidateMatch(const Resume& r, int s) : resume(r), score(s) {}
    CandidateMatch(const Job& j, int s) : job(j), score(s) {}

    // Comparison operators for sorting
    bool operator<(const CandidateMatch& other) const {
        return score < other.score;
    }
    bool operator>(const CandidateMatch& other) const {
        return score > other.score;
    }
};

#endif
