#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <stdexcept>
#include <iostream>

template <typename T>
class ArrayList {
private:
    T* data;               // Pointer to the underlying array
    size_t capacity;       // Current capacity of the ArrayList
    size_t count;          // Number of elements in the ArrayList
    size_t resizeFactor;   // Factor for resizing (e.g., 2 for doubling/halving)

    // Private helper methods
    void expand();         // Expands the capacity of the ArrayList
    void shrink();         // Shrinks the capacity of the ArrayList

public:
    // Constructors and Destructor
    ArrayList(size_t initialCapacity = 2, size_t resizeFactor = 2); // Constructor with initial capacity and resize factor
    ~ArrayList();          // Destructor to free allocated memory

    // Copy constructor
    ArrayList(const ArrayList<T>& other);

    // Move constructor
    ArrayList(ArrayList<T>&& other) noexcept;

    // Move assignment operator
    ArrayList<T>& operator=(ArrayList<T>&& other) noexcept;
    
    // Copy assignment operator
    ArrayList<T>& operator=(const ArrayList<T>& other);

    // Public member functions
    bool isEmpty() const;            // Returns true if the ArrayList is empty
    size_t size() const;             // Returns the number of elements in the ArrayList
    size_t getCapacity() const;      // Returns the current capacity of the ArrayList

    T& get(size_t index);              // For non-const access
    const T& get(size_t index) const;  // For const access

    void add(const T& item);         // Appends an item to the end of the ArrayList
    void add(size_t index, const T& item); // Inserts an item at the specified index
    T remove();                      // Removes and returns the last item in the ArrayList
    T remove(size_t index);          // Removes and returns the item at the specified index
    void set(size_t index, const T& item); // Replaces the item at the specified index
};

#include "ArrayList.tpp" // Include the implementation file

#endif // ARRAYLIST_HPP