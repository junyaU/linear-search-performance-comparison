#include <iostream>
#include <forward_list>
#include <random>
#include <algorithm>
#include <chrono>
#include <vector>
#include <numeric>
#include <cmath>

// Generate random data for testing
template<typename Container>
void generate_data(Container &c, size_t size) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    c.resize(size);
    std::generate(c.begin(), c.end(), [&mt, &dist]() { return dist(mt); });
}

template<typename Container>
void linear_search_test(const Container &c, const std::string &type) {
    const int repeat_count = 200;
    const int warmup_count = 10;
    std::vector<long long> times;

    for (int i = 0; i < repeat_count; ++i) {
        auto start = std::chrono::system_clock::now();

        // Perform a dummy operation to prevent loop optimization
        int sum = 0;
        for (const auto &val: c) {
            sum += val;
        }

        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        times.push_back(elapsed.count());
    }

    double mean_time = std::accumulate(times.begin(), times.end(), 0.0) / repeat_count;
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double stdev_time = std::sqrt(sq_sum / repeat_count - mean_time * mean_time);
    double cv = stdev_time / mean_time * 100;

    std::cout << "Type: " << type << "\n";
    std::cout << "Average time: " << mean_time << "μs\n";
    std::cout << "Time stddev: " << stdev_time << "μs\n";
    std::cout << "Coefficient of variation: " << cv << "%\n\n";
}

int main() {
    const size_t data_size = 10000000;

    std::vector<int> vec;
    std::forward_list<int> list(data_size);

    generate_data(vec, data_size);
    generate_data(list, data_size);

    linear_search_test(vec, "Vector");
    linear_search_test(list, "Forward List");

    return 0;
}