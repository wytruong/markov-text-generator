#pragma once
#include <map>

template <typename Key, typename Value>
class UnorderedWrapper {
public:
    UnorderedWrapper(size_t bucketCount = 101)
        : map_(bucketCount) {}
        
    Value* find(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return nullptr;
        return &it->second;
    }

    void insert(const Key& key, const Value& value) {
        map_[key] = value;
    }

private:
    std::unordered_map<Key, Value> map_;
};