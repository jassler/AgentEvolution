//
//  randwrap.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/15/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "randwrap.hpp"
#include "defines.hpp"
#include <map>

namespace rw {
    
    namespace {
        std::random_device dev;

        std::mt19937::result_type seed_used = dev();
        std::mt19937 rng(seed_used);
        std::uniform_real_distribution<> unit_interval(0, 1);

        // generate every low-/high-distribution once
        std::map<std::tuple<int, int>, std::uniform_int_distribution<>> dist_map;
    }
    
    double from_unit_interval() {
        return unit_interval(rng);
    }
    
    int rand_int(std::uniform_int_distribution<> dist) {
        return dist(rng);
    }

    size_t rand_int(std::uniform_int_distribution<size_t> dist) {
        return dist(rng);
    }
    
    int rand_int(int low, int high) {
        auto t = std::make_tuple(low, high);
        auto dist_it = dist_map.find(t);
        
        if(dist_it == dist_map.end()) {
            // create distribution
            std::uniform_int_distribution<> dist(low, high);
            dist_map[t] = dist;
            return dist(rng);
        }
        
        return dist_it->second(rng);
    }
    
    std::mt19937& get_mt() {
        return rng;
    }
    
    void set_seed(std::mt19937::result_type seed) {
        seed_used = seed;
        rng.seed(seed);
    }

    std::mt19937::result_type get_seed() {
        return seed_used;
    }
};
