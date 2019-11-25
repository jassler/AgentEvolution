#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include "agent.hpp"

template<size_t TPopsize, size_t TGensize, size_t TPhensize>
class World {
private:

    std::array<Agent<TGensize, TPhensize>, TPopsize> population;
    Matrix<TPhensize, TPhensize> payoff;

    size_t generation = 0;

    // random device to select offspring-worthy parents
    std::mt19937 rng;

    // randomly selected players
    std::uniform_int_distribution<size_t> dist_players;

    unsigned int get_rand_device_number() {
        std::random_device dev;
        return dev();
    }

public:

    // Random number generator
    World(Agent<TGensize, TPhensize> default_agent, Matrix<TPhensize, TPhensize> payoff_matrix)
    : payoff(payoff_matrix), rng(get_rand_device_number()), dist_players(0, TPopsize - 1) {
        population.fill(Agent(default_agent));
    }

    World(Agent<TGensize, TPhensize> default_agent, Matrix<TPhensize, TPhensize> payoff_matrix, std::mt19937 number_generator)
    : payoff(payoff_matrix), rng(number_generator), dist_players(0, TPopsize - 1) {
        population.fill(Agent(default_agent));
    }

    void simulate_games(size_t opponents = 8) {

        // each player has actually twice the amount of opponents
        // because they are to the left and right
        for(auto player = begin(population); player != end(population); ++player) {

            // opponents to the right of player
            auto opponent = player;

            for(size_t n = 0; n < opponents; ++n) {
                if(++opponent == end(population))
                    opponent = begin(population);
                
                size_t p1 = player->play();
                size_t p2 = opponent->play();
                player->play_result(payoff[p1][p2]);
                opponent->play_result(payoff[p2][p1]);
            }
        }
    }

    /*
     * Roulette selection who gets to make offspring
     * Create new population
     */
    void next_generation() {
        static std::array<Agent<TGensize, TPhensize>*, TPopsize> ancestors;
        static std::array<Agent<TGensize, TPhensize>, TPopsize> new_population;

        ancestors.fill(nullptr);

        // min- and max-score
        // min-score to be added to all scores as to avoid negative values
        auto minmax = std::minmax_element(begin(population), end(population));
        double offset = minmax.first->get_score();
        double max = minmax.second->get_score();

        // offset definitely negative, make it positive
        // +1 'cause worst score can't be 0
        offset = -offset + 1;
        // for range of random numbers
        max += offset;

        // threshold for agents to exceed for them to create offspring
        std::uniform_real_distribution<double> dist_threshold(0.0, max + 1);

        for(auto& child : new_population) {

            size_t rand_agent;
            double threshold;

            // find player who exceeds threshold
            do {
                rand_agent = dist_players(rng);
                threshold = dist_threshold(rng);
            } while((population[rand_agent].get_score() + offset) < threshold);

            // is it the first time this guy gets to produce offspring?
            if(ancestors[rand_agent] == nullptr)
                ancestors[rand_agent] = new Agent(population[rand_agent]);
            
            // produce offspring!
            child = ancestors[rand_agent]->make_offspring();
        }

        // garbage collection
        // notify all childless parents that they were unworthy
        if(generation > 0)
        {
            for (size_t i = 0; i < TPopsize; ++i)
            {
                if(ancestors[i] == nullptr)
                    population[i].get_ancestor()->child_died();
            }
        }

        ++generation;
        population = new_population;
    }

    void simulate_generation(size_t opponents = 8) {
        simulate_games(opponents);
        next_generation();
    }

    auto get_generation() const noexcept { return generation; }
    auto get_payoff() const noexcept { return payoff; }
    auto& operator[](size_t i) { return population.at(i); }
};