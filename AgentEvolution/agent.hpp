//
//  agent.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef agent_hpp
#define agent_hpp

#include "matrix.hpp"
#include "randwrap.hpp"
#include <vector>
#include <iostream>

class Agent : public std::enable_shared_from_this<Agent> {
private:
    std::vector<double> genome;
    Matrix matrix;
    std::vector<double> phenotype;
    
    std::vector<double> phenotype_added;
    
    int games_played = 0;
    int score = 0;
    
    std::shared_ptr<const Agent> ancestor;
    
    // random index for genome
    std::uniform_int_distribution<> dist_index;
    
    void normalize();
    
public:
    Agent(size_t amount);
    Agent(size_t amount, const Matrix& matrix);
    
    Agent(const std::vector<double>& genome);
    Agent(const std::vector<double>& genome, const Matrix& matrix, std::shared_ptr<const Agent> ancestor);
    
//    ~Agent() {
//        if(--AgentInfo::count % 1000 == 0)
//            std::cout << AgentInfo::count << " Agents\n";
//    }
    
    std::shared_ptr<Agent> make_offspring() const;
    std::shared_ptr<const Agent> get_ancestor() const;
    
    // Returns a number from 0 to length of phenotype - 1
    size_t play();
    // Adds points to score
    void play_result(int points);
    
    void reset_score();
    int get_score() const;
    double avg_score() const;
    std::vector<double> get_genome() const;
    unsigned long size() const;
    
    double operator[] (size_t index);
    bool operator< (const Agent& a) const;
    bool operator> (const Agent& a) const;
    
    friend std::ostream& operator<< (std::ostream& os, const Agent& agent);
};

#endif /* agent_hpp */
