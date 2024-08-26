#ifndef CONFIG
#define CONFIG

#include <iostream>
#include <fstream>

#include "json.hpp"

class Config {
public:
    Config();
    nlohmann::json _config;
};
#endif