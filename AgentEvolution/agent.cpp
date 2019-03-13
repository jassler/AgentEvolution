//
//  agent.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

// all elements of vector have to add up to 1
void normalize(std::vector<double>& vec, std::vector<double>& added) {
    double sum = 0;
    
    // account for negative numbers - can only normalize positive stuff
    double min = 0;
    for(auto it = vec.begin(); it != vec.end(); ++it) {
        if(*it < min)
            min = *it;
    }
    
    // sum up
    for(int i = 0; i < vec.size(); i++) {
        sum += vec[i] - min;
    }
    
    // div by sum
    double added_sum = 0;
    for(int i = 0; i < vec.size(); i++) {
        vec[i] /= sum;
        
        added_sum += vec[i];
        added[i] = added_sum;
    }
}

Agent::Agent(int amount, Agent *ancestor) : ancestor(ancestor) {
    for(int i = 0; i < amount; i++) {
        double num = (double) rand() / RAND_MAX;
        genome.push_back(num);
        genome_added.push_back(num);
    }
    
    normalize(genome, genome_added);
    
}

Agent::Agent(std::vector<double> genome, Agent *ancestor) : ancestor(ancestor) {
    this->genome = genome;
    for(auto it = genome.begin(); it != genome.end(); it++)
        genome_added.push_back(*it);
    
    normalize(this->genome, genome_added);
}

Agent::Agent(std::initializer_list<double> gen, Agent *ancestor) : ancestor(ancestor) {
    for(double g : gen) {
        genome.push_back(g);
        genome_added.push_back(g);
    }
    
    normalize(genome, genome_added);
}

std::shared_ptr<Agent> Agent::make_offspring() {
    std::vector<double> mutation;
    for(auto it = genome.begin(); it != genome.end(); ++it) {
        // variation between +/- 2.5%
        double v = *it + (*it * ((double) rand() / RAND_MAX * 0.05 - 0.025));
        mutation.push_back(v);
    }
    
    std::shared_ptr<Agent> p = std::make_shared<Agent>(mutation, this);
    return p;
}

int Agent::play() {
    double r = (double) rand() / RAND_MAX;
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
