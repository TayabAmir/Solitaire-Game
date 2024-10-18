#pragma once
#include <iostream>
#include "Node.h"
using namespace std;

template <typename T>
class LinkedList
{
private:
    Node<T>* head;
public:
    LinkedList()
    {
        head = NULL;
    }

    LinkedList(const LinkedList<T>& other) {
        if (other.head == NULL) {
            head = NULL;
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
        }
    }

    bool isEmpty()
    {
        return head == NULL;
    }

    Node<T>* getHead() {
        return head;
    }

    void insertAtHead(T val)
    {
        Node<T>* node = new Node<T>(val);
        node->next = head;
        head = node;
    }

    void insertAtEnd(T val)
    {
        if (!head)
        {
            insertAtHead(val);
            return;
        }
        Node<T>* node = new Node<T>(val);
        Node<T>* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = node;
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
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }

    void deleteFromEnd()
    {
        Node<T>* temp = head;
        while (temp->next->next != NULL)
        {
            temp = temp->next;
        }
        delete temp->next;
        temp->next = NULL;
    }
};
