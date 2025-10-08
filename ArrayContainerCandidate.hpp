#ifndef ARRAYCONTAINERCANDIDATE_HPP
#define ARRAYCONTAINERCANDIDATE_HPP

#include <stdexcept>
#include <algorithm>
#include "CandidateMatch.hpp"
using namespace std;

class ArrayContainerCandidate {
private:
    CandidateMatch* arr;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        CandidateMatch* n = new CandidateMatch[capacity];
        for (int i = 0; i < size; i++) n[i] = arr[i];
        delete[] arr;
        arr = n;
    }

    // --- Merge Sort helpers (descending by score) ---
    void merge(int l, int m, int r) {
        int n1 = m - l + 1, n2 = r - m;
        CandidateMatch* L = new CandidateMatch[n1];
        CandidateMatch* R = new CandidateMatch[n2];
        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i].score >= R[j].score) arr[k++] = L[i++];
            else                            arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
        delete[] L; delete[] R;
    }
    void mergeSort(int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        merge(l, m, r);
    }

public:
    ArrayContainerCandidate(int cap = 16) : size(0), capacity(cap) {
        arr = new CandidateMatch[capacity];
    }

    ~ArrayContainerCandidate() { delete[] arr; }

    void clear() {
        delete[] arr;
        arr = new CandidateMatch[capacity];
        size = 0;
    }

    void insert(const CandidateMatch& x) {
        if (size == capacity) resize();
        arr[size++] = x;
    }

    CandidateMatch get(int idx) const {
        if (idx < 0 || idx >= size) throw out_of_range("Index");
        return arr[idx];
    }

    int getSize() const { return size; }

    // --- Bubble Sort (descending) ---
    void bubbleSortByScore() {
        if (size <= 1) return;
        for (int i = 0; i < size - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].score < arr[j + 1].score) {
                    CandidateMatch t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // --- Merge Sort (descending) ---
    void mergeSortByScore() {
        if (size > 1) mergeSort(0, size - 1);
    }

    // --- Linear search: return index of first candidate with score == target (or -1) ---
    int linearSearchByScore(int target) const {
        for (int i = 0; i < size; i++) if (arr[i].score == target) return i;
        return -1;
    }

    // --- Binary search (array must be sorted DESC by score) ---
    int binarySearchByScore(int target) const {
        int lo = 0, hi = size - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid].score == target) return mid;
            // descending order:
            if (arr[mid].score < target) hi = mid - 1;
            else                         lo = mid + 1;
        }
        return -1;
    }
};

#endif
