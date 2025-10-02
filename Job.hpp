#ifndef JOB_HPP
#define JOB_HPP

#include <string>
using namespace std;

class Job {
public:
    string description;

    Job() {}
    Job(string d) {
        description = d;
    }

    string getDescription() const { return description; }
};

#endif
