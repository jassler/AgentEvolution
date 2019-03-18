//
//  main.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "agent.hpp"
#include "population.hpp"
#include "randwrap.hpp"
#include "cxxopts.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <time.h>
#include <sys/stat.h>

#define M_PRINTF(f_, ...) if(verbose) printf((f_ "\n"), __VA_ARGS__)


/*
 * Parse command line arguments with cxxopts-library
 * https://github.com/jarro2783/cxxopts
 *
 * Currently accepted flags:
 * --agents <num>     : population size
 * --opponents <num>  : opponents each agents has to play against
 * --generations <num>: amount of generations to simulate
 * --file <name>      : where to save the output file
 * --verbose          : output info
 * --help             : show list of accepted flags
 */
cxxopts::ParseResult parse(int argc, char* argv[],
                           unsigned int& num_agents, unsigned int& num_opponents, unsigned int& num_generations,
                           std::string& filename, bool& verbose) {
    using namespace cxxopts;
    
    try {
        Options options(argv[0], " - simulate agent based games over multiple generations");
        options.positional_help("[optional args]").show_positional_help();
        
        options
            .allow_unrecognised_options()
            .add_options()
            ("a,agents", "number of agents for each generation", value<unsigned int>(num_agents))
            ("o,opponents", "number of opponents each agent plays against", value<unsigned int>(num_opponents))
            ("g,generations", "number of generations to simulate", value<unsigned int>(num_generations))
            ("f,file", "file to store results in", value<std::string>(filename))
            ("v,verbose", "log output messages", value<bool>(verbose))
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

// work up ancestor tree of child node recursively, append genome to every row of infile
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
    
    // default options, may be overwritten by command line arguments
    unsigned int population_size = 500;
    unsigned int opponents = 999; // TODO: opponents isn't used yet
    unsigned int generations = 500;
    
    std::string filename = "result.csv";
    bool verbose = false;
    
    auto result = parse(argc, argv,
                        population_size, opponents, generations,
                        filename, verbose);
    
    M_PRINTF("%5d agents per generation", population_size);
    M_PRINTF("%5d opponents each player takes on", opponents);
    M_PRINTF("%5d generations being simulated", generations);
    
    std::ofstream file(filename);
    
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
            std::cout << '\r' << i * 100 / generations << '%' << std::flush;
        }
        
        if(i < generations) {
            population.evaluate(100);
        }
    }

    std::cout << "\rDone\n";
    
    file.close();
    
    // is this ok? (creating in-file and out-file for same filename)
    std::ifstream infile(filename);
    file = std::ofstream(filename);
    
    print_ancestors(population[0], infile, file);
}
