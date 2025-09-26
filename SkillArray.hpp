#ifndef SKILLARRAY_HPP
#define SKILLARRAY_HPP

#include <string>
using namespace std;

class SkillArray {
private:
    string* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        string* newArr = new string[capacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

public:
    SkillArray(int cap = 10) {
        capacity = cap;
        size = 0;
        arr = new string[capacity];
    }

    void insert(const string& s) {
        if (size == capacity) resize();
        arr[size++] = s;
    }

    string get(int index) { return arr[index]; }
    int getSize() { return size; }

    ~SkillArray() { delete[] arr; }
};

#endif
