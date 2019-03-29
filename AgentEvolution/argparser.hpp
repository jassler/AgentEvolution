//
//  argparser.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef argparser_hpp
#define argparser_hpp

#include <string>

#define M_PRINTF(f_, ...) if(args::verbose) printf((f_ "\n"), __VA_ARGS__)

namespace args {

    extern bool verbose;
    
    // default options, may be overwritten by command line arguments
    extern unsigned int population_size;
    extern unsigned int opponents;
    extern unsigned int generations;
    extern unsigned int winners;
    extern std::vector<double> mutation_probs;
    
    extern std::string filename;
    extern std::string separator;

    extern std::string out_start;
    extern std::string out_end;

    /*
    * Parse command line arguments with cxxopts-library
    * https://github.com/jarro2783/cxxopts
    *
    * Currently accepted flags:
    * --agents <num>       : population size
    * --opponents <num>    : opponents each agents has to play against
    * --generations <num>  : amount of generations to simulate
    * --winners <num>      : amount of agents that allowed to make offsprings after each generation
    * --probabilities <num>: probability for each genome to mutate
    * --file <name>        : where to save the output file
    * --verbose            : output info
    * --help               : show list of accepted flags
    */
    void parse(int argc, char* argv[]);
}
#endif /* argparser_hpp */
