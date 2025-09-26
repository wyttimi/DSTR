#ifndef ARRAYCONTAINER_HPP
#define ARRAYCONTAINER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include "Utils.hpp"
using namespace std;

template <class T>
class ArrayContainer {
private:
    T* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        T* newArr = new T[capacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

public:
    ArrayContainer(int cap = 10) {
        capacity = cap;
        size = 0;
        arr = new T[capacity];
    }

    void insert(T item) {
        if (size == capacity) resize();
        arr[size++] = item;
    }

    T get(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        return arr[index];
    }

    // ✅ NEW: return reference so sorting works
    T& getRef(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        return arr[index];
    }

    int getSize() const { return size; }

    /* -------- Linear Search (case-insensitive) -------- */
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

    /* -------- Bubble Sort by CandidateMatch score (highest first) -------- */
    void bubbleSortByScore() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].score < arr[j + 1].score) {
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    /* -------- Selection Sort (by description/skills) -------- */
    void selectionSort() {
        for (int i = 0; i < size - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < size; j++) {
                if (arr[j].getDescription() < arr[minIndex].getDescription()) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                T temp = arr[i];
                arr[i] = arr[minIndex];
                arr[minIndex] = temp;
            }
        }
    }

    /* -------- Binary Search (case-insensitive, on description) -------- */
    int binarySearch(string keyword) {
        string key = toLowerStr(keyword);
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            string desc = toLowerStr(arr[mid].getDescription());
            if (desc.find(key) != string::npos) return mid;
            if (desc < key) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

    ~ArrayContainer() { delete[] arr; }
};

#endif
