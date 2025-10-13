#ifndef JOBMATCHER_HPP
#define JOBMATCHER_HPP

#include "Job.hpp"
#include "Resume.hpp"
#include "Utils.hpp"
#include <sstream>
#include <set>
#include <algorithm>

struct JobMatcher {
    static int calculateScore(Job job, Resume resume) {
        set<string> jobWords = tokenize(job.getDescription());
        set<string> resumeWords = tokenize(resume.getDescription());

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
            // Use global helpers
            word = toLowerStr(trim(word));

            // remove punctuation
            word.erase(remove_if(word.begin(), word.end(),
                                 [](char c){ return ispunct(c); }),
                       word.end());

            if (!word.empty() && !isStopword(word)) {
                words.insert(word);
            }
        }
        return words;
    }
};

#endif
