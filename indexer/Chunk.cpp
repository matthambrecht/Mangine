#include "Chunk.h"

Chunk::Chunk(const std::string& command, const std::string& chunk_val, pgvector::Vector& embedding) : 
_command(command),
_chunk_val(chunk_val),
_embedding(embedding)
{}

pgvector::Vector Chunk::getEmbedding() const {return _embedding;}
std::string Chunk::getCommand() const {return _command;}
std::string Chunk::getVal() const {return _chunk_val;}
void Chunk::setEmbedding(pgvector::Vector& embedding) { _embedding = embedding; }