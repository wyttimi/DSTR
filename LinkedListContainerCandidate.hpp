#ifndef LINKEDLISTCONTAINERCANDIDATE_HPP
#define LINKEDLISTCONTAINERCANDIDATE_HPP

#include "CandidateMatch.hpp"
#include <cstddef>
using namespace std;

struct LinkedListContainerCandidate {
private:
    struct Node {
        CandidateMatch data;
        Node* next;
        Node(const CandidateMatch& d) : data(d), next(nullptr) {}
    };

    Node* head;
    int size;

    // --- helpers ---
    Node* getTail(Node* cur) const {
        while (cur && cur->next) cur = cur->next;
        return cur;
    }

    // Split list into halves
    void split(Node* source, Node** frontRef, Node** backRef) {
        Node* slow = source;
        Node* fast = source->next;
        while (fast) {
            fast = fast->next;
            if (fast) { slow = slow->next; fast = fast->next; }
        }
        *frontRef = source;
        *backRef  = slow->next;
        slow->next = nullptr;
    }

    // Merge two sorted lists (descending by score)
    Node* sortedMerge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        Node* result = nullptr;
        if (a->data.score >= b->data.score) {
            result = a;
            result->next = sortedMerge(a->next, b);
        } else {
            result = b;
            result->next = sortedMerge(a, b->next);
        }
        return result;
    }

    void mergeSort(Node** headRef) {
        Node* h = *headRef;
        if (!h || !h->next) return;
        Node* a; Node* b;
        split(h, &a, &b);
        mergeSort(&a);
        mergeSort(&b);
        *headRef = sortedMerge(a, b);
    }

    Node* middle(Node* start, Node* end) const {
        if (!start) return nullptr;
        Node* slow = start;
        Node* fast = start->next;
        while (fast != end) {
            fast = fast ? fast->next : fast;
            if (fast != end) {
                slow = slow->next;
                fast = fast ? fast->next : fast;
            }
        }
        return slow;
    }

public:
    LinkedListContainerCandidate() : head(nullptr), size(0) {}
    ~LinkedListContainerCandidate() { clear(); }

    void clear() {
        Node* cur = head;
        while (cur) { Node* t = cur; cur = cur->next; delete t; }
        head = nullptr; size = 0;
    }

    void insert(const CandidateMatch& x) {
        Node* n = new Node(x);
        if (!head) head = n;
        else {
            Node* t = head;
            while (t->next) t = t->next;
            t->next = n;
        }
        size++;
    }

    int getSize() const { return size; }
    Node* getHead() { return head; }

    // --- Bubble Sort (descending) ---
    void bubbleSortByScore() {
        if (!head || !head->next) return;
        bool swapped;
        do {
            swapped = false;
            Node* cur = head;
            while (cur->next) {
                if (cur->data.score < cur->next->data.score) {
                    CandidateMatch tmp = cur->data;
                    cur->data = cur->next->data;
                    cur->next->data = tmp;
                    swapped = true;
                }
                cur = cur->next;
            }
        } while (swapped);
    }

    // --- Merge Sort ---
    void mergeSortByScore() { mergeSort(&head); }

    // --- Linear Search ---
    int linearSearchByScore(int target) const {
        Node* cur = head; int idx = 0;
        while (cur) {
            if (cur->data.score == target) return idx;
            cur = cur->next; idx++;
        }
        return -1;
    }

    // --- Binary Search (O(n log n)) ---
    int binarySearchByScore(int target) const {
        Node* start = head;
        Node* end = nullptr;
        int baseIdx = 0;
        while (start != end) {
            Node* mid = middle(start, end);
            int midOffset = 0;
            Node* t = start;
            while (t && t != mid) { t = t->next; midOffset++; }
            int midIdx = baseIdx + midOffset;

            if (!mid) return -1;
            if (mid->data.score == target) return midIdx;

            if (mid->data.score > target) {
                start = mid->next;
                baseIdx = midIdx + 1;
            } else {
                end = mid;
            }
        }
        return -1;
    }
};

#endif
