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

public:
    LinkedListContainerCandidate() { head = nullptr; size = 0; }

    void insert(CandidateMatch item) {
        Node* newNode = new Node(item);
        if (!head) head = newNode;
        else {
            Node* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    int getSize() const { return size; }
    Node* getHead() { return head; }

    void bubbleSortByScore() {
        if (!head) return;
        for (int i = 0; i < size; i++) {
            Node* curr = head;
            Node* next = head->next;
            while (next) {
                if (curr->data.score < next->data.score) {
                    CandidateMatch temp = curr->data;
                    curr->data = next->data;
                    next->data = temp;
                }
                curr = next;
                next = next->next;
            }
        }
    }
};

#endif
