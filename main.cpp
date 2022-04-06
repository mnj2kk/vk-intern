#include <iostream>
#include <fstream>
#include <vector>
#include "suggestion.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Write path filename with <line_by_line_suggestion>" << std::endl;
        return 1;
    }
    auto* ptr = new InvertedSuggestion;
    std::vector <std::pair <std::vector <Suggestion>, std::string> > suggestion;
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Can't open this file" << std::endl;
        return 1;
    }
    std::cerr << "Adding suggestion.." << std::endl;
    build(in, suggestion, ptr);
    std::cerr << "Successfully added " << suggestion.size() << " suggestion!" << std::endl;
    std::string line, word;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::vector <size_t> result;
        std::vector <Suggestion> input;
        std::stringstream parse(line);
        while (std::getline(parse, word, ' ')) {
            trim_string(word);
            if (word.empty()) continue;
            input.emplace_back(word, 0);
        }
        if (!search(input, result, ptr)) {
            std::cout << "Nothing to suggest.." << std::endl;
            continue;
        }
        std::cout << "I find " << result.size() << " suggestion for you:" << std::endl;
        for (const auto & index : result) {
            std::cout << suggestion[index].second << std::endl;
        }
    }
    delete ptr;
    return 0;
}
