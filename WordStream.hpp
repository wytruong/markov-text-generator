#pragma once
#include <string>
#include <vector>

class WordStream {
public:
    void addLexItems(const std::string& line);

    std::string nextToken();

    bool hasMoreTokens() const;

private:
    static bool isWhiteSpace(char item);
    static bool isDelimiter(char c);

    std::vector<std::string> lexItems_;
    size_t nextItem_ = 0;
};
