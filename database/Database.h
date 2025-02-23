#ifndef DATABASE
#define DATABASE

#include <sqlite3.h>
#include <unordered_set>
#include <random>
#include <sstream>

#include "../utils/Log.h"
#include "../utils/Config.h"
#include "../indexer/Chunk.h"

class Database {
public:
    Database();
    ~Database();
    void init();
    void reset();
    void insertChunk(const Chunk& chunk);
private:
    Log _log;
    Config _config;
    sqlite3 * _conn;
};

#endif