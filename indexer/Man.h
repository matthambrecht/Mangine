#ifndef MAN_H
#define MAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#include "Chunk.h"
#include "../libs/json.hpp"
#include "../pipeline/Pipeline.h"
#include "../utils/Config.h"
#include "../utils/Log.h"

class Man {
public:
    Man() : _log(Log()), _config(Config()) {
        _max_chunks = _config._config["entry_params"]["chunk_size"].get<int>();
    };
    Man(Log& log, Config& config) : _log(log), _config(config) {
        _max_chunks = _config._config["entry_params"]["chunk_size"].get<int>();
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