#ifndef MAN_H
#define MAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#include "../utils/json.hpp"
#include "Chunk.cpp"
#include "../embedding/Pipeline.h"

class Man {
public:
    Man() : _log(Log()), _config(Config()) {
        _max_chunks = _config._config["embedding"]["max_chunks"].get<int>();
    };
    Man(Log& log, Config& config) : _log(log), _config(config) {
        _max_chunks = _config._config["embedding"]["max_chunks"].get<int>();
    };
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
    int _max_chunks;
};
#endif