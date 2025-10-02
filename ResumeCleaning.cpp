#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// Dictionary of valid skills (lowercase for matching)
vector<string> validSkills = {
    "sql", "excel", "power bi", "tableau", "reporting", "data cleaning",
    "python", "statistics", "machine learning", "deep learning", "nlp", "pandas",
    "git", "docker", "system design", "spring boot", "rest apis", "agile", "scrum",
    "stakeholder management", "product roadmap", "user stories",
    "keras", "tensorflow", "pytorch", "mlops", "computer vision", "cloud", "java"
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

// Remove punctuation characters from start and end
string cleanToken(string s) {
    s = trim(s);
    while (!s.empty() && ispunct(s.back())) s.pop_back();
    while (!s.empty() && ispunct(s.front())) s.erase(0, 1);
    return trim(s);
}

// Check if a word matches a valid skill
bool isSkill(const string &word) {
    string lw = toLowerStr(word);
    for (const string &s : validSkills) {
        if (lw == s) return true;
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

    if (!outfile.is_open()) {
        cerr << "Error: cannot create cleaned_resumes.csv" << endl;
        return 1;
    }

    // Add header
    outfile << "Resume Description\n";

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        size_t pos = line.find("skilled in");
        if (pos == string::npos) continue;

        string skillsPart = line.substr(pos + 10); // after "skilled in"

        // replace '.' with ',' so Tableau. → Tableau,
        for (char &c : skillsPart) {
            if (c == '.' || c == ';') c = ',';
        }

        stringstream ss(skillsPart);
        string token;
        vector<string> cleanSkills;

        while (getline(ss, token, ',')) {
            string original = cleanToken(token); // keep original case
            if (!original.empty() && isSkill(toLowerStr(original))) {
                cleanSkills.push_back(original);
            }
        }

        if (!cleanSkills.empty()) {
            outfile << "Experienced professional skilled in ";
            for (size_t i = 0; i < cleanSkills.size(); i++) {
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
