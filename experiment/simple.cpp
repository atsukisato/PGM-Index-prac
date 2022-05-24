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
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <set>
#include <fstream>
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


std::vector<double> make_uniform_double_data(const int data_length = 1000000){
    const double fMin = 0.0;
    const double fMax = 1.0;

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

struct Result {
    double build_time;
    double size;
    double query_time;
};

template<typename T>
Result measure(std::vector<T>(*gen_data)(int), const int data_length, const int build_num){
    std::vector<double> build_times;
    std::vector<double> sizes;
    std::vector<double> query_times;

    const int query_num_per_build = 1000000;

    for(int i=0;i<build_num;i++){
        std::vector<double> data = gen_data(data_length);
        normalize_data(data);

        // measure build time
        std::chrono::system_clock::time_point start, end;
        start = std::chrono::system_clock::now();
        pgm::PGMIndex<T, epsilon> index(data);
        end = std::chrono::system_clock::now();
        double build_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        build_times.push_back(build_time);

        // measure size
        int size = index.size_in_bytes();

        // measure query time
        std::vector<double> queries;
        for(int j=0;j<query_num_per_build;j++){
            double f = (double)rand() / RAND_MAX;
            queries.push_back(f);
        }

        start = std::chrono::system_clock::now();
        for(int j=0;j<query_num_per_build;j++)auto range = index.search(queries[j]);
        end = std::chrono::system_clock::now();
        double query_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);

        build_times.push_back(build_time);
        sizes.push_back((double)size);
        query_times.push_back(query_time);
    }

    double build_time_ave = get_ave_std(build_times).first;
    double size_ave = get_ave_std(sizes).first;
    double query_time_ave = get_ave_std(query_times).first;

    Result res = {build_time_ave, size_ave, query_time_ave};
    return res;
}

template<typename T>
Result measure_multiset(std::vector<T>(*gen_data)(int), const int data_length, const int build_num){
    std::vector<double> build_times;
    std::vector<double> sizes;
    std::vector<double> query_times;

    const int query_num_per_build = 1000000;

    for(int i=0;i<build_num;i++){
        std::vector<double> data_v = gen_data(data_length);
        normalize_data(data_v);
        double data[data_length];
        for(int j=0;j<data_length;j++){
            data[j] = data_v[j];
        }

        // measure build time
        std::chrono::system_clock::time_point start, end;
        start = std::chrono::system_clock::now();
        std::multiset<T> index(data, data+data_length);
        end = std::chrono::system_clock::now();
        double build_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        build_times.push_back(build_time);

        // measure size
        // int size = index.size_in_bytes();
        int size = 0;

        // measure query time
        std::vector<double> queries;
        for(int j=0;j<query_num_per_build;j++){
            double f = (double)rand() / RAND_MAX;
            queries.push_back(f);
        }

        start = std::chrono::system_clock::now();
        for(int j=0;j<query_num_per_build;j++)auto range = index.find(queries[j]);
        end = std::chrono::system_clock::now();
        double query_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);

        build_times.push_back(build_time);
        sizes.push_back((double)size);
        query_times.push_back(query_time);
    }

    double build_time_ave = get_ave_std(build_times).first;
    double size_ave = get_ave_std(sizes).first;
    double query_time_ave = get_ave_std(query_times).first;

    Result res = {build_time_ave, size_ave, query_time_ave};
    return res;
}

void output_data(const std::vector<double> &data, const char *file_name){
    std::ofstream ofs(file_name);
    for(int i=0;i<data.size();i++){
        ofs << data[i];
        if(i == data.size()-1){
            ofs << std::endl;
        }else{
            ofs << ' ';
        }
    }
}

int main() {
    const int data_length = 1000000;
    const int build_num = 10;

    Result res;
    // PGM
    // uniform data
    res = measure(make_uniform_double_data, data_length, build_num);
    std::cerr << "-- uniform --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;

    // lognormal data
    res = measure(make_lognormal_double_data, data_length, build_num);
    std::cerr << "-- lognormal --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;

    // expo data
    res = measure(make_expo_double_data, data_length, build_num);
    std::cerr << "-- exponential --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;

    // MultiSet
    // uniform data
    res = measure_multiset(make_uniform_double_data, data_length, build_num);
    std::cerr << "-- uniform --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;

    // lognormal data
    res = measure_multiset(make_lognormal_double_data, data_length, build_num);
    std::cerr << "-- lognormal --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;

    // expo data
    res = measure_multiset(make_expo_double_data, data_length, build_num);
    std::cerr << "-- exponential --" << std::endl;
    std::cerr << "construct time: " << res.build_time << std::endl;
    std::cerr << "size: " << res.size << std::endl;
    std::cerr << "query_time: " << res.query_time << std::endl;


    // output each data
    std::vector<double> data;

    // uniform data
    data = make_uniform_double_data(data_length);
    normalize_data(data);
    output_data(data, "./uniform.txt");

    // lognormal data
    data = make_lognormal_double_data(data_length);
    normalize_data(data);
    output_data(data, "./lognormal.txt");

    // expo data
    data = make_expo_double_data(data_length);
    normalize_data(data);
    output_data(data, "./expo.txt");


    return 0;
}
