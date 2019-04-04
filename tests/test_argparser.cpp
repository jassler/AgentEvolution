#include <vector>
#include <memory>
#include "gtest/gtest.h"
#include "argparser.hpp"
// ("a,agents", "number of agents for each generation (default " xstr(DEF_POP_SIZE) ")", cxxopts::value<unsigned int>(population_size))
// ("o,opponents", "number of opponents each agent plays against (default " xstr(DEF_OPP_SIZE) ")", cxxopts::value<unsigned int>(opponents))
// ("g,generations", "number of generations to simulate (default " xstr(DEF_GENERATIONS) ")", cxxopts::value<unsigned int>(generations))
// ("w,winners", "number of agents that are allowed to make offsprings after each generation (default " xstr(DEF_WIN_SIZE) ")", cxxopts::value<unsigned int>(winners))
// ("p,probabilities", "probability for each genome to mutate seperated by commas (default " DEF_MUT_PROB_STR ")", cxxopts::value<std::string>(probs_str))
// ("f,file", "file to store results in (default " DEF_FILENAME ")", cxxopts::value<std::string>(filename))
// ("s,separator", "character or string that separates values in csv-file (default " DEF_SEP ")", cxxopts::value<std::string>(separator))
// ("v,verbose", "log output messages (default false)", cxxopts::value<bool>(verbose))
// ("outstart", "string that output starts with (default \\r)", cxxopts::value<std::string>(out_start))
// ("outend", "string that output ends with (default %)", cxxopts::value<std::string>(out_end))
// ("i,interactive", "set aforementioned options interactively")
// ("h,help", "view command line options");

void reset_args() {
    args::population_size = 1;
    args::opponents = 1;
    args::generations = 1;
    args::winners = 1;

    args::mutation_probs = {0.01, 0.01, 0.01};
    args::filename = "csvs/file.csv";
}

TEST(ArgparserTest, Reset) {
    args::reset();

    // bools
    EXPECT_NE(DEF_VERBOSE, args::verbose);
    EXPECT_EQ(DEF_MUTATE_MATRIX, args::mutate_matrix);
    
    // numbers
    EXPECT_EQ(DEF_POP_SIZE, args::population_size);
    EXPECT_EQ(DEF_OPP_SIZE, args::opponents);
    EXPECT_EQ(DEF_GENERATIONS, args::generations);
    EXPECT_EQ(DEF_WIN_SIZE, args::winners);

    // vectors, matrices
    EXPECT_EQ((std::vector<double> DEF_MUT_PROB), args::mutation_probs);
    EXPECT_EQ((std::vector<double> DEF_GENOME), args::genome);
    EXPECT_EQ(Matrix( DEF_MATRIX ), args::matrix);
    EXPECT_EQ(Matrix( DEF_PAYOFF ), args::payoffMatrix);
    
    // strings
    EXPECT_EQ(DEF_FILENAME, args::filename);
    EXPECT_EQ(DEF_SEP, args::separator);

    EXPECT_EQ("\r", args::out_start);
    EXPECT_EQ("%", args::out_end);
}

TEST(ArgparserTest, Numbers) {
    
    char* args[] = {
        (char*) "-a", (char*) "1234",
        (char*) "-o", (char*) "5678",
        (char*) "-g", (char*) "91023",
        (char*) "-w", (char*) "4567"
    };

    args::reset();
    args::parse(2, args);
    EXPECT_EQ(1234, args::population_size);
    EXPECT_EQ(DEF_OPP_SIZE, args::opponents);
    EXPECT_EQ(DEF_GENERATIONS, args::generations);
    EXPECT_EQ(DEF_WIN_SIZE, args::winners);

    args::reset();
    args::parse(2, args + 2);
    EXPECT_EQ(DEF_POP_SIZE, args::population_size);
    EXPECT_EQ(5678, args::opponents);
    EXPECT_EQ(DEF_GENERATIONS, args::generations);
    EXPECT_EQ(DEF_WIN_SIZE, args::winners);

    args::reset();
    args::parse(2, args + 4);
    EXPECT_EQ(DEF_POP_SIZE, args::population_size);
    EXPECT_EQ(DEF_OPP_SIZE, args::opponents);
    EXPECT_EQ(91023, args::generations);
    EXPECT_EQ(DEF_WIN_SIZE, args::winners);

    args::reset();
    args::parse(2, args + 6);
    EXPECT_EQ(DEF_POP_SIZE, args::population_size);
    EXPECT_EQ(DEF_OPP_SIZE, args::opponents);
    EXPECT_EQ(DEF_GENERATIONS, args::generations);
    EXPECT_EQ(4567, args::winners);

    args::reset();
    args::parse(8, args);
    EXPECT_EQ(1234, args::population_size);
    EXPECT_EQ(5678, args::opponents);
    EXPECT_EQ(91023, args::generations);
    EXPECT_EQ(4567, args::winners);
    
    // bools
    EXPECT_EQ(DEF_VERBOSE, args::verbose);
    EXPECT_EQ(DEF_MUTATE_MATRIX, args::mutate_matrix);

    // vectors, matrices
    EXPECT_EQ((std::vector<double> DEF_MUT_PROB), args::mutation_probs);
    EXPECT_EQ((std::vector<double> DEF_GENOME), args::genome);
    EXPECT_EQ(Matrix( DEF_MATRIX ), args::matrix);
    EXPECT_EQ(Matrix( DEF_PAYOFF ), args::payoffMatrix);
    
    // strings
    EXPECT_EQ(DEF_FILENAME, args::filename);
    EXPECT_EQ(DEF_SEP, args::separator);

    EXPECT_EQ("\r", args::out_start);
    EXPECT_EQ("%", args::out_end);
}