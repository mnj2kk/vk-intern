//
// Created by mnj2kk on 4/18/22.
//

#include <iostream>
#include "suggestion/suggestion.h"
#include "parser/parser.h"
#include "query_generation/query_generation.h"
#include "solve/solve.h"
#include "correcter/correcter.h"

std::vector<Suggestion> correct(std::shared_ptr<Trie> &ptr, std::vector<Suggestion> &query, Lvn &crt);

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
    std::shared_ptr<Trie> ptr = std::make_shared<Trie>();
    Parser logs(input);
    std::vector<std::pair<std::vector<Suggestion>, std::string>> suggestion = std::move(logs.start());
    logs.update(suggestion);
    sort(suggestion.rbegin(), suggestion.rend());
    size_t DEPTH = 0;
    std::vector<std::u16string> dictionary;
    for (size_t i = 0; i < suggestion.size(); i++) {
        for (auto &item: suggestion[i].first) {
            ptr->add(item.word16_, i);
            dictionary.emplace_back(item.word16_);
            DEPTH = std::max(DEPTH, item.word16_.size());
        }
    }
    sort(dictionary.begin(), dictionary.end());
    dictionary.resize(unique(dictionary.begin(), dictionary.end()) - dictionary.begin());
    dictionary.shrink_to_fit();
    Lvn my_correct(DEPTH);
    size_t TEST_COUNT;
    std::cerr << "Input test count: " << std::endl;
    std::cin >> TEST_COUNT;
    Engine query_engine(suggestion);
    CheckSuggest stupid_correct(ptr, dictionary);
    Solver stupid_solver(suggestion);
    for (size_t i = 0; i < TEST_COUNT; i++) {
        auto test = query_engine.get();
        auto my_test_correct = correct(ptr, test, my_correct);
        auto stupid_test_correct = stupid_correct.check(test);
        if (test.empty()) {
            continue;
        }
        if (search(ptr.get(), my_test_correct) != stupid_solver.get_suggest(stupid_test_correct)) {
            std::cerr << "Incorrect answers. Test number " << i << ": " << std::endl;
            for (const auto &item: test) {
                std::cerr << prs::convertU8(item.word16_) << ' ';
            }
            std::cerr << std::endl;
            return 1;
        }
    }
    std::cerr << "All tests passed successfully!" << std::endl;
}

std::vector<Suggestion> correct(std::shared_ptr<Trie> &ptr, std::vector<Suggestion> &query, Lvn &crt) {
    auto n_query = query;
    for (auto &item: n_query) {
        crt.increase(item.word16_.size());
        auto callBack = std::get<2>(crt.correct(item.word16_, ptr.get()));
        if (!callBack.empty()) {
            item.word_ = prs::convertU8((item.word16_ = callBack));
        }
    }
    return n_query;
}