#include "SinglyLinkedList.hpp"

// Default constructor
template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head(nullptr), tail(nullptr), n(0) {}

// Destructor
template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    while (!isEmpty()) {
        remove();
    }
}

// Helper function to clear the list
template <typename T>
void SinglyLinkedList<T>::clear() {
    while (!isEmpty()) {
        remove();
    }
}

// Copy constructor
template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other)
    : head(nullptr), tail(nullptr), n(0) {
    Node* current = other.head;
    while (current != nullptr) {
        addLast(current->item);
        current = current->next;
    }
}

// Copy assignment operator
template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other) {
    if (this != &other) {
        clear();
        Node* current = other.head;
        while (current != nullptr) {
            addLast(current->item);
            current = current->next;
        }
    }
    return *this;
}

// Move constructor
template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept
    : head(other.head), tail(other.tail), n(other.n) {
    other.head = nullptr;
    other.tail = nullptr;
    other.n = 0;
}

// Move assignment operator
template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        n = other.n;
        other.head = nullptr;
        other.tail = nullptr;
        other.n = 0;
    }
    return *this;
}

template <typename T>
bool SinglyLinkedList<T>::isEmpty() const {
    return n == 0;
}

template <typename T>
size_t SinglyLinkedList<T>::size() const {
    return n;
}

template <typename T>
void SinglyLinkedList<T>::rangeCheck(size_t index) const {
    if (index >= n) {
        throw std::out_of_range("Index out of bounds");
    }
}

template <typename T>
T SinglyLinkedList<T>::get(size_t index) const {
    rangeCheck(index);
    Node* current = head;
    while (index-- > 0) {
        current = current->next;
    }
    return current->item;
}

template <typename T>
void SinglyLinkedList<T>::add(const T& item) {
    Node* newNode = new Node(item, head);
    head = newNode;
    if (tail == nullptr) {
        tail = head;
    }
    ++n;
}

template <typename T>
void SinglyLinkedList<T>::add(size_t index, const T& item) {
    if (index == 0) {
        add(item);
    } else {
        rangeCheck(index - 1);
        Node* current = head;
        while (--index > 0) {
            current = current->next;
        }
        current->next = new Node(item, current->next);
        if (current->next->next == nullptr) {
            tail = current->next;
        }
        ++n;
    }
}

template <typename T>
T SinglyLinkedList<T>::remove() {
    if (isEmpty()) {
        throw std::out_of_range("List is empty");
    }
    Node* oldHead = head;
    T item = oldHead->item;
    head = head->next;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete oldHead;
    --n;
    return item;
}

template <typename T>
T SinglyLinkedList<T>::remove(size_t index) {
    if (index == 0) {
        return remove();
    }
    rangeCheck(index);
    Node* current = head;
    while (--index > 0) {
        current = current->next;
    }
    Node* toDelete = current->next;
    T item = toDelete->item;
    current->next = toDelete->next;
    if (current->next == nullptr) {
        tail = current;
    }
    delete toDelete;
    --n;
    return item;
}

template <typename T>
void SinglyLinkedList<T>::addLast(const T& item) {
    Node* newNode = new Node(item);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++n;
}

template <typename T>
T SinglyLinkedList<T>::removeLast() {
    if (isEmpty()) {
        throw std::out_of_range("List is empty");
    }
    if (head == tail) {
        T item = head->item;
        delete head;
        head = tail = nullptr;
        --n;
        return item;
    }
    Node* current = head;
    while (current->next != tail) {
        current = current->next;
    }
    T item = tail->item;
    delete tail;
    tail = current;
    tail->next = nullptr;
    --n;
    return item;
}

template <typename T>
bool SinglyLinkedList<T>::remove(const T& value) {
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->item == value) {
            if (prev == nullptr) {
                head = current->next;
                if (head == nullptr) tail = nullptr;
            } else {
                prev->next = current->next;
                if (prev->next == nullptr) tail = prev;
            }
            delete current;
            --n;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}