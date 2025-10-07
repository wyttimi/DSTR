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

    void quickSortHelper(int low, int high) {
        if (low < 0 || high < 0 || low >= size || high >= size || low >= high)
            return;

        int i = low, j = high;
        int pivot = arr[(low + high) / 2].score;

        while (i <= j) {
            while (arr[i].score > pivot) i++;   // descending
            while (arr[j].score < pivot) j--;
            if (i <= j) {
                CandidateMatch temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++; j--;
            }
        }

        if (low < j) quickSortHelper(low, j);
        if (i < high) quickSortHelper(i, high);
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

    CandidateMatch get(int index) const {
        if (index < 0 || index >= size)
            throw out_of_range("Index out of range");
        return arr[index];
    }

    int getSize() const { return size; }

    void quickSortByScore() {
        if (size > 1)
            quickSortHelper(0, size - 1);
    }

    void clear() {
        delete[] arr;
        arr = new CandidateMatch[capacity];
        size = 0;
    }

    ~ArrayContainerCandidate() {
        delete[] arr;
    }
};

#endif
