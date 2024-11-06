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
        return list.size();
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
            return list.head->val;
        throw runtime_error("Queue is empty.");
    }
};