#include <map>
#include <string>
#include <iostream>
#include <istream>
#include <chrono>
#include <vector>
#include <sciplot/sciplot.hpp>

using namespace sciplot;

// {
//    'a': {
//       'count': 4,
//       'delay': 3.8ms
//    },
//    'b' : {
//       'c'
//    }
// }

std::map<char, std::map<std::string, size_t>> getDelays() {
    std::map<char, std::map<std::string, size_t>> res;

    char sym;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cin >> sym;
        auto end = std::chrono::high_resolution_clock::now();

        if (sym == '.') {
            break;
        }


        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (res.find(sym) != res.end()) {
            ++res[sym]["count"];
            res[sym]["delay"] += delay;
        } else {
            res[sym] = {
                    {"count", 1},
                    {"delay", delay}
            };
        }
    }
    return res;
}

// Среднее арифметическое задержек для каждой клавиши
std::vector<double> averageDelays(const std::map<char, std::map<std::string, size_t>> &delays) {
    std::vector<double> res;
    for (const auto &item: delays) {
        res.push_back(static_cast<double>(item.second.at("delay")) / static_cast<double>(item.second.at("count")));
    }
    return res;
}

// Функция, которая сортирует словарь с задержками по возрастанию
//
//  Например: есть такой массив пар, где первый элемент - средняя задержка, а второй - символ
//  {{1500ms, 'a'}, {1000ms, 'c'}, {1240ms, 'b'}}
//
//  На выходе получим такое
// return: {{1000ms, 1240ms, 1500ms}, {'c', 'b', 'a'}}
std::pair<std::vector<double>, std::vector<std::string>>
sortDelays(const std::map<char, std::map<std::string, size_t>> &delays) {
    // pairs: {{1000ms, 'a'}, {5600ms, 'b'}, {689ms, 'c'}}
    auto avg = averageDelays(delays);

    std::vector<std::pair<double, std::string>> pairs;
    for (const auto &item: delays) {
        pairs.emplace_back(static_cast<double>(item.second.at("delay")) / static_cast<double>(item.second.at("count")),
                         std::string(1, item.first));
    }

    std::sort(pairs.begin(), pairs.end(), [](auto& lhs, auto& rhs) -> bool { return lhs.first < rhs.first;});


    std::vector<double> averageSortedDelays;
    std::vector<std::string> sortedSymbols;
    for (const auto& pair : pairs) {
        averageSortedDelays.push_back(pair.first);
        sortedSymbols.push_back(pair.second);
    }
    return {averageSortedDelays, sortedSymbols};
}

//  Гистограмма со средней задержкой перед нажатием клавиши
void histogramAverageDelay(const std::map<char, std::map<std::string, size_t>> &delays) {
    auto [x, y] = sortDelays(delays);

    Vec sizes(1.0, y.size());

    Plot2D plot;

    plot.xlabel("symbol");
    plot.ylabel("delay");

    plot.drawBoxes(x, y, sizes).fillSolid().fillColor("green").fillIntensity(0.9);

    Figure fig{{plot}};

    Canvas canvas{{fig}};

    canvas.show();

    canvas.save("Hist example");
}

int main(int argc, char **argv) {
    auto res = getDelays();
    for (auto &item: res) {
        std::cout << item.first << ":\n";
        std::cout << "\tcount: " << item.second["count"] << "\n";
        std::cout << "\tdelay: " << item.second["delay"] << "\n";
    }
    histogramAverageDelay(res);
}
