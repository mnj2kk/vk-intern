//
// Created by mnj2kk on 4/5/22.
//

#include "suggestion.h"

// Реализация добавления нашего префикса в бор
void Trie::add(const std::u16string &word, const size_t &id) {
    Trie *node = this;
    for (const char16_t &item : word) {
        if (!node->next_link_.contains(item))
            node->next_link_.insert({ item, new Trie });
        node = node->next_link_[item];
        if (node->ids_.empty() || node->ids_.back() != id)
            node->ids_.push_back(id);
    }
}

// Реализация поиска префикса (в случае отсутствия возвращает nullptr)
Trie *Trie::find(const std::u16string &prefix) {
    Trie *node = this;
    for (const char16_t &item : prefix) {
        if (!node->next_link_.contains(item))
            return nullptr;
        node = node->next_link_[item];
    }
    return node;
}

// Деструктор класса
Trie::~Trie() {
    for (auto &[first, second] : next_link_)
        delete second;
}

void to_lower(std::string &str) {
    std::u16string str16 = convert_u16(str);
    for (char16_t &item : str16) {
        if (item >= 1040 && item <= 1071) { // int(char) для заглавных русских букв
            item += 32;
        } else {
            item = tolower(item);
        }
    }
    str = std::move(convert_u8(str16));
}

void trim_string(std::string &str) {
    // Удаление пунктуации и перевода строки
    while (!str.empty() && (ispunct(str.back()) || str.back() == '\n' || str.back() == '\r' || str.back() == '\t'))
        str.pop_back();
}

// Компаратор работает в точности как написано в README.md
bool cmp(std::pair <std::vector <Suggestion>, std::string> &first_,
         std::pair <std::vector <Suggestion>, std::string> &second_) {
    size_t index = 0;
    while (index < std::min(first_.first.size(), second_.first.size())) {
        if (first_.first[index].population_ > second_.first[index].population_) {
            return true;
        } else {
            if (first_.first[index].population_ < second_.first[index].population_) {
                return false;
            } else {
                if (first_.first[index].word_ == second_.first[index].word_) {
                    ++index;
                } else {
                    return first_.first[index].word_ < second_.first[index].word_;
                }
            }
        }
    }
    return index == first_.first.size();
}

std::u16string convert_u16(const std::string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.from_bytes(source);
}

std::string convert_u8(const std::u16string &source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.to_bytes(source);
}

void build(std::istream &in, std::vector <std::pair <std::vector <Suggestion>,
            std::string> > &suggestion, Trie *node, size_t &DEPTH) {
    std::string line, word;
    std::unordered_map <std::string, unsigned int> word_count;
    while (std::getline(in, line)) {
        trim_string(line);
        if (line.empty()) continue; // пропуск пустых строчек
        std::stringstream parse(line);
        suggestion.emplace_back(std::vector <Suggestion> (), line);
        while (std::getline(parse, word, ' ')) {
            trim_string(word);
            if (word.empty()) continue; // пропуск двойных пробелов
            to_lower(word);
            DEPTH = std::max(DEPTH, word.size());
            if (!word_count.contains(word)) word_count.insert({ word, 1 });
            else ++word_count[word];
            suggestion.back().first.emplace_back(word, convert_u16(word), 0);
        }
    }
    // заполнение кол-во вхождений слов в логи
    for (auto &[first, second] : suggestion) {
        for (Suggestion &item : first) {
            item.population_ = word_count[item.word_];
        }
    }
    sort(suggestion.begin(), suggestion.end(), cmp); // сортировка наших логов, по приоритету
    add_logs(suggestion, node);
}

void add_logs(std::vector <std::pair <std::vector <Suggestion>, std::string> > &suggestion, Trie *node) {
    for (size_t i = 0; i < suggestion.size(); i++) {
        for (const Suggestion &item : suggestion[i].first) {
            node->add(item.word16_, i);
        }
    }
}

bool intersect(const std::vector<const std::vector<size_t> *> &suggest, std::vector<size_t> &top_result) {
    if (suggest.empty()) return false;
    top_result = *suggest[0];
    for (size_t i = 1; i < suggest.size(); i++) {
        std::vector <size_t> item = *suggest[i];
        size_t index = 0;
        std::vector <size_t> new_result;
        for (size_t j = 0; j < top_result.size(); j++) {
            while (index < item.size() && item[index] < top_result[j]) ++index;
            if (index != item.size() && item[index] == top_result[j]) new_result.emplace_back(top_result[j]);
        }
        top_result = std::move(new_result);
    }
    top_result.resize(std::min(TOP_SUGGEST, top_result.size()));
    return !top_result.empty();
}

// поиск нужны нам id
bool search(const std::vector<Suggestion> &input, std::vector<size_t> &result, Trie *node) {
    std::vector <const std::vector <size_t> *> suggest;
    for (const Suggestion &item : input) {
        auto *ptr = node->find(item.word16_);
        if (!ptr) return false;
        suggest.push_back(&ptr->ids_);
    }
    std::vector <size_t> res;
    if (!intersect(suggest, res)) return false;
    for (const size_t &item : res) {
        result.emplace_back(item);
    }
    return true;
}

// Допущение 2 ошибок с помощью Дамерау-Левенштейна
std::pair <std::u16string, int> find_with_corrects(Trie *ptr, const std::u16string &str,
                                                   std::u16string &current_ans, char16_t p,
                                                   std::vector <std::vector <int> > &source) {
    std::pair <std::u16string, int> u16_return(u16_empty, 3);
    if (p != '\0') {
        current_ans += p;
        size_t j = current_ans.size();
        for (size_t i = 1; i <= str.size(); i++) {
            if (i > 1 && j > 1 && str[i - 1] == current_ans[j - 2] && str[i - 2] == current_ans[j - 1]) {
                source[i][j] = std::min(source[i - 1][j] + 1, std::min(source[i][j - 1] + 1,
                                   std::min(source[i - 2][j - 2] + 1,
                                            source[i - 1][j - 1] + (str[i - 1] != current_ans[j - 1]))));
            } else {
                source[i][j] = std::min(source[i - 1][j] + 1,
                                        std::min(source[i][j - 1] + 1,
                                                 source[i - 1][j - 1] + (str[i - 1] != current_ans[j - 1])));
            }
        }
        if (source[str.size()][j] < u16_return.second) {
            u16_return = {current_ans, source[str.size()][j]};
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