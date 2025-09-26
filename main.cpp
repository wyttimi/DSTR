#include <iostream>
#include <fstream>
#include <sstream>
#include "Job.hpp"
#include "Resume.hpp"
#include "ArrayList.hpp"
#include "LinkedList.hpp"
#include "JobMatcher.hpp"
using namespace std;

int main() {
    ArrayContainer<Job> jobsArray;
    LinkedListContainer<Job> jobsList;

    // Load job data
    ifstream jobFile("job_description.csv");
    string line;
    int id = 1;
    while (getline(jobFile, line)) {
        stringstream ss(line);
        string title, desc;
        getline(ss, title, ',');
        getline(ss, desc, ',');
        Job jb(id++, title, desc);
        jobsArray.insert(jb);
        jobsList.insert(jb);
    }

    // Example: Search for skill "Python"
    cout << "Linear search (Array) for 'Python': index = "
         << jobsArray.linearSearch("Python") << endl;

    cout << "Linear search (LinkedList) for 'Python': index = "
         << jobsList.linearSearch("Python") << endl;

    // Sort (both bubble and selection examples)
    jobsArray.bubbleSort();
    jobsList.selectionSort();

    // Binary search after sort
    cout << "Binary search (Array) for 'Developer': index = "
         << jobsArray.binarySearch("Developer") << endl;

    cout << "Binary search (LinkedList) for 'Developer': index = "
         << jobsList.binarySearch("Developer") << endl;

    return 0;
}
