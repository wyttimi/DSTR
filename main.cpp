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
    ifstream jobFile("cleaned_jobs.csv");
    if (!jobFile.is_open()) {
        cerr << "Error: cleaned_jobs.csv not found!" << endl;
        return 1;
    }

    string line;
    while (getline(jobFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) continue;
        Job jb(line);
        jobsArray.insert(jb);
        jobsList.insert(jb);
    }

    // =================== LOAD RESUME DATA ===================
    ifstream resFile("cleaned_resumes.csv");
    if (!resFile.is_open()) {
        cerr << "Error: cleaned_resumes.csv not found!" << endl;
        return 1;
    }

    while (getline(resFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        if (line.size() < 5) continue;
        Resume rs(line);
        resumesArray.insert(rs);
        resumesList.insert(rs);
    }

    // =================== USER INPUT ===================
    cout << "===== Job → Candidate Matching =====" << endl;
    cout << "Enter a job title or skill: ";
    string keyword;
    getline(cin, keyword);
    keyword = toLowerStr(keyword);

    cout << "\nChoose which candidates to display:\n";
    cout << "1. Top 5 matches\n";
    cout << "2. Average 5 matches\n";
    cout << "3. Lowest 5 matches\n";
    cout << "Enter choice (1/2/3): ";
    int choice;
    cin >> choice;
    cin.ignore();

    cout << "Logging results to output.txt..." << endl;

    // Redirect output after user input
    freopen("output.txt", "w", stdout);
    freopen("output.txt", "a", stderr);

    bool jobFound = false;

    // ---------------- ARRAY VERSION ----------------
    ArrayContainerCandidate arrCandidates;
    int totalMatchesArray = 0;
    double arrMatchTime = 0, arrSortTime = 0, arrSearchTime = 0;
    size_t arrMemory = 0;

    for (int j = 0; j < jobsArray.getSize(); j++) {
        arrCandidates.clear();

        Job jb = jobsArray.get(j);
        string jobText = toLowerStr(jb.getDescription());

        // --- Match all words ---
        bool matched = true;
        stringstream ss(keyword);
        string word;

        while (ss >> word) {
            // remove punctuation
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            word = toLowerStr(trim(word));

            if (word.empty()) continue;
            if (isStopword(word)) continue; // skip common words like "with", "in", etc.

            // require every important word to be found
            if (jobText.find(word) == string::npos) {
                matched = false;
                break;
            }
        }


        if (matched) {
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

            // Only sort if >1 candidate
            if (arrCandidates.getSize() > 1)
                arrCandidates.quickSortByScore();

            // Sort by score
            clock_t startSort = clock();
            arrCandidates.quickSortByScore();
            clock_t endSort = clock();
            arrSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            arrSearchTime = arrMatchTime + arrSortTime;
            arrMemory = estimateArrayMemoryCandidates(arrCandidates);
            totalMatchesArray = arrCandidates.getSize();

            // Print candidates based on user choice
            if (choice == 1) {
                for (int i = 0; i < arrCandidates.getSize() && i < 5; i++) {
                    cout << "   -> Candidate Resume: \"" 
                         << arrCandidates.get(i).resume.getDescription() << "\"" << endl;
                    cout << "      Score: " << arrCandidates.get(i).score << endl;
                }
            } else if (choice == 2) {
                int mid = arrCandidates.getSize() / 2;
                int start = max(0, mid - 2);
                for (int i = start; i < arrCandidates.getSize() && i < start + 5; i++) {
                    cout << "   -> Candidate Resume: \"" 
                         << arrCandidates.get(i).resume.getDescription() << "\"" << endl;
                    cout << "      Score: " << arrCandidates.get(i).score << endl;
                }
            } else if (choice == 3) {
                for (int i = max(0, arrCandidates.getSize() - 5); i < arrCandidates.getSize(); i++) {
                    cout << "   -> Candidate Resume: \"" 
                         << arrCandidates.get(i).resume.getDescription() << "\"" << endl;
                    cout << "      Score: " << arrCandidates.get(i).score << endl;
                }
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
        listCandidates.clear();

        Job jb = jobNode->data;
        string jobText = toLowerStr(jb.getDescription());

        // Match all words
        bool matched = true;
        stringstream ss(keyword);
        string word;

        while (ss >> word) {
            // remove punctuation
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            word = toLowerStr(trim(word));

            if (word.empty()) continue;
            if (isStopword(word)) continue;

            // require every important word to be found
            if (jobText.find(word) == string::npos) {
                matched = false;
                break;
            }
        }

        if (matched) {
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

            // Only sort if >1 candidate
            if (listCandidates.getSize() > 1)
                listCandidates.quickSortByScore();

            // Sort
            clock_t startSort = clock();
            listCandidates.quickSortByScore();
            clock_t endSort = clock();
            listSortTime = double(endSort - startSort) / CLOCKS_PER_SEC;

            listSearchTime = listMatchTime + listSortTime;
            listMemory = estimateLinkedListMemoryCandidates(listCandidates);
            totalMatchesList = listCandidates.getSize();

            // Print candidates based on user choice
            if (choice == 1) {
                auto* candNode = listCandidates.getHead();
                int shown = 0;
                while (candNode && shown < 5) {
                    cout << "   -> Candidate Resume: \"" 
                         << candNode->data.resume.getDescription() << "\"" << endl;
                    cout << "      Score: " << candNode->data.score << endl;
                    candNode = candNode->next;
                    shown++;
                }
            } else if (choice == 2) {
                int mid = totalMatchesList / 2;
                int start = max(0, mid - 2);
                auto* candNode = listCandidates.getHead();
                int idx = 0, shown = 0;
                while (candNode && shown < 5) {
                    if (idx >= start) {
                        cout << "   -> Candidate Resume: \"" 
                             << candNode->data.resume.getDescription() << "\"" << endl;
                        cout << "      Score: " << candNode->data.score << endl;
                        shown++;
                    }
                    candNode = candNode->next;
                    idx++;
                }
            } else if (choice == 3) {
                int skip = max(0, totalMatchesList - 5);
                auto* candNode = listCandidates.getHead();
                int idx = 0;
                while (candNode) {
                    if (idx >= skip) {
                        cout << "   -> Candidate Resume: \"" 
                             << candNode->data.resume.getDescription() << "\"" << endl;
                        cout << "      Score: " << candNode->data.score << endl;
                    }
                    candNode = candNode->next;
                    idx++;
                }
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

    // Restore output to the terminal (cross-platform)
    fclose(stdout);

    #ifdef _WIN32
        freopen("CON", "w", stdout);   // Windows console
    #else
        freopen("/dev/tty", "w", stdout);   // macOS / Linux terminal
    #endif

    cout << "\n=== Output successfully written to output.txt ===" << endl;

    return 0;
}
