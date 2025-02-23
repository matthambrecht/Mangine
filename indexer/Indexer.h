#ifndef INDEXER_H
#define INDEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "../utils/Config.h"
#include "../utils/Log.h"
#include "../database/Database.h"
#include "Chunk.h"
#include "Man.h"

class Indexer {
public:
    Indexer(Database* database) : 
        _log(Log("Indexer")),
        _config(Config()),
        _database(database) {};
    bool index(const std::string& command);
private:
    Log _log;
    Config _config;
    Database * _database;
};
#endif