#ifndef INDEXER_H
#define INDEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "../utils/Config.h"
#include "../utils/Log.h"
#include "../database/Database.h"
#include "../database/Corpora.h"
#include "Document.h"
#include "Man.h"

class Indexer {
public:
    Indexer() : 
        _log(Log("Indexer")),
        _config(Config()),
        _corpora(Corpora()) {};
    bool index(const std::string& command);
    void indexAll();
private:
    Log _log;
    Config _config;
    Corpora _corpora;
};
#endif