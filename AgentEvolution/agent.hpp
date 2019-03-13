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

class Agent {
private:
    std::vector<double> genome;
    std::vector<double> genome_added;
    int score = 0;
    
    Agent *ancestor;
    
public:
    Agent(int amount, Agent *ancestor = NULL);
    Agent(std::vector<double> genome, Agent *ancestor = NULL);
    Agent(std::initializer_list<double> genome, Agent *ancestor = NULL);
    
    std::shared_ptr<Agent> make_offspring();
    
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
