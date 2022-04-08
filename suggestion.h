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
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cmath>

// Константа поддерживающая максимальное кол-во возвращаемых запросов
const size_t TOP_SUGGEST = 5;

// Класс хранящий слово, слово в UTF-16 и кол-во раз сколько оно встречается в тексте, для дальнейшей сортировки
class Suggestion {
public:
    std::string word_; // само слово
    std::u16string word16_; // слово в UTF-16
    size_t population_; // сколько раз оно встречается

    // Конструктор нашего класса
    Suggestion(std::string word, std::u16string word16 , size_t x) :
        word_(std::move(word)), word16_(std::move(word16)), population_(x) {}

    // Дефолтный деструктор
    ~Suggestion() = default;
};

// Класс обратных индексов, который для чара хранит индексы где он встречается, а так же является бором
class InvertedSuggestion {
public:
    std::map <char16_t, InvertedSuggestion *> next_link_; // указатели на следующие вершины в боре
    std::vector <size_t> ids_; // список запросов, где встречается данный префикс

    // Функция добавления префикса в бор
    void add(const std::u16string &word, const size_t &id);

    // Функция для поиска заданного префикса в боре
    InvertedSuggestion *find(const std::u16string &prefix);

    // Деструктор класса
    ~InvertedSuggestion();
};

// Функция для конвертации нашей строки из UTF-8 -> UTF-16
std::u16string convert_u16(const std::string &source);

// Функция для конвертации из UTF-8 -> UTF-16
std::string convert_u8(const std::u16string &source);

// Функция для приведения строки к нижнему регистру
void to_lower(std::string &str);

// Функция для обрезания нашей строки по заданным в реализации параметрам
void trim_string(std::string &str);

// Компаратор для сортировки наших логов
bool cmp(std::pair <std::vector <Suggestion>, std::string> &first_,
         std::pair <std::vector <Suggestion>, std::string> &second_);

// Создание нашего датасета на основе логов
void build(std::istream &in, std::vector <std::pair <std::vector <Suggestion>, std::string> > &suggestion,
           std::vector <Suggestion> &dictionary,  InvertedSuggestion *node, size_t &DEPTH);

// Добавление наших запросов в бор
void build(std::vector <std::pair <std::vector <Suggestion>, std::string> > &suggestion, InvertedSuggestion *node);

// Поиск подсказок для нашего запроса
bool search(const std::vector<Suggestion> &input, std::vector<size_t> &result, InvertedSuggestion *node);

// Пересечение получившихся индексов
bool intersect(const std::vector <const std::vector <size_t> *> &suggest, std::vector <size_t> &top_result);