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
#include <climits>
#include "pgm/pgm_index.hpp"
#include "data_generator/data_generator.hpp"
#include "measure/measure.hpp"


const int data_length = 1000000;


int main() {

    {
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

        // stair data
        std::vector<int> idata = make_stair_int_data(data_length);
        // normalize_data(data);
        output_data(idata, "./stair.txt");
    }

    {
        // PGM
        Result res;
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
        std::cerr << "-- expo --" << std::endl;
        std::cerr << "construct time: " << res.build_time << std::endl;
        std::cerr << "size: " << res.size << std::endl;
        std::cerr << "query_time: " << res.query_time << std::endl;
        
        // stair data
        res = measure(make_stair_int_data, data_length, build_num);
        std::cerr << "-- stair --" << std::endl;
        std::cerr << "construct time: " << res.build_time << std::endl;
        std::cerr << "size: " << res.size << std::endl;
        std::cerr << "query_time: " << res.query_time << std::endl;
    }

    {
        // multiset
        Result res;
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
        std::cerr << "-- expo --" << std::endl;
        std::cerr << "construct time: " << res.build_time << std::endl;
        std::cerr << "size: " << res.size << std::endl;
        std::cerr << "query_time: " << res.query_time << std::endl;

        // stair data
        res = measure_multiset(make_stair_int_data, data_length, build_num);
        std::cerr << "-- stair --" << std::endl;
        std::cerr << "construct time: " << res.build_time << std::endl;
        std::cerr << "size: " << res.size << std::endl;
        std::cerr << "query_time: " << res.query_time << std::endl;
    }

    return 0;
}
