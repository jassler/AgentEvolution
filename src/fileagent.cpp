#include "fileagent.hpp"
#include <locale>
#include <iostream>
#include <map>

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

void FileAgent::binaryfile_to_csv(std::string filename, std::string resultfilename, std::vector<std::string> header, char sep) {

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

        out << std::fixed << d;
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

/*
 * Opens all files, calculates average, writes it into new file
 * Determines where to break lines by size of headers
 */
void FileAgent::calculate_averages(std::vector<std::string> filenames, std::string outfilename, std::vector<std::string> headers, char sep) {
    
    // final file with all the averages
    std::ofstream result_file(outfilename);
    result_file << pp::join(headers, sep) << '\n';

    // open all binary files
    std::vector<std::ifstream> infiles;
    infiles.reserve(filenames.size());

    for(auto& fn : filenames)
        infiles.push_back(std::ifstream(fn, std::ios::in | std::ios::binary));

    // read single double from all files (each put into x)
    // sum up, divide
    double sum, x;
    double divisor = static_cast<double>(infiles.size());

    // when columns_left is 0, it starts a new line
    size_t columns_left = headers.size();

    while (true) {
        
        sum = 0.0;
        for(auto& f : infiles) {
            if(!f.read(reinterpret_cast<char *>(&x), sizeof(x))) {
                if(&f != &infiles[0]) {
                    std::cerr << "One of the files seem to have a different length\n";
                }
                // OH YEAH, I'M BADASS
                // TRY ME!!
                goto readloop_end;
            }
            
            sum += x;
        }

        result_file << std::fixed << sum / divisor;

        // determine if new line or separator is printed
        if(--columns_left) {
            result_file << sep;
        } else {
            columns_left = headers.size();
            result_file << '\n';
        }
    }
    readloop_end:

    for(auto& f : infiles)
        f.close();
}

bool prompt_bool(std::string prompt, std::string yes = "y", std::string noo = "n") {
    std::cout << prompt << " (" << yes << "/" << noo << "): ";

    while(true) {
        std::string res;
        std::getline(std::cin, res);

        if(res == yes) return true;
        if(res == noo) return false;

        std::cout << "Didn't get that. " << prompt << " (use " << yes << " or " << noo << "): ";
    }
}

void prepare_output(std::vector<std::string> filenames, std::vector<std::string> headers, char sep) {
    std::string outname;
    bool fine = false;

    while(!fine) {
        std::cout << "What's the output filename: ";
        std::getline(std::cin, outname);

        std::string msg = "Filename is \"" + outname + "\". Are you ok with this?";
        
        if (std::filesystem::exists(outname)) {
            msg = "\"" + outname + "\" already exists. Override this?";
        }

        fine = prompt_bool(msg);
    }

    // --- DO IT ---
    FileAgent::calculate_averages(filenames, outname, headers, sep);
}

// basically the same as binaryfile_to_csv
// just returns the last line of printed csv as a string
std::string get_last_line_from_file(std::string filename, std::vector<std::string> header, char sep) {
    // open binary file
    std::ifstream infile(filename, std::ios::binary);

    if(!infile.is_open()) {
        std::cerr << "Binary file \"" << filename << "\" couldn't be opened. No conversion possible";
        return "";
    }
    
    std::stringstream ss;
    std::string last_line = "";
    size_t columns_left = header.size();
    double d;

    // read binary file
    while (true) {
        if(!infile.read(reinterpret_cast<char *>(&d), sizeof(double)))
            break;

        ss << std::fixed << d;
        if(--columns_left)
            ss << sep;
        else {
            last_line = ss.str();
            ss = std::stringstream();
            columns_left = header.size();
        }
    }

    infile.close();
    return last_line;
}

void FileAgent::prompt_interactive_avg_creator(std::vector<std::string> headers) {
    
    // --- Separator ---
    std::cout << "Choose a separator for you csv file (leave empty for ';'): ";
    std::string sep_str;
    char sep;

    std::getline(std::cin, sep_str);
    if(sep_str.empty())
        sep = ';';
    else
        sep = sep_str[0];

    std::cout << "Separator is " << sep << "\n";

    // --- Header ---
    std::cout << "Header is: " << pp::join(headers, ';') << "\n";

    bool fine = prompt_bool("Is this fine?");

    while(!fine) {
        headers.clear();
        std::cout << "Enter your headers now. Leave empty if you're good.\n";

        std::string l;
        while(true) {
            std::cout << ": ";
            std::getline(std::cin, l);

            if(l.empty())
                break;

            headers.push_back(l);
        }

        std::cout << "Header now is: " << pp::join(headers, ';') << "\n";
        fine = prompt_bool("Is this fine?");
    }

    // --- Files ---
    bool input_individual_files = prompt_bool("Do you want to input all files individually (f) or loop through a directory and categorize bin files (d)?", "f", "d");
    
    if(input_individual_files) {
        // individual file input
        std::vector<std::string> filenames;
        fine = false;

        while(!fine) {
            // if user wasn't happy with his entry, he could start over again
            filenames.clear();
            std::cout << "Give me the binary files you want to calculate the averages to. Leave line empty to continue (you'll be given the option to repeat this process if something went wrong)\n";

            // repeatedly ask for files until empty string is supplied
            std::string l;
            while(true) {
                std::cout << ": ";
                std::getline(std::cin, l);

                if(l.empty())
                    break;

                filenames.push_back(l);
            }

            // make sure user did what he wants
            std::cout << "There are now " << std::size(filenames) << " files in the list.\n";
            fine = prompt_bool("Do you want to repeat this process?");
        }

        prepare_output(filenames, headers, sep);
    }
    else
    {
        // ask for directory, loop through files
        std::map<std::string, std::vector<std::string>> groups;

        std::string dir;
        while (true) {
            std::cout << "Give me a directory: ";
            std::getline(std::cin, dir);

            if(std::filesystem::is_directory(dir))
                break;

            std::cout << dir << " is not a directory.\n";
        }

        std::cout << "What extension do the binary files have? (leave empty for \".bin\"): ";
        std::string ext;
        std::getline(std::cin, ext);

        if(ext.empty())
            ext = ".bin";
        std::cout << "Looking for files ending with \"" << ext << "\" in " << dir << "\n";
        std::cout << "You can give each file listed a category. If that category didn't previously exist, a new category will be created.\n";
        
        // loop through directory
        for(const auto & entry : std::filesystem::directory_iterator(dir)) {
            std::string name = entry.path().string();

            // check if it has the correct suffix
            if(name.size() <= ext.size())
                continue;
            
            if(name.substr(name.size() - ext.size()) != ext)
                continue;

            std::string line = get_last_line_from_file(name, headers, sep);

            // choose category or "s" to skip this file
            std::cout << name << "\n" << line << "\n";
            std::cout << "Which category? (type \"s\" to skipt this file): ";

            std::string cat;
            std::getline(std::cin, cat);

            if(cat == "s")
                continue;
            
            // vector is automatically created if key doesn't exist
            // thus we can immediately push_back
            groups[cat].push_back(name);
        }

        for(const auto & [category, filenames] : groups) {
            std::cout << "Calculating averages for category: \"" << category << "\" (contains " << filenames.size() << " files)\n";
            prepare_output(filenames, headers, sep);
        }
    }

    // --- Output ---
    // already done

    // --- BE THANKFUL ---
    std::cout << "Done. Thanks for your cooperation.\n";
}
