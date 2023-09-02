#include <map>
#include <string>
#include <iostream>
#include <istream>
#include <chrono>
#include <vector>
#include <sciplot/sciplot.hpp>

using namespace sciplot;

typedef std::map<std::string, std::map<std::string, size_t>> delays_dict;

typedef std::pair<std::vector<double>, std::vector<std::string>> sorted_delays_type;

const std::string FILEPATH_HISTOGRAM = "Histogram.pdf";

const std::string FILEPATH_CURVE = "Curve.pdf";

/**
 * Функция, которая обрабатывает нажатия клавиш на клавиатуру и подсчитывает задержку в миллисекундах
 * перед нажатием каждой из клавиш. Символ окончания ввода: '.'
 *
 * @return Словарь следующего формата
 * {
 *     "a": {
 *          'count': 4,
 *          'delay': 3.8ms
 *      },
 *      "b" : {
 *          'count': 2,
 *          'delay': 6.6ms
 *      },
 *      ...
 * }
 */
delays_dict getDelays() {
    delays_dict res;

    char sym;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cin >> sym;
        auto end = std::chrono::high_resolution_clock::now();

        if (sym == '.') {
            break;
        }


        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::string key(1, sym);

        if (res.find(key) != res.end()) {
            ++res[key]["count"];
            res[key]["delay"] += delay;
        } else {
            res[key] = {
                    {"count", 1},
                    {"delay", delay}
            };
        }
    }
    return res;
}

/**
 * Функция, которая обрабатывает нажатия клавиш на клавиатуру и подсчитывает задержку в миллисекундах
 * перед нажатием каждой пары клавиш. Символ окончания ввода: '.'
 *
 * @return Словарь следующего формата
 * {
 *      "ab": {
 *          'count': 4,
 *          'delay': 3.8ms
 *      },
 *      "bc" : {
 *          'count': 2,
 *          'delay': 6.6ms
 *      },
 *      ...
 * }
 */
delays_dict getPairDelays() {
    delays_dict res;

    char sym_1 = '.', sym_2;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cin >> sym_1;
        if (sym_1 == '.') break;

        std::cin >> sym_2;
        if (sym_2 == '.') break;
        auto end = std::chrono::high_resolution_clock::now();



        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::string key = std::string(1, sym_1) + std::string(1, sym_2);

        if (res.find(key) != res.end()) {
            ++res[key]["count"];
            res[key]["delay"] += delay;
        } else {
            res[key] = {
                    {"count", 1},
                    {"delay", delay}
            };
        }
    }
    return res;
}

/**
 * Получение задержек перед нажатием клавиши в простом формате. То есть мы не получаем символ,который был нажат, мы
 * просто создаем вектор задержек, без привязки к символу.
 *
 * @return {задержка_1, ..., задержка_2} (задержки в миллисекундах)
 */
std::vector<long long> getDelaysSimple() {
    std::vector<long long> res;

    char sym;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cin >> sym;
        auto end = std::chrono::high_resolution_clock::now();

        if (sym == '.') {
            break;
        }

        res.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    return res;
}

/**
 * Подсчет средней задержки перед нажатием каждой из нажатых клавиш.
 *
 * @param delays словарь, который получается из функции `delays_dict getDelays()`
 * @return Вектор средних задержек. Задержки расположены в том же порядке, что и символы в словаре `delays`.
 */
std::vector<double> averageDelays(const delays_dict &delays) {
    std::vector<double> res;
    for (const auto &item: delays) {
        res.push_back(static_cast<double>(item.second.at("delay")) / static_cast<double>(item.second.at("count")));
    }
    return res;
}

/**
 * Функция, сортирующая сырой массив с задержками, полученный из функции `delays getDelays()`.
 *
 * Внутри себя, она сначала для каждого символа подсчитает его среднее арифметическое задержки, после чего
 * создаст пару следующего формата: {{<средняя задержка 1>, ..., <средняя задержка n>}, {<символ 1>, ..., <символ n>}},
 * отсортирует эту пару по первому вектору, то есть по вектору средних задержек, при этом меняя порядок элементов и во
 * втором векторе.
 *
 * По итогу индекс средней задержки совпадет с индексом принадлежащего ей символа.
 *
 * @param delays словарь задержек из функции `delays getDelays()`
 * @return пару из вектора задержек в отсортированном порядке и вектор связанных с ними символов
 */
sorted_delays_type sortDelays(const delays_dict &delays) {
    // pairs: {{1000ms, 'a'}, {5600ms, 'b'}, {689ms, 'c'}}
    auto avg = averageDelays(delays);

    std::vector<std::pair<double, std::string>> pairs;
    for (const auto &item: delays) {
        pairs.emplace_back(static_cast<double>(item.second.at("delay")) / static_cast<double>(item.second.at("count")),
                           item.first);
    }

    std::sort(pairs.begin(), pairs.end(), [](auto &lhs, auto &rhs) -> bool { return lhs.first < rhs.first; });

    std::vector<double> averageSortedDelays;
    std::vector<std::string> sortedSymbols;
    for (const auto &pair: pairs) {
        averageSortedDelays.push_back(pair.first);
        sortedSymbols.push_back(pair.second);
    }
    return {averageSortedDelays, sortedSymbols};
}

// Функция, которая сортирует словарь с задержками по возрастанию
//
//  Например: есть такой массив пар, где первый элемент - средняя задержка, а второй - символ
//  {{1500ms, 'a'}, {1000ms, 'c'}, {1240ms, 'b'}}
//
//  На выходе получим такое
// return: {{1000ms, 1240ms, 1500ms}, {'c', 'b', 'a'}}
//  Гистограмма со средней задержкой перед нажатием клавиши
void histogramAverageDelay(const delays_dict &delays) {
    auto [x, y] = sortDelays(delays);

    Vec sizes(1.0, y.size());

    Plot2D plot;

    plot.xlabel("symbol");
    plot.ylabel("average delay (ms)");

    plot.drawBoxes(y, x, sizes).fillSolid().fillColor("blue").fillIntensity(0.6).label("Средние задержки");

    plot.legend().atTopLeft(); // Устанавливаем легенду в верхнем левом углу

    Figure fig{{plot}};

    Canvas canvas{{fig}};

    canvas.size(800, 600);
    canvas.show();

    canvas.save(FILEPATH_HISTOGRAM);
}

/**
 * График плотности распределения вероятности задержек.
 *
 * @param delays задержки, полученные из функции `getDelaysSimple`
 */
void plotDelays(const std::vector<long long>& delays)
{
    Plot2D plot; // Создаем объект Plot2D для построения 2D-графика
    plot.xlabel("Задержка (мс)"); // Устанавливаем подпись по оси x
    plot.ylabel("Доля нажатий"); // Устанавливаем подпись по оси y

    int intervals = 15; // Количество интервалов для разбиения задержек
    auto min = *std::min_element(delays.begin(), delays.end());
    auto max = *std::max_element(delays.begin(), delays.end());
    double step = (static_cast<double>(max) - static_cast<double>(min)) / intervals; // Шаг интервала

    std::vector<double> x(intervals); // Вектор для хранения середин интервалов по оси x
    std::vector<double> y(intervals); // Вектор для хранения долей нажатий по оси y

    // Цикл по интервалам
    for (int i = 0; i < intervals; i++) {
        x[i] = min + (i + 0.5) * step; // Вычисляем середину интервала
        y[i] = 0; // Инициализируем долю нажатий нулем

        double lower_bound = static_cast<double>(min) + i * step; // Нижняя граница интервала
        double upper_bound = static_cast<double>(min) + (i + 1) * step; // Верхняя граница интервала

        for (size_t j = 0; j < delays.size(); ++j) { // Цикл по задержкам
            // Если задержка попадает в интервал
            if (lower_bound <= static_cast<double>(delays[j]) && static_cast<double>(delays[j]) < upper_bound)  {
                y[i]++; // Увеличиваем долю нажатий на единицу
            }
        }
        y[i] /= delays.size(); // Нормируем долю нажатий на общее количество нажатий
    }

    plot.drawCurve(x, y).label("Задержки"); // Рисуем кривую с заданными векторами x и y и подписью
    plot.legend().atTopLeft(); // Устанавливаем легенду в верхнем левом углу

    Figure fig = { {plot} }; // Создаем объект Figure для хранения графика
    Canvas canvas = { {fig} }; // Создаем объект Canvas для хранения фигуры
    //canvas.title("Распределение задержек между нажатиями клавиш"); // Устанавливаем заголовок графика
    canvas.size(800, 600);
    canvas.show();
    canvas.save(FILEPATH_CURVE); // Сохраняем график в файл формата PDF
}