#ifndef LINKEDLISTCONTAINERRESUME_HPP
#define LINKEDLISTCONTAINERRESUME_HPP

#include <iostream>
#include <stdexcept>
#include "Resume.hpp"
#include "Utils.hpp"
using namespace std;

class LinkedListContainerResume {
private:
    struct Node {
        Resume data;
        Node* next;
        Node(Resume d) : data(d), next(nullptr) {}
    };
    Node* head;
    int size;

public:
    LinkedListContainerResume() { head = nullptr; size = 0; }

    void insert(Resume item) {
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

    int linearSearch(string keyword) {
        string key = toLowerStr(keyword);
        Node* temp = head;
        int index = 0;
        while (temp) {
            if (toLowerStr(temp->data.getDescription()).find(key) != string::npos) {
                return index;
            }
            temp = temp->next;
            index++;
        }
        return -1;
    }
};

#endif
