#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <fstream>
#include <vector>

class Document {
public:
    Document(const std::string& command, const std::string& document_val) : 
    _command(command), _document_val(document_val) {};
    std::string getCommand() const { return _command; };
    std::string getVal() const { return _document_val; };
private:
    std::string _command;
    std::string _document_val;
};
#endif