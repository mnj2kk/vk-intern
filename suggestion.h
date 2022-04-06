//
// Created by mnj2kk on 4/5/22.
//

#pragma once

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

const size_t TOP_SUGGEST = 5;

class Suggestion {
public:
    std::string word_;
    size_t population_;

    Suggestion(std::string word, size_t x) : word_(std::move(word)), population_(x) {}

    ~Suggestion() = default;
};

class InvertedSuggestion {
public:
    std::map <char, InvertedSuggestion*> next_link_;
    std::vector <size_t> ids_;

    void add(const std::string &word, const size_t &id);

    InvertedSuggestion *find(const std::string &prefix);

    ~InvertedSuggestion();
};

void trim_string(std::string &str);

bool cmp(std::pair <std::vector <Suggestion>, std::string> &first_,
         std::pair <std::vector <Suggestion>, std::string> &second_);

void build(std::istream &in, std::vector <std::pair <
        std::vector <Suggestion>, std::string> > &suggestion, InvertedSuggestion *node);

void build(std::vector <std::pair <std::vector <Suggestion>, std::string> > &suggestion, InvertedSuggestion *node);

bool search(const std::vector<Suggestion> &input, std::vector<size_t> &result, InvertedSuggestion *node);

bool intersect(const std::vector <const std::vector <size_t> *> &suggest, std::vector <size_t> &top_result);