#ifndef API_H
#define API_H

#include "../database/Corpora.h"
#include "../utils/Config.h"
#include "../utils/Log.h"

class API {
public:
    API() : 
        _config(Config()),
        _log(Log("API")),
        _corpora(new Corpora("../tests/test.db")) {};
    ~API() {
        if (_corpora) {
            delete _corpora;
            _corpora = nullptr;
        }
    };
    std::string info();
private:
    Config _config;
    Log _log;
    Corpora * _corpora;
};

#endif