#include <iostream>
#include <string>
#include <algorithm>
#include "parser/parser.h"
#include "suggestion/suggestion.h"

void suggest_print(std::vector<Suggestion> &suggestion);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No path-file logs -> <line_by_line_logs>" << std::endl;
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Can't open this file: " << argv[1] << std::endl;
        return 1;
    }
    std::cerr << "Loading logs.." << std::endl;
    std::shared_ptr<Trie> ptr = std::make_shared<Trie>();
    Parser logs(input);
    std::vector<std::pair<std::vector<Suggestion>, std::string>> suggestion = std::move(logs.start());
    logs.update(suggestion);
    sort(suggestion.rbegin(), suggestion.rend());
    size_t DEPTH = 0;
    for (size_t i = 0; i < suggestion.size(); i++) {
        for (auto &item : suggestion[i].first) {
            ptr->add(item.word16_, i);
            DEPTH = std::max(DEPTH, item.word16_.size());
        }
    }
    std::cerr << "Successful added " << suggestion.size() << " logs!" << std::endl;
    Lvn strCorrecter(DEPTH);
    Parser user(std::cin);
    while (true) {
        std::pair<std::vector<Suggestion>, std::string> query = user.readLine();
        if (query.first.empty()) {
            break;
        }
        for (auto &item : query.first) {
            strCorrecter.increase(item.word16_.size());
            auto callBack = std::get<2>(strCorrecter.correct(item.word16_, ptr.get()));
            if (!callBack.empty()) {
                item.word_ = prs::convertU8((item.word16_ = callBack));
            }
        }
        suggest_print(query.first);
        auto result = search(ptr.get(), query.first);
        if (!result.empty()) {
            std::cout << "I found " << result.size() << " suggestion for you:" << std::endl;
            for (const size_t &index: result) {
                std::cout << suggestion[index].second << std::endl;
            }
        } else {
            std::cout << "Nothing to suggest.." << std::endl;
        }
    }
    return 0;
}

void suggest_print(std::vector<Suggestion> &suggestion) {
    std::cout << "[System message]" << std::endl;
    std::cout << "I have adjusted your request to:" << std::endl;
    for (const auto &item: suggestion) {
        std::cout << item.word_ << ' ';
    }
    std::cout << std::endl;
    std::cout << "[System message]" << std::endl << std::endl;
}
