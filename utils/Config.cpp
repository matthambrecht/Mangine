#include "Config.h"

Config::Config () {
    std::ifstream json_stream("config.json");
    _config = nlohmann::json::parse(json_stream);
}