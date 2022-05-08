//
// Created by mnj2kk on 4/10/22.
//

#include <sstream>
#include "parser/parser.h"

std::u16string prs::convertU16(const std::string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(source);
}

std::string prs::convertU8(const std::u16string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(source);
}

void prs::trim(std::string &str, prs::Type value) {
    while (!str.empty() && isspace(str.back()))
        str.pop_back();
    if (value == prs::Stay) {
        return;
    }
    auto lower = [](int item) {
        if (item >= rus::UPPER_ALPHABET_START && item <= rus::UPPER_ALPHABET_END) {
            return static_cast<char16_t>(item + rus::LETTERS_COUNT);
        }
        return static_cast<char16_t>(tolower(item));
    };
    std::u16string str16 = prs::convertU16(str);
    for (char16_t &item: str16) {
        item = lower(static_cast<int>(item));
    }
    str = std::move(convertU8(str16));
}

bool Parser::hasNext() {
    return !in_.eof();
}

std::pair<std::vector<Suggestion>, std::string> Parser::readLine() {
    std::getline(in_, token_);
    trim(token_, prs::Stay);
    std::pair<std::vector<Suggestion>, std::string> returned(std::vector<Suggestion>(), token_);
    if (!token_.empty()) {
        returned.first = parseWords();
    }
    return returned;
}

std::vector<Suggestion> Parser::parseWords() {
    std::stringstream words(token_);
    std::string word;
    std::vector <Suggestion> returned;
    while (std::getline(words, word, ' ')) {
        prs::trim(word);
        if (word.empty()) {
            continue;
        }
        auto word16 = prs::convertU16(word);
        returned.emplace_back(word, word16, 0);
        if (word_count.contains(word16)) {
            ++word_count[word16];
        } else {
            word_count.insert({word16, 1});
        }
    }
    return returned;
}

std::unordered_map<std::u16string , int> &Parser::getWordCount() {
    return word_count;
}

std::vector<std::pair<std::vector<Suggestion>, std::string>> Parser::start() {
    std::vector<std::pair<std::vector<Suggestion>, std::string>> returned;
    while (hasNext()) {
        auto emplace = readLine();
        if (!emplace.first.empty()) {
            returned.push_back(std::move(emplace));
        }
    }
    return returned;
}

void Parser::update(std::vector<std::pair<std::vector<Suggestion>, std::string>> &suggestion) {
    for (auto &[first, second] : suggestion) {
        for (auto &item : first) {
            item.population_ = word_count[item.word16_];
        }
    }
}
