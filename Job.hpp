#ifndef JOB_HPP
#define JOB_HPP

#include <string>
using namespace std;

class Job {
public:
    int id;
    string title;
    string description;

    Job() {}
    Job(int i, string t, string d) {
        id = i;
        title = t;
        description = d;
    }

    string getTitle() const { return title; }
    string getDescription() const { return description; }
};

#endif
