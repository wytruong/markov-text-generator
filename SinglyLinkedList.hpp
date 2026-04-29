#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <stdexcept>
#include <iostream>

template <typename T>
class SinglyLinkedList {
private:
    class Node {
    public:
        T item;
        Node* next;

        Node(const T& item, Node* next = nullptr) : item(item), next(next) {}
    };

    Node* head; // Pointer to the first node
    Node* tail; // Pointer to the last node
    size_t n;   // Number of elements in the list

    void rangeCheck(size_t index) const;
    void clear(); // Helper function to clear the list

public:
    SinglyLinkedList();     // Default constructor
    ~SinglyLinkedList();    // Destructor

    SinglyLinkedList(const SinglyLinkedList<T>& other);                 // Copy constructor
    SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other);   // Copy assignment operator
    SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept;             // Move constructor
    SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& other) noexcept; // Move assignment operator


    bool isEmpty() const;
    size_t size() const;
    T get(size_t index) const;
    void add(const T& item); // Add to the front
    void add(size_t index, const T& item);
    T remove();              // Remove from the front
    T remove(size_t index);
    bool remove(const T& value); // Remove by value

    void addLast(const T& item); // Add to the end
    T removeLast();              // Remove from the end
    Node* getHead() const { return head; }
};

#include "SinglyLinkedList.tpp" // Include the implementation file

#endif // SINGLYLINKEDLIST_HPP