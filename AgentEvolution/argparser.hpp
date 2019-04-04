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
#include "matrix.hpp"

#define DEF_VERBOSE false
#define DEF_MUTATE_MATRIX false

#define DEF_POP_SIZE 1000
#define DEF_OPP_SIZE 500
#define DEF_WIN_SIZE 100
#define DEF_GENERATIONS 1000

#define DEF_MUT_PROB { 0.01, 0.01, 0.01 }
#define DEF_MUT_PROB_STR "0.01,0.01,0.01"
#define DEF_GENOME { }
#define DEF_MATRIX { {1,0,0}, {0,1,0}, {0,0,1} }
#define DEF_MATRIX_STR "1,0,0;0,1,0;0,0,1"

#define DEF_PAYOFF { {0,-1,1}, {1,0,-1}, {-1,1,0} }

#define DEF_FILENAME "results/csvs/result.csv"

#define DEF_SEP ","

#define M_PRINTF(f_, ...) if(args::verbose) printf((f_ "\n"), __VA_ARGS__)

namespace args {

    extern bool verbose;
    extern bool mutate_matrix;
    
    // default options, may be overwritten by command line arguments
    extern unsigned int population_size;
    extern unsigned int opponents;
    extern unsigned int generations;
    extern unsigned int winners;

    extern std::vector<double> mutation_probs;
    extern std::vector<double> genome;
    extern Matrix matrix;
    extern Matrix payoffMatrix;
    
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

    // Sets all parameters back to defined values
    void reset();
}
#endif /* argparser_hpp */
