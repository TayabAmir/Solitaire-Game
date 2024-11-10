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
        return list.size();
    }
    void pop()
    {
        if (!list.isEmpty()) {
            list.deleteFromStart();
            return;
        }
        cout << "Segmentation Fault!!";
    }
    T& top()
    {
        if (!list.isEmpty())
            return list.head->val;
        cout << "Segmentation Fault!!";
    }
};
