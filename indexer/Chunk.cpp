#include "Chunk.h"

Chunk::Chunk(std::string command, std::string chunk_val, std::vector<double> embedding) : 
_command(command),
_chunk_val(chunk_val),
_embedding(embedding)
{}

std::vector<double> Chunk::getEmbedding() {return _embedding;}
std::string Chunk::getCommand() {return _command;}
std::string Chunk::getVal() {return _chunk_val;}