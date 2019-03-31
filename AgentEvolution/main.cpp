//
//  main.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/11/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "population.hpp"
#include "agent.hpp"
#include "randwrap.hpp"
#include "argparser.hpp"
#include "matrix.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
#include <chrono>
#include <time.h>
#include <sys/stat.h>

#include <csignal>

class ResultFile {
private:
    std::string filename, tmpfile;
    
    std::ofstream out_file;
    std::ifstream in_file;
    
    int phase = WRITE_PREP;
    
public:
    enum {WRITE_PREP, WRITE_TMP, WRITE_FILE, WRITE_DONE};
    
    explicit ResultFile(std::string filename) : filename(filename), tmpfile(filename + ".tmp") {
    }
    
    ~ResultFile() {
        M_PRINTF("Closing streams for %s and %s", tmpfile.c_str(), filename.c_str());
        if(out_file.is_open())
            out_file.close();
        if(in_file.is_open())
            in_file.close();
        
        remove(tmpfile.c_str());
        M_PRINTF("Deleted temporary file %s", tmpfile.c_str());
    }
    
    template<class T>
    std::ofstream& operator<<(const T& t) {
        if(phase != WRITE_TMP && phase != WRITE_FILE)
            return out_file;
        
        out_file << t;
        return out_file;
    }
    
    // read line, save to dest
    void operator>>(std::string& dest) {
        if(phase != WRITE_FILE)
            return;
        
        std::getline(in_file, dest);
    }
    
    bool change_file() {
        if(phase != WRITE_TMP)
            return false;
        
        M_PRINTF("Closing temporary file: %s", tmpfile.c_str());
        out_file.close();
        in_file.open(tmpfile);
        
        M_PRINTF("Opening actual output file: %s", tmpfile.c_str());
        out_file.open(filename);
        phase = WRITE_FILE;
        return true;
    }
    
    void set_filename(const std::string& name) {
        if(phase != WRITE_PREP)
            return;
        
        this->tmpfile = name + ".tmp";
        this->filename = name;
        
        M_PRINTF("Temporary filename: %s", tmpfile.c_str());
    }
    
    // first operation: open file and start writing data
    void start() {
        if(phase != WRITE_PREP)
            return;
        
        M_PRINTF("Opening temporary file: %s", tmpfile.c_str());
        out_file.open(tmpfile);
        phase = WRITE_TMP;
    }
};

// global population and ResultFile so we can save the file in case execution gets aborted
Population population(0, 0, 0);
ResultFile rf("result.csv");


// work up ancestor tree of child node recursively, append genome to every row of infile
int print_ancestors(const Agent& a, ResultFile& file) {
    std::string str;
    
    int index;
    if(a.get_ancestor() == nullptr) {
        // header row of csv-file
        file >> str;
        file << str << '\n';
        index = 0;
    } else {
        index = print_ancestors(*a.get_ancestor(), file);
    }
    
    file >> str;
    file << str;
    
    auto phenotype = a.get_phenotype();
    
    for(auto it = phenotype.begin(); it != phenotype.end(); ++it) {
        file << args::separator << *it;
    }
    file << '\n';
    
    return index + 1;
}


void simulate_generations(Population& pop, const size_t& generations, ResultFile& file) {

    std::cout << args::out_start << 0 << args::out_end << std::flush;
    size_t prev_perc = 0;
    for(size_t i = 0; i <= generations; ++i) {
        pop.play_games();

        auto avg = pop.get_avg_strategy();
        auto best = pop.get_best_strategy();

        file << i;
        for(const auto& a : avg) {
            file << args::separator << a;
        }
        for(const auto& b : best) {
            file << args::separator << b;
        }
        file << '\n';
        
        // print % done
        if(prev_perc != i * 100 / generations) {
            prev_perc = i * 100 / generations;
            std::cout << args::out_start << prev_perc << args::out_end << std::flush;
        }
        
        // no need to evaluate last generation (otherwise we receive one "ancestor" too many)
        if(i < generations) {
            pop.evaluate(args::winners);
        }
    }

}

// if aborting, print ancestors from agent
void signalHandler( int signum ) {
    if(signum == SIGINT || signum == SIGABRT) {
        if(rf.change_file()) {
            print_ancestors(*population[0], rf);
        }
    }
}


int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    
    args::parse(argc, argv);

    if(args::verbose) {
        printf("%5d agents per generation\n", args::population_size);
        printf("%5d opponents each player takes on\n", args::opponents);
        printf("%5d generations being simulated\n", args::generations);
        std::cout << "Mutation probabilities are: ";
        auto it = args::mutation_probs.begin();
        std::cout << *it;
        while(++it != args::mutation_probs.end()) {
            std::cout << ", " << *it;
        }
        std::cout << "\nSaving file to " << args::filename << '\n';
        std::cout << "Separator used: " << args::separator << '\n';
    }
    
    rf.set_filename(args::filename);
    rf.start();

    rf << "Generation" << args::separator;

    // average for whole generation
    rf << "Avg Rock" << args::separator << "Avg Paper" << args::separator << "Avg Scissor" << args::separator;

    // strategy of agent who's won the most games
    rf << "Best Rock" << args::separator << "Best Paper" << args::separator << "Best Scissor" << args::separator;

    // line of descent, added recursively at the end
    rf << "LOD Rock" << args::separator << "LOD Paper" << args::separator << "LOD Scissor\n";

    population = Population(args::population_size, args::opponents, 3);

    auto t1 = std::chrono::high_resolution_clock::now();
    simulate_generations(population, args::generations, rf);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    
    std::cout << "\rDone\n";
    
    using std::setw;
    std::cout << "Time elapsed: " << std::setfill('0')
    << setw(2) << elapsed / 1000 / 60 / 60 << ':'
    << setw(2) << (elapsed / 1000 / 60) % 60 << ':'
    << setw(2) << (elapsed / 1000) % 60 << '.'
    << setw(3) << elapsed % 1000 << '\n';
    
    rf.change_file();
    print_ancestors(*population[0], rf);
}
