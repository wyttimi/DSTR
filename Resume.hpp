#ifndef RESUME_HPP
#define RESUME_HPP

#include <string>
using namespace std;

class Resume {
public:
    int id;
    string name;
    string description;

    Resume() {}
    Resume(int i, string n, string d) {
        id = i;
        name = n;
        description = d;
    }
};

#endif
