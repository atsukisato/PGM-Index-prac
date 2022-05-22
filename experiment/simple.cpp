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

template<typename T>
std::vector<double> measure_build_times(const std::vector<T> &data, const int build_num = 10){
    std::vector<double> build_times;
    for(int i = 0; i < build_num; i++){
        clock_t start = clock();
        pgm::PGMIndex<T, epsilon> index(data);
        clock_t end = clock();
        build_times.push_back(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0);
    }
    return build_times;
}

template<typename T>
double measure_size(const std::vector<T> &data){
    pgm::PGMIndex<T, epsilon> index(data);
    return (double)index.size_in_bytes();
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


std::vector<double> make_random_double_data(const int data_length = 1000000, const double fMin = 0.0, const double fMax = 1.0){
    std::vector<double> data;
    for(int i = 0; i < data_length; i++){
        double f = (double)rand() / RAND_MAX;
        double rn = fMin + f * (fMax - fMin);
        data.push_back(rn);
    }
    sort(data.begin(), data.end());
    return data;
}

int main() {
    const int data_length = 100000;
    const int build_num = 10;

    // the case of random data
    std::vector<double> random_data = make_random_double_data();

    std::vector<double> build_times = measure_build_times(random_data, build_num);
    std::cout << "--- random data ---" << std::endl;
    show_ave_std(build_times);

    std::vector<double> hoge;
    for(int i=0;i<10;i++){
        double siz = measure_size(random_data);
        hoge.push_back(siz);
    }
    std::cout << "--- random data ---" << std::endl;
    show_ave_std(hoge);
    

    return 0;
}