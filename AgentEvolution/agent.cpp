//
//  agent.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "randwrap.hpp"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

// all elements of vector have to add up to 1
void Agent::normalize() {
    double sum = 0;
    
    // sum up
    for(int i = 0; i < genome.size(); i++) {
        sum += genome[i];
    }
    
    // if sum is zero, then all indeces must be zero (extremely unlikely, but who knows)
    // -> set random index to 1
    if(sum == 0) {
        int index = rw::rand_int(dist_index);
        genome[index] = 1;
        sum = 1;
    }
    
    // div by sum
    double added_sum = 0;
    for(int i = 0; i < genome.size(); i++) {
        genome[i] /= sum;
        
        added_sum += genome[i];
        genome_added[i] = added_sum;
    }
}

Agent::Agent(int amount, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, amount-1) {
    
    genome_added.reserve(amount);
    
    for(int i = 0; i < amount; i++) {
        double r = rw::from_unit_interval();
        genome.push_back(r);
    }
    
    normalize();
    
}

Agent::Agent(std::vector<double> gen, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, (unsigned int) gen.size()-1) {
    genome_added.reserve(gen.size());
    
    this->genome = gen;
    
    normalize();
}

Agent::Agent(std::initializer_list<double> gen, std::shared_ptr<Agent> ancestor) : ancestor(ancestor), dist_index(0, (unsigned int) gen.size()-1) {
    genome_added.reserve(gen.size());
    
    for(double g : gen) {
        genome.push_back(g);
    }
    
    normalize();
}

std::shared_ptr<Agent> Agent::make_offspring() {
    std::vector<double> mutation = genome;
    
    int index = rw::rand_int(dist_index);
    double r = rw::from_unit_interval();
    
    mutation[index] = r;
    
    std::shared_ptr<Agent> p = std::make_shared<Agent>(mutation, std::make_shared<Agent>(*this));
    return p;
}

std::shared_ptr<Agent> Agent::get_ancestor() {
    return ancestor;
}

int Agent::play() {
    double r = rw::from_unit_interval();
    int index = 0;
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

int Agent::get_score() {
    return score;
}

unsigned long Agent::size() {
    return genome.size();
}

std::vector<double> Agent::get_genome() {
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
