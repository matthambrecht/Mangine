#ifndef MAN
#define MAN

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../utils/json.hpp"
#include "Chunk.cpp"
#include "../service/Request.h"

class Man {
public:
    Man();
    std::vector<std::string> getAllCommands();
    std::string getCommandMan(std::string command);
    std::vector<Chunk> getCommandChunks(
        std::string command,
        std::string man_contents
        );
private:
    Config _config;
};
#endif