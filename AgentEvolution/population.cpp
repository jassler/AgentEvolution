//
//  population.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/12/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "population.hpp"
#include "randwrap.hpp"
#include "argparser.hpp"
#include <algorithm>
#include <utility>
#include <iostream>

void Population::play_games() {
    std::shuffle(agents.begin(), agents.end(), rw::get_mt());
    
    // opponents agent play against
    auto it_opp = agents.begin();
    
    // every player playes n games
    for(auto& it_agent : agents) {
        for(size_t i = 0; i < opp_amount; ++i) {
            ++it_opp;
            if(it_opp == agents.end()) {
                // loop back to first player
                it_opp = agents.begin();
            }
            
            size_t p1 = it_agent->play();
            size_t p2 = (*it_opp)->play();
            
            it_agent->play_result(payoff_matrix[p1][p2]);
            (*it_opp)->play_result(payoff_matrix[p2][p1]);
        }
    }
}

// roulette selection, mutate remaining agents
// winner_amount must be in range [1, population_size]
void Population::evaluate(size_t winner_amount) {
    if(winner_amount < 1 || winner_amount > population_size)
        return;
    
    /* worst score (offset) to be added to every score so we don't have to deal with negative scores */
    auto minmax = std::minmax_element(agents.begin(), agents.end(), [](auto a1, auto a2){ return a1->get_score() < a2->get_score(); });
    double offset = (*minmax.first)->get_score();
    double max = (*minmax.second)->get_score();
    
    // worst score has to be at least 1
    offset = -offset + 1;
    // for range for random numbers
    max += offset;
    
    // random number generator
    std::uniform_int_distribution<size_t> dist_threshold(0, static_cast<size_t>(max + 1));
    
    /* choose winners */
    std::vector<size_t> winners;
    
    while(winners.size() < winner_amount) {
        std::uniform_int_distribution<size_t> spin(0, population_size - 1 - winners.size());
        
        // choose random agent that hasn't appeared in winners yet
        size_t potential_winner = rw::rand_int(spin);
        // int potential_winner = rw::rand_int(0, static_cast<int>(population_size - 1 - winners.size()));
//        do {
//            potential_winner = rw::rand_int(dist_population);
//        } while(std::find(winners.begin(), winners.end(), potential_winner) != winners.end());
        
        size_t threshold = rw::rand_int(dist_threshold);
        if(static_cast<size_t>(agents[potential_winner]->get_score() + offset) > threshold) {
            winners.push_back(potential_winner);
            std::swap(agents[potential_winner], agents[population_size - 1 - winners.size()]);
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

std::shared_ptr<Agent> Population::get_best_agent() {
    auto top = std::max_element(agents.begin(), agents.end(), [](auto a1, auto a2){ return *a1 < *a2; });
    return *top;
}

std::vector<double> Population::get_avg_phenotype() {
    auto it = agents.begin();
    auto avg = (*it)->get_phenotype();
    std::vector<double> t(avg.size());

    while(++it != agents.end()) {
        t = (*it)->get_phenotype();
        for(size_t i = 0; i < avg.size(); ++i) {
            avg[i] += t[i];
        }
    }

    for(auto& v : avg) {
        v /= agents.size();
    }

    return avg;
}

std::vector<double> Population::get_avg_genome() {
    auto it = agents.begin();
    auto avg = (*it)->get_genome();
    std::vector<double> t(avg.size());

    while(++it != agents.end()) {
        t = (*it)->get_genome();
        for(size_t i = 0; i < avg.size(); ++i) {
            avg[i] += t[i];
        }
    }

    for(auto& v : avg) {
        v /= agents.size();
    }

    return avg;
}

std::shared_ptr<Agent> Population::operator[] (size_t i) const {
    if(i >= population_size) {
        throw "Index out of bounds (size is " + std::to_string(population_size) + ", but index was " + std::to_string(i) + ")";
    }

    return agents[i];
}
