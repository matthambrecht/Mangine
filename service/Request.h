#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <pgvector/pqxx.hpp>

#include "../utils/Config.h"
#include "../utils/json.hpp"
#include "../utils/Log.h"

class Request {
public:
    Request();
    Request(const Log& log, const Config& config);
    ~Request();
    pgvector::Vector getEmbedding(const std::string& query);
    std::vector<pgvector::Vector> getEmbeddingBatch(const std::vector<std::string>& query_batch);
private:
    const std::string CLASS_NAME = "Request";

    std::string * _normal_endpoint;
    std::string * _batch_endpoint;
    int _vector_size;
    Log _log;
    Config _config;
};
#endif