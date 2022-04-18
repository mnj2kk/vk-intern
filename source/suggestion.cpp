//
// Created by mnj2kk on 4/10/22.
//

#include <unordered_map>
#include "suggestion/suggestion.h"

bool Suggestion::operator<(const Suggestion &other) const {
    return std::tie(this->population_, this->word16_) < std::tie(other.population_, other.word16_);
}

void Trie::add(const std::u16string &word16, size_t id) {
    auto node = this;
    for (const char16_t &item: word16) {
        if (!node->next_vertex_.contains(item)) {
            node->next_vertex_.insert({item, std::make_shared<Trie>()});
        }
        node = node->next_vertex_[item].get();
        if (node->ids_.empty() || node->ids_.back() != id) {
            node->ids_.emplace_back(id);
        }
        node->population_++;
    }
}

Trie *Trie::find(const std::u16string &prefix) {
    auto node = this;
    for (const char16_t &item: prefix) {
        if (!node->next_vertex_.contains(item)) {
            return nullptr;
        }
        node = node->next_vertex_[item].get();
    }
    return node;
}

Lvn::Lvn(size_t DEPTH) {
    DEPTH_ = DEPTH;
    levenshtein_.emplace_back(std::vector<int>(DEPTH_ + 1));
    for (size_t i = 0; i <= DEPTH_; i++) {
        levenshtein_[0][i] = static_cast<int>(i);
    }
}

void Lvn::increase(size_t size) {
    while (levenshtein_.size() <= size) {
        levenshtein_.emplace_back(levenshtein_.back());
        levenshtein_.back()[0] = static_cast<int>(levenshtein_.size() - 1);
    }
}

void Lvn::update(const std::u16string &str) {
    size_t j = current_.size();
    for (size_t i = 1; i <= str.size(); i++) {
        levenshtein_[i][j] = std::min(levenshtein_[i - 1][j] + 1,
                                      levenshtein_[i - 1][j - 1] + (str[i - 1] != current_[j - 1]));
        if (i > 1 && j > 1 && str[i - 1] == current_[j - 2] && str[i - 2] == current_[j - 1]) {
            levenshtein_[i][j] = std::min(levenshtein_[i - 2][j - 2] + 1, levenshtein_[i][j]);
        }
    }
}

std::tuple<int, int, std::u16string> Lvn::correct(const std::u16string &str, Trie *ptr, char16_t prev) {
    bool is_continue = true;
    auto u16_return = std::make_tuple(3, -1, std::u16string{});
    if (prev != '\0') {
        current_ += prev;
        update(str);
        u16_return = std::min(u16_return,
                              std::make_tuple(levenshtein_[str.size()][current_.size()], ptr->population_, current_));
    }
    if (is_continue) {
        for (auto &[first, second]: ptr->next_vertex_) {
            u16_return = std::min(u16_return, correct(str, second.get(), first));
        }
    }
    if (prev != '\0') {
        current_.pop_back();
    }
    return u16_return;
}

std::vector<size_t> search(Trie *ptr, std::vector<Suggestion> &query) {
    std::vector<std::vector<size_t> *> attempt;
    for (auto &item: query) {
        auto node = ptr->find(item.word16_);
        if (!node) {
            return {};
        }
        attempt.push_back(&node->ids_);
    }
    auto result = intersect(attempt);
    result.shrink_to_fit();
    return result;
}

std::vector<size_t> intersect(std::vector<std::vector<size_t> *> &attempt) {
    std::vector<size_t> result;
    std::vector<size_t> index(attempt.size());
    while (index[0] < attempt[0]->size()) {
        bool valid = true;
        for (size_t i = 1; i < attempt.size(); i++) {
            while (index[i] < attempt[i]->size() && (*attempt[i])[index[i]] < (*attempt[0])[index[0]]) {
                index[i]++;
            }
            if (index[i] == attempt[i]->size() || (*attempt[i])[index[i]] != (*attempt[0])[index[0]]) {
                valid = false;
                break;
            }
        }
        if (valid) {
            result.push_back((*attempt[0])[index[0]]);
        }
        ++index[0];
    }
    result.resize(std::min(result.size(), TOP_SUGGEST));
    return result;
}
