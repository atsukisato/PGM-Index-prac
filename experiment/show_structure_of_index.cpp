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
    std::vector<int> data{1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    const int epsilon = 2;

    pgm::PGMIndex<int, epsilon> index(data);

    index.debug();

    auto it = index.search(100);
    std::cerr << it.pos << ' ' << it.lo << ' ' << it.hi << std::endl; 

    return 0;
}
