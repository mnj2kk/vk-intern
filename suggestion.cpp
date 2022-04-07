//
// Created by mnj2kk on 4/5/22.
//

#include "suggestion.h"

void InvertedSuggestion::add(const std::u16string &word, const size_t &id) {
    InvertedSuggestion *node = this;
    for (const char16_t &value : word) {
        if (!node->next_link_.contains(value))
            node->next_link_.insert({ value, new InvertedSuggestion });
        node = node->next_link_[value];
        if (node->ids_.empty() || node->ids_.back() != id)
            node->ids_.push_back(id);
    }
}

InvertedSuggestion *InvertedSuggestion::find(const std::u16string &prefix) {
    InvertedSuggestion *node = this;
    for (const char16_t &value : prefix) {
        if (!node->next_link_.contains(value))
            return nullptr;
        node = node->next_link_[value];
    }
    return node;
}

InvertedSuggestion::~InvertedSuggestion() {
    for (auto &[first, second] : next_link_)
        delete second;
}

void to_lower(std::string &str) {
    std::u16string str16 = convert_u16(str);
    for (char16_t &value : str16) {
        if (value >= 1040 && value <= 1071) {
            value += 32;
        } else {
            value = tolower(value);
        }
    }
    str = std::move(convert_u8(str16));
}

void trim_string(std::string &str) {
    while (!str.empty() && (ispunct(str.back()) || str.back() == '\n' || str.back() == '\r' || str.back() == '\t'))
        str.pop_back();
}

bool cmp(std::pair <std::vector <Suggestion>, std::string> &first_,
         std::pair <std::vector <Suggestion>, std::string> &second_) {
    size_t index = 0;
    while (index < std::min(first_.first.size(), second_.first.size())) {
        if (first_.first[index].population_ > second_.first[index].population_) {
            return true;
        } else {
            if (first_.first[index].population_ < second_.first[index].population_) {
                return false;
            } else {
                if (first_.first[index].word_ == second_.first[index].word_) {
                    ++index;
                } else {
                    return first_.first[index].word_ < second_.first[index].word_;
                }
            }
        }
    }
    return index == first_.first.size();
}

std::u16string convert_u16(const std::string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.from_bytes(source);
}

std::string convert_u8(const std::u16string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.to_bytes(source);
}

void build(std::istream &in, std::vector <std::pair <
        std::vector <Suggestion>, std::string> > &suggestion, InvertedSuggestion *node) {
    std::string line, word;
    std::unordered_map <std::string, unsigned int> word_count;
    while (std::getline(in, line)) {
        std::stringstream parse(line);
        suggestion.emplace_back(std::vector <Suggestion> (), line);
        while (std::getline(parse, word, ' ')) {
            trim_string(word);
            if (word.empty()) continue;
            to_lower(word);
            if (!word_count.contains(word)) word_count.insert({ word, 1 });
            else ++word_count[word];
            suggestion.back().first.emplace_back(Suggestion(word, convert_u16(word), 0));
        }
    }
    for (auto &[first, second] : suggestion) {
        for (Suggestion &value : first) {
            value.population_ = word_count[value.word_];
        }
    }
    sort(suggestion.begin(), suggestion.end(), cmp);
    build(suggestion, node);
}

void build(std::vector <std::pair <std::vector <Suggestion>, std::string> > &suggestion, InvertedSuggestion *node) {
    for (size_t i = 0; i < suggestion.size(); i++) {
        for (const Suggestion &value : suggestion[i].first) {
            node->add(value.word16_, i);
        }
    }
}

bool intersect(const std::vector<const std::vector<size_t> *> &suggest, std::vector<size_t> &top_result) {
    if (suggest.empty()) return false;
    top_result = *suggest[0];
    for (size_t i = 1; i < suggest.size(); i++) {
        size_t index = 0;
        std::vector <size_t> new_result;
        for (const size_t &value : *suggest[i]) {
            while (index < top_result.size() && top_result[index] < value) index++;
            if (index != top_result.size() && value == top_result[index]) new_result.emplace_back(value);
        }
        top_result = std::move(new_result);
    }
    top_result.resize(std::min(TOP_SUGGEST, top_result.size()));
    return !top_result.empty();
}

bool search(const std::vector<Suggestion> &input, std::vector<size_t> &result, InvertedSuggestion *node) {
    std::vector <const std::vector <size_t> *> suggest;
    for (const Suggestion &value : input) {
        auto *ptr = node->find(value.word16_);
        if (!ptr) return false;
        suggest.push_back(&ptr->ids_);
    }
    std::vector <size_t> res;
    if (!intersect(suggest, res)) return false;
    for (const size_t &value : res) {
        result.emplace_back(value);
    }
    return true;
}