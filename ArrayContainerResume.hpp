#ifndef ARRAYCONTAINERRESUME_HPP
#define ARRAYCONTAINERRESUME_HPP

#include <iostream>
#include <stdexcept>
#include "Resume.hpp"
#include "Utils.hpp"
using namespace std;

class ArrayContainerResume {
private:
    Resume* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Resume* newArr = new Resume[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    ArrayContainerResume(int cap = 10) {
        capacity = cap;
        size = 0;
        arr = new Resume[capacity];
    }

    void insert(Resume item) {
        if (size == capacity) resize();
        arr[size++] = item;
    }

    Resume get(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        return arr[index];
    }

    int getSize() const { return size; }

    int linearSearch(string keyword) {
        string key = toLowerStr(keyword);
        for (int i = 0; i < size; i++) {
            if (toLowerStr(arr[i].getDescription()).find(key) != string::npos) {
                return i;
            }
        }
        return -1;
    }

    ~ArrayContainerResume() { delete[] arr; }
};

#endif
