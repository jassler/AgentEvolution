#include "fileagent.hpp"
#include <locale>

FileAgent::FileAgent(std::string _filename, char _delimiter)
: filename(_filename), outfile(_filename), delimiter(_delimiter) {
    if(!outfile.is_open())
        throw "Output file stream for filename \"" + filename + "\"isn't open!";
}

FileAgent& FileAgent::operator<<(size_t n) {
    outfile << n << delimiter;
    return *this;
}

FileAgent& FileAgent::operator<<(std::string s) {
    outfile << s << delimiter;
    return *this;
}

void FileAgent::add_string(std::string s) {
    outfile << s;
}

void FileAgent::linebreak() {
    outfile << '\n';
}
