//
//  agent.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "argparser.hpp"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

#include <sstream>
// #include <boost/algorithm/string/join.hpp>

std::vector<double> random_unit_numbers(size_t length) {
    std::vector<double> result(length);
    
    for(auto& v : result)
        v = rw::from_unit_interval();
    
    return result;
}

Agent::Agent(int amount)
: Agent::Agent(random_unit_numbers(amount)) {}

Agent::Agent(int amount, const Matrix& matrix)
: Agent::Agent(random_unit_numbers(amount), matrix, nullptr) {}

Agent::Agent(const std::vector<double>& gen)
: Agent::Agent(gen, Matrix(gen.size(), gen.size()), nullptr) {}

Agent::Agent(const std::vector<double>& genome, const Matrix& matrix, std::shared_ptr<const Agent> ancestor)
: genome(genome), matrix(matrix), phenotype(matrix * genome), phenotype_added(std::vector<double>(genome.size()))
, ancestor(ancestor), dist_index(0, (int) matrix.height() - 1) {
    
    normalize();
}

// all elements of vector have to add up to 1
void Agent::normalize() {
    
    double sum = std::accumulate(genome.begin(), genome.end(), 0.0);
    double offset = *std::min(genome.begin(), genome.end());
    if(offset < 0)
        sum += -offset * genome.size();
    else
        offset = 0;
    
    // if sum is zero, then all indeces must be zero (extremely unlikely, but who knows)
    // -> set random index to 1
    if(sum == 0) {
        int index = rw::rand_int(dist_index);
        genome[index] = 1;
        sum = 1;
    }
    
    // div every entry by sum
    for(auto& g : genome)
        g = (g - offset) / sum;
    std::partial_sum(genome.begin(), genome.end(), phenotype_added.begin());
}

std::shared_ptr<Agent> Agent::make_offspring() const {
    std::vector<double> mutation = genome;
    
    for(int i = 0; i < mutation.size(); ++i) {
        if(rw::from_unit_interval() < args::mutation_probs[i]) {
            mutation[i] = rw::from_unit_interval();
        }
    }

    std::shared_ptr<Agent> p = std::make_shared<Agent>(mutation, matrix, shared_from_this());
    return p;
}

std::shared_ptr<const Agent> Agent::get_ancestor() const {
    return ancestor;
}

size_t Agent::play() {
    // random number from 0 - 1
    double r = rw::from_unit_interval();
    
    // find index in phenotype to determine play
    // eg. if phenotype_added = [0.4, 0.7, 1] and random number 0.9 -> choose index 2
    auto it = std::lower_bound(phenotype_added.begin(), phenotype_added.end(), r);
    
    return std::distance(phenotype_added.begin(), it);
}

void Agent::play_result(int points) {
    this->score += points;
    this->games_played += 1;
}

void Agent::reset_score() {
    score = 0;
}

int Agent::get_score() const {
    return score;
}

double Agent::avg_score() const {
    return (double) score / games_played;
}

unsigned long Agent::size() const {
    return genome.size();
}

std::vector<double> Agent::get_genome() const {
    return genome;
}

double Agent::operator[] (int index) {
    return genome[index];
}

bool Agent::operator< (const Agent& a) const {
    return avg_score() < a.avg_score();
}

bool Agent::operator> (const Agent& a) const {
    return avg_score() > a.avg_score();
}


// TODO import boost (?)
namespace boost {
    namespace algorithm {
        
        template<typename T>
        std::string join(std::vector<T> list, std::string delimiter) {
            
            std::stringstream ss;
            auto it = list.begin();
            if(it != list.end()) {
                ss << *it;
                
                while(++it != list.end()) {
                    ss << delimiter << *it;
                }
            }
            
            return ss.str();
        }
    }
}

std::ostream& operator<< (std::ostream& os, const Agent& agent) {
    os << "Agent {\n";
    os << "    genome = " << boost::algorithm::join(agent.genome, ", ") << "\n";
    os << "    matrix = " << agent.matrix << "\n";
    os << "    phenotype = " << boost::algorithm::join(agent.phenotype, ", ") << "\n";
    os << "}";
    
    return os;
}
