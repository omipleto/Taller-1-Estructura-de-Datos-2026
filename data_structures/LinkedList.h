#pragma once
#include <cstdlib>
#include <stdexcept>

template<typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node(const T& d) : data(d), next(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;
    
public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {
        srand(12345);
    }
    
    ~LinkedList() {
        clear();
    }
    
    void push_back(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    
    void push_front(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }
    
    void pop_front() {
        if (!head) return;
        Node<T>* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        size--;
    }
    
    void pop_back() {
        if (!head) return;
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }
    
    T& get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Indice fuera de rango");
        }
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    
    const T& get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Indice fuera de rango");
        }
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    
    int getSize() const {
        return size;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }
    
    void shuffle() {
        if (size <= 1) return;
        
        T* arr = new T[size];
        Node<T>* current = head;
        for (int i = 0; i < size; i++) {
            arr[i] = current->data;
            current = current->next;
        }
        
        for (int i = size - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        
        clear();
        for (int i = 0; i < size; i++) {
            push_back(arr[i]);
        }
        
        delete[] arr;
    }
};