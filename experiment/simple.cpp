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

std::vector<double> calc_gradd(const std::vector<double> &data){
    const int n = data.size();

    std::vector<double> build_times = measure_build_times(data);
    std::pair<double, double> base = get_ave_std(build_times);
    const double base_ave = base.first;
    const double base_std = base.second;

    std::vector<double> ddata(n-1);
    for(int i=0;i<n-1;i++){
        ddata[i] = data[i+1] - data[i];
    }

    const double delta = 0.01;

    std::vector<double> grad_ddata(n-1);
    for(int i=0;i<n-1;i++){
        ddata[i] += delta;
        std::vector<double> tmp_data(n);
        tmp_data[0] = data[0];
        for(int j=1;j<n;j++){
            tmp_data[j] = tmp_data[j-1] + ddata [j-1];
        }
        ddata[i] -= delta;

        std::vector<double> build_times = measure_build_times(tmp_data);
        std::pair<double, double> as = get_ave_std(build_times);
        double ave = as.first;
        double std = as.second;
        grad_ddata[i] = (ave - base_ave) / delta;
    }

    return grad_ddata;
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
    const int data_length = 1000;
    const int epoch = 10;
    const double alpha = 0.1;

    // init data
    std::vector<double> data = make_random_double_data(data_length);
    normalize_data(data);

    std::vector<double> build_times = measure_build_times(data);
    std::pair<double, double> as = get_ave_std(build_times);
    double ave = as.first;
    double std = as.second;
    std::cerr << ave << std::endl;

    for(int ep = 0; ep < epoch; ep++){
        std::vector<double> ddata(data_length-1);
        for(int i=0;i<data_length-1;i++){
            ddata[i] = data[i+1] - data[i];
        }
        std::vector<double> gradd = calc_gradd(data);
        for(int i=0;i<data_length-1;i++){
            ddata[i] += alpha * gradd[i];
            if(ddata[i] < 1e-10){
                // to make data "narrowly" monotonous increase
                ddata[i] = 1e-10;
            }
        }

        for(int i=1;i<data_length;i++){
            data[i] = data[i-1] + ddata[i-1];
        }

        normalize_data(data);

        build_times = measure_build_times(data);
        as = get_ave_std(build_times);
        ave = as.first;
        std::cerr << ave << std::endl;

        /*
        for(double d : data){
            std::cerr << d << ' ';
        }
        std::cerr << std::endl;
        */

    }

    return 0;
}
