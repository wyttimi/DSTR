#ifndef JOBMATCHER_HPP
#define JOBMATCHER_HPP

#include "Job.hpp"
#include "Resume.hpp"
#include <sstream>
#include <set>
#include <cctype>
using namespace std;

class JobMatcher {
public:
    static int calculateScore(const Job &job, const Resume &resume) {
        set<string> jobWords = tokenize(job.title + " " + job.description);
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
        string word;
        for (char &c : text) {
            if (isalnum(c)) {
                word += tolower(c);
            } else {
                if (!word.empty()) {
                    words.insert(word);
                    word.clear();
                }
            }
        }
        if (!word.empty()) words.insert(word);
        return words;
    }
};

#endif
