#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <fstream>
#include <vector>

class Chunk {
public:
    Chunk(const std::string& command, const std::string& chunk_val) : 
    _command(command), _chunk_val(chunk_val) {};
    std::string getCommand() const { return _command; };
    std::string getVal() const { return _chunk_val; };
private:
    std::string _command;
    std::string _chunk_val;
};
#endif