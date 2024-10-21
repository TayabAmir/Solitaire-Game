#pragma once
#include <iostream>
#include "LinkedList.h"
using namespace std;

template <typename T>

class Stack
{
    LinkedList<T> list;
public:
    Stack() {}

    Stack(const Stack<T>& other) : list(other.list) {
    }

    void push(T val)
    {
        list.insertAtHead(val);
    }
    bool empty() {
        return list.isEmpty();
    }
    int size() {

        Node<T>* head = list.getHead();
        int s = 0;
        while (head)
            head = head->next, s++;
        return s;
    }
    void pop()
    {
        if (!list.isEmpty()) {
            list.deleteFromStart();
            return;
        }
        cout << "Segmentation Fault!!";
        exit(1);
    }
    T& top()
    {
        if (!list.isEmpty())
            return list.getHead()->val;
        cout << "Segmentation Fault!!";
        exit(1);
    }
};
