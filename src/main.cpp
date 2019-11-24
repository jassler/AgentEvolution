#include "agent.hpp"
#include "args.hpp"
#include "world.hpp"
#include "fileagent.hpp"
#include "defines.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <locale>

int main(int argc, char** argv) {
    try {
        std::cout << "Population size: " POPULATION_STR "\n";
        std::cout << "Genome size    : " GENSIZE_STR "\n";
        std::cout << "Phenotype size : " PHENSIZE_STR "\n";

        std::cout << "Genome mutation: " GENOME_PROB_STR "\n";
        std::cout << "Matrix mutation: " MATRIX_PROB_STR "\n";
        std::cout << "Generations    : " GENERATIONS_STR "\n";

        Agent<GENSIZE, PHENSIZE> *agent = new Agent<GENSIZE, PHENSIZE>(AGENTSTART);
        World<POPULATION, GENSIZE, PHENSIZE> world(*agent, PAYOFF);

        std::cout << "Agent start    : " << agent->get_genome() << "\n";
        std::cout << "Payoff matrix  : " << world.get_payoff() << "\n";

        for(int i = 0; i < GENERATIONS; ++i) {
            world.simulate_generation();
        }

        std::cout << world[0] << "\n";
        std::cout << "Printing ancestors...\n";

        std::vector<Agent<GENSIZE, PHENSIZE>*> agent_vec;
        auto current = &world[0];
        while(current != nullptr) {
            agent_vec.push_back(current);
            current = current->get_ancestor();
        }


        std::string name = "results/csvs/";

        // second argument specifies filename
        if(argc < 2) {
            for(auto& row : world.get_payoff()) {
                for(auto cell : row) {
                    if(cell > 0) name += "+";
                    else if(cell < 0) name += "-";
                    else name += "0";
                }
                name += ",";
            }
            name.erase(name.end() - 1);

            // avoid overwriting old file
            // start counting
            if(std::filesystem::exists(name + ".csv")) {
                size_t counter = 1;
                name += " ";
                while(std::filesystem::exists(name + std::to_string(counter) + ".csv"))
                    ++counter;
                
                name += std::to_string(counter);
            }

            // add suffix
            name += ".csv";
        } else {
            name = std::string(argv[1]);
        }

        std::reverse(begin(agent_vec), end(agent_vec));

        FileAgent file(name, ';');
        file << "generation";
        file.print_header(*agent_vec[0]);

        for(size_t i = 0; i < agent_vec.size(); ++i) {
            file << i << *agent_vec[i];
        }
    } catch(MatrixException e)
    {
        std::cerr << "gotta matrix exception: " << e.what() << "\n\n";
    }
}