#pragma once

const int data_length = 10000;
const int build_num = 10;
const int query_num_per_build = 10000;
const int epsilon = 128;

bool output_each_data = false;
std::set<std::string> measure_index{"PGM", "Dynamic_PGM", "multiset"};
std::set<std::string> measure_data{"uniform", "lognormal", "exponential", "stair"};
