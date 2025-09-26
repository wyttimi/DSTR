#ifndef JOBMATCHER_HPP
#define JOBMATCHER_HPP

#include "Job.hpp"
#include "Resume.hpp"
#include <sstream>
#include <set>
#include <algorithm>

class JobMatcher {
public:
    static int calculateScore(Job job, Resume resume) {
        set<string> jobWords = tokenize(job.getTitle() + " " + job.getDescription());
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

        // --- common stopwords we ignore ---
        set<string> stopwords = {
            "in","with","and","or","needed","experience",
            "professional","skilled","for","to","of","the","a","an",
            "is","are","on","as","at","by","from","that"
        };

        while (ss >> word) {
            // lowercase
            for (char &c : word) c = tolower(c);

            // strip punctuation
            word.erase(remove_if(word.begin(), word.end(),
                                 [](char c){ return ispunct(c); }),
                       word.end());

            if (!word.empty() && stopwords.find(word) == stopwords.end()) {
                words.insert(word);
            }
        }
        return words;
    }
};

#endif
