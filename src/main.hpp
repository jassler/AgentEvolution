#ifndef H_MAIN
#define H_MAIN

#include "agent.hpp"
#include "world.hpp"
#include "fileagent.hpp"
#include "defines.hpp"
#include "prettyprint.hpp"

#include "filesystemsupport.hpp"

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <vector>
#include <locale>
#include <string.h>

void calculate_averages(std::vector<std::string> filenames, std::string outfilename, std::initializer_list<std::string> headers, char sep = ';');

#endif
