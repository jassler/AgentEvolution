#pragma once

#include "matrix.hpp"
#include "randwrap.hpp"
#include "defines.hpp"
#include <numeric>

#include <iostream>


template<size_t TGensize, size_t TPhensize>
class Agent {
private:
    std::array<double, TGensize> genome;
    std::array<double, TPhensize> phenotype;
    std::array<double, TPhensize> phenotype_added;

    Matrix<TPhensize, TGensize> matrix;

    Agent<TGensize, TPhensize> *ancestor = nullptr;
    size_t children = 0;
    double score = 0;

    void normalize();

public:
    Agent();
    Agent(const std::array<double, TGensize> genome_start, Agent *parent = nullptr);
    Agent(const std::array<double, TGensize> genome_start, Matrix<TPhensize, TGensize> matrix_start, Agent *parent = nullptr);
    Agent(const Agent& agent);

    Agent make_offspring();

    inline size_t play() {
        return play(rw::from_unit_interval());
    }

    constexpr size_t play(double random_number) {
        // find index in phenotype to determine play
        // eg. if phenotype_added = [0.4, 0.7, 1] and random number 0.9 -> choose index 2
        auto it = std::lower_bound(phenotype_added.begin(), phenotype_added.end(), random_number);
        return static_cast<size_t>(std::distance(phenotype_added.begin(), it));
    }

    inline void play_result(double result) {
        score += result;
    }

    // memory management
    // if no children are left, notify parent about it and kill itself
    // (don't worry, it doesn't feel anything)
    void child_died();
    void kill();

    constexpr auto& get_genome()          const noexcept { return genome; }
    constexpr auto& get_matrix()          const noexcept { return matrix; }
    constexpr auto& get_phenotype()       const noexcept { return phenotype; }
    constexpr auto  get_score()           const noexcept { return score; }
    constexpr auto  get_ancestor()        const noexcept { return ancestor; }
    constexpr auto  get_children_amount() const noexcept { return children; }

    constexpr bool operator< (const Agent& other) const noexcept { return score < other.score; }
    constexpr bool operator> (const Agent& other) const noexcept { return score > other.score; }
    constexpr bool operator<=(const Agent& other) const noexcept { return score <= other.score; }
    constexpr bool operator>=(const Agent& other) const noexcept { return score >= other.score; }


    friend std::ostream& operator<<(std::ostream& os, Agent a) {
        os << "Agent {\n"
            << "\tgenome   : "; for(auto n : a.get_genome()) os << n << ", "; os << "\n"
            << "\tphenotype: "; for(auto n : a.get_phenotype()) os << n << ", "; os << "\n"
            << "}";
        
        return os;
    }
};

template<size_t TGensize, size_t TPhensize>
Agent<TGensize, TPhensize>::Agent()
: ancestor(nullptr) {
    genome.fill(1.0 / TGensize);
    phenotype.fill(1.0 / TPhensize);
}

template<size_t TGensize, size_t TPhensize>
Agent<TGensize, TPhensize>::Agent(const std::array<double, TGensize> genome_start, Agent *parent)
: genome(genome_start), ancestor(parent)  {
    phenotype = matrix * genome_start;
    normalize();
}

template<size_t TGensize, size_t TPhensize>
Agent<TGensize, TPhensize>::Agent(const std::array<double, TGensize> genome_start, Matrix<TPhensize, TGensize> matrix_start, Agent *parent)
: genome(genome_start), phenotype(matrix_start * genome_start), matrix(matrix_start), ancestor(parent) {
    normalize();
}

template<size_t TGensize, size_t TPhensize>
Agent<TGensize, TPhensize>::Agent(const Agent& agent) {
    this->genome = agent.genome;
    this->phenotype = agent.phenotype;
    this->phenotype_added = agent.phenotype_added;
    this->matrix = agent.matrix;
    this->ancestor = agent.ancestor;
    this->children = agent.children;
}

template<size_t TGensize, size_t TPhensize>
Agent<TGensize, TPhensize> Agent<TGensize, TPhensize>::make_offspring() {
    ++children;

    std::array<double, TGensize> genome_start = genome;
    Matrix<TPhensize, TGensize> matrix_start = matrix;

    if constexpr(GENOME_PROB > 0) {
        for(size_t i = 0; i < TGensize; ++i) {
            if(rw::from_unit_interval() < GENOME_PROB)
                genome_start[i] = rw::from_unit_interval();
        }
    }

    if constexpr(MATRIX_PROB > 0) {
        for(size_t y = 0; y < TGensize; ++y) {
            for(size_t x = 0; x < TPhensize; ++x) {
                if(rw::from_unit_interval() < MATRIX_PROB)
                    matrix_start[y][x] = rw::from_unit_interval();
            }
        }
    }

    return Agent(genome_start, matrix_start, this);
}


template<size_t TGensize, size_t TPhensize>
void Agent<TGensize, TPhensize>::kill() {
    if(children != 0) {
        std::cout << "Error: Children amount is not 0 (zero), it is: " << children << "\n";
        return;
    }

    if(ancestor != nullptr)
        ancestor->child_died();
    delete this;
}

template<size_t TGensize, size_t TPhensize>
void Agent<TGensize, TPhensize>::child_died() {
    --children;

    if(children != 0)
        return;

    // avoid recursion, since program would break after
    // 10.000 or so recursive calls
    // -> delete parents using loop
    Agent<TGensize, TPhensize> *parent = ancestor, *next;
    while(parent != nullptr) {
        parent->children -= 1;
        if(parent->children != 0)
            break;
        
        next = parent->ancestor;
        delete parent;
        parent = next;
    }

    delete this;
}

template<size_t TGensize, size_t TPhensize>
void Agent<TGensize, TPhensize>::normalize() {
    double sum = std::accumulate(begin(phenotype), end(phenotype), 0.0);
    double offset = *std::min(begin(phenotype), end(phenotype));
    if(offset < 0)
        sum += -offset * phenotype.size();
    else
        offset = 0;
    
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
    
    std::partial_sum(begin(phenotype), end(phenotype), begin(phenotype_added));
}
