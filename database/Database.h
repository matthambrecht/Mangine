#ifndef DATABASE
#define DATABASE

#include <pgvector/pqxx.hpp>
#include <pqxx/pqxx>
#include <unordered_set>

#include "../utils/Log.h"
#include "../utils/Config.h"

class Database {
public:
    Database();
    Database(const Log& log, const Config& config);
    ~Database();
    void init();
    void reset();
    void insertCommand(const std::string& command);
    void insertChunk(const Chunk& chunk);
    std::unordered_set<std::string> getAllCommands();
    std::vector<Chunk> retrieve(const pgvector::Vector& query_embeddings, int k_best);
private:
    const std::string CLASS_NAME = "Database";

    Log _log;
    Config _config;
    pqxx::connection * _conn;
};

#endif