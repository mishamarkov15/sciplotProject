#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "graphs.cpp"

/**
 * Функция проверки существования файла с заданным именем
 *
 * @param filename путь к файлу
 * @return true, если файл существует, иначе - false.
 */
inline bool fileExists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}

/**
 * Функция проверки на равенство двух пар векторов заданного формата.
 *
 * @param lhs пара 1
 * @param rhs пара 2
 * @return true, если lhs == rhs, иначе - false.
 */
bool sorted_delays_equal(const sorted_delays_type& lhs, const sorted_delays_type& rhs) {
    for (size_t i = 0; i < lhs.first.size(); ++i) {
        if (lhs.first[i] != rhs.first[i] || lhs.second[i] != rhs.second[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Проверка работоспособности функции `sortDelays(delays)`.
 */
TEST_CASE("sortingDelays") {
    delays_dict delays = {
            {"a", {{"count", 5}, {"delay", 5000},}},    // avg: 1000
            {"b", {{"count", 3}, {"delay", 1800},}},    // avg: 600
            {"c", {{"count", 10}, {"delay", 20000},}},  // avg: 2000
    };

    auto sorted = sortDelays(delays);

    CHECK(sorted_delays_equal(sorted, {{600, 1000, 2000}, {"b", "a", "c"}}));
}


/**
 * Проверка создания файла с распределением.
 */
TEST_CASE("plotDelays") {
    std::vector<long long> delays = {0, 100, 200, 300, 400, 500};

    plotDelays(delays);

    CHECK(fileExists(FILEPATH_CURVE));
}

/**
 * Проверка создания файла с гистограммой.
 */
TEST_CASE("plotHistograms")
{
    delays_dict delays = {
            {"a", {{"count", 5}, {"delay", 5000},}},    // avg: 1000
            {"b", {{"count", 3}, {"delay", 1800},}},    // avg: 600
            {"c", {{"count", 10}, {"delay", 20000},}},  // avg: 2000
            {"d", {{"count", 6}, {"delay", 5600},}},  // avg: 933
            {"e", {{"count", 8}, {"delay", 12460},}},  // avg: 1560
    };

    histogramAverageDelay(delays);

    CHECK(fileExists(FILEPATH_HISTOGRAM));
}


/**
 * Проверка создания файла с гистограммой (пары клавиш).
 */
TEST_CASE("plotHistogramsPairs")
{
    delays_dict delays = {
            {"ab", {{"count", 5}, {"delay", 5000},}},    // avg: 1000
            {"bd", {{"count", 3}, {"delay", 1800},}},    // avg: 600
            {"ce", {{"count", 10}, {"delay", 20000},}},  // avg: 2000
            {"dd", {{"count", 6}, {"delay", 5600},}},  // avg: 933
            {"ec", {{"count", 8}, {"delay", 12460},}},  // avg: 1560
    };

    histogramAverageDelay(delays);

    CHECK(fileExists(FILEPATH_HISTOGRAM));
}
