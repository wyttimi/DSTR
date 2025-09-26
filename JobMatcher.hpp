#ifndef JOBMATCHER_HPP
#define JOBMATCHER_HPP

#include "Job.hpp"
#include "Resume.hpp"
#include <sstream>
#include <set>

class JobMatcher {
public:
    static int calculateScore(Job job, Resume resume) {
        set<string> jobWords = tokenize(job.description);
        set<string> resumeWords = tokenize(resume.description);

        int score = 0;
        for (auto &w : resumeWords) {
            if (jobWords.find(w) != jobWords.end()) {
                score++;
            }
        }
        return score;
    }

private:
    static set<string> tokenize(string text) {
        set<string> words;
        stringstream ss(text);
        string word;
        while (ss >> word) {
            words.insert(word);
        }
        return words;
    }
};

#endif
