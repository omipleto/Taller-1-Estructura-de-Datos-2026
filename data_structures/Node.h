#pragma once
template <typename T>
class Node{
    public:
        T data;
        Node<T>* next;

        Node(const T& data);
};