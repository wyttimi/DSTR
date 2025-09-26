#ifndef LINKEDLISTCONTAINER_HPP
#define LINKEDLISTCONTAINER_HPP

#include <iostream>
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

    int getSize() { return size; }
    Node* getHead() { return head; }

    /* -------- Linear Search -------- */
    int linearSearch(string keyword) {
        Node* temp = head;
        int index = 0;
        while (temp) {
            if (temp->data.getTitle().find(keyword) != string::npos ||
                temp->data.getDescription().find(keyword) != string::npos) {
                return index;
            }
            temp = temp->next;
            index++;
        }
        return -1;
    }

    /* -------- Bubble Sort -------- */
    void bubbleSort() {
        if (!head) return;
        for (int i = 0; i < size; i++) {
            Node* curr = head;
            Node* next = head->next;
            while (next) {
                if (curr->data.getTitle() > next->data.getTitle()) {
                    T temp = curr->data;
                    curr->data = next->data;
                    next->data = temp;
                }
                curr = next;
                next = next->next;
            }
        }
    }

    /* -------- Selection Sort -------- */
    void selectionSort() {
        if (!head) return;
        Node* start = head;
        while (start) {
            Node* minNode = start;
            Node* curr = start->next;
            while (curr) {
                if (curr->data.getTitle() < minNode->data.getTitle()) {
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

    /* -------- Binary Search -------- */
    // Trick: Copy to array and then apply binary search
    int binarySearch(string keyword) {
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
            string title = tempArr[mid].getTitle();
            if (title.find(keyword) != string::npos) {
                delete[] tempArr;
                return mid;
            }
            if (title < keyword) left = mid + 1;
            else right = mid - 1;
        }
        delete[] tempArr;
        return -1;
    }
};

#endif
