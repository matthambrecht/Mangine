#ifndef MAN_H
#define MAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#include "../utils/json.hpp"
#include "Chunk.cpp"
#include "../service/Request.h"
#include "../utils/Log.h"
#include "../utils/Config.h"

class Man {
public:
    Man() : _log(Log()), _config(Config()) {};
    Man(Log& log, Config& config) : _log(log), _config(config) {};
    std::vector<std::string> getAllCommands();
    std::string getCommandMan(const std::string& command);
    std::vector<Chunk> getCommandChunks(
        const std::string& command,
        const std::string& man_contents
        );
private:
    const std::string CLASS_NAME = "Man";

    Log _log;
    Config _config;
};
#endif