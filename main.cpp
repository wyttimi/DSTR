#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "Job.hpp"
#include "Resume.hpp"
#include "CandidateMatch.hpp"
#include "ArrayContainerJob.hpp"
#include "ArrayContainerResume.hpp"
#include "ArrayContainerCandidate.hpp"
#include "LinkedListContainerJob.hpp"
#include "LinkedListContainerResume.hpp"
#include "LinkedListContainerCandidate.hpp"
#include "Utils.hpp"
#include "JobMatcher.hpp"

using namespace std;

// --- Memory estimation helpers ---
size_t estimateArrayMemoryJobs(ArrayContainerJob& arr) {
    return arr.getSize() * sizeof(Job);
}
size_t estimateArrayMemoryResumes(ArrayContainerResume& arr) {
    return arr.getSize() * sizeof(Resume);
}
size_t estimateArrayMemoryCandidates(ArrayContainerCandidate& arr) {
    return arr.getSize() * sizeof(CandidateMatch);
}
size_t estimateLinkedListMemoryCandidates(LinkedListContainerCandidate& list) {
    return list.getSize() * (sizeof(CandidateMatch) + sizeof(void*));
}

int main() {
    // =================== DATA STRUCTURES ===================
    ArrayContainerJob jobsArray;
    ArrayContainerResume resumesArray;
    LinkedListContainerJob jobsList;
    LinkedListContainerResume resumesList;

    // =================== LOAD JOB DATA ===================
    ifstream jobFile("job_description.csv");
    if (!jobFile.is_open()) {
        cerr << "Error: job_description.csv not found!" << endl;
        return 1;
    }

    string line;
    int id = 1;
    while (getline(jobFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) continue; // malformed line

        Job jb(id++, line, "");
        jobsArray.insert(jb);
        jobsList.insert(jb);
    }

    // =================== LOAD RESUME DATA ===================
    ifstream resFile("resume.csv");
    if (!resFile.is_open()) {
        cerr << "Error: resume.csv not found!" << endl;
        return 1;
    }

    int rid = 1;
    while (getline(resFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) continue; // malformed line

        Resume rs(rid++, line);
        resumesArray.insert(rs);
        resumesList.insert(rs);
    }

    // =================== USER INPUT ===================
    cout << "===== Job → Candidate Matching =====" << endl;
    cout << "Enter a job title or skill: ";
    string keyword;
    getline(cin, keyword);
    keyword = toLowerStr(keyword);

    bool jobFound = false;

    // ---------------- ARRAY VERSION ----------------
    ArrayContainerCandidate arrCandidates;
    int totalMatchesArray = 0;
    double arrMatchTime = 0, arrSortTime = 0, arrSearchTime = 0;
    size_t arrMemory = 0;

    for (int j = 0; j < jobsArray.getSize(); j++) {
        Job jb = jobsArray.get(j);
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            jobFound = true;
            cout << "\n[Array] Job: \"" << jb.getDescription() << "\"" << endl;

            clock_t startMatch = clock();

            for (int r = 0; r < resumesArray.getSize(); r++) {
                Resume res = resumesArray.get(r);
                int score = JobMatcher::calculateScore(jb, res);
                if (score > 0) arrCandidates.insert({res, score});
            }

            clock_t endMatch = clock();
            arrMatchTime = double(endMatch - startMatch) / CLOCKS_PER_SEC;

            // Sort by score
            clock_t startSort = clock();
            arrCandidates.bubbleSortByScore();
            clock_t endSort = clock();
            arrSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            arrSearchTime = arrMatchTime + arrSortTime;
            arrMemory = estimateArrayMemoryCandidates(arrCandidates);
            totalMatchesArray = arrCandidates.getSize();

            // Print Top 5
            for (int i = 0; i < arrCandidates.getSize() && i < 5; i++) {
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
    LinkedListContainerCandidate listCandidates;
    int totalMatchesList = 0;
    double listMatchTime = 0, listSortTime = 0, listSearchTime = 0;
    size_t listMemory = 0;

    auto* jobNode = jobsList.getHead();
    while (jobNode) {
        Job jb = jobNode->data;
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            cout << "\n[LinkedList] Job: \"" << jb.getDescription() << "\"" << endl;

            clock_t startMatch = clock();
            auto* resNode = resumesList.getHead();
            while (resNode) {
                Resume res = resNode->data;
                int score = JobMatcher::calculateScore(jb, res);
                if (score > 0) listCandidates.insert({res, score});
                resNode = resNode->next;
            }
            clock_t endMatch = clock();
            listMatchTime = double(endMatch - startMatch) / CLOCKS_PER_SEC;

            // Sort by score
            clock_t startSort = clock();
            listCandidates.bubbleSortByScore();
            clock_t endSort = clock();
            listSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            listSearchTime = listMatchTime + listSortTime;
            listMemory = estimateLinkedListMemoryCandidates(listCandidates);
            totalMatchesList = listCandidates.getSize();

            // Print Top 5
            auto* candNode = listCandidates.getHead();
            int shown = 0;
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

    // =================== PERFORMANCE SUMMARY ===================
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
