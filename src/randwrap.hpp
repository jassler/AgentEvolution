#pragma once

#include <random>

// a wrapper for random functions
namespace rw {
    
    double from_unit_interval();
    
    int rand_int(std::uniform_int_distribution<> dist);
    size_t rand_int(std::uniform_int_distribution<size_t> dist);
    int rand_int(int low, int high);
    
    void set_seed(std::mt19937::result_type seed);
    std::mt19937 get_mt();
}
