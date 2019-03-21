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


class ResultFile {
private:
    const std::string filename, tmpfile;
    
    std::ofstream out_file;
    std::ifstream in_file;
    
    bool changed;
    
    // check if file name already exists (to create tmp file)
    // gotten from here: https://stackoverflow.com/questions/12774207
    inline bool file_exists (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
    
    // random 10-letter string (to create tmp file)
    std::string random_jargon() {
        const static char available[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
        
        std::string result = "";
        for(int i = 0; i < 10; ++i) {
            int r = rw::rand_int(0, sizeof(available) - 1);
            result += available[r];
        }
        
        return result;
    }
    
    std::string find_tmpfile_name() {
        // find random file name to temporarily store generation data without line of descent
        std::string s = "tmpresult_";
        std::string tmp;
        do {
            tmp = s + random_jargon() + ".csv";
        } while(file_exists(tmp));
        
        return tmp;
    }
    
public:
    ResultFile(std::string filename) : filename(filename), tmpfile(find_tmpfile_name()) {
        
        M_PRINTF("Temporary filename: %s", tmpfile.c_str());
        out_file.open(tmpfile);
        
        changed = false;
        
    }
    
    ~ResultFile() {
        if(out_file.is_open())
            out_file.close();
        if(in_file.is_open())
            in_file.close();
        
        // remove(tmpfile.c_str());
        M_PRINTF("Deleted temporary file %s", tmpfile.c_str());
    }
    
    template<class T>
    std::ofstream& operator<<(const T& t) {
        out_file << t;
        return out_file;
    }
    
    // read line, save to dest
    void operator>>(std::string& dest) {
        std::getline(in_file, dest);
    }
    
    void change_file() {
        if(changed)
            return;
        
        changed = true;
        out_file.close();
        in_file.open(tmpfile);
        out_file.open(filename);
    }
};


// work up ancestor tree of child node recursively, append genome to every row of infile
int print_ancestors(const Agent& a, ResultFile& rf /*std::ifstream& infile, std::ofstream& outfile*/) {
    std::string str;
    
    int index;
    if(a.get_ancestor() == nullptr) {
        // header row of csv-file
        rf >> str;
        rf << str << '\n';
        index = 0;
    } else {
        index = print_ancestors(*a.get_ancestor(), rf);
    }
    
    rf >> str;
    rf << str;
    
    auto genome = a.get_genome();
    
    for(auto it = genome.begin(); it != genome.end(); ++it) {
        rf << args::separator << *it;
    }
    rf << '\n';
    
    return index + 1;
}


void simulate_generations(Population& population, const int& generations, ResultFile& file) {

    for(int i = 0; i <= generations; ++i) {
        population.play_games();

        auto avg = population.get_avg_strategy();
        auto best = population.get_best_strategy();

        file << i;
        for(auto it = avg.begin(); it != avg.end(); ++it) {
            file << args::separator << *it;
        }
        for(auto it = best.begin(); it != best.end(); ++it) {
            file << args::separator << *it;
        }
        file << '\n';
        
        // print % done
        if((i - 1) * 100 / generations != i * 100 / generations) {
            std::cout << '\r' << i * 100 / generations << '%' << std::flush;
        }
        
        // no need to evaluate last generation (otherwise we receive one "ancestor" too many)
        if(i < generations) {
            population.evaluate(100);
        }
    }

}


int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));

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
    
    ResultFile rf(args::filename);

    rf << "Generation" << args::separator;

    // average for whole generation
    rf << "Avg Rock" << args::separator << "Avg Paper" << args::separator << "Avg Scissor" << args::separator;

    // strategy of agent who's won the most games
    rf << "Best Rock" << args::separator << "Best Paper" << args::separator << "Best Scissor" << args::separator;

    // line of descent, added recursively at the end
    rf << "LOD Rock" << args::separator << "LOD Paper" << args::separator << "LOD Scissor\n";

    Population population(args::population_size, args::opponents, 3);

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
    print_ancestors(population[0], rf);
}
