#ifndef LINKEDLISTCONTAINER_HPP
#define LINKEDLISTCONTAINER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include "Utils.hpp"
using namespace std;

template <class T>
class LinkedListContainer {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    LinkedListContainer() {
        head = nullptr;
        size = 0;
    }

    void insert(T item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    int getSize() const { return size; }
    Node* getHead() { return head; }

    /* -------- get by index (for fair comparison with array) -------- */
    T get(int index) {
        if (index < 0 || index >= size) throw out_of_range("Index out of range");
        Node* temp = head;
        int i = 0;
        while (temp) {
            if (i == index) return temp->data;
            temp = temp->next;
            i++;
        }
        throw out_of_range("Index not found");
    }

    /* -------- Linear Search (case-insensitive) -------- */
    int linearSearch(string keyword) {
        string key = toLowerStr(keyword);
        Node* temp = head;
        int index = 0;
        while (temp) {
            if (toLowerStr(temp->data.getTitle()).find(key) != string::npos ||
                toLowerStr(temp->data.getDescription()).find(key) != string::npos) {
                return index;
            }
            temp = temp->next;
            index++;
        }
        return -1;
    }

    /* -------- Bubble Sort (by CandidateMatch score) -------- */
    void bubbleSortByScore() {
        if (!head) return;
        for (int i = 0; i < size; i++) {
            Node* curr = head;
            Node* next = head->next;
            while (next) {
                if (curr->data.score < next->data.score) {
                    // swap using T (not CandidateMatch directly)
                    T temp = curr->data;
                    curr->data = next->data;
                    next->data = temp;
                }
                curr = next;
                next = next->next;
            }
        }
    }



    /* -------- Selection Sort (by description/skills) -------- */
    void selectionSort() {
        if (!head) return;
        Node* start = head;
        while (start) {
            Node* minNode = start;
            Node* curr = start->next;
            while (curr) {
                if (curr->data.getDescription() < minNode->data.getDescription()) {
                    minNode = curr;
                }
                curr = curr->next;
            }
            if (minNode != start) {
                T temp = start->data;
                start->data = minNode->data;
                minNode->data = temp;
            }
            start = start->next;
        }
    }

    /* -------- Binary Search (case-insensitive, on description) -------- */
    int binarySearch(string keyword) {
        string key = toLowerStr(keyword);
        // copy to array first
        T* tempArr = new T[size];
        Node* temp = head;
        int idx = 0;
        while (temp) {
            tempArr[idx++] = temp->data;
            temp = temp->next;
        }
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            string desc = toLowerStr(tempArr[mid].getDescription());
            if (desc.find(key) != string::npos) {
                delete[] tempArr;
                return mid;
            }
            if (desc < key) left = mid + 1;
            else right = mid - 1;
        }
        delete[] tempArr;
        return -1;
    }
};

#endif
