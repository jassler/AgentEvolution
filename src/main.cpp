#include "main.hpp"

#define LOG_PROGRESS_MSG(x) if(LOG_PROGRESS) { std::cout << x; }
#define LOG_DEBUG_MSG(x) if(LOG_DEBUG) { std::cout << x << '\n'; }

#define LOG_GENERATION_BUFFER (PHENSIZE + 1)
#define LOG_LOD_BUFFER (GENSIZE * PHENSIZE + GENSIZE + PHENSIZE)
#define OUTBUFFERSIZE ((GENERATIONS + 1) * (LOG_GENERATION_BUFFER + LOG_LOD_BUFFER))

World<POPULATION, GENSIZE, PHENSIZE> world({AGENTSTART}, PAYOFF);
double outbuffer[OUTBUFFERSIZE];
std::array<Agent<GENSIZE, PHENSIZE> *, GENERATIONS+1> lod_vector;

// skip specifies how many cells the main function wants to utilise (for LOD logging)
void do_simulations(const size_t amount) {

    std::array<double, PHENSIZE> avg;
    double *buffer_ptr = outbuffer;
    double *end_ptr = outbuffer + OUTBUFFERSIZE;

    auto log_generation = [&]() {
        world.calculate_average_phenotype(avg);
        
        // Population average
        buffer_ptr = std::copy(begin(avg), end(avg), buffer_ptr);

        // Amount of agents that have strategy 1/3 1/3 1/3
        *(buffer_ptr++) = world.count_agent_if([](auto agent) { return AGENTCONDITION ; });

        // leave space for LOD
        buffer_ptr += LOG_LOD_BUFFER;
    };

    log_generation();

    for(size_t i = 0; i < amount; ++i) {
        world.simulate_generation();
        log_generation();

        LOG_PROGRESS_MSG("\r" << i << " / " << amount << " (" << i * 100 / amount << "%)");
    }

    if(buffer_ptr != end_ptr) {
        std::cerr << "Bufferpointer and Endpointer should be the same! (" << buffer_ptr << " != " << end_ptr <<
                ", or index " << std::distance(outbuffer, buffer_ptr) << " != " << std::distance(outbuffer, end_ptr) << ")\n";
        LOG_DEBUG_MSG("Size calculation was: LOG_GENERATION_BUFFER = " << PHENSIZE << " + 1, LOG_LOD_BUFFER = " << GENSIZE << " * " << PHENSIZE << " + " << GENSIZE << " + " << PHENSIZE << " = " << LOG_LOD_BUFFER << ",\nBUFFERSIZE = ((GENERATIONS + 1) * (LOG_GENERATION_BUFFER + LOG_LOD_BUFFER)) = (" << GENERATIONS << " + 1) * (" << LOG_GENERATION_BUFFER << " + " << LOG_LOD_BUFFER << ") = " << OUTBUFFERSIZE);
        getchar();
    }
}

std::string generate_filename() {
    std::string name;

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

    return name;
}

// attach number to basename
std::string next_available_filename(std::string basename, std::string extension) {
    static size_t counter = 0;
    std::string f;

    do {
        f = basename + "_" + std::to_string(counter) + extension;
        ++counter;
    } while (std::filesystem::exists(f));

    return f;
}

void parse_args(int argc, char** argv, std::string& filename, size_t& rounds) {
    filename = generate_filename();
    rounds = 1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            if(++i == argc) {
                std::cerr << "Missing int value for -r argument (rounds)\n";
                exit(1);
            }
            int rounds_int = std::atoi(argv[i]);
            if(rounds_int < 1) {
                std::cerr << "Argument for -r (rounds) has to be at least 1 (got " << rounds_int << ")\n";
                exit(1);
            }
            rounds = static_cast<size_t>(rounds_int);
        
        } else if(strcmp(argv[i], "-f") == 0) {
            if(++i == argc) {
                std::cerr << "Missing string value for -f argument (filename)\n";
                exit(1);
            }
            filename = std::string(argv[i]);
        
        } else if(strcmp(argv[i], "-h") == 0) {
            std::cout << "Usage: " << argv[0] << " [-r <# of rounds>] [-f <filename>]\n\n";
            std::cout << "\t-r\tHow often should this simulation be run\n";
            std::cout << "\t-f\tWhere should the results be saved\n\t\tDuplicate filenames automatically get incremented\n\n";
            exit(0);

        }
    }

    if(filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv") {
        filename = filename.substr(0, filename.size() - 4);
        std::cout << "Warning: Filename argument (-f) ended with .csv, it is removed to allow counting (will add .csv in the end)";
    }
}

void log_info(std::string outfile, size_t rounds) {
    std::cout << "Population size: " POPULATION_STR "\n";
    std::cout << "Genome size    : " GENSIZE_STR "\n";
    std::cout << "Phenotype size : " PHENSIZE_STR "\n";

    std::cout << "Genome mutation: " GENOME_PROB_STR "\n";
    std::cout << "Matrix mutation: " MATRIX_PROB_STR "\n";
    std::cout << "Generations    : " GENERATIONS_STR "\n";

    std::array<double, PHENSIZE> avgs;
    world.calculate_average_phenotype(avgs);
    // std::cout << "Agent start    : ";
    // operator<<(std::cout, AGENTSTART);
    // std::cout << '\n';
    std::cout << "Avg start      : " << avgs << "\n";
    std::cout << "Payoff matrix  : " << world.get_payoff() << "\n";
    std::cout << "Output file    : " << outfile << "\n";
    std::cout << "Rounds         : " << rounds << "\n";
}


template<typename TDuration>
std::string format_time(TDuration duration, size_t rounds) {
    std::stringstream s;

    s << "\nTime elapsed: " << std::setfill('0') << std::setw(2)
            << duration / 1000 / 60 / 60 << ':' << std::setfill('0') << std::setw(2)
            << duration / 1000 / 60 % 60 << ':' << std::setfill('0') << std::setw(2)
            << duration / 1000 % 60 << '.'
            << duration % 1000 << '\n';

    auto each = duration / static_cast<long long>(rounds);
    s << "For each of the " << rounds << " rounds it averages to " << std::setfill('0') << std::setw(2)
            << each / 1000 / 60 / 60 << ':' << std::setfill('0') << std::setw(2)
            << each / 1000 / 60 % 60 << ':' << std::setfill('0') << std::setw(2)
            << each / 1000 % 60 << '.'
            << each % 1000 << " per round\n";

    return s.str();
}

/*
 * Opens all files, calculates average, writes it into new file
 * Determines where to break lines by size of headers
 */
void calculate_averages(std::vector<std::string> filenames, std::string outfilename, std::array<std::string, LOG_GENERATION_BUFFER + LOG_LOD_BUFFER> headers, char sep) {
    
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

        result_file << sum / divisor;

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

int main(int argc, char** argv) {
    /*
     * SETUP
     */
    LOG_DEBUG_MSG("Setting up world");

    // filename structure: <base_filename>_#.csv
    // # = 0 to number of rounds, or avg when calculation is applied at the end
    std::string base_filename;
    size_t rounds_total, rounds;

    LOG_DEBUG_MSG("Parsing arguments");
    parse_args(argc, argv, base_filename, rounds_total);
    rounds = rounds_total;
    LOG_DEBUG_MSG("Arguments: filename = " << base_filename << ", rounds = " << rounds_total);

    std::vector<std::string> filenames;
    filenames.reserve(rounds);

    /*
     * LOGGING
     */
    log_info(base_filename, rounds);

    /*
     * SIMULATIONS
     */
    auto begin = std::chrono::steady_clock::now();

    LOG_DEBUG_MSG("Starting simulation");
    while (rounds--) {
        LOG_PROGRESS_MSG('\n' << rounds + 1 << ((rounds == 0) ? " round" : " rounds") << " left\n");
        LOG_PROGRESS_MSG("Resetting world");
        world.reset_population();

        // Simulation
        do_simulations(GENERATIONS);

        // LOD
        LOG_DEBUG_MSG("\nDoing LOD");
        auto lod_it = lod_vector.begin();
        auto current = &world[0]; //world.get_best_agent();
        LOG_DEBUG_MSG("Best agent: " << *current);

        while (current != nullptr)
        {
            *(lod_it++) = current;
            current = current->get_ancestor();
        }

        if(lod_it != lod_vector.end()) {
            std::cerr << "LOD Iterator is not at the end of the vector! (couldn't get enough ancestors?)\n";
            std::cerr << "lod_it is at index " << std::distance(lod_vector.begin(), lod_it) << ", array length is " << lod_vector.size() << "\n";
            std::cout << "Press enter to continue";
            getchar();
        }

        // write to buffer
        LOG_DEBUG_MSG("Writing to buffer")
        double *out_ptr = outbuffer;
        for (auto it = lod_vector.rbegin(); it != lod_vector.rend(); ++it) {
            out_ptr += LOG_GENERATION_BUFFER;

            for(double d : (*it)->get_genome())
                *(out_ptr++) = d;
            
            for(double d : (*it)->get_phenotype())
                *(out_ptr++) = d;

            for(auto& row : (*it)->get_matrix())
                for(auto cell : row)
                    *(out_ptr++) = cell;
        }

        // write to file
        std::string binary_filename = next_available_filename(base_filename, ".bin");
        LOG_DEBUG_MSG("Writing to binary file \"" << binary_filename << "\"");
        std::ofstream binary_file(binary_filename, std::ios::binary);
        if(!binary_file.is_open()) {
            std::cerr << "Could not open file: " << binary_filename << "\nInstead dumping result to stdout\n";

            out_ptr = outbuffer;
            size_t countdown = LOG_GENERATION_BUFFER + LOG_LOD_BUFFER;
            for (size_t x = 0; x < OUTBUFFERSIZE; ++x) {
                std::cout << *(out_ptr++);
                if(--countdown == 0)
                    std::cout << '\n';
                else
                    std::cout << ';';
            }
            continue;
        }

        filenames.push_back(binary_filename);

        if(std::distance(outbuffer, out_ptr) != OUTBUFFERSIZE) {
            std::cerr << "Endpointer not at the end. Calculation wrong? (end_ptr = " << std::distance(outbuffer, out_ptr) << ", buffer size = " << OUTBUFFERSIZE << ")\n";
            std::cout << "Press enter to continue...";
            getchar();
        }
        binary_file.write(reinterpret_cast<char *>(outbuffer), std::distance(outbuffer, out_ptr) * sizeof(double));
        binary_file.close();
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << '\n' << format_time(elapsed, rounds_total) << '\n';

    /*
     * Create all csvs
     * Calculate averages of all files
     */
    LOG_DEBUG_MSG("Creating average csv-file");

    LOG_DEBUG_MSG("Generating header");
    std::array<std::string, LOG_GENERATION_BUFFER + LOG_LOD_BUFFER> header;
    auto header_it = header.begin();
    for (size_t i = 0; i < PHENSIZE; ++i)
        *(header_it++) = "phenotype_" + std::to_string(i);
    
    *(header_it++) = "mixed";

    for(size_t i = 0; i < GENSIZE; ++i)
        *(header_it++) = "LOD_gensize_" + std::to_string(i);

    for(size_t i = 0; i < PHENSIZE; ++i)
        *(header_it++) = "LOD_phensize_" + std::to_string(i);

    for(size_t y = 0; y < world[0].get_matrix().height(); ++y) {
        for(size_t x = 0; x < world[0].get_matrix().width(); ++x) {
            *(header_it++) = "m_" + std::to_string(x) + "_" + std::to_string(y);
        }
    }

    LOG_DEBUG_MSG("Header is " << pp::join(header, ','));
    if (header_it != header.end()) {
        std::cerr << "Somehow header_it doesn't point to end (" << header_it << " != " << header.end() << "). Continue?\n";
        int c = getchar();
        if(!(c == 'y' || c == 'Y' || c == 'j' || c == 'J'))
            exit(2);
    }

    LOG_DEBUG_MSG("Calculating averages");
    calculate_averages(filenames, base_filename + "_avg.csv", header, ';');

    /*
     * LOD
     */
    LOG_DEBUG_MSG("Turning all binary files into csvs");
    for (auto f : filenames)
        binaryfile_to_csv(f, f.replace(f.end() - 4, f.end(), ".csv"), header, ';');

    LOG_DEBUG_MSG("Deleting binary files");
    for(auto f : filenames)
        remove(f.c_str());
    // std::vector<Agent<GENSIZE, PHENSIZE> *>
    //     agent_vec;
    // auto current = &world[0];

    // while (current != nullptr)
    // {
    //     agent_vec.push_back(current);
    //     current = current->get_ancestor();
    // }


    // FileAgent file(name, ';');
    // std::ifstream generation_file(name + ".tmp");

    // file << "generation";
    // // printing header
    // for(size_t i = 0; i < PHENSIZE; ++i) {
    //     file << "phenotype_" + std::to_string(i);
    // }
    // file << "mixedstrat";

    // for(size_t i = 0; i < GENSIZE; ++i) {
    //     file << "LOD_gensize_" + std::to_string(i);
    // }

    // for(size_t i = 0; i < PHENSIZE; ++i) {
    //     file << "LOD_phensize_" + std::to_string(i);
    // }

    // for(size_t y = 0; y < world[0].get_matrix().height(); ++y) {
    //     for(size_t x = 0; x < world[0].get_matrix().width(); ++x) {
    //         file << "m_" + std::to_string(x) + "_" + std::to_string(y);
    //     }
    // }

    // file.linebreak();

    // std::string line;
    // for(auto it = agent_vec.rbegin(); it != agent_vec.rend(); ++it) {
    //     std::getline(generation_file, line);
    //     file.add_string(line);
    //     file << **it;
    // }

    // generation_file.close();
    // remove((name + ".tmp").c_str());
}
