#pragma once
#include "ArrayList.hpp"
#include <functional>
#include <utility>
#include <stdexcept>
#include <optional>

template <typename Key, typename Value>
class LinearProbing {
public:
    LinearProbing(size_t initialCapacity = 101);

    void insert(const Key& key, const Value& value);

    bool remove(const Key& key);

    Value* find(const Key& key);

    Value& operator[](const Key& key);

    size_t size() const;

    size_t capacity() const;

private:
    enum class SlotStatus { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        std::optional<Key> key;
        std::optional<Value> value;
        SlotStatus status = SlotStatus::EMPTY;
    };

    ArrayList<Slot> table_;
    size_t count_;
    size_t capacity_;
    static constexpr double LOAD_FACTOR = 0.7;

    size_t hash(const Key& key) const;
    void rehash();
};

// Implementation

template <typename Key, typename Value>
LinearProbing<Key, Value>::LinearProbing(size_t initialCapacity)
    : table_(initialCapacity), count_(0), capacity_(initialCapacity) {}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::hash(const Key& key) const {
    // TODO: implement hash function

    return std::hash<Key>{}(key) % capacity_;

}

template <typename Key, typename Value>
void LinearProbing<Key, Value>::insert(const Key& key, const Value& value) {
    // TODO: implement insert
    // rehash if load factor exceeded

    // Check if rehashing is needed
    if (static_cast<double>(count_) / capacity_ >= LOAD_FACTOR) {
        rehash();
    }
    
    size_t index = hash(key);
    size_t originalIndex = index;
    
    // Linear probing
    while (true) {
        auto& slot = table_.get(index);
        
        // Empty or deleted slot - can insert here
        if (slot.status == SlotStatus::EMPTY || slot.status == SlotStatus::DELETED) {
            slot.key = key;
            slot.value = value;
            slot.status = SlotStatus::OCCUPIED;
            count_++;
            return;
        }
        
        // Occupied slot - check if it's the same key (update)
        if (slot.status == SlotStatus::OCCUPIED && slot.key.value() == key) {
            slot.value = value;
            return;
        }
        
        // Move to next slot (linear probing)
        index = (index + 1) % capacity_;
        
        // If we've wrapped around completely, rehash
        if (index == originalIndex) {
            rehash();
            insert(key, value);  // Try again after rehashing
            return;
        }
    }

}

template <typename Key, typename Value>
bool LinearProbing<Key, Value>::remove(const Key& key) {
    // TODO: implement remove

    size_t index = hash(key);
    size_t originalIndex = index;
    
    while (true) {
        auto& slot = table_.get(index);
        
        // Empty slot - key not found
        if (slot.status == SlotStatus::EMPTY) {
            return false;
        }
        
        // Found the key
        if (slot.status == SlotStatus::OCCUPIED && slot.key.value() == key) {
            slot.status = SlotStatus::DELETED;
            count_--;
            return true;
        }
        
        // Move to next slot
        index = (index + 1) % capacity_;
        
        // Wrapped around - key not found
        if (index == originalIndex) {
            return false;
        }
    }

}

template <typename Key, typename Value>
Value* LinearProbing<Key, Value>::find(const Key& key) {
    // TODO: implement find

    size_t index = hash(key);
    size_t originalIndex = index;
    
    while (true) {
        auto& slot = table_.get(index);
        
        // Empty slot - key not found
        if (slot.status == SlotStatus::EMPTY) {
            return nullptr;
        }
        
        // Found the key
        if (slot.status == SlotStatus::OCCUPIED && slot.key.value() == key) {
            return &(slot.value.value());
        }
        
        // Move to next slot
        index = (index + 1) % capacity_;
        
        // Wrapped around - key not found
        if (index == originalIndex) {
            return nullptr;
        }
    }
}

template <typename Key, typename Value>
Value& LinearProbing<Key, Value>::operator[](const Key& key) {
    // TODO: implement operator[] to return reference to value associated with key
    // if key not found, insert default-constructed Value and return its reference

    Value* val = find(key);
    if (val != nullptr) {
        return *val;
    }
    
    // Key not found, insert default-constructed Value
    insert(key, Value{});
    return *find(key);

}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::size() const {
    return count_;
}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::capacity() const {
    return capacity_;
}

template <typename Key, typename Value>
void LinearProbing<Key, Value>::rehash() {
    // TODO: Implement rehashing by doubling capacity and reinserting all items

    // Save old table
    ArrayList<Slot> oldTable = std::move(table_);
    size_t oldCapacity = capacity_;
    
    // Double capacity
    capacity_ *= 2;
    table_ = ArrayList<Slot>(capacity_);
    count_ = 0;
    
    // Reinsert all occupied slots
    for (size_t i = 0; i < oldCapacity; ++i) {
        const auto& slot = oldTable.get(i);
        if (slot.status == SlotStatus::OCCUPIED) {
            insert(slot.key.value(), slot.value.value());
        }
    }
    
}