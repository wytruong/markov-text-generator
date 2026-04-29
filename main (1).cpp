#include "TextGenerator.hpp"
#include "WordStream.hpp"
#include "SeparateChaining.hpp"
#include "LinearProbing.hpp"
#include "CuckooHash.hpp"
#include "UnorderedWrapper.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_set>

struct StringPairHash {
    std::size_t operator()(const StringPair& sp) const {
        return std::hash<std::string>{}(sp.first) ^ (std::hash<std::string>{}(sp.second) << 1);
    }
};

size_t estimateUniquePairs(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "Can't load file: " << path << "\n";
        return 0;
    }
    WordStream ws;
    std::string line;
    while (std::getline(input, line)) {
        ws.addLexItems(TextGenerator<UnorderedWrapper<StringPair, FreqList>>::toLower(line));
    }
    std::unordered_set<StringPair, StringPairHash> uniquePairs;
    if (ws.hasMoreTokens()) {
        std::string first = ws.nextToken();
        if (ws.hasMoreTokens()) {
            std::string second = ws.nextToken();
            while (ws.hasMoreTokens()) {
                uniquePairs.insert(StringPair(first, second));
                first = second;
                second = ws.nextToken();
            }
        }
    }
    return uniquePairs.size();
}

template <typename HashTable>
void runGenerator(const std::string& path, const std::string& label, size_t bucketCount) {
    WordStream ws;
    std::ifstream input(path);
    if (!input) { std::cerr << "Can't load file: " << path << "\n"; return; }

    std::string line;
    while (std::getline(input, line)) {
        ws.addLexItems(TextGenerator<HashTable>::toLower(line));
    }

    TextGenerator<HashTable> table(bucketCount);
    auto start = std::chrono::high_resolution_clock::now();

    try {
        std::string first = ws.nextToken();
        std::string second = ws.nextToken();
        std::string origFirst = first;
        std::string origSecond = second;
        while (ws.hasMoreTokens()) {
            std::string third = ws.nextToken();
            table.enter(first, second, third);
            first = second;
            second = third;
        }

        first = origFirst;
        second = origSecond;
        std::string newText = origFirst + " " + origSecond;
        for (int wordNo = 2; wordNo < 400; ++wordNo) {
            std::string third = table.getNextWord(first, second);
            newText += " " + third;
            if (wordNo % 20 == 0) newText += "\n";
            first = second;
            second = third;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "[" << label << "] Generated data:\n" << newText << "\n";
        std::cout << "[" << label << "] Time elapsed: " << elapsed.count() << " seconds\n";
    } catch (const std::exception& e) {
        std::cerr << "[" << label << "] Error: " << e.what() << "\n";
    }
    
}

int main() {
    std::string path;
    std::cout << "Enter input file path: ";
    if (!std::getline(std::cin, path)) { std::cerr << "No path given\n"; return 1; }

    std::cout << "\n--- Comparing Hash Table Implementations ---\n\n";

    size_t N = estimateUniquePairs(path);
    std::cout << "Estimated unique StringPair count (N): " << N << std::endl; // <-- Print N

    if (N == 0) {
        std::cerr << "No unique pairs found or file error.\n";
        return 1;
    }
    size_t chainingBuckets = N;
    size_t openAddressingBuckets = 2 * N;
    size_t cuckooBuckets = 3 * N;

    std::cout << "SeparateChaining bucket count: " << chainingBuckets << std::endl;
    std::cout << "LinearProbing bucket count: " << openAddressingBuckets << std::endl;
    std::cout << "CuckooHash bucket count: " << cuckooBuckets << std::endl;

    runGenerator<UnorderedWrapper<StringPair, FreqList>>(path, "std::unordered_map", chainingBuckets);
    runGenerator<SeparateChaining<StringPair, FreqList>>(path, "SeparateChaining", chainingBuckets);
    runGenerator<LinearProbing<StringPair, FreqList>>(path, "LinearProbing", openAddressingBuckets);
    runGenerator<CuckooHash<StringPair, FreqList>>(path, "CuckooHash", cuckooBuckets);
    return 0;
}