#include "WordStream.hpp"
#include <sstream>
#include <stdexcept>

void WordStream::addLexItems(const std::string& line) {
    std::string token;
    std::stringstream ss(line);
    char c;
    auto flush = [&](){ if (!token.empty()) { lexItems_.push_back(token); token.clear(); } };
    while (ss.get(c)) {
        if (isDelimiter(c)) {
            flush();
            lexItems_.push_back(std::string(1, c));
        } else {
            token.push_back(c);
        }
    }
    flush();
}

std::string WordStream::nextToken() {
    while (nextItem_ < lexItems_.size() && isWhiteSpace(lexItems_[nextItem_][0])) {
        ++nextItem_;
    }
    if (nextItem_ < lexItems_.size()) {
        return lexItems_[nextItem_++];
    }
    throw std::out_of_range("out of tokens");
}

bool WordStream::hasMoreTokens() const { return nextItem_ < lexItems_.size(); }

bool WordStream::isWhiteSpace(char item) { return item == ' ' || item == '\n' || item == '\t'; }

bool WordStream::isDelimiter(char c) {
    static const std::string delims = " -'\";:,.!\n\t";
    return delims.find(c) != std::string::npos;
}
