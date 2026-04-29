#pragma once
#include <string>
#include <unordered_map>
#include <sstream>

class FreqList {
public:
    explicit FreqList() = default;
    void add(const std::string& word);

    std::string get(double p) const;

    std::string toString() const;

    size_t size() const;
    
    std::unordered_map<std::string, int> flist_;
    double totalFreq_ = 0.0;
};
