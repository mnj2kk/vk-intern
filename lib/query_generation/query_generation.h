//
// Created by mnj2kk on 4/17/22.
//

#pragma once

#include <vector>
#include <random>
#include <ctime>
#include "suggestion/suggestion.h"

class Engine {
    std::mt19937_64 engine_;
    std::vector<std::pair<std::vector<Suggestion>, std::string>> &suggestion_;

    std::u16string change(std::u16string str);

public:

    Engine(std::vector<std::pair<std::vector<Suggestion>, std::string>> &suggestion);

    std::vector<Suggestion> get();

};
