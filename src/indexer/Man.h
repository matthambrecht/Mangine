#ifndef MAN_H
#define MAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#include "Document.h"
#include "../include/json.hpp"
#include "../indexer/TextProcessor.h"
#include "../utils/Config.h"
#include "../utils/Log.h"

class Man {
public:
    Man() : _log(Log()), _config(Config()) {
        _document_size = _config._config["entry_params"]["document_size"].get<int>();
    };
    std::vector<std::string> getAllCommands();
    Document getCommandMan(const std::string& command);
private:
    Log _log;
    Config _config;
    int _document_size;
};
#endif