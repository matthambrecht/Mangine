#ifndef INDEXER_H
#define INDEXER_H

#include <iostream>
#include <string>
#include <vector>

class Indexer {
public:
    Indexer(
        const Log& log,
        const Config& config,
        Database* database,
        Request* request
        ) : _log(log), _config(config), _database(database), _request(request) {};
    void index_all();
    void chunker();
    void batch_embed(std::vector<Chunk>& chunks, std::vector<std::string> chunk_strs);
private:
    std::string CLASS_NAME = "Indexer";

    Log _log;
    Config _config;
    Database * _database;
    Request * _request;
};
#endif