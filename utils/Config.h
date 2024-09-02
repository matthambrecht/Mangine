#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>

#include "json.hpp"

class Config {
public:
    Config();
    nlohmann::json _config;
};
#endif