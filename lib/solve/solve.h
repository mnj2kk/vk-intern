//
// Created by mnj2kk on 4/18/22.
//

#pragma once

#include <vector>
#include <utility>
#include <set>
#include "suggestion/suggestion.h"

class Solver {
    std::vector<std::pair<std::vector<Suggestion>, std::string>> &logs_;

public:
    Solver(std::vector<std::pair<std::vector<Suggestion>, std::string>> &logs);

    std::vector<size_t> get_suggest(std::vector<Suggestion> &query);

};
