//
//  population.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/12/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "population.hpp"
#include "randwrap.hpp"

void Population::play_games() {
    //    R   P   S
    // R  0  -1   1
    // P  1   0  -1
    // S -1   1   0
    // eg. at [2][1] -> Scissor beats Paper -> 1
    int payoff_matrix[3][3] = {
        {  0, -1,  1 },
        {  1,  0, -1 },
        { -1,  1,  0 }
    };
    
    for(auto it_agent = agents.begin(); it_agent != agents.end(); ++it_agent) {
        
        // opponent that agent is playing against
        auto it_opp = it_agent;
        for(int i = 0; i < population_size - 1; ++i) {
            ++it_opp;
            if(it_opp == agents.end()) {
                // loop back to first player
                it_opp = agents.begin();
            }
            
            int p1 = (*it_agent)->play();
            int p2 = (*it_opp)->play();
            int res = payoff_matrix[p1][p2];
            
            (*it_agent)->play_result(res);
            (*it_opp)->play_result(-res);
        }
    }
}

// roulette selection, mutate remaining agents
// winner_amount must be in range [1, population_size]
void Population::evaluate(int winner_amount) {
    
    /* worst score (offset) to be added to every score so we don't have to deal with negative scores */
    auto minmax = std::minmax_element(agents.begin(), agents.end(), [](auto a1, auto a2){ return a1->get_score() < a2->get_score(); });
    int offset = (*minmax.first)->get_score();
    int max = (*minmax.second)->get_score();
    
    // worst score has to be at least 1
    offset = -offset + 1;
    // for range for random numbers
    max += offset;
    
    // random number generator
    std::uniform_int_distribution<> dist_population(0, population_size-1);
    std::uniform_int_distribution<> dist_threshold(0, max);
    
    /* choose winners */
    std::vector<int> winners;
    
    while(winners.size() < winner_amount) {
        
        // choose random agent that hasn't appeared in winners yet
        int potential_winner;
        do {
            potential_winner = rw::rand_int(dist_population);
        } while(std::find(winners.begin(), winners.end(), potential_winner) != winners.end());
        
        int threshold = rw::rand_int(dist_threshold);
        if(agents[potential_winner]->get_score() + offset > threshold) {
            winners.push_back(potential_winner);
        }
    }
    
    /* create offsprings */
    auto win_it = winners.begin();
    std::vector<std::shared_ptr<Agent>> next_generation;
    while(next_generation.size() < population_size) {
        next_generation.push_back(agents[*win_it]->make_offspring());
        
        ++win_it;
        if(win_it == winners.end()) {
            win_it = winners.begin();
        }
    }
    
    agents = next_generation;
}

std::vector<double> Population::get_best_strategy() {
    auto top = std::max_element(agents.begin(), agents.end(), [](auto a1, auto a2){ return *a1 < *a2; });
    
    return (*top)->get_genome();
}

std::vector<double> Population::get_avg_strategy() {
    auto it = agents.begin();
    std::vector<double> result = (*it)->get_genome();
    
    while(++it != agents.end()) {
        auto it_gen = (*it)->get_genome();
        for(int i = 0; i < result.size(); ++i) {
            result[i] += it_gen[i];
        }
    }
    
    for(int i = 0; i < result.size(); ++i) {
        result[i] /= (double) population_size;
    }
    
    return result;
}

unsigned int Population::size() {
    return population_size;
}
