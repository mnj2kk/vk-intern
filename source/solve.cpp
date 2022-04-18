//
// Created by mnj2kk on 4/18/22.
//

#include "solve/solve.h"

Solver::Solver(std::vector<std::pair<std::vector<Suggestion>, std::string>> &logs) : logs_(logs) {}

std::vector<size_t> Solver::get_suggest(std::vector<Suggestion> &query) {
    std::vector<size_t> returned;
    for (size_t i = 0; i < logs_.size(); i++) {
        std::set<Suggestion> unused_prefix(query.begin(), query.end());
        for (const auto &value : logs_[i].first) {
            auto copy_set = unused_prefix;
            for (const auto &prefix : copy_set) {
                if (prefix.word16_.size() > value.word16_.size()) {
                    continue;
                }
                bool valid = true;
                for (size_t ind = 0; ind < prefix.word16_.size(); ind++) {
                     valid = prefix.word16_[ind] == value.word16_[ind];
                     if (!valid) {
                         break;
                     }
                }
                if (valid) {
                    unused_prefix.erase(prefix);
                }
            }
        }
        if (unused_prefix.empty()) {
            returned.emplace_back(i);
        }
        if (returned.size() == 5) {
            break;
        }
    }
    return returned;
}
