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
#include <ctime>
#include <time.h>
#include <sys/stat.h>
#include <string>

#include <csignal>

#include <boost/algorithm/string/replace.hpp>

#define WRITE_VEC(file, vec) for(const auto& v : vec) { file << args::separator << v; }

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
        
        M_PRINTF("Opening actual output file: %s", filename.c_str());
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
Population population(0, 0, Matrix(0, 0), static_cast<size_t>(0));
ResultFile rf("result.csv");

std::string format_time(long long t) {
    std::stringstream ss;
    ss << std::setfill('0')
    << std::setw(2) << t / 1000 / 60 / 60 << ':'
    << std::setw(2) << t / 1000 / 60 % 60 << ':'
    << std::setw(2) << t / 1000 % 60 << '.'
    << std::setw(3) << t % 1000;

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, std::vector<double> vec) {
    auto it = vec.begin();

    if(it == vec.end()) {
        os << "{ }";
    } else {
        os << *it;
        while(++it != vec.end()) {
            os << "," << *it;
        }
    }


    return os;
}

std::ostream& operator<<(std::ostream& os, Matrix m) {
    char index_sep = ';';
    if(args::separator == std::string(1, index_sep))
        index_sep = ',';

    for(size_t y = 0; y < m.height(); ++y){
        if(y > 0) os << '/'; 

        for(size_t x = 0; x < m.width();++x) {
            if(x > 0)
                os << index_sep;
            os << m[y][x];
        }
    }

    return os;
}

// work up ancestor tree of child node recursively, append genome to every row of infile
int print_ancestors(std::shared_ptr<Agent> a, ResultFile& file) {
    std::string str;

    if(a == nullptr) {
        // header row of csv-file

        file >> str;
        file << str << '\n';
        return 0;
    }

    int index = print_ancestors(a->get_ancestor(), file);

    file >> str;
    file << str;

    WRITE_VEC(file, a->get_genome());
    WRITE_VEC(file, a->get_phenotype());
    file << args::separator << a->get_matrix();
    file << '\n';
    
    return index + 1;
}

void simulate_generations(Population& pop, const size_t& generations, ResultFile& file) {

    auto start = std::chrono::high_resolution_clock::now();

    auto out_str = args::out_format;
    boost::replace_all(out_str, "%d", "??:??:??.???");
    boost::replace_all(out_str, "%p", "0");
    boost::replace_all(out_str, "%n", "\n");
    std::cout << out_str << std::flush;
    size_t prev_perc = 0;
    size_t perc_div = args::percent_multiplier / 100;
    if(perc_div == 0)
        perc_div = 1;

    for(size_t i = 0; i <= generations; ++i) {
        pop.play_games();
        file << i;
        WRITE_VEC(file, pop.get_avg_genome());
        WRITE_VEC(file, pop.get_avg_phenotype());

        file << '\n';
        
        // print % done
        if(prev_perc != i * args::percent_multiplier / generations) {
            prev_perc = i * args::percent_multiplier / generations;

            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( now - start ).count();

            // total time it would take to get from 0-100%
            long long total_needed = (elapsed * static_cast<long long>(args::percent_multiplier)) / static_cast<long long>(prev_perc);

            // subtract percentage of time already passed
            total_needed -= (total_needed * static_cast<long long>(prev_perc)) / static_cast<long long>(args::percent_multiplier);

            out_str = args::out_format;
            boost::replace_all(out_str, "%d", format_time(total_needed));
            
            if(args::percent_multiplier > 100)
                boost::replace_all(out_str, "%p", std::to_string(prev_perc / perc_div) + '.' + std::to_string(prev_perc % perc_div));
            else
                boost::replace_all(out_str, "%p", std::to_string(prev_perc * (100 / args::percent_multiplier)));
            boost::replace_all(out_str, "%n", "\n");
            std::cout << out_str << std::flush;
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
            print_ancestors(population[0], rf);
            rf << "\nThis file may have been corrupted";
        }
    } else {
        rf.~ResultFile();
    }

    std::cout << "Aborted with signal " << signum << "\n";
    exit(signum);
}

void print_info(char *cmd_str) {

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::string out_format_replaced = args::out_format;
    boost::replace_all(out_format_replaced, "\r", "\\r");
    boost::replace_all(out_format_replaced, "\n", "%n");

    std::stringstream ss;

    // replace: ^.*?\("(?:\w,)?(\w+)".*\((\w+)\)\)$
    // with: << " --$1 " << args::$2
    ss << "Simulation started on " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "\n"
    << "Command run with the following arguments:\n"
    << cmd_str << "\\\n"
    "    --agents         \"" << args::population_size << "\"\\\n"
    "    --opponents      \"" << args::opponents << "\"\\\n"
    "    --generations    \"" << args::generations << "\"\\\n"
    "    --winners        \"" << args::winners << "\"\\\n"
    "    --genomestart    \"" << args::genome << "\"\\\n"
    "    --matrixstart    \"" << args::matrix << "\"\\\n"
    "    --probabilities  \"" << args::mutation_probs << "\"\\\n"
    "    --matrixmutation \"" << args::matrix_mutation << "\"\\\n"
    "    --payoff         \"" << args::payoff_matrix << "\"\\\n"
    "    --file           \"" << args::filename << "\"\\\n"
    "    --separator      \"" << args::separator << "\"\\\n"
    "    --outformat      \"" << out_format_replaced << "\"\\\n"
    "    --infofile       \"" << args::create_info_file << "\"\\\n"
    "    --verbose        \"" << args::verbose << "\"\n";

    auto str = ss.str();

    std::cout << str;

    if(args::create_info_file) {
        std::string info_filename = args::filename.substr(0, args::filename.length()-4) + "-info.txt";
        std::ofstream info_file(info_filename);
        info_file << str;
        info_file.close();
    }

}

// check if string ends with suffix
// snippet from here: https://stackoverflow.com/a/42844629/7669319
static bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}


int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    
    args::parse(argc, argv);
    if(!endsWith(args::filename, ".csv"))
        args::filename += ".csv";
    
    print_info(argv[0]);

    rf.set_filename(args::filename);
    rf.start();

    rf << "Generation" << args::separator;

    // average for whole generation
    rf << "Avg Rock Genome" << args::separator << "Avg Paper Genome" << args::separator << "Avg Scissor Genome" << args::separator;
    rf << "Avg Rock Phenotype" << args::separator << "Avg Paper Phenotype" << args::separator << "Avg Scissor Phenotype" << args::separator;

    // line of descent, added recursively at the end
    rf << "LOD Rock Genome" << args::separator << "LOD Paper Genome" << args::separator << "LOD Scissor Genome" << args::separator;
    rf << "LOD Rock Phenotype" << args::separator << "LOD Paper Phenotype" << args::separator << "LOD Scissor Phenotype" << args::separator;
    rf << "LOD Matrix\n";
    
    if(args::genome.empty())
        population = Population(args::population_size, args::opponents, args::payoff_matrix, static_cast<size_t>(3));
    else
        population = Population(args::population_size, args::opponents, args::payoff_matrix, args::genome, args::matrix);

    auto t1 = std::chrono::high_resolution_clock::now();
    simulate_generations(population, args::generations, rf);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    
    std::cout << "\rDone\n";
    std::cout << "Time elapsed: " << format_time(elapsed) << '\n';
    
    rf.change_file();
    print_ancestors(population[0], rf);

    return EXIT_SUCCESS;
}
