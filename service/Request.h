#ifndef REQUEST
#define REQUEST

#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>

#include "../utils/Config.h"
#include "../utils/json.hpp"
#include "../utils/Log.h"

class Request {
public:
    Request();
    Request(const Log& log, const Config& config);
    Request(const std::string& endpoint, const int vector_size, const Log& log, const Config& config);
    ~Request();
    std::vector<double> getEmbedding(const std::string& query);
private:
    const std::string CLASS_NAME = "Request";

    std::string * _endpoint;
    int _vector_size;
    Log _log;
    Config _config;
};
#endif