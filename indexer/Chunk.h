#ifndef CHUNK
#define CHUNK

#include <iostream>
#include <fstream>
#include <vector>

#include "../utils/json.hpp"

class Chunk {
public:
    Chunk(std::string command, std::string chunk_val, std::vector<double> embedding);
private:
    std::string _command;
    std::string _chunk_val;
    std::vector<double> _embedding;
};
#endif