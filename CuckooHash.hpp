#pragma once
#include "ArrayList.hpp"
#include <functional>
#include <utility>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#include <string>

// FNV-1a hash for hash1 (for std::string keys)
inline size_t fnv1a_hash1(const std::string& s, size_t cap) {
    size_t hash = 14695981039346656037ULL;
    for (char c : s) {
        hash ^= static_cast<unsigned char>(c);
        hash *= 1099511628211ULL;
    }
    return hash % cap;
}

// FNV-1a hash for hash2 (different offset basis and salt for independence)
inline size_t fnv1a_hash2(const std::string& s, size_t cap) {
    size_t hash = 1099511628211ULL; // Different offset basis
    for (char c : s) {
        hash ^= static_cast<unsigned char>(c + 31); // Add salt to each char
        hash *= 14695981039346656037ULL;
    }
    return hash % cap;
}

template <typename Key, typename Value>
class CuckooHash {
public:
    CuckooHash(size_t initialCapacity = 128, size_t maxKick = 256);

    void insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    Value* find(const Key& key);
    Value& operator[](const Key& key);

    size_t size() const;
    size_t capacity() const;

private:
    struct Slot {
        std::optional<Key> key;
        std::optional<Value> value;
        bool occupied = false;
    };

    ArrayList<Slot> table_;
    size_t count_;
    size_t capacity_;
    size_t maxKick_;

    size_t hash1(const Key& key, size_t cap) const;
    size_t hash2(const Key& key, size_t cap) const;
    void rehash();

    static constexpr double LOAD_FACTOR = 0.5;
};

// Implementation

template <typename Key, typename Value>
CuckooHash<Key, Value>::CuckooHash(size_t initialCapacity, size_t maxKick)
    : table_(initialCapacity), count_(0), capacity_(initialCapacity)
{
    if (maxKick == 0) {
        maxKick_ = static_cast<size_t>(std::log2(capacity_) * 8);
        if (maxKick_ < 32) maxKick_ = 32;
    } else {
        maxKick_ = maxKick;
    }
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::hash1(const Key& key, size_t cap) const {
    if constexpr (std::is_same<Key, std::string>::value) {
        return fnv1a_hash1(key, cap);
    } else {
        return std::hash<Key>{}(key) % cap;
    }
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::hash2(const Key& key, size_t cap) const {
    if constexpr (std::is_same<Key, std::string>::value) {
        return fnv1a_hash2(key, cap);
    } else {
        // Use a different mixing for non-string keys
        std::size_t h = std::hash<Key>{}(key);
        h ^= (h << 13);
        h ^= (h >> 7);
        h ^= (h << 17);
        h *= 0x9e3779b97f4a7c15ULL;
        return h % cap;
    }
}

template <typename Key, typename Value>
void CuckooHash<Key, Value>::insert(const Key& key, const Value& value) {
    // TODO: Implement insert with cuckoo hashing and rehashing

   // Check if load factor exceeded
    if (static_cast<double>(count_) / capacity_ >= LOAD_FACTOR) {
        rehash();
    }
    
    // Check if key already exists and update
    Value* existing = find(key);
    if (existing != nullptr) {
        *existing = value;
        return;
    }
    
     // Prepare item to insert
    Key currentKey = key;
    Value currentValue = value;
    bool useHash1 = true;  // Start with hash1
    
    // Try to insert with cuckoo hashing
    for (size_t kicks = 0; kicks < maxKick_; ++kicks) {
        size_t idx = useHash1 ? hash1(currentKey, capacity_) : hash2(currentKey, capacity_);
        auto& slot = table_.get(idx);
        
        if (!slot.occupied) {
            // Found empty slot!
            slot.key = currentKey;
            slot.value = currentValue;
            slot.occupied = true;
            count_++;
            return;
        }
        
        // Kick out the existing item
        Key tempKey = slot.key.value();
        Value tempValue = slot.value.value();
        slot.key = currentKey;
        slot.value = currentValue;
        currentKey = tempKey;
        currentValue = tempValue;
        
        // Toggle to use the OTHER hash function for the kicked-out item
        useHash1 = !useHash1;
    }
    
    // Too many kicks - rehash and retry ONCE
    rehash();

    // Simple retry: just look for any empty slot
    size_t idx1 = hash1(currentKey, capacity_);
    if (!table_.get(idx1).occupied) {
        auto& slot = table_.get(idx1);
        slot.key = currentKey;
        slot.value = currentValue;
        slot.occupied = true;
        count_++;
        return;
    }
    
    size_t idx2 = hash2(currentKey, capacity_);
    if (!table_.get(idx2).occupied) {
        auto& slot = table_.get(idx2);
        slot.key = currentKey;
        slot.value = currentValue;
        slot.occupied = true;
        count_++;
        return;
    }

    // If still failing after rehash, something is wrong
    // Just do linear probing as fallback
    for (size_t i = 0; i < capacity_; ++i) {
        if (!table_.get(i).occupied) {
            auto& slot = table_.get(i);
            slot.key = currentKey;
            slot.value = currentValue;
            slot.occupied = true;
            count_++;
            return;
        }
    }
}

template <typename Key, typename Value>
bool CuckooHash<Key, Value>::remove(const Key& key) {
    // TODO: Implement remove

    size_t idx1 = hash1(key, capacity_);
    auto& slot1 = table_.get(idx1);
    
    if (slot1.occupied && slot1.key.value() == key) {
        slot1.occupied = false;
        slot1.key.reset();
        slot1.value.reset();
        count_--;
        return true;
    }
    
    size_t idx2 = hash2(key, capacity_);
    auto& slot2 = table_.get(idx2);
    
    if (slot2.occupied && slot2.key.value() == key) {
        slot2.occupied = false;
        slot2.key.reset();
        slot2.value.reset();
        count_--;
        return true;
    }
    
    return false;
}

template <typename Key, typename Value>
Value* CuckooHash<Key, Value>::find(const Key& key) {
    // TODO: Implement find

    size_t idx1 = hash1(key, capacity_);
    auto& slot1 = table_.get(idx1);
    
    if (slot1.occupied && slot1.key.value() == key) {
        return &(slot1.value.value());
    }
    
    size_t idx2 = hash2(key, capacity_);
    auto& slot2 = table_.get(idx2);
    
    if (slot2.occupied && slot2.key.value() == key) {
        return &(slot2.value.value());
    }
    
    return nullptr;
}

template <typename Key, typename Value>
Value& CuckooHash<Key, Value>::operator[](const Key& key) {
    // TODO: implement operator[] to return reference to value associated with key
    // If key not found, insert default-constructed Value and return its reference

    Value* val = find(key);
    if (val != nullptr) {
        return *val;
    }

    // Key not found, insert default-constructed Value
    insert(key, Value{});
    return *find(key);

}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::size() const {
    return count_;
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::capacity() const {
    return capacity_;
}

template <typename Key, typename Value>
void CuckooHash<Key, Value>::rehash() {
    // TODO: Implement rehashing by doubling capacity and reinserting all items

    // Save old table
    ArrayList<Slot> oldTable = std::move(table_);
    size_t oldCapacity = capacity_;
    
    // Double capacity
    capacity_ *= 2;
    table_ = ArrayList<Slot>(capacity_);
    count_ = 0;
    
    // Update maxKick based on new capacity
    maxKick_ = static_cast<size_t>(std::log2(capacity_) * 8);
    if (maxKick_ < 32) maxKick_ = 32;
    
    // Reinsert all occupied slots
    for (size_t i = 0; i < oldCapacity; ++i) {
        const auto& slot = oldTable.get(i);
        if (slot.occupied) {
            insert(slot.key.value(), slot.value.value());
        }
    }
    
}