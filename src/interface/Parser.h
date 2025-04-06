#ifndef PARSER_H
#define PARSER_H

#include "../include/argparse.hpp"
#include "API.h"
#include <iostream>
#include <string>

namespace Parser {
    std::string getArgs(char argv[][48], int argc, API * api);
}

#endif