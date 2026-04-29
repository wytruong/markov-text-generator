#include "FreqList.hpp"
#include <stdexcept>
#include <sstream>

void FreqList::add(const std::string& word) {
    // TODO: implement add

    //If word exists, increment its frequency 
    //If word doesn't exist, add it with frequency 1
    flist_[word]++;
    totalFreq_++;

}

std::string FreqList::get(double p) const {
    // TODO: implement get

    //We should validate the input 
    if (p < 0.0 || p > 1.0) {
        throw std::invalid_argument("Probability p must be between 0.0 and 1.0");
    }

    // If empty, return empty string
    if (flist_.empty() || totalFreq_ == 0) {
        return "";
    }
    
    // Calculate cumulative probabilities and select word
    double cumulative = 0.0;
    std::string lastWord;
    
    for (const auto& kv : flist_) {
        lastWord = kv.first;  // Keep track of last word
        cumulative += kv.second;
        double probability = cumulative / totalFreq_;
        
        // Check if p falls in this word's range
        if (p < probability) {
            return kv.first;
        }
    }
    
    // If we get here (p == 1.0 exactly or rounding), return the last word we saw
    return lastWord;

}

std::string FreqList::toString() const {
    std::ostringstream rep;
    rep << "Frequency List: ";
    for (const auto& kv : flist_) {
        rep << "<" << kv.first << "=" << kv.second << ">";
    }
    return rep.str();
}

size_t FreqList::size() const {
    return flist_.size();
}