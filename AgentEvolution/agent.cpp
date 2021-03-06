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
#include <iostream>

#include <sstream>
#include <boost/algorithm/string/join.hpp>

std::vector<double> random_unit_numbers(size_t length) {
    std::vector<double> result(length);
    
    for(auto& v : result)
        v = rw::from_unit_interval();
    
    return result;
}

Agent::Agent(size_t amount)
: Agent::Agent(random_unit_numbers(amount)) {}

Agent::Agent(size_t amount, const Matrix& matrix)
: Agent::Agent(random_unit_numbers(amount), matrix) {}

Agent::Agent(const std::vector<double>& gen)
: Agent::Agent(gen, Matrix(gen.size(), gen.size())) {}

Agent::Agent(const std::vector<double>& genome, const Matrix& matrix, std::shared_ptr<Agent> ancestor)
: genome(genome), matrix(matrix), phenotype(matrix * genome), phenotype_added(std::vector<double>(genome.size()))
, ancestor(ancestor), dist_index(0, static_cast<int>(matrix.height() - 1)) {

    normalize();
}

// all elements of phenotype have to add up to 1
void Agent::normalize() {
    
    // double sum = std::accumulate(phenotype.begin(), phenotype.end(), 0.0);
    // double offset = -1 * (*std::min(phenotype.begin(), phenotype.end()));
    // if(offset > 0)
    //     sum += offset * phenotype.size();
    // else
    //     offset = 0;

    
    
    // // if sum is zero, then all indeces must be zero (extremely unlikely, but who knows)
    // // -> set random index to 1
    // if(sum == 0) {
    //     double value = 1.0 / phenotype.size();
    //     for(auto& p : phenotype)
    //         p = value;
    // } else {
    //     // div every entry by sum
    //     for(auto& p : phenotype)
    //         p = (p + offset) / sum;
    // }
    
    double min = 0;
    for(double v : phenotype) {
        if(v < min)
            min = v;
    }
    if(min < 0) {
        for(auto& p : phenotype)
            p -= min;
    }

    double sum = std::accumulate(phenotype.begin(), phenotype.end(), 0.0);
    if(sum == 0) {
        for(auto& p : phenotype)
            p = 1.0 / static_cast<double>(phenotype.size());
    } else {
        for(auto& p : phenotype)
            p /= sum;
    }
    std::partial_sum(phenotype.begin(), phenotype.end(), phenotype_added.begin());

    for(double p : phenotype) {
        if(p < 0 || p > 1) {
            std::cout << p << " shouldn't exist (min=" << min << ", sum=" << sum << ")\n";
        }
    }
}

std::shared_ptr<Agent> Agent::make_offspring() {
    std::vector<double> mutation = genome;
    Matrix m = matrix;
    
    for(size_t i = 0; i < mutation.size(); ++i) {
        if(rw::from_unit_interval() < args::mutation_probs[i]) {
            mutation[i] = rw::from_unit_interval();
        }
    }
    
    if(args::matrix_mutation > 0) {
        for(auto& value : m) {
            if(rw::from_unit_interval() < args::matrix_mutation) {
                value += (rw::from_unit_interval() * 10) - 5;
            }
        }
    }

    return std::make_shared<Agent>(mutation, m, shared_from_this());
}

size_t Agent::play() {
    return play(rw::from_unit_interval());
}

size_t Agent::play(double random_result) {
    // find index in phenotype to determine play
    // eg. if phenotype_added = [0.4, 0.7, 1] and random number 0.9 -> choose index 2
    auto it = std::lower_bound(phenotype_added.begin(), phenotype_added.end(), random_result);

    return static_cast<size_t>(std::distance(phenotype_added.begin(), it));
}

void Agent::play_result(double points) {
    this->score += points;
    this->games_played += 1;
}

std::ostream& operator<< (std::ostream& os, const Agent& agent) {
    // os << "Agent {\n";
    // os << "    genome = " << boost::algorithm::join(agent.genome, ", ") << "\n";
    // // os << "    matrix = " << agent.matrix << "\n";
    // os << "    phenotype = " << boost::algorithm::join(agent.phenotype, ", ") << "\n";
    // os << "}";
    os << agent.phenotype.size() << '\n';
    
    return os;
}
