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
#include "matrix.hpp"
#include <vector>
#include <random>

class Population {
private:
    std::vector<Agent> agents;
    unsigned int population_size;
    unsigned int opp_amount;
    
    std::uniform_int_distribution<> dist_population;
    
public:
    template<class ..._Args>
    Population(unsigned int population_size, unsigned int opp_amount, _Args&& ...__args);
    
    void play_games();
    
    // roulette selection, mutate remaining agents
    // winner_amount must be in range [1, population_size]
    void evaluate(int winner_amount);
    
    std::vector<double> get_best_strategy();
    std::vector<double> get_avg_strategy();
    
    unsigned int size();
    
    Agent operator[] (int i) {
        return agents[i];
    }
};

// template constructor must stay in header
template<class ..._Args>
Population::Population(unsigned int population_size, unsigned int opp_amount, _Args&& ...__args) : opp_amount(opp_amount), dist_population(0, population_size - 1) {
    this->population_size = population_size;
    
    // create all agents
    for(unsigned int i = 0; i < population_size; ++i) {
        Agent a(__args...);
        agents.push_back(a);
    }
}

#endif /* population_hpp */
