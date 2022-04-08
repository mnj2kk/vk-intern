# Система автоматического дополнения запросов

В данной статье рассказано о задаче и подходу к её решению

## Постановка задачи

> Написать систему автоматического дополнения запросов, которая на запрос 
пользователя будет выдавать на основе датасета не более 5 лучших подсказок
## Подход к реализации

Основополагающим компонентом в реализации будут прямые и обратные индексы. В кратце, прямой индекс - это номер запроса в нашем датасете, а обратный индекс это слово и соответвующие ему номера запросов, в которых оно встречается. 

На введенный пользователем запрос мы будем для каждого слова (префикса) искать набор прямых индексов, в которых оно встречается и пересекать их, получая нужные набор подсказок. 

Для быстрого поиска префиксов мы будем использовать бор (т.к. поиск префикса в нем происходит `O(|prefix|)`).

Чтобы ранжировать наши подсказки будем сортировать изначальный датасет, таким образом, что чем выше занимает позицию запрос - тем он имеет больший приоритет.

Ранжирование происходит следующим образом:
<ol>
  <li> Сравниваются по порядку слова в запросах, если слово из первого запроса имеет большее вхождение в датасет, то они остаются на своих местах, если наоборот, то меняются. В случае равенства кол-ва вхождений переходим к следующему пункту. </li>
  <li> Сравниваем слова лексикографически. Лексикографически меньшее слово должно стоять выше. В случае, если слова лексикографически идентичные, то переходим к следующему пункту. </li>
  <li> В случае, если слова в запросах закончились, то сравниваем кол-во слов в запросах (меньшее кол-во занимает позицию выше). </li>
</ol>

### Исправление ошибок в введенном пользователем запросе 

Для корректировки запроса будем допускать расстояние [Левенштейна](https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D1%81%D1%81%D1%82%D0%BE%D1%8F%D0%BD%D0%B8%D0%B5_%D0%9B%D0%B5%D0%B2%D0%B5%D0%BD%D1%88%D1%82%D0%B5%D0%B9%D0%BD%D0%B0) `= 3` для слов в запросе и словаря. 

Обход нашего бора с подсчетом расстояния [Левенштейна](https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D1%81%D1%81%D1%82%D0%BE%D1%8F%D0%BD%D0%B8%D0%B5_%D0%9B%D0%B5%D0%B2%D0%B5%D0%BD%D1%88%D1%82%D0%B5%D0%B9%D0%BD%D0%B0) и последующим поиском по наименьшей дельте расстояния.

## Задействованые библиотеки / фреймворки / язык программирования

Основной язык программирования: `C++ 20`

Библиотеки: Стандартные библиотеки, такие как: `vector; map; string; fstream; unordered_map; sstream; etc..`

Фреймворки: не задействованы 

## Оценка эффективности данного подхода

Основная проблема данного подхода - то что все данные хранятся в RAM. На очень больших датасетах данное решение надо будет распределять на несколько серверов. Но в рамках подхода к тестовой задаче данное решение подходит.

## Примеры работоспособности

В качестве аргументов программы для склонированного репозитория используется: ```../test_logs/genius.txt ```

Примеры работоспособности данного решения:

![Пример работоспособности предложенных подсказок](https://github.com/mnj2kk/pictures/blob/main/vk-intern/correct_work_1.png)

![Пример работоспособности предложенных подсказок](https://github.com/mnj2kk/pictures/blob/main/vk-intern/correct_work_2.png)

![Пример работоспособности исправление слов](https://github.com/mnj2kk/pictures/blob/main/vk-intern/correct_word_misstakes.jpg)

![Пример работоспособности отсутсвия вариантов](https://github.com/mnj2kk/pictures/blob/main/vk-intern/nothing_to_suggest.png)

## Возможные оптимизации:

<ol>
  <li> Индексы для каждого эллемента в боре можно хранить в БД, что сэкономит оперативную память. </li>
  <li> Будут добавляться.. </li>
</ol>
