#include "settings.hpp"

template<typename T>
void output_data(const std::vector<T> &data, const char *file_name){
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

void normalize_data(std::vector<double> &data){
    const int n = data.size();
    double a = 1.0 / (data[n-1] - data[0]);
    double b = data[0];
    for(int i=0;i<n;i++){
        data[i] = (data[i] - b) * a;
    }
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
    normalize_data(data);
    return data;
}

std::vector<double> make_exponential_double_data(const int data_length = 1000000){
    std::vector<double> data;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(1.0);

    for(int i = 0; i < data_length; i++){
        double f = d(gen);
        data.push_back(f);
    }
    sort(data.begin(), data.end());
    normalize_data(data);
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
    normalize_data(data);
    return data;
}

std::vector<int> make_stair_int_data(const int data_length = 1000000){
    int step_width = epsilon * 3;   // ステップ幅
    // i番目に登場する数字 = [1.01^i] とする
    
    std::vector<double> data;

    for(int i = 0; i < data_length; i++){
        double f = pow(1.001, (int)(i / step_width) + 1);
        data.push_back(f);
    }
    std::vector<int> idata;
    double a = (INT_MAX - 1) / (data[data_length-1] - data[0]);
    double b = data[0];
    for(int i = 0; i < data_length; i++) {
        int x = (data[i] - b ) * a;
        idata.push_back(x);
    }
    sort(idata.begin(), idata.end());
    return idata;
}

