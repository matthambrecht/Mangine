#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <fstream>
#include <vector>

#include "../utils/json.hpp"

class Chunk {
public:
    Chunk(const std::string& command, const std::string& chunk_val, pgvector::Vector& embedding);
    pgvector::Vector getEmbedding() const;
    std::string getCommand() const;
    std::string getVal() const;
    void setEmbedding(pgvector::Vector& embedding);
private:
    std::string _command;
    std::string _chunk_val;
    pgvector::Vector _embedding;
};
#endif