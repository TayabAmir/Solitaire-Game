#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Node
{
public:
    T val;
    Node* next;

    Node(T& val)
    {
        this->val = val;
        next = NULL;
    }
};
