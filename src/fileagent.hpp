#ifndef H_FILEAGENT
#define H_FILEAGENT

#include <array>
#include <string>
#include <fstream>
#include <algorithm>
#include "prettyprint.hpp"
#include "agent.hpp"

// write csv file
// as soon as value is added, a semicolon gets appended
class FileAgent {
private:
    std::string filename;
    std::ofstream outfile;

    char delimiter;

public:
    FileAgent(std::string _filename, char _delimiter = ',');

    FileAgent &operator<<(size_t n);
    FileAgent &operator<<(std::string s);
    void add_string(std::string s);

    template <size_t S>
    FileAgent &operator<<(std::array<double, S> arr);

    template <size_t TGensize, size_t TPhensize>
    void print_header(Agent<TGensize, TPhensize> &a);

    template <size_t TGensize, size_t TPhensize>
    FileAgent &operator<<(Agent<TGensize, TPhensize> &a);

    static void binaryfile_to_csv(std::string filename, std::string resultfilename, std::vector<std::string> header, char sep = ';');
    
    static void calculate_averages(std::vector<std::string> filenames, std::string outfilename, std::vector<std::string> headers, char sep = ';');
    static void prompt_interactive_avg_creator(std::vector<std::string> headers);
};

template<size_t S>
FileAgent& FileAgent::operator<<(std::array<double, S> arr) {
    for(auto& v : arr) {
        outfile << v << delimiter;
    }

    return *this;
}

template<size_t TGensize, size_t TPhensize>
void FileAgent::print_header(Agent<TGensize, TPhensize>& a) {
    // print g
    for(size_t i = 0; i < TGensize; ++i) {
        outfile << "genome_" << i << delimiter;
    }
    for(size_t i = 0; i < TPhensize; ++i) {
        outfile << "phenotype_" << i << delimiter;
    }
    for(size_t y = 0; y < a.get_matrix().height(); ++y) {
        for(size_t x = 0; x < a.get_matrix().width(); ++x) {
            outfile << "m_" << x << "_" << y << delimiter;
        }
    }
    
    outfile << '\n';
}

template<size_t TGensize, size_t TPhensize>
FileAgent& FileAgent::operator<<(Agent<TGensize, TPhensize>& a) {
    // print g
    *this << a.get_genome();
    *this << a.get_phenotype();

    std::for_each(
        a.get_matrix().begin(),
        a.get_matrix().end(),
        [&](auto& row) { *this << row; }
    );

    outfile << '\n';
    return *this;
}

#endif
