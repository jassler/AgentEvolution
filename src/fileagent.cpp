#include "fileagent.hpp"
#include <locale>

template <class charT, charT sep>
class punct_facet: public std::numpunct<charT> {
protected:
    charT do_decimal_point() const { return sep; }
};

FileAgent::FileAgent(std::string _filename, char _delimiter)
: filename(_filename), outfile(_filename), delimiter(_delimiter) {
    if(!outfile.is_open())
        throw "Output file stream for filename \"" + filename + "\"isn't open!";
    
    outfile.imbue(std::locale(std::cout.getloc(), new punct_facet<char, ','>));
}

FileAgent& FileAgent::operator<<(size_t n) {
    outfile << n << delimiter;
    return *this;
}

FileAgent& FileAgent::operator<<(std::string s) {
    outfile << s << delimiter;
    return *this;
}

void FileAgent::linebreak() {
    outfile << '\n';
}
