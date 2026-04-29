#pragma once
#include <random>
#include <string>
#include <cctype>
#include "StringPair.hpp"
#include "FreqList.hpp"
#include "WordStream.hpp"

template <typename HashTable>
class TextGenerator {
public:
    TextGenerator();
    
    TextGenerator(size_t bucketCount = 101)
        : stringPairList_(bucketCount), rng_(std::random_device{}()) {}

    void enter(const std::string& first, const std::string& second, const std::string& third);

    std::string getNextWord(const std::string& first, const std::string& second);

    static std::string toLower(const std::string& s) {
        std::string result;
        for (char c : s) result += std::tolower(c);
        return result;
    }

private:
    HashTable stringPairList_;
    std::mt19937 rng_;
};

// --- Implementation ---

template <typename HashTable>
TextGenerator<HashTable>::TextGenerator() : rng_(std::random_device{}()) {}

template <typename HashTable>
void TextGenerator<HashTable>::enter(const std::string& first, const std::string& second, const std::string& third) {
    // TODO: implement enter

    // Create StringPair key from first two words
    StringPair key(first, second);
    
    // Get or create FreqList for this pair
    FreqList* freqList = stringPairList_.find(key);
    
    if (freqList == nullptr) {
        // Create new FreqList
        FreqList newList;
        newList.add(third);
        stringPairList_.insert(key, newList);
    } else {
        // Add to existing FreqList
        freqList->add(third);
    }

}

template <typename HashTable>
std::string TextGenerator<HashTable>::getNextWord(const std::string& first, const std::string& second) {
    // TODO: implement getNextWord

    StringPair key(first, second);
    
    FreqList* freqList = stringPairList_.find(key);
    
    if (freqList == nullptr || freqList->size() == 0) {
        return "";  // No words follow this pair
    }
    
    // Generate random probability between 0.0 and 1.0
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double p = dist(rng_);
    
    // Get word based on probability
    return freqList->get(p);

}

