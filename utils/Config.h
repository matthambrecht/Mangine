#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>

#include "../libs/json.hpp"

class Config {
public:
    Config() {
        std::ifstream json_stream("../config.json");
        _config = nlohmann::json::parse(json_stream);
    };
    nlohmann::json _config;
};
#endif