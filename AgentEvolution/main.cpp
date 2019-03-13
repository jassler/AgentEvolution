//
//  main.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "population.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <time.h>

#define NUM_AGENTS 1000

// number of opponents each player plays against
// eg. if set to 5, player 1 will play against 2, 3, 4, 5, 6, player 2 plays against 3, 4, ..., 7, ...
#define NUM_OPPONENTS 999

//void play_games(std::vector<Agent>& agents) {
//
//    //    R   P   S
//    // R  0  -1   1
//    // P  1   0  -1
//    // S -1   1   0
//    // eg. at [2][1] -> Scissor beats Paper -> 1
//    int payoff_matrix[3][3] = {
//        {  0, -1,  1 },
//        {  1,  0, -1 },
//        { -1,  1,  0 }
//    };
//
//    for(auto it_agent = agents.begin(); it_agent != agents.end(); ++it_agent) {
//
//        // opponent that agent is playing against
//        auto it_opp = it_agent;
//        for(int i = 0; i < NUM_OPPONENTS; ++i) {
//            ++it_opp;
//            if(it_opp == agents.end()) {
//                // loop back to first player
//                it_opp = agents.begin();
//            }
//
//            int p1 = it_agent->play();
//            int p2 = it_opp->play();
//            int res = payoff_matrix[p1][p2];
//
//            it_agent->play_result(res);
//            it_opp->play_result(-res);
//        }
//    }
//}
//
//void roulette_selection(std::vector<Agent>& agents) {
//    // worst score to be added to every score so we don't have to deal with negative scores
//    int offset = 0;
//    for(auto it = agents.begin(); it != agents.end(); ++it) {
//        if(it->get_score() < offset)
//            offset = it->get_score();
//    }
//    offset = (-offset) + 1;
//
//    // random numbers determine worthy players to reproduce.
//    // to do this, we calculate the accumulative scores which gives us a range of numbers
//    // for each players. Players with good scores have higher ranges and thus a better chance to reproduce.
//    std::vector<int> score_added;
//
//    int before = 0;
//    for(auto it = agents.begin(); it != agents.end(); ++it) {
//        int added = it->get_score() + offset + before;
//        score_added.push_back(added);
//        before = added;
//    }
//
//    // last index has highest value
//    int max = *(score_added.end() - 1) + offset;
//
//    std::random_device dev;
//    std::mt19937 rng(dev());
//    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, max);
//
//    std::vector<int> winners;
//    for(int i = 0; i < 100; ++i) {
//        int winner = dist6(rng);
//
//        if(std::find(winners.begin(), winners.end(), winner) != winners.end()) {
//            // no duplicate! new number!
//            --i;
//            continue;
//        }
//
//        int j = 0;
//        while(j < score_added.size() - 1 && score_added[j] <= winner) {
//            ++j;
//        }
//
//        winners.push_back(j);
//    }
//
//    std::sort(winners.begin(), winners.end());
//
//    // remove losers from agents
//    std::vector<Agent> winner_agents;
//    for(auto it = winners.begin(); it != winners.end(); ++it) {
//        agents[*it].reset_score();
//        winner_agents.push_back(agents[*it]);
//    }
//    agents = winner_agents;
//
//    // make mutations
//    for(auto it = winner_agents.begin(); it != winner_agents.end(); ++it) {
//        // 1. decrement all by 1-5%
//        std::vector<double> genome = it->get_genome();
//        for(auto it = genome.begin(); it != genome.end(); ++it) {
//            double num = (double) rand() / RAND_MAX / 20 + 0.01;
//            *it = *it - (*it * num);
//        }
//        // constructor automatically normalizes genome-vector
//        agents.push_back(Agent(genome));
//
//
//        // 2. increment all by 1-5%
//        genome = it->get_genome();
//        for(auto it = genome.begin(); it != genome.end(); ++it) {
//            double num = (double) rand() / RAND_MAX / 20 + 0.01;
//            *it = *it + (*it * num);
//        }
//        agents.push_back(Agent(genome));
//
//        // 3. range [-1%,1%]
//        genome = it->get_genome();
//        for(auto it = genome.begin(); it != genome.end(); ++it) {
//            double num = (double) rand() / RAND_MAX / 50 - 0.01;
//            *it = *it + (*it * num);
//        }
//        agents.push_back(Agent(genome));
//
//        // 4. favor index 0
//        genome = it->get_genome();
//        double num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[0] += (genome[0] * num);
//        agents.push_back(Agent(genome));
//
//        // 5. favor index 1
//        genome = it->get_genome();
//        num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[1] += (genome[1] * num);
//        agents.push_back(Agent(genome));
//
//        // 6. favor index 2
//        genome = it->get_genome();
//        num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[2] += (genome[2] * num);
//        agents.push_back(Agent(genome));
//
//        // 7. dis-favor index 0
//        genome = it->get_genome();
//        num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[0] -= (genome[0] * num);
//        agents.push_back(Agent(genome));
//
//        // 8. dis-favor index 1
//        genome = it->get_genome();
//        num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[1] -= (genome[1] * num);
//        agents.push_back(Agent(genome));
//
//        // 9. dis-favor index 2
//        genome = it->get_genome();
//        num = (double) rand() / RAND_MAX / 20 + 0.01;
//        genome[2] -= (genome[2] * num);
//        agents.push_back(Agent(genome));
//    }
//
////    long int upper = agents.size();
////    for(auto it = winners.rbegin(); it != winners.rend(); ++it) {
////        if(*it + 1 < upper) {
////            agents.erase(agents.begin() + *it + 1, agents.begin() + upper);
////        }
////
////        upper = *it;
////    }
////    if(upper > 0) {
////        agents.erase(agents.begin(), agents.begin() + upper);
////    }
//
//
//}


int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));
    
    std::ofstream file("result.csv");
    file << "Generation;Avg Rock;Avg Paper;Avg Scissor;Best Rock; Best Paper; Best Scissor\n";

    Population population(1000, 3);

    for(int i = 0; i < 100; ++i) {
        population.play_games();

        std::vector<double> best = population.get_best_strategy();
        std::vector<double> avg = population.get_avg_strategy();

        file << i << ";" << avg[0] << ";" << avg[1] << ";" << avg[2] << ";" << best[0] << ";" << best[1] << ";" << best[2] << "\n";
        std::cout << i << "%\n";

        population.evaluate(100);
    }

    file.close();
    
    
    
//    std::vector<Agent> agents;
//    for(int i = 0; i < NUM_AGENTS; ++i) {
//
//        // Rock Paper Scissors has three options
//        agents.push_back(Agent({ 1.0/3, 1.0/3, 1.0/3 }));
//    }
//
//    std::ofstream file("result.csv");
//    file << "Generation;Rock;Paper;Scissor;Score\n";
//
//    for(int i = 0; i <= 10000; ++i) {
//        play_games(agents);
//        std::sort(agents.begin(), agents.end());
//
//        if(i % 10000 == 0) {
//            std::cout << "Top 10 players from game " << i << "\n";
//            std::cout << "Rank |      Strategy       | Score\n";
//            for(auto i = agents.size() - 1; i >= agents.size() - 10; --i) {
//                Agent a = agents[i];
//                printf("%3lu. | %5.2f, %5.2f, %5.2f | %5d\n", agents.size() - i, 100 * a[0], 100 * a[1], 100 * a[2], a.get_score());
//            }
//            std::cout << i * 100 / 1000000 << "% done\n";
//        }
//        Agent best = agents[agents.size() - 1];
//        file << i << ";" << best[0] << ";" << best[1] << ";" << best[2] << ";" << best.get_score() << '\n';
//
//        roulette_selection(agents);
//    }
    
//    file.close();
}
