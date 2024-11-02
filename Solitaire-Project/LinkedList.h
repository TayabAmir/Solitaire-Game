#pragma once
#include <iostream>
#include "Node.h"
using namespace std;

template <typename T>
class LinkedList
{
private:
    Node<T>* head;
    Node<T>* tail;

public:
    LinkedList()
    {
        head = NULL;
        tail = NULL;
    }

    LinkedList(const LinkedList<T>& other) {
        if (other.head == NULL) {
            head = NULL;
            tail = NULL;
        }
        else {
            head = new Node<T>(other.head->val);
            Node<T>* temp = head;
            Node<T>* current = other.head->next;

            while (current) {
                temp->next = new Node<T>(current->val);
                temp = temp->next;
                current = current->next;
            }
            tail = temp;  // Set the tail to the last node
        }
    }

    bool isEmpty()
    {
        return head == NULL;
    }

    Node<T>* getHead() {
        return head;
    }

    Node<T>* getTail() {
        return tail;
    }

    void insertAtHead(T val)
    {
        Node<T>* node = new Node<T>(val);
        node->next = head;
        head = node;
        if (tail == NULL) {
            tail = head;  // Update tail if the list was empty
        }
    }

    void insertAtEnd(T val)
    {
        Node<T>* node = new Node<T>(val);
        if (!head) {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }

    bool findNode(T x)
    {
        Node<T>* temp = head;
        while (temp)
        {
            if (temp->val == x)
                return true;
            temp = temp->next;
        }
        return false;
    }

    void deleteFromStart()
    {
        if (head) {
            Node<T>* temp = head;
            head = head->next;
            if (head == NULL) {
                tail = NULL; 
            }
            delete temp;
        }
    }

    void deleteFromEnd()
    {
        if (head) {
            if (head == tail) {
                delete head;
                head = tail = NULL;
            }
            else {
                Node<T>* temp = head;
                while (temp->next != tail) {
                    temp = temp->next;
                }
                delete tail;
                tail = temp;
                tail->next = NULL;
            }
        }
    }

    int size() {
        int s = 0;
        Node<T>* temp = head;
        while (temp)
        {
            s++;
            temp = temp->next;
        }
        return s;
    }
};
