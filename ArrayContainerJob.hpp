#ifndef ARRAYCONTAINERJOB_HPP
#define ARRAYCONTAINERJOB_HPP

#include <iostream>
#include <stdexcept>
#include "Job.hpp"
#include "Utils.hpp"
using namespace std;

class ArrayContainerJob {
private:
    Job* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Job* newArr = new Job[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    ArrayContainerJob(int cap = 10) {
        capacity = cap;
        size = 0;
        arr = new Job[capacity];
    }

    void insert(Job item) {
        if (size == capacity) resize();
        arr[size++] = item;
    }

    Job get(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        return arr[index];
    }

    int getSize() const { return size; }

    int linearSearch(string keyword) {
        string key = toLowerStr(keyword);
        for (int i = 0; i < size; i++) {
            if (toLowerStr(arr[i].getTitle()).find(key) != string::npos ||
                toLowerStr(arr[i].getDescription()).find(key) != string::npos) {
                return i;
            }
        }
        return -1;
    }

    ~ArrayContainerJob() { delete[] arr; }
};

#endif
