#include "ArrayList.hpp"
#include <cstring> 

template <typename T>
ArrayList<T>::ArrayList(size_t initialCapacity, size_t resizeFactor)
    : capacity(initialCapacity), count(initialCapacity), resizeFactor(resizeFactor) {
    if (capacity < 1) {
        throw std::invalid_argument("Initial capacity must be at least 1");
    }
    if (resizeFactor < 1) {
        throw std::invalid_argument("Resize factor must be at least 1");
    }
    data = new T[capacity]; // Allocate the initial array
}

template <typename T>
ArrayList<T>::~ArrayList() {
    delete[] data; // Free the allocated memory
}

// Copy constructor
template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& other)
    : capacity(other.capacity), count(other.count), resizeFactor(other.resizeFactor) {
    data = new T[capacity]; // Allocate new memory for the copy
    for (size_t i = 0; i < count; ++i) {
        data[i] = other.data[i]; // Copy each element
    }
}

// Move constructor
template <typename T>
ArrayList<T>::ArrayList(ArrayList<T>&& other) noexcept
    : data(other.data), capacity(other.capacity), count(other.count), resizeFactor(other.resizeFactor) {
    other.data = nullptr;
    other.capacity = 0;
    other.count = 0;
    other.resizeFactor = 0;
}

// Move assignment operator
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        capacity = other.capacity;
        count = other.count;
        resizeFactor = other.resizeFactor;

        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
        other.resizeFactor = 0;
    }
    return *this;
}

// Copy assignment operator
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    if (this != &other) { // Avoid self-assignment
        // Free the existing memory
        delete[] data;

        // Copy the data from the other ArrayList
        capacity = other.capacity;
        count = other.count;
        resizeFactor = other.resizeFactor;

        data = new T[capacity]; // Allocate new memory
        for (size_t i = 0; i < count; ++i) {
            data[i] = other.data[i]; // Copy each element
        }
    }
    return *this;
}

template <typename T>
bool ArrayList<T>::isEmpty() const {
    return count == 0;
}

template <typename T>
size_t ArrayList<T>::size() const {
    return count;
}

template <typename T>
size_t ArrayList<T>::getCapacity() const {
    return capacity;
}


template <typename T>
T& ArrayList<T>::get(size_t index) {
    if (index >= count) { 
        std::cerr << "ArrayList::get index out of range: index=" << index << ", count=" << count << std::endl;
        throw std::out_of_range("Index out of range"); }
    return data[index];
}

template <typename T>
const T& ArrayList<T>::get(size_t index) const {
    if (index >= count) { throw std::out_of_range("Index out of range"); }
    return data[index];
}

template <typename T>
void ArrayList<T>::add(const T& item) {
    if (count == capacity) {
        expand(); // Expand the array if it's full
    }
    data[count++] = item; // Add the item and increment the count
}

template <typename T>
void ArrayList<T>::add(size_t index, const T& item) {
    if (index > count) {
        std::cerr << "ArrayList::add index out of range: index=" << index << ", count=" << count << std::endl;
        throw std::out_of_range("Index out of bounds");
    }
    if (count == capacity) {
        expand(); // Expand the array if it's full
    }
    // Shift elements to the right to make space for the new item
    for (size_t i = count; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = item; // Insert the new item
    ++count;
}

template <typename T>
T ArrayList<T>::remove() {
    if (isEmpty()) {
        throw std::out_of_range("ArrayList is empty");
    }
    T item = data[--count]; // Remove the last item
    if (count < capacity / 4) {
        shrink(); // Shrink the array if it's less than 1/4 full
    }
    return item;
}

template <typename T>
T ArrayList<T>::remove(size_t index) {
    if (index >= count) {
        throw std::out_of_range("Index out of bounds");
    }
    T item = data[index]; // Get the item to remove
    // Shift elements to the left to fill the gap
    for (size_t i = index; i < count - 1; ++i) {
        data[i] = data[i + 1];
    }
    --count;
    if (count < capacity / 4) {
        shrink(); // Shrink the array if it's less than 1/4 full
    }
    return item;
}

template <typename T>
void ArrayList<T>::set(size_t index, const T& item) {
    if (index >= count) {
        throw std::out_of_range("Index out of bounds");
    }
    data[index] = item; // Replace the item at the specified index
}

template <typename T>
void ArrayList<T>::expand() {
    size_t newCapacity = capacity * resizeFactor; // Increase capacity by the resize factor
    T* newData = new T[newCapacity]; // Allocate a new array
    // Copy existing data to the new array
    for (size_t i = 0; i < count; ++i) {
        newData[i] = data[i];
    }
    delete[] data; // Free the old array
    data = newData; // Point to the new array
    capacity = newCapacity;
}

template <typename T>
void ArrayList<T>::shrink() {
    size_t newCapacity = capacity / resizeFactor; // Reduce capacity by the resize factor
    if (newCapacity < 1) {
        newCapacity = 1; // Ensure capacity is at least 1
    }
    T* newData = new T[newCapacity]; // Allocate a new array
    // Copy existing data to the new array
    for (size_t i = 0; i < count; ++i) {
        newData[i] = data[i];
    }
    delete[] data; // Free the old array
    data = newData; // Point to the new array
    capacity = newCapacity;
}