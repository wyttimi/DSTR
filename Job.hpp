#ifndef JOB_HPP
#define JOB_HPP

#include <string>
using namespace std;

struct Job {
    string description;

    Job() {} // default constructor
    Job(string d) {
        description = d;
    }

    string getDescription() const { return description; }
};
#endif