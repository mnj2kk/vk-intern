# Система автоматического дополнения запросов

В данной статье рассказано о задаче и подходу к её решению

## Постановка задачи

> Написать систему автоматического дополнения запросов, которая на запрос 
пользователя будет выдавать на основе датасета не более 5 лучших подсказок

## Подход к реализации

Основополагающим компонентом в реализации будут прямые и обратные индексы, в кратце прямой индекс - это номер запроса в нашем датасете, а обратный индекс это слово и соответвующие ему номера запросов где оно встречается. 

На введенный пользователем запрос мы будем для каждого слова (префикса) искать набор индексов где оно встречается и пересекать ответы, получая нужные нам индексы. 

Для быстрого поиска префиксов мы будем использовать бор (т.к. поиск префикса в нем происходит `O(|S|)`.

Чтобы ранжировать наши подсказки будем сортировать изначальный датасет, где чем выше занимает позицию запрос - тем он имеет больший приоритет.

Ранжирование происходит следующим образом:
<ol>
  <li> Сравниваются по порядку слова в запросах, если слово из первого запроса имеет большее вхождение в датасет, то они остаются на своих местах, если наоборот, то меняются. В случае равенства кол-ва вхождений переходим к следующему пункту. </li>
  <li> Если слова одинаковые, то переходим к следующему пункту. Иначе сравниваем лексикографически наши слова. </li>
  <li> В случае если слова в запросах заканчиваются сравниваем кол-во слов, чем меньше, тем больший приоритет имеет наш запрос. </li>
</ol>

## Задействованые библиотеки / фреймворки / язык программирования

Основной язык программирования: `C++`

Библиотеки: Стандартные библиотеки, такие как: `vector; map; string; fstream; unordered_map; etc..`

Фреймворки: не задействованы 

## Оценка эффективности данного подхода

Основная проблема данного подхода - то что все данные хранятся в RAM. На очень больших датасетах данное решение надо будет распределять на несколько серверов. Но в рамках подхода к тестовой задаче данное решение подходит.

Возможные оптимизации:

<ol>
  <li> Индексы для каждого эллемента в боре можно хранить в БД, что сэкономит оперативную память. </li>
  <li> Будут добавляться.. </li>
</ol>
