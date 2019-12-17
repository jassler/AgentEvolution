#ifndef H_FILEAGENT
#define H_FILEAGENT

#include <array>
#include <string>
#include <fstream>
#include <algorithm>
#include "prettyprint.hpp"
#include "agent.hpp"

template <size_t S>
void binaryfile_to_csv(std::string filename, std::string resultfilename, std::array<std::string, S> header, char sep = ';') {

    // open binary file
    std::ifstream infile(filename, std::ios::binary);
    if(!infile.is_open()) {
        std::cerr << "Binary file \"" << filename << "\" couldn't be opened. No conversion possible";
        return;
    }

    // setup outstream
    // if outfile can't be opened, print to cout
    std::streambuf *buf;
    std::ofstream outfile(resultfilename);
    if(outfile.is_open()) {
        buf = outfile.rdbuf();
    } else {
        std::cerr << "File \"" << resultfilename << "\" couldn't be opened. Instead ouptutting to stdout\n";
        buf = std::cout.rdbuf();
    }
    std::ostream out(buf);

    // header
    out << pp::join(header, sep) << '\n';
    
    // every time columns_left hits 0, a new line gets printed
    size_t columns_left = header.size();

    // value in binary file gets read into d
    double d;

    // read binary file
    while (true) {
        if(!infile.read(reinterpret_cast<char *>(&d), sizeof(double)))
            break;

        out << d;
        if(--columns_left)
            out << sep;
        else {
            out << '\n';
            columns_left = header.size();
        }
    }

    infile.close();
    if(outfile.is_open())
        outfile.close();
}

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

    void linebreak();
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
