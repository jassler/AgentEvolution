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
    extern unsigned int opponents; // TODO: opponents isn't used yet
    extern unsigned int generations;
    extern std::vector<double> mutation_probs;
    
    extern std::string filename;
    extern std::string separator;

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
    void parse(int argc, char* argv[]);
}
#endif /* argparser_hpp */
