//
//  agent.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "randwrap.hpp"
#include "argparser.hpp"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

// all elements of vector have to add up to 1
void Agent::normalize() {
    
    double sum = std::accumulate(genome.begin(), genome.end(), 0.0);
    
    // if sum is zero, then all indeces must be zero (extremely unlikely, but who knows)
    // -> set random index to 1
    if(sum == 0) {
        int index = rw::rand_int(dist_index);
        genome[index] = 1;
        sum = 1;
    }
    
    // div every entry by sum
    std::transform(genome.begin(), genome.end(), genome.begin(), [sum](double d) { return d / sum; });
    std::partial_sum(genome.begin(), genome.end(), genome_added.begin());
}

Agent::Agent(int amount, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, amount-1), genome_added(amount), matrix(amount, amount) {
    for(int i = 0; i < amount; i++) {
        double r = rw::from_unit_interval();
        genome.push_back(r);
    }
    
    me = std::make_shared<Agent>(*this);
    normalize();
    
}

Agent::Agent(std::vector<double> gen, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, (unsigned int) gen.size()-1), genome_added(gen.size()), matrix(gen.size(), gen.size()) {
    for(const double& g : gen) {
        genome.push_back(g);
    }
    
    me = std::make_shared<Agent>(*this);
    normalize();
}

Agent::Agent(std::initializer_list<double> gen, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, (unsigned int) gen.size()-1), genome_added(gen.size()), matrix(gen.size(), gen.size()) {
    for(const double& g : gen) {
        genome.push_back(g);
    }
    
    me = std::make_shared<Agent>(*this);
    normalize();
}

//Agent::Agent(Matrix genome, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, genome.height()-1), genome(genome), genome_added(genome.height()) {
//    me = std::make_shared<Agent>(*this);
//    normalize();
//}

Agent Agent::make_offspring() const {
    std::vector<double> mutation = genome;
    
    for(int i = 0; i < mutation.size(); ++i) {
        if(rw::from_unit_interval() < args::mutation_probs[i]) {
            mutation[i] = rw::from_unit_interval();
        }
    }
    
    Agent p(mutation, std::shared_ptr<Agent>(me));
    return p;
}

std::shared_ptr<Agent> Agent::get_ancestor() const {
    return ancestor;
}

int Agent::play() {
    double r = rw::from_unit_interval();
    int index = 0;
    
    // TODO lower / upper bound
    // std::distance
    while(r >= genome_added[index] && index < genome.size() - 1) {
        index++;
    }
    
    return index;
}

void Agent::play_result(int points) {
    this->score += points;
}

void Agent::reset_score() {
    score = 0;
}

int Agent::get_score() const {
    return score;
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
    return score < a.score;
}

bool Agent::operator> (const Agent& a) const {
    return score > a.score;
}

std::ostream& operator<< (std::ostream& os, const Agent& agent) {
    os << '[';
    auto it = agent.genome.begin();
    if(it != agent.genome.end()) {
        os << *it;
        
        while(++it != agent.genome.end()) {
            os << ", " << *it;
        }
    }
    
    os << ']';
    
    return os;
}
