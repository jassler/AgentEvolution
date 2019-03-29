//
//  argparser.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "argparser.hpp"
#include "cxxopts.hpp"

namespace args {
    bool verbose = false;
    
    // default options, may be overwritten by command line arguments
    unsigned int population_size = 1000;
    unsigned int opponents = 500;
    unsigned int generations = 1000;
    unsigned int winners = population_size / 10;
    std::vector<double> mutation_probs = { 0.01, 0.01, 0.01 };
    std::string filename = "result.csv";

    std::string separator = ";";

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
                ("a,agents", "number of agents for each generation", cxxopts::value<unsigned int>(population_size))
                ("o,opponents", "number of opponents each agent plays against", cxxopts::value<unsigned int>(opponents))
                ("g,generations", "number of generations to simulate", cxxopts::value<unsigned int>(generations))
                ("w,winners", "number of agents that are allowed to make offsprings after each generation", cxxopts::value<unsigned int>(winners))
                ("p,probabilities", "probability for each genome to mutate seperated by commas (eg. 0.01,0.01,0.02)", cxxopts::value<std::string>(probs_str))
                ("f,file", "file to store results in", cxxopts::value<std::string>(filename))
                ("s,separator", "character or string that separates values in csv-file", cxxopts::value<std::string>(separator))
                ("v,verbose", "log output messages", cxxopts::value<bool>(verbose))
                ("outstart", "string that output starts with (default \\r)", cxxopts::value<std::string>(out_start))
                ("outend", "string that output ends with (default %)", cxxopts::value<std::string>(out_end))
                ("h,help", "view command line options");
            
            auto result = options.parse(argc, argv);
            
            if(result.count("help")) {
                std::cout << options.help() << '\n';
                exit(0);
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
