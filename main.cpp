#include <iostream>
#include <fstream>
#include <vector>
#include "suggestion.h"

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
    std::cerr << "Adding suggestion.." << std::endl;
    build(in, suggestion, ptr); // загрузка логов
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
