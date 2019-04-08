#include <vector>
#include <memory>
#include "gtest/gtest.h"
#include "argparser.hpp"
// ("a,agents"        , "number of agents for each generation (default " xstr(DEF_POP_SIZE) ")"                                      , value<unsigned int>(population_size))
// ("o,opponents"     , "number of opponents each agent plays against (default " xstr(DEF_OPP_SIZE) ")"                              , value<unsigned int>(opponents))
// ("g,generations"   , "number of generations to simulate (default " xstr(DEF_GENERATIONS) ")"                                      , value<unsigned int>(generations))
// ("w,winners"       , "number of agents that are allowed to make offsprings after each generation (default " xstr(DEF_WIN_SIZE) ")", value<unsigned int>(winners))
// ("m,matrixmutation", "mutate matrices over each generation (default " xstr(DEF_MATRIX_MUTATION) ")"                               , value<double>(matrix_mutation))
// ("s,genomestart"   , "values that genome should start with, separated by comma (default empty for random values)"                 , value<std::string>(genome_str))
// ("matrixstart"     , "matrix to start with (default " DEF_MATRIX_STR ")"                                                          , value<std::string>(matrix_str))
// ("p,payoff"        , "payoff matrix (default " DEF_PAYOFF_STR ")"                                                                 , value<std::string>(payoff_str))
// ("probabilities"   , "probability for each genome to mutate (default " DEF_MUT_PROB_STR ")"                                       , value<std::string>(probs_str))
// ("f,file"          , "file to store results in (default " DEF_FILENAME ")"                                                        , value<std::string>(filename))
// ("separator"       , "character or string that separates values in csv-file (default " DEF_SEP ")"                                , value<std::string>(separator))
// ("v,verbose"       , "log output messages (default false)"                                                                        , value<bool>(verbose))
// ("outstart"        , "string that output starts with (default \\r)"                                                               , value<std::string>(out_start))
// ("outend"          , "string that output ends with (default %)"                                                                   , value<std::string>(out_end))
// ("i,interactive"   , "set aforementioned options interactively")
// ("h,help"          , "view command line options");

TEST(ArgparserTest, Reset) {
    args::reset();

    // bools
    EXPECT_EQ(DEF_VERBOSE, args::verbose);
    
    // numbers
    EXPECT_EQ(DEF_POP_SIZE, args::population_size);
    EXPECT_EQ(DEF_OPP_SIZE, args::opponents);
    EXPECT_EQ(DEF_GENERATIONS, args::generations);
    EXPECT_EQ(DEF_WIN_SIZE, args::winners);

    EXPECT_EQ(DEF_MATRIX_MUTATION, args::matrix_mutation);

    // vectors, matrices
    EXPECT_EQ((std::vector<double> DEF_MUT_PROB), args::mutation_probs);
    EXPECT_EQ((std::vector<double> DEF_GENOME), args::genome);
    EXPECT_EQ(Matrix( DEF_MATRIX ), args::matrix);
    EXPECT_EQ(Matrix( DEF_PAYOFF ), args::payoff_matrix);
    
    // strings
    EXPECT_EQ(DEF_FILENAME, args::filename);
    EXPECT_EQ(DEF_SEP, args::separator);

    EXPECT_EQ("\r", args::out_start);
    EXPECT_EQ("%", args::out_end);
}

void check_defaults(std::initializer_list<std::string> ignoring) {
#define TEST_DEFAULT(def, var) if(std::find(ignoring.begin(), ignoring.end(), #var) == ignoring.end()) EXPECT_EQ(def, args::var)

    // bools
    TEST_DEFAULT(DEF_VERBOSE, verbose);
    
    // numbers
    TEST_DEFAULT(DEF_POP_SIZE, population_size);
    TEST_DEFAULT(DEF_OPP_SIZE, opponents);
    TEST_DEFAULT(DEF_GENERATIONS, generations);
    TEST_DEFAULT(DEF_WIN_SIZE, winners);

    TEST_DEFAULT(DEF_MATRIX_MUTATION, matrix_mutation);

    // vectors, matrices
    TEST_DEFAULT((std::vector<double> DEF_MUT_PROB), mutation_probs);
    TEST_DEFAULT((std::vector<double> DEF_GENOME), genome);
    TEST_DEFAULT(Matrix( DEF_MATRIX ), matrix);
    TEST_DEFAULT(Matrix( DEF_PAYOFF ), payoff_matrix);
    
    // strings
    TEST_DEFAULT(DEF_FILENAME, filename);
    TEST_DEFAULT(DEF_SEP, separator);

    TEST_DEFAULT("\r", out_start);
    TEST_DEFAULT("%", out_end);

#undef TEST_DEFAULT
}

TEST(ArgparserTest, Numbers) {
    
    char* in[] = {
        (char*) "myprogram",
        (char*) "-a", (char*) "1234",
        (char*) "-o", (char*) "5678",
        (char*) "-g", (char*) "91023",
        (char*) "-w", (char*) "4567",
        (char*) "-m", (char*) "0.25"
    };

    args::reset();
    args::parse(3, in);
    EXPECT_EQ(1234, args::population_size);
    check_defaults({ "population_size" });

    args::reset();
    args::parse(3, in + 2);
    EXPECT_EQ(5678, args::opponents);
    check_defaults({ "opponents" });

    args::reset();
    args::parse(3, in + 4);
    EXPECT_EQ(91023, args::generations);
    check_defaults({ "generations" });

    args::reset();
    args::parse(3, in + 6);
    EXPECT_EQ(4567, args::winners);
    check_defaults({ "winners" });

    args::reset();
    args::parse(3, in + 8);
    EXPECT_EQ(0.25, args::matrix_mutation);
    check_defaults({ "matrix_mutation" });

    args::reset();
    args::parse(11, in);
    EXPECT_EQ(1234, args::population_size);
    EXPECT_EQ(5678, args::opponents);
    EXPECT_EQ(91023, args::generations);
    EXPECT_EQ(4567, args::winners);
    EXPECT_EQ(0.25, args::matrix_mutation);

    check_defaults({"population_size", "opponents", "generations", "winners", "matrix_mutation"});
}

TEST(ArgparserTest, Vectors) {
    
    char* in[] = {
        (char*) "myprogram",
        (char*) "--genomestart", (char*) "0.5,20,-2,0",
        (char*) "--probabilities", (char*) "0.25,0.75,0"
    };
    
    args::reset();
    args::parse(3, in);
    EXPECT_EQ((std::vector<double>{0.5, 20, -2, 0}), args::genome);
    EXPECT_EQ((std::vector<double> DEF_MUT_PROB), args::mutation_probs);
    
    args::reset();
    args::parse(3, in + 2);
    EXPECT_EQ((std::vector<double> DEF_GENOME), args::genome);
    EXPECT_EQ((std::vector<double>{0.25, 0.75, 0}), args::mutation_probs);
    
    args::reset();
    args::parse(5, in);
    EXPECT_EQ((std::vector<double>{0.5, 20, -2, 0}), args::genome);
    EXPECT_EQ((std::vector<double>{0.25, 0.75, 0}), args::mutation_probs);
    
    check_defaults({"genome", "mutation_probs"});
}

TEST(ArgparserTest, Matrix) {
    char* in[] = {
        (char*) "myprogram",
        (char*) "--matrixstart", (char*) "0.5,20,-2/100,0,-100/3,0.25,2.5/7,-7,7",
        (char*) "--payoff", (char*) "0.25,0.75,0/3,-3,9/7,4,8"
    };
    
    args::reset();
    args::parse(3, in);
    EXPECT_EQ(Matrix( {{0.5, 20, -2}, {100, 0, -100}, {3, 0.25, 2.5}, {7, -7, 7}} ), args::matrix);
    EXPECT_EQ(Matrix( DEF_PAYOFF ), args::payoff_matrix);
    
    args::reset();
    args::parse(3, in + 2);
    EXPECT_EQ(Matrix( DEF_MATRIX ), args::matrix);
    EXPECT_EQ(Matrix( {{0.25, 0.75, 0}, {3, -3, 9}, {7, 4, 8}} ), args::payoff_matrix);
    
    args::reset();
    args::parse(5, in);
    EXPECT_EQ(Matrix( {{0.5, 20, -2}, {100, 0, -100}, {3, 0.25, 2.5}, {7, -7, 7}} ), args::matrix);
    EXPECT_EQ(Matrix( {{0.25, 0.75, 0}, {3, -3, 9}, {7, 4, 8}} ), args::payoff_matrix);
    
    check_defaults({"matrix", "payoff_matrix"});
}

TEST(ArgparserTest, Strings) {
    char* in[] = {
        (char*) "myprogram",
        (char*) "--file", (char*) "somenewfilename.csv",
        (char*) "--separator", (char*) "^",
        (char*) "--outstart", (char*) "now at",
        (char*) "--outend", (char*) "percentages"
    };

    args::reset();
    args::parse(9, in);

    EXPECT_EQ("somenewfilename.csv", args::filename);
    EXPECT_EQ("^", args::separator);
    EXPECT_EQ("now at", args::out_start);
    EXPECT_EQ("percentages", args::out_end);

    check_defaults({"filename", "separator", "out_start", "out_end"});
}

TEST(ArgparserTest, Booleans) {
    char* in[] = {
        (char*) "myprogram",
        (char*) "-v"
    };

    args::reset();
    args::parse(2, in);

    EXPECT_TRUE(args::verbose);

    check_defaults({"verbose"});
}
