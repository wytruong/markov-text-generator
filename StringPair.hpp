#pragma once
#include <string>
#include <functional>

struct StringPair {
    std::string first;
    std::string second;

    StringPair() = default;
    StringPair(std::string f, std::string s) : first(std::move(f)), second(std::move(s)) {}

    bool operator==(const StringPair& other) const {
        return first == other.first && second == other.second;
    }
};

namespace std {
    template<> struct hash<StringPair> {
        std::size_t operator()(const StringPair& sp) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(sp.first);
            std::size_t h2 = std::hash<std::string>{}(sp.second);
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
        }
    };
}
