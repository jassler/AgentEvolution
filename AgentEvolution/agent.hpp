//
//  agent.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef agent_hpp
#define agent_hpp

#include <vector>
#include <memory>
#include <iostream>
#include <random>

class Agent {
private:
    std::vector<double> genome;
    std::vector<double> genome_added;
    int score = 0;
    
    std::shared_ptr<Agent> ancestor;
    
    // random index for genome
    std::uniform_int_distribution<> dist_index;
    
    void normalize();
    
public:
    Agent(int amount, std::shared_ptr<Agent> ancestor = nullptr);
    Agent(std::vector<double> genome, std::shared_ptr<Agent> ancestor = nullptr);
    Agent(std::initializer_list<double> genome, std::shared_ptr<Agent> ancestor = nullptr);
    
    std::shared_ptr<Agent> make_offspring();
    std::shared_ptr<Agent> get_ancestor();
    
    // Returns a number from 0 to length of genome - 1
    int play();
    // Adds points to score
    void play_result(int points);
    
    void reset_score();
    int get_score();
    std::vector<double> get_genome();
    unsigned long size();
    
    double operator[] (int);
    bool operator< (const Agent& a) const;
    bool operator> (const Agent& a) const;
    
    friend std::ostream& operator<< (std::ostream& os, const Agent& agent);
};

#endif /* agent_hpp */
