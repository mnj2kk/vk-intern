//
// Created by mnj2kk on 4/17/22.
//

#pragma once

#include <vector>
#include <string>
#include "suggestion/suggestion.h"

class CheckSuggest {
    std::shared_ptr<Trie> &ptr_;
    std::vector<std::u16string> &dictionary_;

    std::tuple<int, int, std::u16string> get_levenshtein(std::u16string &str, std::u16string &correct);

public:

    CheckSuggest(std::shared_ptr<Trie> &ptr, std::vector<std::u16string> &dictionary);

    std::vector<Suggestion> check(std::vector<Suggestion> &query);
};
