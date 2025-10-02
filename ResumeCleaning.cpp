#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// Fixed size for valid skills dictionary
const int VALID_SKILL_COUNT = 29;
string validSkills[VALID_SKILL_COUNT] = {
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

// Remove punctuation from start and end
string cleanToken(string s) {
    s = trim(s);
    while (!s.empty() && ispunct(s.back())) s.pop_back();
    while (!s.empty() && ispunct(s.front())) s.erase(0, 1);
    return trim(s);
}

// Check if token is a valid skill
bool isSkill(const string &word) {
    string lw = toLowerStr(word);
    for (int i = 0; i < VALID_SKILL_COUNT; i++) {
        if (lw == validSkills[i]) return true;
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

        string skillsPart = line.substr(pos + 10);

        // Replace punctuation
        for (char &c : skillsPart) {
            if (c == '.' || c == ';') c = ',';
        }

        stringstream ss(skillsPart);
        string token;
        string cleanSkills[50];
        int skillCount = 0;

        while (getline(ss, token, ',')) {
            string original = cleanToken(token);
            if (!original.empty() && isSkill(toLowerStr(original))) {
                cleanSkills[skillCount++] = original; // store in array
            }
        }

        if (skillCount > 0) {
            outfile << "Experienced professional skilled in ";
            for (int i = 0; i < skillCount; i++) {
                outfile << cleanSkills[i];
                if (i < skillCount - 1) outfile << ", ";
            }
            outfile << "\n";
        }
    }

    infile.close();
    outfile.close();

    cout << "Cleaning resumes done! Saved as cleaned_resumes.csv" << endl;
    return 0;
}
