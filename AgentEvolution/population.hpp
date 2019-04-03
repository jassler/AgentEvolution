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
#include <initializer_list>
#include <vector>
#include <random>

class Population {
private:
    std::vector<std::shared_ptr<Agent>> agents;
    size_t population_size;
    size_t opp_amount;
    
    std::uniform_int_distribution<size_t> dist_population;
    
public:
    template<class ..._Args>
    Population(size_t population_size, size_t opp_amount, _Args&& ...__args);
    
    void play_games();
    
    // roulette selection, mutate remaining agents
    // winner_amount must be in range [1, population_size]
    void evaluate(size_t winner_amount);
    
    std::shared_ptr<Agent> get_best_agent();
    std::vector<double> get_avg_genome();
    std::vector<double> get_avg_phenotype();
    
    size_t size();
    
    std::shared_ptr<Agent> operator[] (size_t i);
};

// template constructor must stay in header
// _args is passed to Agent constructor
template<class ..._Args>
Population::Population(size_t population_size, size_t opp_amount, _Args&& ...__args) : opp_amount(opp_amount), dist_population(0, population_size - 1) {
    this->population_size = population_size;
    
    // create all agents
    for(size_t i = 0; i < population_size; ++i) {
        std::shared_ptr<Agent> a = std::make_shared<Agent>(__args...);
        agents.push_back(a);
    }
}

#endif /* population_hpp */
