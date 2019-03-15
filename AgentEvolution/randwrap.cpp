//
//  randwrap.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/15/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "randwrap.hpp"

namespace rw {
    
    namespace {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> unit_interval(0, 1);
    }
    
    double from_unit_interval() {
        return unit_interval(rng);
    }
    
    int rand_int(std::uniform_int_distribution<> dist) {
        return dist(rng);
    }
    
    
};
