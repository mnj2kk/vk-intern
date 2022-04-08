#include <iostream>
#include <vector>
#include "suggestion.h"

int main(int argc, char *argv[]) {
    // Обработка отсутствия названия файла с логами
    if (argc < 2) {
        std::cerr << "Write path filename with <line_by_line_suggestion>" << std::endl;
        return 1;
    }
    auto *ptr = new Trie; // создание бора
    std::ifstream in(argv[1]); // открытие файла с логами
    if (!in) { // проверка на корректность открытие файла и выдача ошибки, в случае некорректности
        std::cerr << "Can't open this file" << std::endl;
        delete ptr;
        return 1;
    }
    std::cerr << "Adding suggestion.." << std::endl;
    std::vector <std::pair <std::vector <Suggestion>, std::string> > suggestion;
    size_t DEPTH = 0;
    build(in, suggestion, ptr, DEPTH); // загрузка логов
    in.close(); // закрытие файла
    std::cerr << "Successfully added " << suggestion.size() << " suggestion!" << std::endl;
    std::string line, word;

    std::vector <std::vector <int> > source(1, std::vector <int> (DEPTH + 1));
    for (size_t i = 0; i <= DEPTH; i++) source[0][i] = i;

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
            while (source.size() < str.size() + 1) {
                source.emplace_back(source.back());
                source.back()[0] = source.size() - 1;
            }
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
