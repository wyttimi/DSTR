#ifndef LINKEDLISTCONTAINERCANDIDATE_HPP
#define LINKEDLISTCONTAINERCANDIDATE_HPP

#include <iostream>
#include <stdexcept>
#include "CandidateMatch.hpp"
using namespace std;

class LinkedListContainerCandidate {
private:
    struct Node {
        CandidateMatch data;
        Node* next;
        Node(CandidateMatch d) : data(d), next(nullptr) {}
    };
    Node* head;
    int size;

    Node* getTail(Node* cur) {
        while (cur && cur->next)
            cur = cur->next;
        return cur;
    }

    Node* partition(Node* low, Node* high, Node** newLow, Node** newHigh) {
        Node* pivot = high;
        Node* prev = nullptr;
        Node* curr = low;
        Node* tail = pivot;

        while (curr != pivot) {
            if (curr->data.score > pivot->data.score) { // descending
                if (*newLow == nullptr)
                    *newLow = curr;
                prev = curr;
                curr = curr->next;
            } else {
                if (prev)
                    prev->next = curr->next;
                Node* tmp = curr->next;
                curr->next = nullptr;
                tail->next = curr;
                tail = curr;
                curr = tmp;
            }
        }

        if (*newLow == nullptr)
            *newLow = pivot;

        *newHigh = tail;
        return pivot;
    }

    Node* quickSortHelper(Node* low, Node* high) {
        if (!low || low == high)
            return low;

        Node* newLow = nullptr;
        Node* newHigh = nullptr;
        Node* pivot = partition(low, high, &newLow, &newHigh);

        if (newLow != pivot) {
            Node* temp = newLow;
            while (temp->next != pivot)
                temp = temp->next;
            temp->next = nullptr;

            newLow = quickSortHelper(newLow, temp);

            Node* tail = newLow;
            while (tail->next)
                tail = tail->next;
            tail->next = pivot;
        }

        pivot->next = quickSortHelper(pivot->next, newHigh);
        return newLow;
    }

public:
    LinkedListContainerCandidate() {
        head = nullptr;
        size = 0;
    }

    void insert(CandidateMatch item) {
        Node* newNode = new Node(item);
        if (!head)
            head = newNode;
        else {
            Node* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    int getSize() const { return size; }
    Node* getHead() { return head; }

    void quickSortByScore() {
        if (size <= 1) return;
        Node* tail = getTail(head);
        head = quickSortHelper(head, tail);
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* tmp = current;
            current = current->next;
            delete tmp;
        }
        head = nullptr;
        size = 0;
    }

    ~LinkedListContainerCandidate() {
        clear();
    }
};

#endif
