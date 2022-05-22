/*
 * This example shows how to index and query a vector of random integers with the PGM-index.
 * Compile with:
 *   g++ simple.cpp -std=c++17 -I../include -o simple
 * Run with:
 *   ./simple
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "pgm/pgm_index.hpp"

int main() {
    // Generate some random data
    std::vector<int> data;
    for(int i=1;i<1000;i++){
        data.push_back(i);
    }
    for(int i=1;i<1000;i++){
        if(i > 1){
            data.push_back(100000 + i + 100);
        }else{
            data.push_back(100000 + i);
        }
    }
    std::sort(data.begin(), data.end());

    // Construct the PGM-index
    const int epsilon = 128; // space-time trade-off parameter
    pgm::PGMIndex<int, epsilon> index(data);

    // Query the PGM-index

    for(int q = 0; q <= 1010; q++){
        auto range = index.search(q);
        auto lo = data.begin() + range.lo;
        auto hi = data.begin() + range.hi;
        std::cout << q << ' ' << range.lo << ' ' << range.hi << ' ' << std::endl;
        auto it = std::lower_bound(lo, hi, q);
        std::cout << std::distance(data.begin(), it) << std::endl;
    }
    for(int q = 99999; q <= 100100; q++){
        auto range = index.search(q);
        auto lo = data.begin() + range.lo;
        auto hi = data.begin() + range.hi;
        std::cout << q << ' ' << range.lo << ' ' << range.hi << ' ' << std::endl;
        auto it = std::lower_bound(lo, hi, q);
        std::cout << std::distance(data.begin(), it) << std::endl;
    }
    

    return 0;
}