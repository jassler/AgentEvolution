//
//  argparser.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "argparser.hpp"
#include "cxxopts.hpp"

#define DEF_POP_SIZE 1000
#define DEF_OPP_SIZE 500
#define DEF_WIN_SIZE 100
#define DEF_GENERATIONS 1000
#define DEF_MUT_PROB 0.01,0.01,0.01
#define DEF_MUT_PROB_STR "0.01,0.01,0.01"
#define DEF_FILENAME "results/csvs/result.csv"

#define DEF_SEP ","

#define xstr(a) str(a)
#define str(a) #a

#define PROMPT(str, var, def, cmp) {std::cout << str " (leave empty for " xstr(def) "): "; std::cin >> var; if(var == cmp) {std::cout << "Setting to default\n"; var = def;}}

namespace args {
    bool verbose = false;
    
    // default options, may be overwritten by command line arguments
    unsigned int population_size = DEF_POP_SIZE;
    unsigned int opponents = DEF_OPP_SIZE;
    unsigned int generations = DEF_GENERATIONS;
    unsigned int winners = DEF_WIN_SIZE;
    std::vector<double> mutation_probs = { DEF_MUT_PROB };
    std::string filename = DEF_FILENAME;

    std::string separator = DEF_SEP;

    std::string out_start = "\r";
    std::string out_end = "%";


    void parse(int argc, char* argv[]) {

        try {
            cxxopts::Options options(argv[0], " - simulate agent based games over multiple generations");
            options.positional_help("[optional args]").show_positional_help();

            std::string probs_str = "";
            
            options
            .allow_unrecognised_options()
            .add_options()
                ("a,agents", "number of agents for each generation (default " xstr(DEF_POP_SIZE) ")", cxxopts::value<unsigned int>(population_size))
                ("o,opponents", "number of opponents each agent plays against (default " xstr(DEF_OPP_SIZE) ")", cxxopts::value<unsigned int>(opponents))
                ("g,generations", "number of generations to simulate (default " xstr(DEF_GENERATIONS) ")", cxxopts::value<unsigned int>(generations))
                ("w,winners", "number of agents that are allowed to make offsprings after each generation (default " xstr(DEF_WIN_SIZE) ")", cxxopts::value<unsigned int>(winners))
                ("p,probabilities", "probability for each genome to mutate seperated by commas (default " DEF_MUT_PROB_STR ")", cxxopts::value<std::string>(probs_str))
                ("f,file", "file to store results in (default " DEF_FILENAME ")", cxxopts::value<std::string>(filename))
                ("s,separator", "character or string that separates values in csv-file (default " DEF_SEP ")", cxxopts::value<std::string>(separator))
                ("v,verbose", "log output messages (default false)", cxxopts::value<bool>(verbose))
                ("outstart", "string that output starts with (default \\r)", cxxopts::value<std::string>(out_start))
                ("outend", "string that output ends with (default %)", cxxopts::value<std::string>(out_end))
                ("i,interactive", "set aforementioned options interactively")
                ("h,help", "view command line options");
            
            auto result = options.parse(argc, argv);
            
            if(result.count("help")) {
                std::cout << options.help() << '\n';
                exit(0);
            }

            if(result.count("interactive")) {
                std::cout << "Interactive flag activated, prompting for each option\n";
                std::cout << "Out of laziness there won't be any hard type-checking. Please enter data with care.\n";

                PROMPT("Number of agents", population_size, DEF_POP_SIZE, 0);
                PROMPT("Number of opponents each player will face", opponents, DEF_OPP_SIZE, 0);
                PROMPT("Number of generations to simulate", generations, DEF_GENERATIONS, 0);
                PROMPT("Number of winners that can reproduce after each generation", winners, DEF_WIN_SIZE, 0);
                PROMPT("Probability for each genome to mutate seperated by commas", probs_str, DEF_MUT_PROB_STR, "");
                PROMPT("File to save to", filename, DEF_FILENAME, "");
                PROMPT("csv separator", separator, DEF_SEP, "");
            }

            if(probs_str.length() > 0) {
                mutation_probs.clear();
                std::stringstream ss(probs_str);
                std::string item;
                while (std::getline(ss, item, ',')) {
                    mutation_probs.push_back(std::stod(item));
                }
            }
            
        } catch(const cxxopts::OptionException& e) {
            std::cout << "error parsing options: " << e.what() << "\n"
            "type '" << argv[0] << " -h' to display usage";
            exit(1);
        }
    }
}
