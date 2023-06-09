Общие требования для всех задач
Ввод/вывод отделены от решения.
Не должно быть утечек памяти, за каждую утечку памяти - штраф “-1”.

[Ссылка на задания] https://docs.google.com/document/d/1Lv4xzehypQLz56H2BFKiTnP3tMgEHQ637jmBXXCc_LI/edit#
[Ссылка на Яндекс Контест] https://contest.yandex.ru/contest/47405/enter
[Ведомость ML] https://docs.google.com/spreadsheets/d/1B6Y0vlbaL4sQZrIUWFjr2attgw4ig5k9E5ccRaDGqIg/edit?usp=share_link
[Ссылка на правила] https://docs.google.com/document/d/1TJHGNKRv7cbzXnZLLCawPjYl1Q71r4avJfR94AVkI-o/edit?usp=sharing


### Задача № 1 (3 балла)
> Во всех задачах необходимо использование битовых операций. 
Использование арифметических операций запрещено.
Входное число лежит в диапазоне 0..232 -1 и вводится в десятичном виде.

#### 1.4 Инвертируйте значение бита в числе N по его номеру K.
Формат входных данных. Число N, номер бита K.
Формат выходных данных. Число с инвертированным битом в десятичном виде.

----
### Задача № 2 (4 балла)
#### 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
> Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
Формат входных данных.
В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.

---
### Задача № 3 (4 балла)
> Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
#### 3_2. Реализовать дек с динамическим зацикленным буфером.
Требования: Дек должен быть реализован в виде класса.

---
### Задача № 4 (4 балла)
> Обязательная задача
Требование для всех вариантов Задачи 4
Решение всех задач данного раздела предполагает использование кучи, реализованной в виде шаблонного класса. 
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.

#### 4.2 Топ K пользователей из лога 
Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости. Количество заходов и идентификаторы пользователей не повторяются.
Требования: время работы O(N * logK), где N - кол-во пользователей. Ограничение на размер кучи O(K).
Формат входных данных: Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).
Формат выходных данных: Идентификаторы пользователей в порядке возрастания посещаемости

---
### Задача № 5
>Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции. 
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

#### 5_2. Современники.
Группа людей называется современниками если был такой момент, когда они могли собраться вместе. Для этого в этот момент каждому из них должно было  уже исполниться 18 лет, но ещё не исполниться 80 лет.
Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.
Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие в собраниях он не мог.


---
### Задача № 6 (3 балла)
> Обязательная задача
Дано множество целых чисел из [0..10^9] размера n. 
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

>Требования: к дополнительной памяти: O(n). 
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition. 
Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.


>Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

#### 6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

---
### Задача № 7 (3 балла)
#### 7_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 106. Отсортировать массив методом поразрядной сортировки LSD по байтам.
