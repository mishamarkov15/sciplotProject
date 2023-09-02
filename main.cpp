#include "graphs.cpp"


int main(int argc, char **argv) {
    int plot_type;
    do {
        std::cout << "Enter plot type to draw:\n"
                     "------------------------\n"
                     "1. Curve\n"
                     "2. Histogram (one symbol)\n"
                     "3. Histogram (pair symbols)\n"
                     "0. Exit\n";
        std::cin >> plot_type;
        if (plot_type == 1) {
            std::cout << "Enter characters, separating them by enter ('.' for break)\n"
                         "----------------------------------------------------------\n";
            auto res = getDelaysSimple();
            std::cout << "Delays: ";
            for (const auto &item: res) {
                std::cout << item << ' ';
            }
            std::cout << '\n';
            plotDelays(res);
        } else if (plot_type == 2) {
            std::cout << "Enter characters, separating them by enter ('.' for break)\n"
                         "----------------------------------------------------------\n";
            auto res = getDelays();
            for (auto &item: res) {
                std::cout << item.first << ":\n";
                std::cout << "\tcount: " << item.second["count"] << "\n";
                std::cout << "\tdelay: " << item.second["delay"] << "\n";
            }
            histogramAverageDelay(res);
        } else if (plot_type == 3) {
            std::cout << "Enter characters, separating them by enter ('.' for break)\n"
                         "----------------------------------------------------------\n";
            auto res = getPairDelays();
            for (auto &item: res) {
                std::cout << item.first << ":\n";
                std::cout << "\tcount: " << item.second["count"] << "\n";
                std::cout << "\tdelay: " << item.second["delay"] << "\n";
            }
            histogramAverageDelay(res);
        }
    } while (plot_type != 0);
}
