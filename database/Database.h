#ifndef DATABASE
#define DATABASE

#include <pgvector/pqxx.hpp>
#include <pqxx/pqxx>

#include "../utils/Log.h"
#include "../utils/Config.h"

class Database {
public:
    Database();
    Database(const Log& log, const Config& config);
    ~Database();
    void init();
    void reset();
private:
    const std::string CLASS_NAME = "Database";

    Log _log;
    Config _config;
    pqxx::connection * _conn;

    void send(const std::string& query);
};

#endif