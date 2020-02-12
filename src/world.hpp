#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include "agent.hpp"


template<size_t S>
void operator+=(std::array<double, S>& a, const std::array<double, S> b) {
    for(size_t i = 0; i < S; ++i) {
        a[i] += b[i];
    }
}

template<size_t TPopsize, size_t TGensize, size_t TPhensize, size_t TOpponents = 8>
class World {
private:

    std::array<Agent<TGensize, TPhensize>, TPopsize> population;
    std::array<Agent<TGensize, TPhensize>, TPopsize> start_population;
    Matrix<TPhensize, TPhensize> payoff;

    size_t generation = 0;

    // randomly selected players
    std::uniform_int_distribution<size_t> dist_players;

public:

    // Random number generator
    World(Agent<TGensize, TPhensize> default_agent, Matrix<TPhensize, TPhensize> payoff_matrix)
    : payoff(payoff_matrix), dist_players(0, TPopsize - 1) {
        start_population.fill(Agent(default_agent));

        reset_population();
    }

    World(std::initializer_list<Agent<TGensize, TPhensize>> default_agents, Matrix<TPhensize, TPhensize> payoff_matrix)
    : payoff(payoff_matrix), dist_players(0, TPopsize - 1) {

        auto type_amount = default_agents.size();
        auto it = begin(start_population);

        size_t prev = 0, count = 0;
        size_t current;

        for(const auto& a : default_agents) {
            ++count;
            current = static_cast<size_t>(std::round(static_cast<double>(count * TPopsize) / static_cast<double>(type_amount)));
            it = std::fill_n(it, current - prev, a);
            prev = current;
        }

        reset_population();
    }

    void reset_population() noexcept {

        if(generation > 0) {
            for (auto &a : population) {
                a.get_ancestor()->child_died();
            }
            generation = 0;

        } else if(LOG_DEBUG) {
            // first round (probably), show initialized agents
            std::cout << "{\n";
            auto prev = start_population[0].get_phenotype();
            const size_t max = 3;
            size_t count = max;

            for (size_t i = 0; i < TPopsize; ++i)
            {
                if(start_population[i].get_phenotype() != prev)
                    count = max;
                
                if(count > 0 || i >= TPopsize - max) {
                    std::cout << "\t[" << i << "] " << start_population[i] << "\n";
                    
                    if(--count == 0 && i < TPopsize - max - 1)
                        std::cout << "\t...\n";
                }
                
                prev = start_population[i].get_phenotype();
            }
            std::cout << "}\n";
        }
        population = start_population;
        if(LOG_DEBUG) {
            std::cout << "Reset population\n";
        }
    }

    void simulate_games() {

        // each player has actually twice the amount of opponents
        // because they are to the left and right
        for(auto player = begin(population); player != end(population); ++player) {

            // opponents to the right of player
            auto opponent = player;

            for(size_t n = 0; n < TOpponents; ++n) {
                // ignoring if player plays against himself
                // since in 0-dimensions this affects everyone evenly
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
#define DO_SORT_WAY
#ifdef DO_SORT_WAY
        static std::array<Agent<TGensize, TPhensize>*, TPopsize> ancestors;
        static std::array<Agent<TGensize, TPhensize>, TPopsize> new_population;

        static std::array<double, TPopsize> accumulated_score;

        ancestors.fill(nullptr);
        double max = 0;

        // initialize accumulated_score
        {
            double offset = -std::min_element(begin(population), end(population))->get_score();
            auto pop_it = begin(population);

            for(auto& accumulated : accumulated_score) {
                accumulated = max = pop_it->get_score() + max + offset;
                ++pop_it;
            }
        }
        std::uniform_real_distribution<double> dist_threshold(0.0, max);

        for(auto& child : new_population) {
            double lucky_score = dist_threshold(rw::get_mt());

            auto it = std::lower_bound(accumulated_score.begin(), accumulated_score.end(), lucky_score);
            size_t index = static_cast<size_t>(std::distance(accumulated_score.begin(), it));

            // is it the first time this guy gets to produce offspring?
            if(ancestors[index] == nullptr)
                ancestors[index] = new Agent(population[index]);

            // produce offspring!
            child = ancestors[index]->make_offspring();
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
#else
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
                rand_agent = dist_players(rw::get_mt());
                threshold = dist_threshold(rw::get_mt());
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
#endif
    }

    void simulate_generation() {
        simulate_games();
        next_generation();
    }

    void calculate_average_phenotype(std::array<double, TPhensize>& result) {
        result.fill(0);

        // accumulate
        std::for_each(
            begin(population),
            end(population),
            [&](auto agent){ result += agent.get_phenotype(); }
        );

        // divide
        std::for_each(
            begin(result),
            end(result),
            [](auto& v){ v /= TPopsize; }
        );
    }

    template <class TPredicate>
    inline auto count_agent_if(TPredicate predicate) const noexcept {
        return std::count_if(begin(population), end(population), predicate);
    }

    auto get_generation() const noexcept { return generation; }
    auto get_payoff() const noexcept { return payoff; }
    auto get_best_agent() noexcept { return std::max(begin(population), end(population)); }
    
    auto &operator[](size_t i) { return population.at(i); }
    constexpr size_t getPopsize() const noexcept { return TPopsize; }
    constexpr size_t getGensize() const noexcept { return TGensize; }
    constexpr size_t getPhensize() const noexcept { return TPhensize; }
    constexpr auto getPopulation() const noexcept { return population; }
};
