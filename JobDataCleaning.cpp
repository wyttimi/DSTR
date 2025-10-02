#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string trimQuotes(string s) {
    if (!s.empty() && s.front() == '"') s.erase(0, 1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return s;
}

int main() {
    ifstream inFile("job_description.csv");
    ofstream outFile("cleaned_jobs.csv");

    if (!inFile.is_open()) {
        cerr << "Error: job_description.csv not found!" << endl;
        return 1;
    }
    if (!outFile.is_open()) {
        cerr << "Error: cannot create cleaned_jobs.csv" << endl;
        return 1;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        string key = "needed with experience in";
        size_t pos = line.find(key);

        if (pos != string::npos) {
            string jobTitle = line.substr(0, pos);
            string skills = line.substr(pos + key.length());

            // stop at first period
            size_t dotPos = skills.find(".");
            if (dotPos != string::npos) {
                skills = skills.substr(0, dotPos);
            }

            // trim spaces
            while (!skills.empty() && skills[0] == ' ')
                skills.erase(0, 1);

            // remove stray quotes from jobTitle and skills
            jobTitle = trimQuotes(jobTitle);
            skills = trimQuotes(skills);

            outFile << jobTitle << "needed with experience in " << skills << "\n";
        }
    }

    inFile.close();
    outFile.close();

    cout << "Cleaning done! Saved as cleaned_jobs.csv" << endl;
    return 0;
}
