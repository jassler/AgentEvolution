//
//  randwrap.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/15/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef randwrap_hpp
#define randwrap_hpp

#include <random>

// a wrapper for random functions
namespace rw {
    
    double from_unit_interval();
    
    int rand_int(std::uniform_int_distribution<> dist);
}

#endif /* randwrap_hpp */
