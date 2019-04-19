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
#include <memory>

class Agent : public std::enable_shared_from_this<Agent> {
private:
    std::vector<double> genome;
    Matrix matrix;
    std::vector<double> phenotype;
    
    std::vector<double> phenotype_added;
    
    int games_played = 0;
    double score = 0;
    
    std::shared_ptr<Agent> ancestor;
    
    // random index for genome
    std::uniform_int_distribution<> dist_index;
    
    void normalize();
    
public:
    Agent(size_t amount);
    Agent(size_t amount, const Matrix& matrix);
    
    Agent(const std::vector<double>& genome);
    Agent(const std::vector<double>& genome, const Matrix& matrix, std::shared_ptr<Agent> ancestor = nullptr);
    
    std::shared_ptr<Agent> make_offspring();
    std::shared_ptr<Agent> get_ancestor() const { return ancestor; }
    
    // Returns a number from 0 to length of phenotype - 1
    size_t play();
    
    // Pretends that the random number random_result was generated
    // Prerequisite: 0 <= random_result <= 1
    size_t play(double random_result);
    // Adds points to score
    void play_result(double points);
    
    auto reset_score() -> void { score = 0; games_played = 0; }
    auto get_score() const -> double { return score; }
    auto avg_score() const -> double { return score / (games_played == 0 ? 1 : games_played); }
    auto get_genome() const -> std::vector<double> { return genome; }
    auto get_phenotype() const -> std::vector<double> { return phenotype; }
    auto get_matrix() const -> Matrix { return matrix; }
    
    bool operator< (const Agent& a) const { return score < a.score; }
    bool operator> (const Agent& a) const { return score > a.score; }
    bool operator<= (const Agent& a) const { return score <= a.score; }
    bool operator>= (const Agent& a) const { return score >= a.score; }
    bool operator== (const Agent& a) const { return score == a.score; }
    
    friend std::ostream& operator<< (std::ostream& os, const Agent& agent);
};

#endif /* agent_hpp */
