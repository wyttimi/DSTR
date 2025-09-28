#ifndef ARRAYCONTAINERCANDIDATE_HPP
#define ARRAYCONTAINERCANDIDATE_HPP

#include <iostream>
#include <stdexcept>
#include "CandidateMatch.hpp"
using namespace std;

class ArrayContainerCandidate {
private:
    CandidateMatch* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        CandidateMatch* newArr = new CandidateMatch[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    ArrayContainerCandidate(int cap = 10) {
        capacity = cap;
        size = 0;
        arr = new CandidateMatch[capacity];
    }

    void insert(CandidateMatch item) {
        if (size == capacity) resize();
        arr[size++] = item;
    }

    CandidateMatch get(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        return arr[index];
    }

    int getSize() const { return size; }

    void bubbleSortByScore() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].score < arr[j + 1].score) {
                    CandidateMatch temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    ~ArrayContainerCandidate() { delete[] arr; }
};

#endif
