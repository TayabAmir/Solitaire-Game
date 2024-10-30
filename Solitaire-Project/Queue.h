#pragma once
#include <iostream>
#include "LinkedList.h"
using namespace std;

template <typename T>

class Queue
{
    LinkedList<T> list;

public:
    Queue() {}

    Queue(const Queue<T>& other) : list(other.list)
    {
    }

    void enqueue(T val)
    {
        list.insertAtEnd(val);
    }
    bool empty()
    {
        return list.isEmpty();
    }
    int size()
    {
        Node<T>* head = list.getHead();
        int s = 0;
        while (head)
            head = head->next, s++;
        return s;
    }
    void dequeue()
    {
        if (!list.isEmpty())
        {
            list.deleteFromStart();
        }
        else
        {
            throw runtime_error("Queue is empty. Cannot dequeue.");
        }
    }
    T& peek()
    {
        if (!list.isEmpty())
            return list.getHead()->val;
        cout << "Segmentation Fault!!";
        exit(1);
    }
};