//
// Created by mnj2kk on 4/8/22.
//

#include "../suggestion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

std::mt19937 engine;

Suggestion void_suggestion("", convert_u16(""), 0);

void gen_second_input(std::vector <Suggestion> &input, std::vector <Suggestion> &second_input) {
    std::vector <bool> used(input.size());
    if (input.empty()) return;
    second_input.assign((engine() % input.size()) + 1, void_suggestion);
    for (auto &item : second_input) {
        auto index = engine() % input.size();
        auto cnt = (int) 2;
        while (!used[index] && cnt--) index = engine() % input.size();
        if (used[index]) {
            for (size_t i = 0; i < input.size(); i++) {
                if (!used[i]) {
                    item = input[i];
                    used[i] = true;
                    break;
                }
            }
        } else {
            used[index] = true;
            item = input[index];
        }
    }
}

int main(int argc, char *argv[]) {
    std::random_device device;
    engine.seed(device());
    if (argc < 2) {
        std::cerr << "Write path filename with <line_by_line_suggestion>" << std::endl;
        return 1;
    }
    auto *ptr = new Trie;
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Can't open this file" << std::endl;
        delete ptr;
        return 1;
    }
    std::vector <std::pair <std::vector <Suggestion>, std::string> > suggestion;
    size_t DEPTH = 0;
    build(in, suggestion, ptr, DEPTH);
    in.close();
    in.open(argv[1]);
    std::string line, word;
    auto verdict = true;

    std::vector <std::vector <int> > source(1, std::vector <int> (DEPTH + 1));
    for (size_t i = 0; i <= DEPTH; i++) source[0][i] = i;

    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::vector <size_t> result;
        std::vector <Suggestion> input;
        std::stringstream parse(line);
        while (std::getline(parse, word, ' ')) {
            trim_string(word);
            if (word.empty()) continue;
            to_lower(word);
            auto str = convert_u16(word), ans = u16_empty;
            while (source.size() < str.size() + 1) {
                source.emplace_back(source.back());
                source.back()[0] = source.size() - 1;
            }
            auto item = find_with_corrects(ptr, str, ans, '\0', source);
            if (item.first != u16_empty) word = convert_u8(item.first);
            input.emplace_back(word, convert_u16(word), 0);
        }
        if (input.empty()) continue;
        if (!search(input, result, ptr)) {
            verdict = false;
            break;
        }
        result.clear();
        std::vector <Suggestion> second_input;
        gen_second_input(input, second_input);
        if (!second_input.empty() && !search(second_input, result, ptr)) {
            verdict = false;
            break;
        }
    }
    in.close();
    std::cout << (verdict ? "Correct" : "Sad..");
    delete ptr;
}