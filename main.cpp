#include <iostream>
#include <fstream>
#include <vector>
#include "suggestion.h"

const std::u16string u16_empty = convert_u16("");

// Допущение 3 ошибок в написании
std::pair <std::u16string, int> find_with_corrects(InvertedSuggestion *ptr, const std::u16string &str,
                                                   std::u16string &current_ans, char16_t p,
                                                   std::vector <std::vector <int> > &source) {
    std::pair <std::u16string, int> u16_return(u16_empty, 4);
    if (p != '\0') {
        current_ans += p;
        size_t i = current_ans.size();
        source[i][0] = (int) i;
        for (size_t j = 1; j <= str.size(); j++) {
            source[i][j] = std::min(source[i - 1][j] + 1,
                            std::min(source[i][j - 1] + 1, source[i - 1][j - 1] + (str[j - 1] != current_ans[i - 1])));
        }
        if (source[i][str.size()] < u16_return.second) {
            u16_return = {current_ans, source[i][str.size()]};
        }
    }
    for (auto &[first, second] : ptr->next_link_) {
        auto item = find_with_corrects(second, str, current_ans, first, source);
        if (u16_return.second > item.second) u16_return = item;
    }
    if (p != '\0')
        current_ans.pop_back();
    return u16_return;
}

int main(int argc, char *argv[]) {
    // Обработка отсутствия названия файла с логами
    if (argc < 2) {
        std::cerr << "Write path filename with <line_by_line_suggestion>" << std::endl;
        return 1;
    }
    auto *ptr = new InvertedSuggestion; // создание бора
    std::vector <std::pair <std::vector <Suggestion>, std::string> > suggestion;
    std::ifstream in(argv[1]); // открытие файла с логами
    if (!in) { // проверка на корректность открытие файла и выдача ошибки, в случае некорректности
        std::cerr << "Can't open this file" << std::endl;
        delete ptr;
        return 1;
    }
    std::vector <Suggestion> dictionary;
    std::cerr << "Adding suggestion.." << std::endl;
    size_t DEPTH = 0;
    build(in, suggestion, dictionary, ptr, DEPTH); // загрузка логов
    in.close(); // закрытие файла
    std::cerr << "Successfully added " << suggestion.size() << " suggestion!" << std::endl;
    std::string line, word;

    // считывание запросов
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
            std::vector <std::vector <int> > source(DEPTH + 1, std::vector <int> (str.size() + 1));
            for (size_t j = 0; j <= str.size(); j++) source[0][j] = (int) j;
            auto item = find_with_corrects(ptr, str, ans, '\0', source);
            if (item.first != u16_empty) word = convert_u8(item.first);
            input.emplace_back(word, convert_u16(word), 0);
        }
        if (!search(input, result, ptr)) {
            std::cout << "Nothing to suggest.." << std::endl;
            continue;
        }
        std::cout << "I find " << result.size() << " suggestion for you:" << std::endl;
        for (const size_t &index : result) {
            std::cout << suggestion[index].second << std::endl;
        }
    }
    delete ptr;
    return 0;
}
