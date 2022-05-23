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
#include <chrono>
#include <cmath>
#include <random>
#include "pgm/pgm_index.hpp"


const int epsilon = 128;

template<typename T>
std::vector<double> measure_build_times(const std::vector<T> &data, const int build_num = 10){
    std::vector<double> build_times;
    for(int i = 0; i < build_num; i++){
        std::chrono::system_clock::time_point start, end;
        start = std::chrono::system_clock::now();
        pgm::PGMIndex<T, epsilon> index(data);
        end = std::chrono::system_clock::now();
        double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        build_times.push_back(time);
    }
    return build_times;
}

template<typename T>
double measure_size(const std::vector<T> &data){
    pgm::PGMIndex<T, epsilon> index(data);
    return (double)index.size_in_bytes();
}

std::pair<double, double> get_ave_std(std::vector<double> vec){
    int n = (int)vec.size();
    double sum = 0.0;
    double sum2 = 0.0;
    for(int i = 0; i < n; i++){
        sum += vec[i];
        sum2 += vec[i] * vec[i];
    }
    double ave = sum / n;
    double std = std::sqrt(sum2 / n - ave * ave);
    return std::pair<double, double>(ave, std);
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

std::vector<double> make_expo_double_data(const int data_length = 1000000){
    std::vector<double> data;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(1.0);

    for(int i = 0; i < data_length; i++){
        double f = d(gen);
        data.push_back(f);
    }
    sort(data.begin(), data.end());
    return data;
}

std::vector<double> make_lognormal_double_data(const int data_length = 1000000){
    std::vector<double> data;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::lognormal_distribution<> d(0.0, 1.0);

    for(int i = 0; i < data_length; i++){
        double f = d(gen);
        data.push_back(f);
    }
    sort(data.begin(), data.end());
    return data;
}

void normalize_data(std::vector<double> &data){
    const int n = data.size();
    double a = 1.0 / (data[n-1] - data[0]);
    double b = data[0];
    for(int i=0;i<n;i++){
        data[i] = (data[i] - b) * a;
    }
}

int main() {
    const int data_length = 1000000;
    const int build_num = 10;
    std::vector<double> build_times(build_num);
    std::vector<double> sizes(build_num);

    // random data
    for(int i=0;i<build_num;i++){
        std::vector<double> data = make_random_double_data(data_length);
        normalize_data(data);
        double build_time = measure_build_times(data, 1)[0];
        int size = measure_size(data);
        build_times[i] = build_time;
        sizes[i] = size;
    }
    std::cerr << "-- uniform --" << std::endl;
    std::cerr << "construct time: " << get_ave_std(build_times).first << std::endl;
    std::cerr << "size: " << get_ave_std(sizes).first << std::endl;

    // lognormal data
    for(int i=0;i<build_num;i++){
        std::vector<double> data = make_lognormal_double_data(data_length);
        normalize_data(data);
        double build_time = measure_build_times(data, 1)[0];
        int size = measure_size(data);
        build_times[i] = build_time;
        sizes[i] = size;
    }
    std::cerr << "-- lognormal --" << std::endl;
    std::cerr << "construct time: " << get_ave_std(build_times).first << std::endl;
    std::cerr << "size: " << get_ave_std(sizes).first << std::endl;

    // expo data
    for(int i=0;i<build_num;i++){
        std::vector<double> data = make_expo_double_data(data_length);
        normalize_data(data);
        double build_time = measure_build_times(data, 1)[0];
        int size = measure_size(data);
        build_times[i] = build_time;
        sizes[i] = size;
    }
    std::cerr << "-- exponentail --" << std::endl;
    std::cerr << "construct time: " << get_ave_std(build_times).first << std::endl;
    std::cerr << "size: " << get_ave_std(sizes).first << std::endl;

    return 0;
}
