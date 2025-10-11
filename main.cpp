#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <algorithm>
#include <iomanip>
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

// ===== Helper: exact word boundary check =====
bool isWordMatch(const string &text, const string &word) {
    if (word.empty() || text.empty()) return false;
    string lowerText = toLowerStr(text);
    string lowerWord = toLowerStr(word);
    size_t pos = 0;
    while ((pos = lowerText.find(lowerWord, pos)) != string::npos) {
        bool startOk = (pos == 0) || !isalnum(lowerText[pos - 1]);
        bool endOk = (pos + lowerWord.length() >= lowerText.length()) ||
                     !isalnum(lowerText[pos + lowerWord.length()]);
        if (startOk && endOk) return true;
        pos++;
    }
    return false;
}

// ===== Job / Resume Relevance =====
int relevanceScore(const string &text, const string &keyword) {
    string lowerText = toLowerStr(text);
    stringstream ss(keyword);
    string w;
    int score = 0;
    while (ss >> w) {
        w.erase(remove_if(w.begin(), w.end(), ::ispunct), w.end());
        w = toLowerStr(trim(w));
        if (w.empty() || isStopword(w)) continue;
        if (isWordMatch(lowerText, w)) score++;
    }
    return score;
}

// ===== Memory Estimation =====
size_t estimateArrayMemory(ArrayContainerCandidate &arr) {
    return arr.getSize() * sizeof(CandidateMatch);
}
size_t estimateLinkedMemory(LinkedListContainerCandidate &list) {
    return list.getSize() * (sizeof(CandidateMatch) + sizeof(void *));
}

// ===== Sort helper =====
void sortByScoreDesc(Job jobs[], int scores[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (scores[j] < scores[j + 1]) {
                swap(scores[j], scores[j + 1]);
                swap(jobs[j], jobs[j + 1]);
            }
}
void sortByScoreDesc(Resume resumes[], int scores[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (scores[j] < scores[j + 1]) {
                swap(scores[j], scores[j + 1]);
                swap(resumes[j], resumes[j + 1]);
            }
}

// ===== MAIN =====
int main() {
    while (true) {  // Loop entire program until user chooses Quit
        ArrayContainerJob jobsArray;
        ArrayContainerResume resumesArray;
        LinkedListContainerJob jobsList;
        LinkedListContainerResume resumesList;

        // ==== LOAD FILES ====
        ifstream jobFile("cleaned_jobs.csv");
        if (!jobFile.is_open()) { cerr << "Error: cleaned_jobs.csv not found!\n"; return 1; }
        string line;
        while (getline(jobFile, line)) {
            if (line.size() < 5) continue;
            Job j(line); jobsArray.insert(j); jobsList.insert(j);
        }

        ifstream resFile("cleaned_resumes.csv");
        if (!resFile.is_open()) { cerr << "Error: cleaned_resumes.csv not found!\n"; return 1; }
        while (getline(resFile, line)) {
            if (line.size() < 5) continue;
            Resume r(line); resumesArray.insert(r); resumesList.insert(r);
        }

        // ==== MODE SELECTION ====
        cout << "============================== Matching System ==============================\n";
        int mode;
        while (true) {
            cout << "Select mode:\n1. Job Seeker (Find Resumes for a Job)\n2. Candidate Seeker (Find Jobs for a Resume)\n3. Quit\nEnter choice (1/2/3): ";
            if (cin >> mode && (mode == 1 || mode == 2 || mode == 3)) {
                cin.ignore();
                break;
            } else {
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        if (mode == 3) {
            cout << "\nExit Matching System.\n";
            break;  // exit whole program
        }

        cout << (mode == 1 ? "\nEnter job title or keywords: " : "\nEnter candidate's skills or keywords: ");
        string keyword; getline(cin, keyword); keyword = toLowerStr(keyword);

        int choice;
        while (true) {
            cout << "\nChoose which results to display:\n"
                 << "1. Top 10 matches\n"
                 << "2. Average 10 matches\n"
                 << "3. Lowest 10 matches\n"
                 << "Enter choice (1/2/3): ";
            if (cin >> choice && (choice >= 1 && choice <= 3)) {
                cin.ignore();
                break;
            } else {
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // ==== FIND MOST RELEVANT JOB OR RESUME ====
        Job bestJob; Resume bestResume; int bestScore = 0;

        if (mode == 1) {
            for (int i = 0; i < jobsArray.getSize(); i++) {
                int sc = relevanceScore(jobsArray.get(i).getDescription(), keyword);
                if (sc > bestScore) { bestScore = sc; bestJob = jobsArray.get(i); }
            }
            if (bestScore == 0) { cout << "\nNo job found for input.\n"; continue; }
            cout << "\n============================== Selected Job ==============================\n" << bestJob.getDescription() << " [Relevance: " << bestScore << "]\n";
        } else {
            for (int i = 0; i < resumesArray.getSize(); i++) {
                int sc = relevanceScore(resumesArray.get(i).getDescription(), keyword);
                if (sc > bestScore) { bestScore = sc; bestResume = resumesArray.get(i); }
            }
            if (bestScore == 0) { cout << "\nNo resume found for input.\n"; continue; }
            cout << "\n============================== Selected Resume ==============================\n" << bestResume.getDescription() << " [Relevance: " << bestScore << "]\n";
        }

    // ==== PERFORMANCE TABLE ====
    struct Summary { string ds, sortAlgo, searchAlgo; double matchT, sortT, searchT; size_t mem; } results[4];
    int resCount = 0;

    // ==== PRINTERS ====
    auto printArray = [&](ArrayContainerCandidate &c) {
        int total = c.getSize();
        if (total == 0) { cout << "   No matches found.\n"; return; }
        int start = 0, end = 0;
        if (choice == 1) { start = 0; end = min(total, 10); }
        else if (choice == 2) { int mid = total / 2; start = max(0, mid - 5); end = min(total, start + 10); }
        else { start = max(0, total - 10); end = total; }

        for (int i = start; i < end; i++) {
            if (mode == 1)
                cout << "   Candidate " << (i - start + 1) << ": " << c.get(i).resume.getDescription()
                     << " [Score: " << c.get(i).score << "]\n";
            else
                cout << "   Job " << (i - start + 1) << ": " << c.get(i).job.getDescription()
                     << " [Score: " << c.get(i).score << "]\n";
        }
    };

    auto printList = [&](LinkedListContainerCandidate &l) {
        int total = l.getSize();
        if (total == 0) { cout << "   No matches found.\n"; return; }

        int start = 0, end = 0;
        if (choice == 1) { start = 0; end = min(total, 10); }
        else if (choice == 2) { int mid = total / 2; start = max(0, mid - 5); end = min(total, start + 10); }
        else { start = max(0, total - 10); end = total; }

        auto *n = l.getHead(); int i = 0;
        while (n) {
            if (i >= start && i < end) {
                if (mode == 1)
                    cout << "   Candidate " << (i - start + 1) << ": " << n->data.resume.getDescription()
                         << " [Score: " << n->data.score << "]\n";
                else
                    cout << "   Job " << (i - start + 1) << ": " << n->data.job.getDescription()
                         << " [Score: " << n->data.score << "]\n";
            }
            n = n->next; i++;
        }
    };

    // ==== MAIN ALGO LOOP ====
    for (int algo = 1; algo <= 4; algo++) {
        string ds, sortA, searchA;
        if (algo == 1) { ds = "Array"; sortA = "Bubble"; searchA = "Linear"; }
        if (algo == 2) { ds = "Array"; sortA = "Merge";  searchA = "Binary"; }
        if (algo == 3) { ds = "LinkedList"; sortA = "Bubble"; searchA = "Linear"; }
        if (algo == 4) { ds = "LinkedList"; sortA = "Merge";  searchA = "Binary"; }

        cout << "\n============================== " << algo << ". " << ds << " + " << sortA << " Sort + " << searchA << " Search ==============================\n";

        double tMatch = 0, tSort = 0, tSearch = 0; size_t mem = 0;

        // === ARRAY VERSION ===
        if (ds == "Array") {
            ArrayContainerCandidate cand;
            clock_t s1 = clock();
            if (mode == 1) {
                for (int r = 0; r < resumesArray.getSize(); r++) {
                    int sc = JobMatcher::calculateScore(bestJob, resumesArray.get(r));
                    if (sc > 0) cand.insert({resumesArray.get(r), sc});
                }
            } else {
                for (int j = 0; j < jobsArray.getSize(); j++) {
                    int sc = JobMatcher::calculateScore(jobsArray.get(j), bestResume);
                    if (sc > 0) cand.insert({jobsArray.get(j), sc});
                }
            }
            clock_t e1 = clock(); tMatch = double(e1 - s1) / CLOCKS_PER_SEC;

            // prevent crash if empty
            if (cand.getSize() == 0) { cout << "No matching " << (mode == 1 ? "candidates" : "jobs") << " found.\n"; continue; }

            clock_t s2 = clock(); 
            if (sortA == "Bubble") cand.bubbleSortByScore(); else cand.mergeSortByScore();
            clock_t e2 = clock(); tSort = double(e2 - s2) / CLOCKS_PER_SEC;

            clock_t s3 = clock(); 
            if (searchA == "Linear") cand.linearSearchByScore(cand.get(0).score);
            else cand.binarySearchByScore(cand.get(0).score);
            clock_t e3 = clock(); tSearch = double(e3 - s3) / CLOCKS_PER_SEC;

            printArray(cand);
            cout << "   → Total " << (mode == 1 ? "candidates" : "jobs") << " matched: " << cand.getSize() << "\n";
            mem = estimateArrayMemory(cand);
            cand.clear();
        }

        // === LINKED LIST VERSION ===
        else {
            LinkedListContainerCandidate cand;
            clock_t s1 = clock();
            if (mode == 1) {
                auto *n = resumesList.getHead();
                while (n) {
                    int sc = JobMatcher::calculateScore(bestJob, n->data);
                    if (sc > 0) cand.insert({n->data, sc});
                    n = n->next;
                }
            } else {
                auto *n = jobsList.getHead();
                while (n) {
                    int sc = JobMatcher::calculateScore(n->data, bestResume);
                    if (sc > 0) cand.insert({n->data, sc});
                    n = n->next;
                }
            }
            clock_t e1 = clock(); tMatch = double(e1 - s1) / CLOCKS_PER_SEC;

            if (cand.getSize() == 0) { cout << "No matching " << (mode == 1 ? "candidates" : "jobs") << " found.\n"; continue; }

            clock_t s2 = clock(); 
            if (sortA == "Bubble") cand.bubbleSortByScore(); else cand.mergeSortByScore();
            clock_t e2 = clock(); tSort = double(e2 - s2) / CLOCKS_PER_SEC;

            clock_t s3 = clock(); 
            if (searchA == "Linear") cand.linearSearchByScore(cand.getHead()->data.score);
            else cand.binarySearchByScore(cand.getHead()->data.score);
            clock_t e3 = clock(); tSearch = double(e3 - s3) / CLOCKS_PER_SEC;

            printList(cand);
            cout << "   → Total " << (mode == 1 ? "candidates" : "jobs") << " matched: " << cand.getSize() << "\n";
            mem = estimateLinkedMemory(cand);
            cand.clear();
        }

        results[resCount++] = {ds, sortA, searchA, tMatch, tSort, tSearch, mem};
    }

    // ==== SUMMARY ====
    cout << "\n================================== Final Performance Summary ===================================\n";
    cout << left << setw(15) << "Data Structure" << setw(12) << "Sort" << setw(12)
         << "Search" << setw(12) << "Match(s)" << setw(12) << "Sort(s)" << setw(12)
         << "Search(s)" << "Memory(bytes)\n";
    cout << string(80, '-') << endl;

    for (int i = 0; i < resCount; i++) {
        cout << left << setw(15) << results[i].ds << setw(12) << results[i].sortAlgo
             << setw(12) << results[i].searchAlgo << setw(12) << fixed << setprecision(6)
             << results[i].matchT << setw(12) << results[i].sortT
             << setw(12) << results[i].searchT << results[i].mem << endl;
    }

    cout << "\n=============================== Execution completed successfully ===============================\n";
    cout << "\nReturning to main menu...\n\n";
    }
    
    return 0;
}
