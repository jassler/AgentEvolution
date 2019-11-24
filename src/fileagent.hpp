#ifndef H_FILEAGENT
#define H_FILEAGENT

#include <string>
#include <fstream>
#include "agent.hpp"

class FileAgent {
private:
    std::string filename;
    std::ofstream ofstream;

    char delimiter;

public:
    FileAgent(std::string _filename, char _delimiter = ',')
    : filename(_filename), ofstream(_filename), delimiter(_delimiter) {
        if(!ofstream.is_open())
            throw "Output file stream for filename \"" + filename + "\"isn't open!";
    }

    FileAgent& operator<<(size_t n) {
        ofstream << n << delimiter;
        return *this;
    }

    FileAgent& operator<<(std::string s) {
        ofstream << s << delimiter;
        return *this;
    }

    template<size_t TGensize, size_t TPhensize>
    void print_header(Agent<TGensize, TPhensize>& a) {
        // print g
        for(size_t i = 0; i < TGensize; ++i) {
            ofstream << "genome_" << i << delimiter;
        }
        for(size_t i = 0; i < TPhensize; ++i) {
            ofstream << "phenotype_" << i << delimiter;
        }
        for(size_t y = 0; y < a.get_matrix().height(); ++y) {
            for(size_t x = 0; x < a.get_matrix().width(); ++x) {
                ofstream << "m_" << x << "_" << y << delimiter;
            }
        }
        
        ofstream << '\n';
    }

    template<size_t TGensize, size_t TPhensize>
    FileAgent& operator<<(Agent<TGensize, TPhensize>& a) {
        // print g
        for(auto g : a.get_genome()) ofstream << g << delimiter;
        for(auto p : a.get_phenotype()) ofstream << p << delimiter;

        for(auto& row : a.get_matrix()) {
            for(auto cell : row) {
                ofstream << cell << delimiter;
            }
        }

        ofstream << '\n';
        return *this;
    }


};

#endif
