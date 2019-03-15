//
//  population.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/12/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef population_hpp
#define population_hpp

#include "agent.hpp"
#include <vector>
#include <random>

class Population {
private:
    std::vector<std::shared_ptr<Agent>> agents;
    unsigned int population_size;
    
    std::random_device dev;
    std::mt19937 rng;
    
public:
    template<class ..._Args>
    Population(unsigned int population_size, _Args&& ...__args);
    
    void play_games();
    
    // roulette selection, mutate remaining agents
    // winner_amount must be in range [1, population_size]
    void evaluate(int winner_amount);
    
    std::vector<double> get_best_strategy();
    std::vector<double> get_avg_strategy();
    
    unsigned int size();
    
    std::shared_ptr<Agent>& operator[] (int i) {
        return agents[i];
    }
};

template<class ..._Args>
Population::Population(unsigned int population_size, _Args&& ...__args) {
    this->population_size = population_size;
    rng = std::mt19937(dev());
    
    // create all agents
    for(unsigned int i = 0; i < population_size; ++i) {
        agents.push_back(std::make_shared<Agent>(__args...));
    }
}

#endif /* population_hpp */
