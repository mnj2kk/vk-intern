//
// Created by mnj2kk on 4/17/22.
//

#include "query_generation/query_generation.h"
#include <iostream>

std::vector<Suggestion> Engine::get() {
    auto source = suggestion_[static_cast<size_t>(engine_()) % suggestion_.size()].first;
    std::vector<Suggestion> n_suggest(static_cast<size_t>(engine_()) % source.size(),
                                      Suggestion(std::string(), std::u16string(), 0));
    for (auto &n_item: n_suggest) {
        auto pos = static_cast<size_t>(engine_()) % source.size();
        Suggestion ins = source[pos];
        if (!source[pos].word16_.empty()) {
            ins = Suggestion(std::string(), change(source[pos].word16_), 0);
        }
        n_item = ins;
    }
    std::vector<Suggestion> t_suggest;
    for (auto &t_item : n_suggest) {
        if (!t_item.word16_.empty()) {
            t_suggest.push_back(t_item);
        }
    }
    n_suggest = std::move(t_suggest);
    return n_suggest;
}

Engine::Engine(std::vector<std::pair<std::vector<Suggestion>, std::string>> &suggestion)
        : engine_(time(nullptr)), suggestion_(suggestion) {}

std::u16string Engine::change(std::u16string source) {
    auto str = std::move(source);
    int count = static_cast<int>(engine_()) % 3;
    for (int iteration = 0; iteration < count; iteration++) {
        int type = static_cast<int>(engine_()) % 3;
        if (str.empty()) {
            break;
        }
        size_t pos = static_cast<size_t>(engine_()) % str.size();
        size_t from = static_cast<size_t>(engine_()) % str.size();
        switch (type) {
            case 0:
                str = str.substr(0, pos) + str[from] + str.substr(pos);
                break;
            case 1:
                str.erase(pos);
                break;
            case 2:
                std::swap(str[pos], str[from]);
                break;
            default:
                break;
        }
    }
    return str;
}
