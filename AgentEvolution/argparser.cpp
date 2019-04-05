//
//  argparser.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "argparser.hpp"
#include "cxxopts.hpp"

#define xstr(a) str(a)
#define str(a) #a

#define PROMPT_STR(msg, var, def) {std::cout << msg " (default " def "): "; getline(std::cin, var); if(var.empty()) {std::cout << "Setting to default\n"; var = def;}}
#define PROMPT_INT(msg, var, def) {std::cout << msg " (default " xstr(def) "): "; std::string s; getline(std::cin, s); if(s.empty()) {std::cout << "Setting to default\n"; var = def;} else var = static_cast<unsigned int>( std::stoi(s, nullptr, 10));}

namespace args {
    bool verbose;
    bool mutate_matrix;
    
    // default options, may be overwritten by command line arguments
    unsigned int population_size;
    unsigned int opponents;
    unsigned int generations;
    unsigned int winners;
    
    std::vector<double> mutation_probs;
    std::vector<double> genome;
    Matrix matrix(0, 0);
    Matrix payoff_matrix(0, 0);
    
    std::string filename;
    std::string separator;

    std::string out_start;
    std::string out_end;

    void reset() {
        verbose = DEF_VERBOSE;
        mutate_matrix = DEF_MUTATE_MATRIX;

        // default options, may be overwritten by command line arguments
        population_size = DEF_POP_SIZE;
        opponents       = DEF_OPP_SIZE;
        generations     = DEF_GENERATIONS;
        winners         = DEF_WIN_SIZE;
        
        mutation_probs = DEF_MUT_PROB;
        genome         = DEF_GENOME;
        matrix         = Matrix( DEF_MATRIX );
        payoff_matrix  = Matrix( DEF_PAYOFF );
        
        filename  = DEF_FILENAME;
        separator = DEF_SEP;
        out_start = "\r";
        out_end   = "%";
    }
    
    std::vector<double> str_to_vec(std::string str) {
        std::vector<double> result;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, ',')) {
            result.push_back(std::stod(item));
        }
        
        return result;
    }
    
    Matrix str_to_matrix(std::string str) {
        std::vector<std::vector<double>> result;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, '/')) {
            result.push_back(str_to_vec(item));
        }
        
        return Matrix(result);
    }

    using namespace cxxopts;
    void parse(int argc, char* argv[]) {
        reset();

        try {
            Options options(argv[0], " - simulate agent based games over multiple generations");
            options.positional_help("[optional args]").show_positional_help();

            std::string probs_str = "";
            std::string genome_str = "";
            std::string matrix_str = "";
            std::string payoff_str = "";
            
            options
            .allow_unrecognised_options()
            .add_options()
                ("a,agents"     , "number of agents for each generation (default " xstr(DEF_POP_SIZE) ")"                                      , value<unsigned int>(population_size))
                ("o,opponents"  , "number of opponents each agent plays against (default " xstr(DEF_OPP_SIZE) ")"                              , value<unsigned int>(opponents))
                ("g,generations", "number of generations to simulate (default " xstr(DEF_GENERATIONS) ")"                                      , value<unsigned int>(generations))
                ("w,winners"    , "number of agents that are allowed to make offsprings after each generation (default " xstr(DEF_WIN_SIZE) ")", value<unsigned int>(winners))
                ("s,genomestart", "values that genome should start with, separated by comma (default empty for random values)"                 , value<std::string>(genome_str))
                ("m,matrix"     , "matrix to start with (default " DEF_MATRIX_STR ")"                                                          , value<std::string>(matrix_str))
                ("p,payoff"     , "payoff matrix (default " DEF_PAYOFF_STR ")"                                                                 , value<std::string>(payoff_str))
                ("probabilities", "probability for each genome to mutate (default " DEF_MUT_PROB_STR ")"                                       , value<std::string>(probs_str))
                ("f,file"       , "file to store results in (default " DEF_FILENAME ")"                                                        , value<std::string>(filename))
                ("separator"  , "character or string that separates values in csv-file (default " DEF_SEP ")"                                  , value<std::string>(separator))
                ("v,verbose"    , "log output messages (default false)"                                                                        , value<bool>(verbose))
                ("outstart"     , "string that output starts with (default \\r)"                                                               , value<std::string>(out_start))
                ("outend"       , "string that output ends with (default %)"                                                                   , value<std::string>(out_end))
                ("i,interactive", "set aforementioned options interactively")
                ("h,help"       , "view command line options");
            
            auto result = options.parse(argc, argv);
            
            if(result.count("help")) {
                std::cout << options.help() << '\n';
                exit(0);
            }

            if(result.count("interactive")) {
                std::cout << "Interactive flag activated, prompting for each option\n";
                std::cout << "Out of laziness there won't be any hard type-checking. Please enter data with care.\n";

                PROMPT_INT("Number of agents", population_size, DEF_POP_SIZE);
                PROMPT_INT("Number of opponents each player will face", opponents, DEF_OPP_SIZE);
                PROMPT_INT("Number of generations to simulate", generations, DEF_GENERATIONS);
                PROMPT_INT("Number of winners that can reproduce after each generation", winners, DEF_WIN_SIZE);
                PROMPT_STR("Probability for each genome to mutate seperated by commas", probs_str, DEF_MUT_PROB_STR);
                PROMPT_STR("File to save to", filename, DEF_FILENAME);
                PROMPT_STR("csv separator", separator, DEF_SEP);
            }
            
            if(!genome_str.empty()) {
                genome = str_to_vec(genome_str);
            }

            if(!probs_str.empty()) {
                mutation_probs = str_to_vec(probs_str);
            }
            
            if(!matrix_str.empty()) {
                matrix = str_to_matrix(matrix_str);
            }
            
            if(!payoff_str.empty()) {
                payoff_matrix = str_to_matrix(payoff_str);
            }
            
        } catch(const OptionException& e) {
            std::cout << "error parsing options: " << e.what() << "\n"
            "type '" << argv[0] << " -h' to display usage\n";
            exit(1);
        }
    }
}
