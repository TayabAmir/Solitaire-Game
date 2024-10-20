#pragma once
#include <iostream>
#include "LinkedList.h"
using namespace std;

template <typename T>

class Stack
{
    LinkedList<T> listToHide;
    LinkedList<T> listToShow;
public:
    Stack() {}

    Stack(const Stack<T>& other) : listToHide(other.listToHide), listToShow(other.listToShow) {}

    LinkedList<T> getHideList() {
        return listToHide;
    }
    LinkedList<T> getShowList() {
        return listToShow;
    }
    void push(T val)
    {
        if (listToShow.isEmpty()) {
            listToShow.insertAtHead(val);
        }
        else {
            T temp = listToShow.getHead()->val;
            listToShow.deleteFromStart();
            listToHide.insertAtHead(temp);
            listToShow.insertAtHead(val);
        }
    }
    void getCards(Stack<T> &st, int noOfCards) {
        while (noOfCards--) {
            listToShow.insertAtHead(st.top());
            st.pop();
        }
    }
    bool empty() {
        return listToHide.isEmpty() && listToShow.isEmpty();
    }
    int size() {
        
        return listToHide.size() + listToShow.size();
    }
    void pop()
    {
        if (!listToShow.isEmpty()) {
            listToShow.deleteFromStart();
            if (!listToHide.isEmpty()) {
                T temp = listToHide.getHead()->val;
                listToHide.deleteFromStart();
                listToShow.insertAtHead(temp);
            }
        }
        else{
            cout << "Segmentation Fault!!";
            exit(1);
        }
    }
    T& top()
    {
        if (!listToShow.isEmpty())
            return listToShow.getHead()->val;
        cout << "Segmentation Fault!!";
        exit(1);
    }
};