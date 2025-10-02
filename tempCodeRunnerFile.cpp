#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Dictionary of valid skills (lowercase for matching)
vector<string> validSkills = {
    "sql", "excel", "power bi", "tableau", "reporting", "data cleaning",
    "python", "statistics", "machine learning", "deep learning", "nlp", "pandas",
    "git", "docker", "system design", "spring boot", "rest apis", "agile", "scrum",
    "stakeholder management", "product roadmap", "user stories",
    "keras", "tensorflow", "pytorch", "mlops", "computer vision", "cloud"
};

// Lowercase helper
string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Trim spaces
string trim(string s) {
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
    return s;
}

// Clean token (trim + strip punctuation for matching)
string cleanToken(string s) {
    s = trim(s);
    // remove punctuation at start and end
    while (!s.empty() && ispunct(s.back())) s.pop_back();
    while (!s.empty() && ispunct(s.front())) s.erase(0, 1);
    s = trim(s);
    return s;
}

// Check if token is valid skill (ignores case)
bool isSkill(string word) {
    string lowWord = toLowerStr(cleanToken(word));
    for (string s : validSkills) {
        if (lowWord == s) return true;
    }
    return false;
}

int main() {
    ifstream infile("resume.csv");
    ofstream outfile("cleaned_resumes.csv");

    if (!infile.is_open()) {
        cerr << "Error: resume.csv not found!" << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        size_t pos = line.find("skilled in");
        if (pos == string::npos) continue;

        string skillsPart = line.substr(pos + 10); // text after "skilled in"

        stringstream ss(skillsPart);
        string token;
        vector<string> cleanSkills;

        // Split by comma and filter valid skills
        while (getline(ss, token, ',')) {
            string original = trim(cleanToken(token)); // keep original case
            string lower = toLowerStr(original);       // lowercase for matching
            if (isSkill(lower)) {
                cleanSkills.push_back(original);       // keep original case in output
            }
        }

        // Write output line
        if (!cleanSkills.empty()) {
            outfile << "Experienced professional skilled in ";
            for (int i = 0; i < cleanSkills.size(); i++) {
                outfile << cleanSkills[i];
                if (i < cleanSkills.size() - 1) outfile << ", ";
            }
            outfile << "\n";
        }
    }

    infile.close();
    outfile.close();

    cout << "Cleaning resumes done! Saved as resume_clean.csv" << endl;
    return 0;
}
