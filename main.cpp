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
#include "CandidateMatch.hpp"

using namespace std;

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

// --- Memory estimation helpers ---
template <typename T>
size_t estimateArrayMemory(ArrayContainer<T>& arr) {
    return arr.getSize() * sizeof(T);
}

template <typename T>
size_t estimateLinkedListMemory(LinkedListContainer<T>& list) {
    return list.getSize() * (sizeof(T) + sizeof(void*));
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
        // Robust validation
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) {
            cerr << "Warning: Skipping malformed job line: \"" << line << "\"" << endl;
            continue;
        }

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
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) {
            cerr << "Warning: Skipping malformed resume line: \"" << line << "\"" << endl;
            continue;
        }

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
    int totalMatchesArray = 0;
    double arrMatchTime = 0, arrSortTime = 0, arrSearchTime = 0;
    size_t arrMemory = 0;

    for (int j = 0; j < jobsArray.getSize(); j++) {
        Job jb = jobsArray.get(j);
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            jobFound = true;
            cout << "\n[Array] Job: \"" << jb.getTitle() << "\"" << endl;

            clock_t startMatch = clock();
            ArrayContainer<CandidateMatch> arrCandidates;

            // Score each resume
            for (int r = 0; r < resumesArray.getSize(); r++) {
                Resume res = resumesArray.get(r);
                int score = JobMatcher::calculateScore(jb, res);
                if (score > 0) arrCandidates.insert({res, score});
            }
            clock_t endMatch = clock();
            arrMatchTime = double(endMatch - startMatch) / CLOCKS_PER_SEC;

            // --- Sort by score (Bubble Sort, highest first) ---
            clock_t startSort = clock();
            arrCandidates.bubbleSortByScore();
            clock_t endSort = clock();
            arrSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            arrSearchTime = arrMatchTime + arrSortTime;
            arrMemory = estimateArrayMemory(arrCandidates);
            totalMatchesArray = arrCandidates.getSize();

            // --- Print Top 5 ---
            int shown = 0;
            for (int i = 0; i < arrCandidates.getSize() && shown < 5; i++, shown++) {
                cout << "   -> Candidate Resume: \"" 
                    << arrCandidates.get(i).resume.getDescription() << "\"" << endl;
                cout << "      Score: " << arrCandidates.get(i).score << endl;
            }
            cout << "   [Total Matches: " << totalMatchesArray
                << " | Match Time: " << arrMatchTime << "s"
                << " | Sort Time: " << arrSortTime << "s]" << endl;
            cout << "[Array] Search Time: " << arrSearchTime << "s" << endl;
        }
    }

    // ---------------- LINKEDLIST VERSION ----------------
    int totalMatchesList = 0;
    double listMatchTime = 0, listSortTime = 0, listSearchTime = 0;
    size_t listMemory = 0;

    auto* jobNode = jobsList.getHead();
    while (jobNode) {
        Job jb = jobNode->data;
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            cout << "\n[LinkedList] Job: \"" << jb.getTitle() << "\"" << endl;

            clock_t startMatch = clock();
            LinkedListContainer<CandidateMatch> listCandidates;

            auto* resNode = resumesList.getHead();
            while (resNode) {
                Resume res = resNode->data;
                int score = JobMatcher::calculateScore(jb, res);
                if (score > 0) listCandidates.insert({res, score});
                resNode = resNode->next;
            }
            clock_t endMatch = clock();
            listMatchTime = double(endMatch - startMatch) / CLOCKS_PER_SEC;

            // --- Sort by score (Bubble Sort) ---
            clock_t startSort = clock();
            listCandidates.bubbleSortByScore();
            clock_t endSort = clock();
            listSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            listSearchTime = listMatchTime + listSortTime;
            listMemory = estimateLinkedListMemory(listCandidates);
            totalMatchesList = listCandidates.getSize();

            // --- Print Top 5 ---
            int shown = 0;
            auto* candNode = listCandidates.getHead();
            while (candNode && shown < 5) {
                cout << "   -> Candidate Resume: \"" 
                    << candNode->data.resume.getDescription() << "\"" << endl;
                cout << "      Score: " << candNode->data.score << endl;
                candNode = candNode->next;
                shown++;
            }
            cout << "   [Total Matches: " << totalMatchesList
                << " | Match Time: " << listMatchTime << "s"
                << " | Sort Time: " << listSortTime << "s]" << endl;
            cout << "[LinkedList] Search Time: " << listSearchTime << "s" << endl;
        }
        jobNode = jobNode->next;
    }

    // ---------------- PERFORMANCE COMPARISON TABLE ----------------
    if (jobFound) {
        cout << "\n===== Performance Comparison =====" << endl;
        cout << "Structure     | Matches | Match Time | Sort Time | Search Time | Memory (bytes)" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "Array         | " << totalMatchesArray
            << "      | " << arrMatchTime << "s | " << arrSortTime
            << "s | " << arrSearchTime << "s | " << arrMemory << endl;

        cout << "LinkedList    | " << totalMatchesList
            << "      | " << listMatchTime << "s | " << listSortTime
            << "s | " << listSearchTime << "s | " << listMemory << endl;
    }

    if (!jobFound) {
        cout << "No job found matching: " << keyword << endl;
    }

    return 0;
}
