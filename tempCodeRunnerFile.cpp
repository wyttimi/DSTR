#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "Job.hpp"
#include "Resume.hpp"
#include "ArrayContainer.hpp"
#include "LinkedListContainer.hpp"
#include "Utils.hpp"
#include "JobMatcher.hpp"

using namespace std;

// Helper struct for candidate matching
struct CandidateMatch {
    Resume resume;
    int score;
};

// --- Tokenizer: split by any non-alphanumeric character ---
ArrayContainer<string> splitTokens(const string &line) {
    ArrayContainer<string> tokens;
    string token;
    for (char c : line) {
        if (isalnum(c)) token += tolower(c);
        else {
            if (!token.empty()) {
                tokens.insert(token);
                token.clear();
            }
        }
    }
    if (!token.empty()) tokens.insert(token);
    return tokens;
}

int main() {
    ArrayContainer<Job> jobsArray;
    ArrayContainer<Resume> resumesArray;
    LinkedListContainer<Job> jobsList;
    LinkedListContainer<Resume> resumesList;

    // Load job data (1 column only)
    ifstream jobFile("job_description.csv");
    if (!jobFile.is_open()) {
        cerr << "Error: job_description.csv not found!" << endl;
        return 1;
    }
    string line;
    int id = 1;
    while (getline(jobFile, line)) {
        if (line.empty()) continue;
        Job jb(id++, line, "");   // whole line is the job text
        jobsArray.insert(jb);
        jobsList.insert(jb);
    }

    // Load resumes (1 column only)
    ifstream resFile("resume.csv");
    if (!resFile.is_open()) {
        cerr << "Error: resume.csv not found!" << endl;
        return 1;
    }
    int rid = 1;
    while (getline(resFile, line)) {
        if (line.empty()) continue;
        Resume rs(rid++, line);
        resumesArray.insert(rs);
        resumesList.insert(rs);
    }

    cout << "===== Job → Candidate Matching =====" << endl;
    cout << "Enter a job title or skill: ";
    string keyword;
    getline(cin, keyword);
    keyword = toLowerStr(keyword);

    bool jobFound = false;

    // ---------------- ARRAY VERSION ----------------
    for (int j = 0; j < jobsArray.getSize(); j++) {
        Job jb = jobsArray.get(j);
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            jobFound = true;
            cout << "\n[Array] Job: \"" << jb.getTitle() << "\"" << endl;

            clock_t start = clock();
            ArrayContainer<CandidateMatch> arrCandidates;

            // Score each resume
            for (int r = 0; r < resumesArray.getSize(); r++) {
                Resume res = resumesArray.get(r);

                int score = JobMatcher::calculateScore(jb, res);

                if (score > 0) arrCandidates.insert({res, score});
            }
            clock_t end = clock();
            double arrTime = double(end - start) / CLOCKS_PER_SEC;

            // --- Sort by score (Bubble Sort) ---
            for (int a = 0; a < arrCandidates.getSize() - 1; a++) {
                for (int b = 0; b < arrCandidates.getSize() - a - 1; b++) {
                    if (arrCandidates.get(b).score < arrCandidates.get(b + 1).score) {
                        CandidateMatch temp = arrCandidates.get(b);
                        arrCandidates.get(b) = arrCandidates.get(b + 1);
                        arrCandidates.get(b + 1) = temp;
                    }
                }
            }

            // --- Print Top 5 ---
            int shown = 0;
            for (int i = 0; i < arrCandidates.getSize() && shown < 5; i++, shown++) {
                cout << "   -> Candidate Resume: " 
                    << arrCandidates.get(i).resume.getDescription() << endl;
                cout << "      Score: " << arrCandidates.get(i).score << endl;

            }
            cout << "   [Total Matches: " << arrCandidates.getSize()
                 << " | Time: " << arrTime << " sec]" << endl;
        }
    }

    // ---------------- LINKEDLIST VERSION ----------------
    auto* jobNode = jobsList.getHead();
    while (jobNode) {
        Job jb = jobNode->data;
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            cout << "\n[LinkedList] Job: \"" << jb.getTitle() << "\"" << endl;

            clock_t start = clock();
            LinkedListContainer<CandidateMatch> listCandidates;

            auto* resNode = resumesList.getHead();
            while (resNode) {
                Resume res = resNode->data;
                int score = JobMatcher::calculateScore(jb, res);

                if (score > 0) listCandidates.insert({res, score});
                resNode = resNode->next;
            }
            clock_t end = clock();
            double listTime = double(end - start) / CLOCKS_PER_SEC;

            // --- Print Top 5 ---
            int shown = 0;
            auto* candNode = listCandidates.getHead();
            while (candNode && shown < 5) {
                cout << "   -> Candidate Resume: " 
                    << candNode->data.resume.getDescription() << endl;
                cout << "      Score: " << candNode->data.score << endl;

                candNode = candNode->next;
                shown++;
            }
            cout << "   [Total Matches: " << listCandidates.getSize()
                << " | Time: " << listTime << " sec]" << endl;
        }
        jobNode = jobNode->next;
    }



    if (!jobFound) {
        cout << "No job found matching: " << keyword << endl;
    }

    return 0;
}
