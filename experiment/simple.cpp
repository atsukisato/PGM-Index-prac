/*
 * Measure the time PGM-Index takes to index.
 * Compile with:
 *   g++ simple.cpp -std=c++17 -I../include -o simple
 * Run with:
 *   ./simple
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <cmath>
#include "pgm/pgm_index.hpp"


const int epsilon = 128;

std::vector<double> measure_build_times(std::vector<int> data, const int build_num = 10){
    std::vector<double> build_times;
    for(int i = 0; i < build_num; i++){
        clock_t start = clock();
        pgm::PGMIndex<int, epsilon> index(data);
        clock_t end = clock();
        build_times.push_back(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0);
    }
    return build_times;
}

void show_ave_std(std::vector<double> vec){
    int n = (int)vec.size();
    double sum = 0.0;
    double sum2 = 0.0;
    for(int i = 0; i < n; i++){
        sum += vec[i];
        sum2 += vec[i] * vec[i];
    }
    double ave = sum / n;
    double std = std::sqrt(sum2 / n - ave * ave);
    std::cout << "ave:" << ave << std::endl;
    std::cout << "std:" << std << std::endl;
}

int main() {
    // Generate some random data
    std::vector<int> data(1000000);
    std::generate(data.begin(), data.end(), std::rand);
    std::sort(data.begin(), data.end());

    std::vector<double> build_times = measure_build_times(data, 10);
    show_ave_std(build_times);

    return 0;
}