//
//  main.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "population.hpp"
#include "cxxopts.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <time.h>

#define NUM_AGENTS 1000

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

cxxopts::ParseResult parse(int argc, char* argv[]) {
    using namespace cxxopts;
    
    try {
        Options options(argv[0], " - command line options");
        options.positional_help("[optional args]").show_positional_help();
        
        unsigned int agent_amount = 1000;
        unsigned int opponent_amount = 999;
        unsigned int generations = 1000;
        
        std::string filename = "result.csv";
        
        options
            .allow_unrecognised_options()
            .add_options()
            ("a,agents", "number of agents for each generation", value<unsigned int>(agent_amount))
            ("o,opponents", "number of opponents each agent plays against", value<unsigned int>(opponent_amount))
            ("g,generations", "number of generations to simulate", value<unsigned int>(generations))
            ("f,file", "file to store results in", value<std::string>(filename))
            ("v,verbose", "log output messages", value<bool>()->default_value("false"))
            ("h,help", "view command line options");
        
        auto result = options.parse(argc, argv);
        
        if(result.count("help")) {
            std::cout << options.help() << '\n';
            exit(0);
        }
        
        return result;
    } catch(const OptionException& e) {
        std::cout << "error parsing options: " << e.what() << "\n"
        "type '" << argv[0] << " -h' to display usage";
        exit(1);
    }
}

int print_ancestors(const std::shared_ptr<Agent> a, std::ifstream& infile, std::ofstream& outfile) {
    std::string str;
    
    if(a == nullptr) {
        // header row of csv-file
        std::getline(infile, str);
        outfile << str << '\n';
        return 0;
    }
    
    int index = print_ancestors(a->get_ancestor(), infile, outfile);
    std::getline(infile, str);
    outfile << str;
    
    auto genome = a->get_genome();
    
    for(auto it = genome.begin(); it != genome.end(); ++it) {
        outfile << ';' << *it;
    }
    outfile << '\n';
    
    return index + 1;
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));
    
    int population_size = 100;
    int generations = 500;
    
    std::string filename = "result.csv";
    
    // TODO argument parsing
    auto result = parse(argc, argv);
    
    
    std::string tmp_file = "temporaryfile_deleteme.csv";
    std::ofstream file(tmp_file);
    file << "Generation;";
    
    // average for whole generation
    file << "Avg Rock;Avg Paper;Avg Scissor;";
    
    // statregy of agent who's won the most games
    file << "Best Rock;Best Paper;Best Scissor;";
    
    // line of descent, added recursively at the end
    file << "LOD Rock;LOD Paper;LOD Scissor\n";

    Population population(population_size, 3);

    for(int i = 0; i <= generations; ++i) {
        population.play_games();

        std::vector<double> avg = population.get_avg_strategy();
        std::vector<double> best = population.get_best_strategy();

        file << i << ";";
        file << avg[0] << ";" << avg[1] << ";" << avg[2] << ";";
        file << best[0] << ";" << best[1] << ";" << best[2] << "\n";
        
        // print % done
        if((i - 1) * 100 / generations != i * 100 / generations) {
            std::cout << i * 100 / generations << "%\n";
        }
        
        if(i < generations) {
            population.evaluate(100);
        }
    }
    
    file.close();
    
    std::ifstream infile(tmp_file);
    file = std::ofstream(filename);
    
    
    print_ancestors(population[0], infile, file);
}
