//
// Created by mnj2kk on 4/10/22.
//

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <unordered_map>
#include "suggestion/suggestion.h"

namespace rus {
    const int UPPER_ALPHABET_START = 1040;
    const int UPPER_ALPHABET_END = 1071;
    const int LOWER_ALPHABET_START = 1072;
    const int LOWER_ALPHABET_END = 1103;
    const int LETTERS_COUNT = 32;
}

namespace prs {
    enum Type {
        Stay, Get
    };

    std::u16string convertU16(const std::string &source);

    std::string convertU8(const std::u16string &source);

    void trim(std::string &str, Type value = Get);
}

class Parser {
    std::istream &in_;
    std::string token_;
    std::unordered_map<std::u16string , int> word_count;

    std::vector<Suggestion> parseWords();

    bool hasNext();

public:

    explicit Parser(std::istream &in) : in_(in) {}

    std::unordered_map<std::u16string , int> &getWordCount();

    std::pair<std::vector<Suggestion>, std::string> readLine();

    std::vector<std::pair<std::vector<Suggestion>, std::string>> start();

    void update(std::vector<std::pair<std::vector<Suggestion>, std::string>> &suggestion);

    ~Parser() = default;
};
