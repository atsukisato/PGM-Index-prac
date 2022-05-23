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

void normalize_data(std::vector<double> &data){
    const int n = data.size();
    double a = 1.0 / (data[n-1] - data[0]);
    double b = data[0];
    for(int i=0;i<n;i++){
        data[i] = (data[i] - b) * a;
    }
}


double PROBABILITY(double e1, double e2, double t){
    if (e1 >= e2){
        return 1;
    }else{
        return exp((e1 - e2) / t);
    }
}

double TEMPERATURE(double r){
    double alpha = 0.99;
    return pow(alpha, r);
}

int main() {
    const int data_length = 1000000;
    const int epoch = 100;
    double alpha = 0.1;

    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    std::normal_distribution<> dist(0.0, 1.0);
    std::uniform_real_distribution<> uni_dist(0, 1.0);

    // init data
    std::vector<double> init_data = make_random_double_data(data_length);
    normalize_data(init_data);

    /*
    for(double d : init_data){
        std::cout << d << ' ';
    }
    std::cout << std::endl;
    */

    std::vector<double> data = init_data;

    std::vector<double> build_times = measure_build_times(data, 5);
    double ave = - get_ave_std(build_times).first;
    double std = - get_ave_std(build_times).second;
    std::cerr << ave <<std::endl;
    std::cerr << std <<std::endl;


    std::vector<double> best_data = data;
    double best_ave = ave;

    std::vector<double> now_data = data;
    double now_ave = ave;

    for(int ep = 0; ep < epoch; ep++){
        std::vector<double> ddata(data_length-1);
        for(int i=0;i<data_length-1;i++){
            ddata[i] = data[i+1] - data[i];
        }

        std::vector<double> new_ddata(data_length-1);
        for(int i=0;i<data_length-1;i++){
            new_ddata[i] = ddata[i] + alpha * dist(engine);
            if(new_ddata[i] < 1e-10){
                new_ddata[i] = 1e-10;
            }
        }

        std::vector<double> new_data(data_length);
        new_data[0] = data[0];
        for(int i=1;i<data_length;i++){
            new_data[i] = new_data[i-1] + new_ddata[i-1];
        }
        normalize_data(new_data);

        std::vector<double> new_build_times = measure_build_times(new_data, 5);
        double new_ave = - get_ave_std(new_build_times).first;

        if(new_ave < best_ave){
            best_ave = new_ave;
            best_data = new_data;
            now_ave = new_ave;
            now_data = new_data;
        }else{
            if(uni_dist(engine) < PROBABILITY(now_ave, new_ave, TEMPERATURE(ep / epoch))){
                now_ave = new_ave;
                now_data = new_data;
            }
        }

        if(ep % (epoch/10) == 0){
            std::cerr << "-- "<< ep << " --"<< std::endl;
            std::cerr << now_ave << std::endl;
        }

        std::cout << now_ave << std::endl;

    }

    /*
    for(double d : best_data){
        std::cout << d << ' ';
    }
    std::cout << std::endl;
    */

    build_times = measure_build_times(best_data, 5);
    ave = - get_ave_std(build_times).first;
    std = - get_ave_std(build_times).second;
    std::cerr << ave <<std::endl;
    std::cerr << std <<std::endl;
    


    return 0;
}
