#ifndef RESUME_HPP
#define RESUME_HPP

#include <string>
using namespace std;

class Resume {
public:
    int id;
    string description;

    Resume() {}
    Resume(int i, string d) {
        id = i;
        description = d;
    }

    string getDescription() const { return description; }
};

#endif
