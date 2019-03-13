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
    Population(unsigned int population_size, _Args&& ...__args) {
        this->population_size = population_size;
        rng = std::mt19937(dev());
        
        for(unsigned int i = 0; i < population_size; ++i) {
            agents.push_back(std::make_shared<Agent>(__args...));
        }
    }
    
    void play_games() {
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
        
        for(auto it = agents.begin(); it != agents.end(); ++it) {
            if((*it)->get_score() != 0)
                return;
        }
        
        return;
    }
    
    // roulette selection, mutate remaining agents
    // winner_amount must be in range [1, population_size]
    void evaluate(int winner_amount) {
        
        /* worst score to be added to every score so we don't have to deal with negative scores */
        int offset = 0;
        int max = 0;
        for(auto it = agents.begin(); it != agents.end(); ++it) {
            if((*it)->get_score() < offset)
                offset = (*it)->get_score();
            
            else if((*it)->get_score() > max)
                max = (*it)->get_score();
        }
        offset = -offset + 1;
        max += offset;
        
        // random number generator
        std::uniform_int_distribution<std::mt19937::result_type> dist_population(0, population_size-1);
        std::uniform_int_distribution<std::mt19937::result_type> dist_threshold(0, max);
        
        /* choose winners */
        std::vector<int> winners;
        
        while(winners.size() < winner_amount) {
            
            // choose random agent that hasn't appeared in winners yet
            int potential_winner;
            do {
                potential_winner = dist_population(rng);
            } while(std::find(winners.begin(), winners.end(), potential_winner) != winners.end());
            
            int threshold = dist_threshold(rng);
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
    
    std::vector<double> get_best_strategy() {
        auto it = agents.begin();
        std::shared_ptr<Agent> top = *it;
        
        while(++it != agents.end()) {
            if(**it > *top) {
                top = *it;
            }
        }
        
        return top->get_genome();
    }
    
    std::vector<double> get_avg_strategy() {
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
    
    unsigned int size() {
        return population_size;
    }
};

#endif /* population_hpp */
