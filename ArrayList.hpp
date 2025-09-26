#ifndef ARRAYCONTAINER_HPP
#define ARRAYCONTAINER_HPP

#include <iostream>
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

    T get(int index) { return arr[index]; }
    int getSize() { return size; }

    /* -------- Linear Search -------- */
    int linearSearch(string keyword) {
        for (int i = 0; i < size; i++) {
            if (arr[i].getTitle().find(keyword) != string::npos ||
                arr[i].getDescription().find(keyword) != string::npos) {
                return i;
            }
        }
        return -1;
    }

    /* -------- Bubble Sort (by title) -------- */
    void bubbleSort() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].getTitle() > arr[j + 1].getTitle()) {
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    /* -------- Selection Sort (by title) -------- */
    void selectionSort() {
        for (int i = 0; i < size - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < size; j++) {
                if (arr[j].getTitle() < arr[minIndex].getTitle()) {
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

    /* -------- Binary Search (needs sorted) -------- */
    int binarySearch(string keyword) {
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            string title = arr[mid].getTitle();
            if (title.find(keyword) != string::npos) return mid;
            if (title < keyword) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

    ~ArrayContainer() { delete[] arr; }
};

#endif
