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
    std::vector<long int> data;
    long int now = 10000;
    while(now <= LONG_MAX / 2){
        data.push_back(now);
        now *= 1.0001;
    }

    const int epsilon = 2;

    pgm::PGMIndex<long int, epsilon> index(data);

    index.debug();

    auto it = index.search(100);
    std::cerr << it.pos << ' ' << it.lo << ' ' << it.hi << std::endl; 

    return 0;
}
