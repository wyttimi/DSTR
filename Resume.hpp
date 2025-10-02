#ifndef RESUME_HPP
#define RESUME_HPP

#include <string>
using namespace std;

class Resume {
public:
    string description;

    Resume() {}
    Resume(string d) {
        description = d;
    }

    string getDescription() const { return description; }
};

#endif
