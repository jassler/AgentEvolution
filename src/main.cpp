#include "agent.hpp"
#include "world.hpp"
#include "fileagent.hpp"
#include "defines.hpp"
#include "prettyprint.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <vector>
#include <locale>

template<size_t TPopsize, size_t TGensize, size_t TPhensize>
void do_simulations(World<TPopsize, TGensize, TPhensize>& world, const size_t amount, std::string filename = "") {
    
    #if LOG_GENERATION
    FileAgent file(filename, ';');
    std::array<double, TPhensize> avg;

    world.calculate_average_phenotype(avg);
    file << avg;
    file.linebreak();
    #endif

    for(size_t i = 0; i < amount; ++i) {
        world.simulate_generation();

        #if LOG_PROGRESS
        std::cout << "\r" << i << " / " << amount << " (" << i * 100 / amount << "%)";
        #endif

        #if LOG_GENERATION
        world.calculate_average_phenotype(avg);
        file << avg;
        file.linebreak();
        #endif
    }
}

template<size_t TPopsize, size_t TGensize, size_t TPhensize>
std::string generate_filename(World<TPopsize, TGensize, TPhensize> world) {
    std::string name(RESULTS_FOLDER);

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
    return name;
}

int main(int argc, char** argv) {
    /*
     * SETUP
     */
    Agent<GENSIZE, PHENSIZE> *agent = new Agent<GENSIZE, PHENSIZE>(AGENTSTART);
    World<POPULATION, GENSIZE, PHENSIZE> world(*agent, PAYOFF);
    std::string name;

    // second argument specifies filename
    if(argc < 2) {
        name = generate_filename(world);
    } else {
        name = std::string(argv[1]);
    }

    /*
     * LOGGING
     */
    std::cout << "Population size: " POPULATION_STR "\n";
    std::cout << "Genome size    : " GENSIZE_STR "\n";
    std::cout << "Phenotype size : " PHENSIZE_STR "\n";

    std::cout << "Genome mutation: " GENOME_PROB_STR "\n";
    std::cout << "Matrix mutation: " MATRIX_PROB_STR "\n";
    std::cout << "Generations    : " GENERATIONS_STR "\n";

    std::cout << "Agent start    : " << agent->get_genome() << "\n";
    std::cout << "Payoff matrix  : " << world.get_payoff() << "\n";
    std::cout << "Output file    : " << name << "\n";

    /*
     * SIMULATIONS
     */
    auto begin = std::chrono::steady_clock::now();
    do_simulations(world, GENERATIONS, name + ".tmp");
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "\nTime elapsed: " << std::setfill('0') << std::setw(2)
        << elapsed / 1000 / 60 / 60 << ':' << std::setfill('0') << std::setw(2)
        << elapsed / 1000 / 60 % 60 << ':' << std::setfill('0') << std::setw(2)
        << elapsed / 1000 % 60 << '.'
        << elapsed % 1000;

    /*
     * LOD
     */
    std::cout << "\nPrinting ancestors...\n";
    std::vector<Agent<GENSIZE, PHENSIZE>*> agent_vec;
    auto current = &world[0];

    while(current != nullptr) {
        agent_vec.push_back(current);
        current = current->get_ancestor();
    }

    FileAgent file(name, ';');
    std::ifstream generation_file(name + ".tmp");

    // printing header
    for(size_t i = 0; i < PHENSIZE; ++i) {
        file << "phenotype_" + std::to_string(i);
    }

    for(size_t i = 0; i < GENSIZE; ++i) {
        file << "LOD_gensize_" + std::to_string(i);
    }

    for(size_t i = 0; i < PHENSIZE; ++i) {
        file << "LOD_phensize_" + std::to_string(i);
    }

    for(size_t y = 0; y < world[0].get_matrix().height(); ++y) {
        for(size_t x = 0; x < world[0].get_matrix().width(); ++x) {
            file << "m_" + std::to_string(x) + "_" + std::to_string(y);
        }
    }

    file.linebreak();

    std::string line;
    for(auto it = agent_vec.rbegin(); it != agent_vec.rend(); ++it) {
        std::getline(generation_file, line);
        file << line << **it;
    }

    generation_file.close();
    remove((name + ".tmp").c_str());
}
