# Планирование в среде с динамическими препятствиями с помощью алгоритма SIPP

## О программе
Программа позволяет искать путь между двумя точками на клеточной карте, содержащей динамические препятствия, с помощью алгоритма SIPP. Как для агента, так и для динамических препятствий доступны только перемещения по вертикали и горизонтали. Все скорости равны 1. Если путь есть, он будет найден. Если путь найден, то гарантируется, что он оптимальный в смысле времени прибытия на финиш.

## Формат данных 
Примеры входных и выходных файлов можно посмотреть в папке `Examples`

### Входной файл
Cодержится в теге `root`, в котором расположены следующие теги:
+ `map` &mdash; обязательный тег, содержащий основную информацию о задаче
    + `width`, `height` &mdash; размеры карты
    + `startx`, `starty` &mdash; координаты начальной клетки
    + `finishx`, `finishy` &mdash; координаты конечной клетки
    + `grid` &mdash; содержит поле: `height` тегов `row`, в каждом из которых `width` нулей или единиц через пробел, причем нули означают свободную клетку, а единицы &mdash; занятую
+ `options` &mdash; содержит настройки алгоритма
    + `heuristic` &mdash; содержит тип эвристики &mdash; `manhattan` или `dijkstra`
+ `obstacles` &mdash; содержит информацию о динамических препятствиях: несколько (можно 0) тегов `obstacle`
    + `obstacle` &mdash; содержит информацию об одном динамическом препятствии: множество тегов `node` с параметрами `x` и `y`

### Выходной файл
Cодержит всю информацию из входного файла, а также тег `log` со следующими тегами:
+ `result` &mdash; содержит строку `Path found` или `Path not found`. Если путь не найден, дальнейшие теги отсутствуют
+ `summary` &mdash; содержит следующие параметры
    + `result` &mdash; время прибытия на финиш
    + `length` &mdash; длина пути в клетках
    + `steps` &mdash; количество итераций алгоритма
    + `time` &mdash; время работы алгоритма в секундах
+ `path` &mdash; содержит путь: множество тегов `node` с параметрами `x`, `y`, `time` означающих, что агент задержался в клетке `(x,y)` на `time` единиц времени. Например, если агент проходит, не останавливаясь, `time = 0`

### Текстовый файл
При запуске без флага `-noview` генерируется текстовый файл, позволяющий посмотреть визуализацию результата работы. Визуализация производится с помощью программы [Viewer](https://github.com/maksim1744/Viewer). Генерация текстового файла может занимать некоторое время, а сам файл может быть размером несколько Мб, поэтому доступен флаг `-noview`, отключающий генерацию текстового файла.

Уже готовые визуализации можно посмотреть в папке `demo`

## Сборка и запуск
Сборка осуществляется с помощью `cmake` и `make`. Необходима версия `c++` не ниже `17`. Запуск производится командой `SIPP.exe input_file.xml`, то есть входной файл передается в первом параметре при запуске из командной строки. Флаг `-noview` опционально можно передать вторым параметром.
