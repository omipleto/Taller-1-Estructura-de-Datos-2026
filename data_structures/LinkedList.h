#pragma once
#include "Node.h"
template <typename T>

class LinkedList{
    private:
        Node<T>* head;
        Node<T>* tail;
        int size;
    public: 
        LinkedList();
        ~LinkedList();
    
        void push_back(const T& data);
        void push_front(const T& data);
        void pop_back();
        void pop_front();
        T& get(int index);
        void insert(int index, const T& data);
        void remove(int index);
        int getSize() const;
        bool isEmpty() const;
        void clear();
        void shuffle();

};