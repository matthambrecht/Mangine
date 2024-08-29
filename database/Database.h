#ifndef DATABASE
#define DATABASE

#include <pgvector/pqxx.hpp>

class Database {
public:
    Database();
    void init();
private:
    Config _config;
};

#endif