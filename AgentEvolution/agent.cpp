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

Agent::Agent(size_t amount)
: Agent::Agent(random_unit_numbers(amount)) {}

Agent::Agent(size_t amount, const Matrix& matrix)
: Agent::Agent(random_unit_numbers(amount), matrix, nullptr) {}

Agent::Agent(const std::vector<double>& gen)
: Agent::Agent(gen, Matrix(gen.size(), gen.size()), nullptr) {}

Agent::Agent(const std::vector<double>& genome, const Matrix& matrix, std::shared_ptr<const Agent> ancestor)
: genome(genome), matrix(matrix), phenotype(matrix * genome), phenotype_added(std::vector<double>(genome.size()))
, ancestor(ancestor), dist_index(0, static_cast<int>(matrix.height() - 1)) {
    
    normalize();
}

// all elements of vector have to add up to 1
void Agent::normalize() {
    
    double sum = std::accumulate(phenotype.begin(), phenotype.end(), 0.0);
    double offset = *std::min(phenotype.begin(), phenotype.end());
    if(offset < 0)
        sum += -offset * phenotype.size();
    else
        offset = 0;
    
    if(phenotype.size() == 8) {
        std::cout << "\n\nHeeeeey! Got length 8! sum is " << sum << ", offset is " << offset << "\n\n";
    }
    // if sum is zero, then all indeces must be zero (extremely unlikely, but who knows)
    // -> set random index to 1
    if(sum == 0) {
        double value = 1.0 / phenotype.size();
        for(auto& p : phenotype)
            p = value;
    } else {
        // div every entry by sum
        for(auto& p : phenotype)
            p = (p - offset) / sum;
    }
    
    std::partial_sum(phenotype.begin(), phenotype.end(), phenotype_added.begin());
}

std::shared_ptr<Agent> Agent::make_offspring() const {
    std::vector<double> mutation = genome;
    
    for(size_t i = 0; i < mutation.size(); ++i) {
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
    return play(rw::from_unit_interval());
}

size_t Agent::play(double random_result) {
    std::cout << "Phenotype is";
    for(const auto& p : phenotype_added)
        std::cout << ", " << p;
    // find index in phenotype to determine play
    // eg. if phenotype_added = [0.4, 0.7, 1] and random number 0.9 -> choose index 2
    auto it = std::lower_bound(phenotype_added.begin(), phenotype_added.end(), random_result);
    std::cout << " | got index " << static_cast<size_t>(std::distance(phenotype_added.begin(), it)) << " with number " << random_result << "\n";
    return static_cast<size_t>(std::distance(phenotype_added.begin(), it));
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
    return static_cast<double>(score) / games_played;
}

unsigned long Agent::size() const {
    return genome.size();
}

std::vector<double> Agent::get_genome() const {
    return genome;
}

std::vector<double> Agent::get_phenotype() const {
    return phenotype;
}

bool Agent::operator< (const Agent& a) const {
    return score < a.score;
}

bool Agent::operator> (const Agent& a) const {
    return score > a.score;
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
