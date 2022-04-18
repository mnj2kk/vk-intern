//
// Created by mnj2kk on 4/17/22.
//

#include "correcter/correcter.h"

std::vector<Suggestion> CheckSuggest::check(std::vector<Suggestion> &query) {
    std::vector<Suggestion> n_query;
    for (auto &item: query) {
        auto n_item = std::make_tuple(3, -1, item.word16_);
        for (auto &value: dictionary_) {
            std::u16string cur_string;
            for (auto &char_value: value) {
                cur_string += char_value;
                n_item = std::min(n_item, get_levenshtein(item.word16_, cur_string));
            }
        }
        n_query.emplace_back(std::string(), std::get<2>(n_item), 0);
    }
    return n_query;
}

std::tuple<int, int, std::u16string> CheckSuggest::get_levenshtein(std::u16string &str, std::u16string &correct) {
    std::vector<std::vector<int>> levenshtein_(str.size() + 1, std::vector<int>(correct.size() + 1));
    for (size_t i = 1; i <= str.size(); i++) {
        levenshtein_[i][0] = static_cast<int>(i);
    }
    for (size_t i = 1; i <= correct.size(); i++) {
        levenshtein_[0][i] = static_cast<int>(i);
    }
    for (size_t i = 1; i <= str.size(); i++) {
        for (size_t j = 1; j <= correct.size(); j++) {
            levenshtein_[i][j] = std::min(levenshtein_[i - 1][j] + 1,
                                          levenshtein_[i - 1][j - 1] + (str[i - 1] != correct[j - 1]));
            if (i > 1 && j > 1 && str[i - 1] == correct[j - 2] && str[i - 2] == correct[j - 1]) {
                levenshtein_[i][j] = std::min(levenshtein_[i - 2][j - 2] + 1, levenshtein_[i][j]);
            }
        }
    }
    return std::make_tuple(levenshtein_.back().back(), ptr_->find(correct)->population_, correct);
}

CheckSuggest::CheckSuggest(std::shared_ptr<Trie> &ptr, std::vector<std::u16string> &dictionary)
        : ptr_(ptr), dictionary_(dictionary) {}
