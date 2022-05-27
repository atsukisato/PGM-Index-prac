#include "settings.hpp"

struct Result {
    double build_time;
    double size;
    double query_time;
};

template<typename T>
Result measure(std::vector<T>(*gen_data)(int), const int data_length, const int build_num){
    std::vector<double> build_times;
    std::vector<double> sizes;
    std::vector<double> heights;
    std::vector<double> query_times;

    for(int i=0;i<build_num;i++){
        std::vector<T> data = gen_data(data_length);

        // measure build time
        std::chrono::system_clock::time_point start, end;
        try{
            pgm::PGMIndex<T, epsilon> index(data);
        }catch(...){
            output_data(data, "err.txt");
            std::cerr << "ERR" << std::endl;
            exit(1);
        }
        start = std::chrono::system_clock::now();
        pgm::PGMIndex<T, epsilon> index(data);
        end = std::chrono::system_clock::now();
        double build_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        build_times.push_back(build_time);

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
        sizes.push_back((double)index.size_in_bytes());
        query_times.push_back(query_time);
    }

    double build_time_ave = get_ave_std(build_times).first;
    double size_ave = get_ave_std(sizes).first;
    double query_time_ave = get_ave_std(query_times).first;

    Result res = {build_time_ave, size_ave, query_time_ave};
    return res;
}

template<typename T>
Result measure_dynamic_pgm(std::vector<T>(*gen_data)(int), const int data_length, const int build_num){
    std::vector<double> build_times;
    std::vector<double> sizes;
    std::vector<double> query_times;

    for(int i=0;i<build_num;i++){
        std::vector<T> data = gen_data(data_length);
        std::vector<std::pair<T, int>> pair_data;
        for(int j=0;j<data.size();j++){
            pair_data.push_back(std::pair<T, int>(data[i], i));
        }

        // measure build time
        std::chrono::system_clock::time_point start, end;
        /*
        try{
            pgm::DynamicPGMIndex<T, int> dynamic_index(pair_data.begin(), pair_data.end());
        }catch(...){
            output_data(pair_data, "err.txt");
            std::cerr << "ERR" << std::endl;
            exit(1);
        }*/
        start = std::chrono::system_clock::now();
        pgm::DynamicPGMIndex<T, int> dynamic_index(pair_data.begin(), pair_data.end());
        end = std::chrono::system_clock::now();
        double build_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        build_times.push_back(build_time);

        // measure query time
        std::vector<double> queries;
        for(int j=0;j<query_num_per_build;j++){
            double f = (double)rand() / RAND_MAX;
            queries.push_back(f);
        }

        start = std::chrono::system_clock::now();
        for(int j=0;j<query_num_per_build;j++)auto range = dynamic_index.find(queries[j]);
        end = std::chrono::system_clock::now();
        double query_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);

        build_times.push_back(build_time);
        sizes.push_back((double)dynamic_index.size_in_bytes());

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

    for(int i=0;i<build_num;i++){
        std::vector<T> data_v = gen_data(data_length);
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