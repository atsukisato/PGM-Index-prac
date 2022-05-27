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
#include "pgm/pgm_index_dynamic.hpp"
#include "data_generator/data_generator.hpp"
#include "measure/measure.hpp"
#include "settings.hpp"

int main() {
    
    // output each data
    if(output_each_data){
        if(measure_data.find("uniform") != measure_data.end()){
            // uniform data
            std::vector<double> data = make_uniform_double_data(data_length);
            normalize_data(data);
            output_data(data, "./uniform.txt");
        }

        if(measure_data.find("lognormal") != measure_data.end()){
            // lognormal data
            std::vector<double> data = make_lognormal_double_data(data_length);
            normalize_data(data);
            output_data(data, "./lognormal.txt");
        }

        if(measure_data.find("exponential") != measure_data.end()){
            // exponential data
            std::vector<double> data = make_exponential_double_data(data_length);
            normalize_data(data);
            output_data(data, "./exponential.txt");
        }

        if(measure_data.find("stair") != measure_data.end()){
            // stair data
            std::vector<int> idata = make_stair_int_data(data_length);
            // normalize_data(data);
            output_data(idata, "./stair.txt");
        }
    }

    // PGM
    if(measure_index.find("PGM") != measure_index.end()){
        std::cerr << "-- PGM --" << std::endl;
        std::cerr << std::endl;
        
        if(measure_data.find("uniform") != measure_data.end()){
            // uniform data
            Result res = measure(make_uniform_double_data, data_length, build_num);
            std::cerr << "-- uniform --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("lognormal") != measure_data.end()){
            // lognormal data
            Result res = measure(make_lognormal_double_data, data_length, build_num);
            std::cerr << "-- lognormal --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("exponential") != measure_data.end()){
            // exponential data
            Result res = measure(make_exponential_double_data, data_length, build_num);
            std::cerr << "-- exponential --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }
        
        if(measure_data.find("stair") != measure_data.end()){
            // stair data
            Result res = measure(make_stair_int_data, data_length, build_num);
            std::cerr << "-- stair --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }
    }

    // Dynamic_PGM
    if(measure_index.find("Dynamic_PGM") != measure_index.end()){
        std::cerr << "-- Dynamic_PGM --" << std::endl;
        std::cerr << std::endl;
        
        if(measure_data.find("uniform") != measure_data.end()){
            // uniform data
            Result res = measure_dynamic_pgm(make_uniform_double_data, data_length, build_num);
            std::cerr << "-- uniform --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("lognormal") != measure_data.end()){
            // lognormal data
            Result res = measure_dynamic_pgm(make_lognormal_double_data, data_length, build_num);
            std::cerr << "-- lognormal --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("exponential") != measure_data.end()){
            // exponential data
            Result res = measure_dynamic_pgm(make_exponential_double_data, data_length, build_num);
            std::cerr << "-- exponential --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }
        
        if(measure_data.find("stair") != measure_data.end()){
            // stair data
            Result res = measure_dynamic_pgm(make_stair_int_data, data_length, build_num);
            std::cerr << "-- stair --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }
    }

    // multiset
    if(measure_index.find("multiset") != measure_index.end()){
        std::cerr << "-- multiset --" << std::endl;
        std::cerr << std::endl;

        if(measure_data.find("uniform") != measure_data.end()){
            // uniform data
            Result res = measure_multiset(make_uniform_double_data, data_length, build_num);
            std::cerr << "-- uniform --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("lognormal") != measure_data.end()){
            // lognormal data
            Result res = measure_multiset(make_lognormal_double_data, data_length, build_num);
            std::cerr << "-- lognormal --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("exponential") != measure_data.end()){
            // exponential data
            Result res = measure_multiset(make_exponential_double_data, data_length, build_num);
            std::cerr << "-- exponential --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }

        if(measure_data.find("stair") != measure_data.end()){
            // stair data
            Result res = measure_multiset(make_stair_int_data, data_length, build_num);
            std::cerr << "-- stair --" << std::endl;
            std::cerr << "construct time: " << res.build_time << std::endl;
            std::cerr << "size: " << res.size << std::endl;
            std::cerr << "query_time: " << res.query_time << std::endl;
        }
    }

    return 0;
}
